#include "mcpserver.hpp"
#include "sysmlv2/sysmlv2apiclient.hpp"
#include "httptoolclient.hpp"

#include <iostream>

using namespace std;

MCPServer::MCPServer(const string_view name, const string_view version) noexcept :
  MCPServer { name, version, ProgramOptions() } {
}

MCPServer::MCPServer(const string_view name, const string_view version,
  const ProgramOptions& programOptions) noexcept :
  name_(name), version_(version), programOptions_(move(programOptions)) {
  setupCapabilities();
  httpClient_ = std::make_unique<SysMLv2APIClient>(*this);
}

void MCPServer::setTransport(unique_ptr<MCPTransport> mcpTransport) {
  if (mcpTransport_) {
    mcpTransport_->stop();
  }
  mcpTransport_ = move(mcpTransport);
}

void MCPServer::run() {
  if (! mcpTransport_) {
    throw runtime_error("No transport for MCP request/respones configured!");
  }
  
  mcpTransport_->start([this](const json& request) {
    return this->handleRequest(request);
  });
}

void MCPServer::stop() noexcept {
  if (mcpTransport_) {
    mcpTransport_->stop();
  }
}

bool MCPServer::isRunning() const noexcept {
  return mcpTransport_ && mcpTransport_->isRunning();
}

json MCPServer::handleRequest(const json& request) noexcept {
  try {
    checkJsonRpcVersion(request);
    checkIfParameterExists(JSONPARAM_METHOD, request);

    const string method = request[JSONPARAM_METHOD];
    const json parameters = request.value("params", json::object());
    const json id = request.value("id", nullptr);

    json result;

    if (method == "initialize") {
      result = performInitialization(parameters);
    } else if (method == "tools/list") {
      result = determineListOfAvailableTools();
    } else if (method == "tools/call") {
      result = callTool(parameters);
    } else if (method == "resources/list") {
      result = determineListOfAvailableResources();
    } else if (method == "resources/read") {
      result = readResource(parameters);
    } else {
      throw runtime_error("Unknown method: " + method);
    }
    
    return {
      {JSONPARAM_JSONRPC_VERSION, "2.0"},
      {"id", id},
      {"result", result}
    };

  } catch (const exception& ex) {
    json id = request.value("id", nullptr);
    return {
      {JSONPARAM_JSONRPC_VERSION, "2.0"},
      {"id", id},
      {"error", {
        {"code", -1},
        {"message", ex.what()}
      }}
    };
  }
}

void MCPServer::registerTool(const string& toolName,
                             const string& description,
                             const json& inputSchema,
                             function<json(const json&)> handler) {
  tools_[toolName] = {toolName, description, inputSchema, handler};
}

void MCPServer::registerResource(const string& resourceName,
                                 const string& uri,
                                 const string& description,
                                 const string& mimeType,
                                 function<json()> handler) {
  resources_[uri] = {uri, resourceName, description, mimeType, handler};
}

void MCPServer::setupCapabilities() noexcept {
  capabilities_ = {
    {"tools", json::object()},
    {"resources", json::object()},
    {"prompts", json::object()},
    {"logging", json::object()}
  };
}

void MCPServer::registerEchoTool() {
  registerTool("echo",
    "A simple tool that returns the passed message as its response.",
    {
      {"type", "object"},
      {"properties", {
        {"message", {
          {"type", "string"},
          {"description", "The message that shall be echoed."}
        }}
      }},
      {"required", {"message"}}
    },
    [](const json& parameters) -> json {
      return {
        {"content", {{
          {"type", "text"},
            {"text", "Echo: " + parameters["message"].get<std::string>()}
        }}}
      };
  });
}

void MCPServer::registerSpecificTools() {

}

json MCPServer::performInitialization(const json& parameters) {
  if (! initialized_) {
    checkMcpProtocolVersion(parameters);
    registerEchoTool();
    registerSpecificTools();
    initialized_ = true;
  }
  
  return {
    {JSONPARAM_PROTOCOL_VERSION, SUPPORTED_MCP_PROTOCOL_VERSION},
    {"capabilities", capabilities_},
    {"serverInfo", {
      {"name", name_},
      {"version", version_}
      }
    }
  };
}

json MCPServer::determineListOfAvailableTools() const {
  checkIfServerIsInitialized();
  
  json availableTools = json::array();
  for (const auto& [name, tool] : tools_) {
    json toolInfo = {
      {"name", tool.name_},
      {"description", tool.description_},
      {"inputSchema", tool.inputSchema_}
    };
    availableTools.push_back(toolInfo);
  }
  
  return {
    {"tools", availableTools}
  };
}

json MCPServer::callTool(const json& parameters) {
  checkIfServerIsInitialized();
  checkIfParameterExists("name", parameters);
  
  const std::string toolName = parameters["name"];
  checkIfToolExists(toolName);
  json arguments = parameters.value("arguments", json::object());
  
  try {
    return invokeToolHandler(toolName, arguments);
  } catch (const exception& ex) {
    return {
      {"content", {{
          {"type", "text"},
          {"text", "Error executing tool '" + toolName + "'. Reason: " +
            string(ex.what())}
        }}},
        {"isError", true}
    };
  }
}

json MCPServer::invokeToolHandler(const std::string& toolName, const json& arguments) {
  json result = tools_[toolName].handler_(arguments);
  return {
    {"content", result.value("content", json::array())},
    {"isError", false}
  };
}

json MCPServer::determineListOfAvailableResources() const {
  checkIfServerIsInitialized();
  
  json resourcesList = json::array();
  
  for (const auto& [uri, resource] : resources_) {
    json resourceInfo = {
      {"uri", resource.uri_},
      {"name", resource.name_},
      {"description", resource.description_},
      {"mimeType", resource.mimeType_}
    };
    resourcesList.push_back(resourceInfo);
  }
  
  return {
    {"resources", resourcesList}
  };
}

json MCPServer::readResource(const json& parameters) {
  checkIfServerIsInitialized();
  checkIfParameterExists("uri", parameters);
  
  const string uri = parameters["uri"];
        
  checkIfResourceExists(uri);
  
  try {
    json content = resources_[uri].handler_();
    return {
      {"contents", {{
        {"uri", uri},
        {"mimeType", resources_[uri].mimeType_},
        {"text", content}
      }}}
    };
    } catch (const exception& ex) {
      throw runtime_error("Error reading resource: " + std::string(ex.what()));
    }
}

void MCPServer::checkIfServerIsInitialized() const {
  if (! initialized_) {
    throw runtime_error("MCP Server not initialized!");
  }
}

void MCPServer::checkJsonRpcVersion(const json &request) const {
  if (! request.contains(JSONPARAM_JSONRPC_VERSION) ||
        request[JSONPARAM_JSONRPC_VERSION] != REQUIRED_JSONRPC_VERSION) {
    throw runtime_error("Missing or invalid JSON-RPC version -- must be version 2.0!");
  }
}

void MCPServer::checkMcpProtocolVersion(const json &parameters) const {
  checkIfParameterExists(JSONPARAM_PROTOCOL_VERSION, parameters);

  const string protocolVersion = parameters[JSONPARAM_PROTOCOL_VERSION];
  if (protocolVersion != SUPPORTED_MCP_PROTOCOL_VERSION) {
    throw runtime_error("Unsupported MCP protocol version: " + protocolVersion);
  }
}

void MCPServer::checkIfParameterExists(const string_view paramName,
  const json& jsonToCheck) const {
  if (! jsonToCheck.contains(paramName)) {
    string exceptionMessage("Missing parameter '");
    exceptionMessage += paramName;
    exceptionMessage += "' in JSON object.";
    throw runtime_error(exceptionMessage);
  }
}

void MCPServer::checkIfToolExists(const string& toolName) const {
  if (! tools_.contains(toolName)) {
    throw runtime_error("Tool not found: " + toolName);
  }
}

void MCPServer::checkIfResourceExists(const std::string& uri) const {
  if (! resources_.contains(uri)) {
    throw runtime_error("Resource not found: " + uri);
  }
}

const char* const MCPServer::JSONPARAM_JSONRPC_VERSION = "jsonrpc";
const char* const MCPServer::REQUIRED_JSONRPC_VERSION = "2.0";
const char* const MCPServer::JSONPARAM_PROTOCOL_VERSION = "protocolVersion";
const char* const MCPServer::SUPPORTED_MCP_PROTOCOL_VERSION = "2024-11-05";
const char* const MCPServer::JSONPARAM_METHOD = "method";