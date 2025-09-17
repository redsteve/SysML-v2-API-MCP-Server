#include "httpmcptransport.hpp"
#include <spdlog/spdlog.h>
#include <thread>

using namespace std;

HttpMcpTransport::HttpMcpTransport(const string& serverName, const string serverVersion) :
  HttpMcpTransport("127.0.0.1", 8080, serverName, serverVersion) { }

HttpMcpTransport::HttpMcpTransport(const std::string& hostAddress, const uint16_t port,
  const std::string& serverName, const std::string serverVersion) :
  hostAddress_(hostAddress), port_(port), serverName_(serverName), serverVersion_(serverVersion) {
    server_ = std::make_unique<httplib::Server>();
}

void HttpMcpTransport::start(function<json(const json&)> requestHandler) {
  if (running_)
    return;

  // Cross-Origin Resource Sharing (CORS) Headers for browser compatibility
  server_->set_pre_routing_handler([]([[maybe_unused]]const httplib::Request& request, httplib::Response& response) {
    response.set_header("Access-Control-Allow-Origin", "*");
    response.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    response.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    return httplib::Server::HandlerResponse::Unhandled;
  });

  createEndpoints(requestHandler);

  running_ = true;
  serverThread_ = std::thread([this]() {
    spdlog::info("Starting {0} on {1}:{2}.", serverName_, hostAddress_, port_);
    if (! server_->listen(hostAddress_, port_)) {
      spdlog::error("Fatal error: Server could not be started!");
    }
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void HttpMcpTransport::stop() {
  if (! running_)
    return;
  running_ = false;
  if (server_) {
    server_->stop();
  }
  
  if (serverThread_.joinable()) {
    serverThread_.join();
  }
}

bool HttpMcpTransport::isRunning() const noexcept {
  return running_;
}

HttpMcpTransport::~HttpMcpTransport() {
  stop();
}

void HttpMcpTransport::createEndpoints(function<json(const json&)> requestHandler) {
  // OPTIONS handler for CORS preflight
  server_->Options(".*", [](const httplib::Request&, httplib::Response&)
    { return; });

  // Main end point for MCP requests
  server_->Post("/mcp", [requestHandler](const httplib::Request& req, httplib::Response& res) {
    spdlog::trace("MCP request received. Content: '{}'.", req.body);
    try {
      json request = json::parse(req.body);
      json response = requestHandler(request);
      spdlog::trace("Response to MCP request is: '{}'.", response.dump());
      res.set_content(response.dump(), JSON_MIME_TYPE);
      res.status = HTTP_STATUS_OK;
    } catch (const std::exception& ex) {
      json errorResponse = {
        {"jsonrpc", "2.0"},
        {"id", nullptr},
        {"error", {
          {"code", -32700},
          {"message", std::string(ex.what())}
        }}
      };
      spdlog::error("Error while processing MCP request: {}", errorResponse.dump());
      res.set_content(errorResponse.dump(), JSON_MIME_TYPE);
      res.status = HTTP_STATUS_BAD_REQUEST;
    }
  });

  // Health check endpoint
  server_->Get("/health", [](const httplib::Request &, httplib::Response &res) {
    json healthResponse = {
      {"status", "ok"},
      {"timestamp", std::time(nullptr)}
    };
    res.set_content(healthResponse.dump(), JSON_MIME_TYPE);
  });

  // Server info endpoint
  server_->Get("/info", [this](const httplib::Request &, httplib::Response &res) {
    json infoResponse = {
      {"server", serverName_},
      {"version", serverVersion_},
      {"transport", "HTTP"},
      {"endpoints", {
        {"mcp", "/mcp"},
        {"health", "/health"},
        {"info", "/info"}
      }}
    };
    res.set_content(infoResponse.dump(), JSON_MIME_TYPE);
  });
}

const char* const HttpMcpTransport::JSON_MIME_TYPE = "application/json";
const int HttpMcpTransport::HTTP_STATUS_OK = 200;
const int HttpMcpTransport::HTTP_STATUS_BAD_REQUEST = 400;