#include "commandlineargumentparser.hpp"
#include "httpmcptransport.hpp"
#include "mcpserver.hpp"
#include "stdinstdoutmcptransport.hpp"
#include "sysmlv2/sysmlv2apiclient.hpp"

const char* const APPLICATION_NAME = "SysMLv2MCPServer";
const char* const APPLICATION_VERSION = "0.0.9 BETA";

int main(int argc, const char** argv) {
  CommandLineArgumentParser parser{ APPLICATION_NAME, APPLICATION_VERSION };
  ProgramOptions programOptions = parser.parse(argc, argv);

  // Step 1: Configure the server
  MCPServer server { APPLICATION_NAME, APPLICATION_VERSION, programOptions };
  if (programOptions.mcpTransportKind_ == McpTransportKind::stdinout) {
    server.setMcpTransport(std::make_unique<StdinStdoutMcpTransport>());
  } else {
    server.setMcpTransport(std::make_unique<HttpMcpTransport>(APPLICATION_NAME, APPLICATION_VERSION));
  }

  // Step 2: Start the server
  try {
    server.run();
  } catch (const std::runtime_error& ex) {
    std::cerr << "FATAL ERROR - Server start failed: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  // Step 3: Wait for interrupt
  while (server.isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  server.stop();
  return EXIT_SUCCESS;
}