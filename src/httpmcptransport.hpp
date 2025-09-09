#pragma once

#include "mcptransport.hpp"
// #include <httplib.h>

#include <memory>
#include <string>
#include <thread>

/// @brief Implements MCP transport via HTTP/HTTPS.
class HttpMcpTransport final : public MCPTransport {
public:
  void start(std::function<json(const json&)> requestHandler) override;
  void stop() override;
  bool isRunning() const noexcept override;
  ~HttpMcpTransport() override;

private:
  // std::unique_ptr<httplib::Server> server_;
  std::thread serverThread_;
  std::string hostAddress_;
  int port_;
  bool running_;
};