# pragma once

#include <string>

/// @brief The possible kinds of transport for MCP protocol messages
enum class McpTransportKind {
  stdinout, http
};

enum class LogLevel {
  trace, info, warn, error
};

/// @brief A data structure that contains the program options selected by the user.
struct ProgramOptions {
  McpTransportKind mcpTransportKind_;
  std::string mcpTransportUrl_;
  std::string sysmlv2ApiUrl_;
  LogLevel logLevel_;
  std::string logFileName_;
};
