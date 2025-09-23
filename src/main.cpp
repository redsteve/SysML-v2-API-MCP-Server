#include "globals.hpp"
#include "commandlineargumentparser.hpp"
#include "httpmcptransport.hpp"
#include "mcpserver.hpp"
#include "stdinstdoutmcptransport.hpp"
#include "sysmlv2/sysmlv2apiclient.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

// Forward declarations
void initializeLogging(const spdlog::filename_t& logFileName);

int main(int argc, const char** argv) {
  CommandLineArgumentParser parser{ globals::APPLICATION_NAME,
    globals::APPLICATION_VERSION };
  ProgramOptions programOptions = parser.parse(argc, argv);
  initializeLogging(programOptions.logFileName_);

  // Step 1: Configure the server
  MCPServer server { globals::APPLICATION_NAME, globals::APPLICATION_VERSION,
    programOptions };
  if (programOptions.mcpTransportKind_ == McpTransportKind::stdinout) {
    server.setMcpTransport(std::make_unique<StdinStdoutMcpTransport>());
    spdlog::info("MCP transport configured to stdin/stdout.");
  } else {
    server.setMcpTransport(std::make_unique<HttpMcpTransport>(globals::APPLICATION_NAME,
      globals::APPLICATION_VERSION));
    spdlog::info("MCP transport configured to HTTP.");
  }

  // Step 2: Start the server
  try {
    server.run();
  } catch (const std::runtime_error& ex) {
    spdlog::critical("FATAL ERROR - Server start failed. Reason: {}", ex.what());
    return EXIT_FAILURE;
  }

  // Step 3: Wait for interrupt
  while (server.isRunning()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  server.stop();
  spdlog::info("Server successfully stopped. Good bye!");
  return EXIT_SUCCESS;
}

void initializeLogging([[maybe_unused]]const spdlog::filename_t& logFileName) {
  try {
    auto logger = spdlog::basic_logger_mt(globals::APPLICATION_NAME, logFileName);
    spdlog::set_default_logger(logger);
    spdlog::default_logger()->set_level(spdlog::level::trace);
    spdlog::flush_every(std::chrono::seconds(2));
    spdlog::trace("Initialization of logging successful.");
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Initialization of logging failed: " << ex.what() << std::endl;
  }
}