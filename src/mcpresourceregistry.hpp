#pragma once

#include <nlohmann/json.hpp>

#include <functional>
#include <string>

/// @brief The MCP Resource Registry Interface
///
/// An interface (abstract class) that defines the services that
/// implementing classes must support to act as a MCP resource registry.
class MCPResourceRegistry {
public:
  virtual void registerResource(const std::string& resourceName,
    const std::string& uri,
    const std::string& description,
    const std::string& mimeType,
    std::function<nlohmann::json()> handler) = 0;
  virtual ~MCPResourceRegistry() = default;
};