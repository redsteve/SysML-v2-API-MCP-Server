# pragma once

#include <string>

enum class LogLevel {
  Info, Warn, Error
};

struct ProgramOptions {
  std::string url_;
  LogLevel logLevel_;
};
