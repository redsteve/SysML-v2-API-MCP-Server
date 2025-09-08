#pragma once

#include "programoptions.hpp"

#include <nlohmann/json.hpp>
#include <functional>
#include <map>
#include <string>

using json = nlohmann::json;

/// A MCP server is a software application that expose specific capabilities
/// to AI applications (agents) through standardized protocol interfaces.
class MCPServer {
public:
  /// @brief An initialization constructor.
  /// @param name is the server's name.
  /// @param version is a string that describes the version of the server.
  MCPServer(const std::string_view name, const std::string_view version) noexcept;

  /// @brief An initialization constructor.
  /// @param name is the server's name.
  /// @param version is a string that describes the version of the server.
  /// @param programOptions is an object that contains information for configuring
  /// and parameterizing the server.
  MCPServer(const std::string_view name, const std::string_view version,
    const ProgramOptions& programOptions) noexcept;
  
  /// @brief Starts the server.
  void run();
  
  /// @brief
  /// @param request is the request as a JSON-RPC object.
  /// @return the response of the request as a JSON object.
  json handleRequest(const json &request) noexcept;

  void registerTool(const std::string& toolName,
                    const std::string& description,
                    const json& inputSchema,
                    std::function<json(const json&)> handler);

  void registerResource(const std::string& resourceName,
                        const std::string& uri,
                        const std::string& description,
                        const std::string& mimeType,
                        std::function<json()> handler);

  MCPServer() = delete;

private:
    void setupCapabilities() noexcept;
    void registerEchoTool();
    json performInitialization(const json& parameters);
    json determineListOfAvailableTools() const;
    json callTool(const json &parameters);
    json invokeToolHandler(const std::string &toolName, const json &arguments);
    json determineListOfAvailableResources() const;
    json readResource(const json& parameters);

    void checkIfServerIsInitialized() const;
    void checkMcpProtocolVersion(const json& parameters) const;
    void checkJsonRpcVersion(const json& request) const;
    void checkIfParameterExists(const std::string_view paramName, const json& jsonToCheck) const;
    void checkIfToolExists(const std::string& toolName) const;
    void checkIfResourceExists(const std::string& uri) const;

    struct ToolDefinition {
        std::string name_;
        std::string description_;
        json inputSchema_;
        std::function<json(const json&)> handler_;
    };

    std::map<std::string, ToolDefinition> tools_;

    struct ResourceDefinition {
        std::string uri_;
        std::string name_;
        std::string description_;
        std::string mimeType_;
        std::function<json()> handler_;
    };

    std::map<std::string, ResourceDefinition> resources_;

    std::string name_;
    std::string version_;
    ProgramOptions programOptions_;
    json capabilities_;
    bool initialized_ { false };

    static const char* const JSONPARAM_JSONRPC_VERSION;
    static const char* const REQUIRED_JSONRPC_VERSION;
    static const char* const JSONPARAM_PROTOCOL_VERSION;
    static const char* const SUPPORTED_MCP_PROTOCOL_VERSION;
    static const char* const JSONPARAM_METHOD;
};