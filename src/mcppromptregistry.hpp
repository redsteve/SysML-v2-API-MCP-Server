#pragma once

#include <nlohmann/json.hpp>

#include <functional>
#include <string>

/// @brief The MCP Prompt Registry Interface
///
/// An interface (abstract class) that defines the services that
/// implementing classes must support to act as a MCP prompt registry.
class MCPPromptRegistry {
public:
  virtual void registerPrompt(const std::string& promptName,
    const std::string& title,
    const std::string& description,
    const nlohmann::json& arguments) = 0;
  virtual ~MCPPromptRegistry() = default;
};