#pragma once

#include "mcptransport.hpp"

// IMPORTANT: httplib.h must be included BEFORE Windows.h!
#include <httplib.h>
#include <Windows.h>

#include <functional>
#include <memory>
#include <string>
#include <thread>

/// @brief Implements MCP transport via HTTP/HTTPS.
class HttpMcpTransport final : public MCPTransport {
public:
  /// @brief An initialization constructor.
  /// @param serverName is the name of the server.
  /// @param serverVersion is the version of the server.
  HttpMcpTransport(const std::string& serverName, const std::string serverVersion);

  HttpMcpTransport(const std::string& hostAddress, const uint16_t port,
    const std::string& serverName, const std::string serverVersion);
  HttpMcpTransport() = delete;

  void start(std::function<json(const json&)> requestHandler) override;
  void stop() override;
  bool isRunning() const noexcept override;

  ~HttpMcpTransport() override;

private:
  void createEndpoints(std::function<json(const json&)> requestHandler);

  std::string hostAddress_;
  uint16_t port_;
  std::string serverName_;
  std::string serverVersion_;
  std::thread serverThread_;
  std::unique_ptr<httplib::Server> server_;
  bool running_ { false };

  static const char* const JSON_MIME_TYPE;
  static const int HTTP_STATUS_OK;
  static const int HTTP_STATUS_BAD_REQUEST;
};