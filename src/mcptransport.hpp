#pragma once

#include <nlohmann/json.hpp>
#include <functional>

using json = nlohmann::json;

/// @brief The MCP Transport Interface
///
/// An interface (abstract class) that defines the methods for transporting requests/responses
/// in the Model Context Protocol (MCP) format.
class MCPTransport {
public:
  virtual void start(std::function<json(const json&)> requestHandler) = 0;
  virtual void stop() = 0;
  virtual bool isRunning() const noexcept = 0;
  virtual ~MCPTransport() = default;
};