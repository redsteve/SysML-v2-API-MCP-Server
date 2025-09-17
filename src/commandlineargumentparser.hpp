#pragma once

#include "programoptions.hpp"
#include <argparse/argparse.hpp>
#include <string>

/// @brief 
class CommandLineArgumentParser {
  public:
  /// @brief An initialization constructor.
  /// @param applicationName is the file name of the application.
  /// @param applicationVersion is the application's version.
  CommandLineArgumentParser(const std::string_view applicationName,
    const std::string_view applicationVersion);

  /// Parses the given command line arguments and returns the result as
  /// an instance of type ProgramOptions.
  ProgramOptions parse(int argc, const char **argv) const;

  CommandLineArgumentParser() = delete;

private:
  void addArgumentsToParser(argparse::ArgumentParser& parser) const;
  McpTransportKind determineMcpTransportKind(const std::string_view parsedTransport) const;
  LogLevel determineLogLevel(const std::string_view parsedLogLevel) const;

  const std::string applicationName_ { "n/a" };
  const std::string applicationVersion_ { "n/a" };
};