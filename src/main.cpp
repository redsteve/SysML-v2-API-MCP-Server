#include "commandlineargumentparser.hpp"
#include "mcpserver.hpp"

const char* const APPLICATION_NAME = "SysMLv2MCPServer";
const char* const APPLICATION_VERSION = "0.0.9 BETA";

int main(int argc, const char** argv) {
  CommandLineArgumentParser parser{ APPLICATION_NAME, APPLICATION_VERSION };
  ProgramOptions programOptions = parser.parse(argc, argv);

  MCPServer server { APPLICATION_NAME, APPLICATION_VERSION, programOptions };
  server.run();

  return EXIT_SUCCESS;
}