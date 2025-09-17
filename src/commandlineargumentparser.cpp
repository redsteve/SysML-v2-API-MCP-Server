#include "commandlineargumentparser.hpp"

CommandLineArgumentParser::CommandLineArgumentParser(const std::string_view applicationName,
    const std::string_view applicationVersion) :
    applicationName_(applicationName), applicationVersion_(applicationVersion) { }

ProgramOptions CommandLineArgumentParser::parse(int argc, const char** argv) const {
  argparse::ArgumentParser parser(applicationName_, applicationVersion_);

  addArgumentsToParser(parser);
  parser.parse_args(argc, argv);

  ProgramOptions options { };
  options.sysmlv2ApiUrl_ = parser.get("apiurl");
  options.mcpTransportUrl_ = parser.get("serverurl");
  options.mcpTransportKind_ = determineMcpTransportKind(parser.get("transport"));
  options.logLevel_ = determineLogLevel(parser.get("loglevel"));
  return options;
}

void CommandLineArgumentParser::addArgumentsToParser(argparse::ArgumentParser &parser) const {
  parser.add_argument("-t", "--transport")
    .help("defines the transport layer for MCP requests/responses. Can be either one of the\n"
          "values 'stdinout' or 'http'.")
    .required()
    .default_value("http");

  parser.add_argument("-s", "--serverurl")
    .help("the uniform resource locator (URL) specifying the address at which this MCP server should\n"
          "be accessible if 'http' is chosen as MCP transport (default). Examples: 'http://api.hostname.tld',\n"
          "'http://sysml2mcp.domain.com:8080'. Default is 'http://127.0.0.1:8080' if no explicit URL has been specified.\n")
    .required()
    .default_value("127.0.0.1:8080");

  parser.add_argument("-a", "--apiurl")
    .help("the uniform resource locator (URL) specifying the address of a REST endpoint providing\n"
          "a SysML v2 API for accessing models. Examples: 'http://api.hostname.tld', 'http://sysml2.domain.com:9000'.\n"
          "Default is 'http://127.0.0.1:9000' if no explicit URL has been specified.")
    .required()
    .default_value("http://127.0.0.1:9000");

  parser.add_argument("-l", "--loglevel")
    .help("the log level, which defines the scope (verbosity) of logging. Can be INFO, WARN or ERROR.\n"
          "ERROR is the default log level if no explicit level has been specified.")
    .default_value("ERROR");
}

McpTransportKind CommandLineArgumentParser::determineMcpTransportKind(const std::string_view parsedTransport) const {
  if (parsedTransport == "stdinout") {
    return McpTransportKind::stdinout;
  } else {
    return McpTransportKind::http;
  }
}

LogLevel CommandLineArgumentParser::determineLogLevel(const std::string_view parsedLogLevel) const {
  if (parsedLogLevel == "INFO") {
    return LogLevel::info;
  } else if (parsedLogLevel == "WARN") {
    return LogLevel::warn;
  } else {
    return LogLevel::error;
  }
}