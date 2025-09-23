#pragma once

///@file Global constants and configuration data

#include <cstdint>

namespace globals {
  const char* const APPLICATION_NAME { "SysMLv2MCPServer" };
  const char* const APPLICATION_VERSION { "0.0.9 BETA" };

  const char* const REQUIRED_JSONRPC_VERSION { "2.0" };
  const char* const JSON_MIME_TYPE { "application/json" };
  const char* const SUPPORTED_MCP_PROTOCOL_VERSION { "2025-06-18" };
  const char* const SERVERSENTEVENTS_CONTENT_TYPE { "text/event-stream" };
  const char* const DEFAULT_SERVER_ADDRESS { "127.0.0.1" };
  const char* const DEFAULT_SERVER_PORT { "8080" };

  const int16_t JSONRPC_ERROR_METHOD_NOT_FOUND = -32601;
  const int16_t JSONRPC_ERROR_GENERAL = -31999;

  const int16_t HTTP_STATUS_OK = 200;
  const int16_t HTTP_STATUS_BAD_REQUEST = 400;
}