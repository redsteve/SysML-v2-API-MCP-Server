#include "../src/mcpserver.hpp"
#include "testdata.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Verifying SysML v2 API MCP-Server") {

  SECTION("Incorrect JSON-RPC version leads to an error response") {
    MCPServer server { SERVER_NAME, SERVER_VERSION };
    const json response = server.handleRequest(requestWithInvalidJsonRpcVersion);
    REQUIRE(response == expectedJsonRpcVersionErrorResponse);
  }

  SECTION("Unsupported MCP protocol version leads to an error response") {
    MCPServer server { SERVER_NAME, SERVER_VERSION };
    const json response = server.handleRequest(requestWithInvalidMcpProtocolVersion);
    REQUIRE(response == expectedMcpProtocolVersionErrorResponse);
  }

  SECTION("Check whether the server is properly initialized") {
    MCPServer server { SERVER_NAME, SERVER_VERSION };
    const json response = server.handleRequest(initServerRequest);
    REQUIRE(response == expectedInitServerResponse);
  }

  SECTION("Test the echo tool") {
    MCPServer server { SERVER_NAME, SERVER_VERSION };
    server.handleRequest(initServerRequest);
    const json response = server.handleRequest(callEchoToolRequest);
    REQUIRE(response == expectedEchoToolResponse);
  }

  SECTION("Mandatory MCP method 'tools/list' works as expected") {
    MCPServer server { SERVER_NAME, SERVER_VERSION };
    server.handleRequest(initServerRequest);
    const json response = server.handleRequest(listAvailableToolsRequest);
    REQUIRE(response == expectedToolListResponse);
  }
}
