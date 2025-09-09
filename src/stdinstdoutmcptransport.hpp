#pragma once

#include "mcptransport.hpp"

#include <thread>

/// @brief Implements MCP transport via stdin and stdout.
class StdinStdoutMcpTransport final : public MCPTransport {
public:
  StdinStdoutMcpTransport() noexcept = default;
  void start(std::function<json(const json&)> requestHandler) override;
  void stop() override;
  bool isRunning() const noexcept override;
  ~StdinStdoutMcpTransport() override;

private:
    bool running_ { false };
    std::thread thread_;
};