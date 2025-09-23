#include "stdinstdoutmcptransport.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

using namespace std;

void StdinStdoutMcpTransport::start(function<json(const json&)> requestHandler) {
  if (running_)
    return;

  running_ = true;
  spdlog::info("Starting worker thread waiting for requests via stdin...");
  thread_ = thread([this, requestHandler]() {
    string line;
    while (running_ && std::getline(std::cin, line)) {
      try {
        if (line.empty())
          continue;
          
        json request = json::parse(line);
        spdlog::debug("Request is: {}", request.dump());
        json response = requestHandler(request);
        spdlog::debug("Received response from request handler: {}", response.dump());
        std::cout << response << std::flush;
      } catch (const exception& ex) {
        const json errorResponse = {
          {"jsonrpc", "2.0"},
          {"id", nullptr},
          {"error", {
            {"code", -32700},
            {"message", std::string(ex.what())}
          }}
        };
        spdlog::error("while parsing received data from stdin: {}", ex.what());
        std::cout << errorResponse << std::flush;
      }
    }
  });
}

void StdinStdoutMcpTransport::stop() {
  running_ = false;
  if (thread_.joinable()) {
    thread_.join();
  }
}

bool StdinStdoutMcpTransport::isRunning() const noexcept {
  return running_;
}

StdinStdoutMcpTransport::~StdinStdoutMcpTransport() {
  stop();
}