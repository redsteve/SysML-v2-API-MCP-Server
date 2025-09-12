#include "commandlineargumentparser.hpp"

#include <argparse/argparse.hpp>

CommandLineArgumentParser::CommandLineArgumentParser(const std::string_view applicationName,
    const std::string_view applicationVersion) :
    applicationName_(applicationName), applicationVersion_(applicationVersion) { }

ProgramOptions CommandLineArgumentParser::parse(int argc, const char** argv) const {
  argparse::ArgumentParser parser(applicationName_, applicationVersion_);
  
  parser.add_argument("-u", "--url")
    .help("the uniform resource locator (url) specifying the address of an endpoint providing a\n"
      "SysML v2 REST API. Examples: https://api.localhost.tld, http://sysml2.domain.com:9000\n"
      "Default address is http://localhost:9000 if no explicit url has been specified.")
    .default_value("http://localhost:9000");

  parser.add_argument("-l", "--loglevel")
    .help("the log level, which defines the scope (verbosity) of logging. Can be INFO, WARN or ERROR.\n"
      "ERROR is the default log level if no explicit level has been specified.")
    .default_value("ERROR");

  parser.parse_args(argc, argv);

  ProgramOptions options { };
  options.url_ = parser.get("url");
  options.logLevel_ = determineLogLevel(parser.get("loglevel"));
  return options;
}

LogLevel CommandLineArgumentParser::determineLogLevel(const std::string_view parsedLogLevel) const {
  if (parsedLogLevel == "INFO") {
    return LogLevel::Info;
  } else if (parsedLogLevel == "WARN") {
    return LogLevel::Warn;
  } else {
    return LogLevel::Error;
  }
}