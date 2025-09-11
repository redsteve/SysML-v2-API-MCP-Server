#pragma once

#include <nlohmann/json.hpp>

#include <functional>
#include <string>

/// @brief The MCP Tool Registry Interface
///
/// An interface (abstract class) that defines the services that
/// implementing classes must support to act as a MCP tool registry.
class MCPToolRegistry {
public:
  virtual void registerTool(const std::string& toolName,
    const std::string& description,
    const nlohmann::json& inputSchema,
    std::function<nlohmann::json(const nlohmann::json&)> handler) = 0;
  virtual ~MCPToolRegistry() = default;
};