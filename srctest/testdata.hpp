#pragma once

/// @file Provides prepared test data.

#include "../src/globals.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char* const SERVER_NAME = "SysMLv2MCPServer4Testing";
const char* const SERVER_VERSION = "0.0.0";

const json requestWithInvalidJsonRpcVersion = {
  {"jsonrpc", "1.0"}, // !!!
  {"id", 1},
  {"method", "initialize"},
  {"params", {
    {"protocolVersion", globals::SUPPORTED_MCP_PROTOCOL_VERSION},
    {"clientInfo", {
      {"name", "MCPHostApplication"},
      {"version", "0.0.0"}
      }}
    }}
};

const json expectedJsonRpcVersionErrorResponse = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 0},
  {"error" , { {"code", globals::JSONRPC_ERROR_GENERAL},
    {"message", "Missing or invalid JSON-RPC version -- must be version 2.0!"}}
  }
};

const json requestWithInvalidMcpProtocolVersion = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 2},
  {"method", "initialize"},
  {"params", {
    {"protocolVersion", "1972-01-01"}, // !!!
    {"clientInfo", {
      {"name", "MCPHostApplication"},
      {"version", "0.0.0"}
      }}
    }}
};

const json expectedMcpProtocolVersionErrorResponse = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 0},
  {"error" , { {"code", globals::JSONRPC_ERROR_GENERAL},
    {"message", "Unsupported MCP protocol version: 1972-01-01"}}
  }
};

const json initServerRequest = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 3},
  {"method", "initialize"},
  {"params", {
    {"protocolVersion", globals::SUPPORTED_MCP_PROTOCOL_VERSION},
    {"clientInfo", {
      {"name", "TestApplication"},
      {"version", "0.0.0"}
      }}
    }}
};

const json expectedInitServerResponse = {
  {"id" , 3} ,
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION} ,
  {"result",
    { { "capabilities", {
      {"logging", json::object() },
      {"prompts", { {"ListChanged", false} } },
      {"resources", { {"ListChanged", false} } },
      {"tools", { {"ListChanged", false} } } } },
    {"protocolVersion", globals::SUPPORTED_MCP_PROTOCOL_VERSION},
    {"serverInfo", {
      {"name", "SysMLv2MCPServer4Testing"},
      {"version", "0.0.0"} } }
    }}
};

const json callEchoToolRequest = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 4},
  {"method", "tools/call"},
  {"params", {
    {"name", "echo"},
    {"arguments", {
      {"message", "The big brown fox jumps over the lazy dog."}
      }}
    }}
};

const json expectedEchoToolResponse = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 4},
  {"result", {
    {"content", {{
      {"type", "text"},
      {"text", "Echo: The big brown fox jumps over the lazy dog."}
    }}},
    { "isError", json(false)}
  }}
};

const json listAvailableToolsRequest = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 5},
  {"method", "tools/list"},
  {"params", {
    {"protocolVersion", "2024-11-05"},
    {"clientInfo", {
      {"name", "MCPHostApplication"},
      {"version", "0.0.0"}
      }}
    }}
};

const json expectedToolListResponse = {
  {"jsonrpc", globals::REQUIRED_JSONRPC_VERSION},
  {"id", 5},
  {"result", {
    {"tools", {{
      {"name", "echo"},
      {"description", "A simple tool that returns the passed message as its response."},
      {"inputSchema",
        {"properties",
          {"message",
            {{"type", "string"}, {"description", "The message that shall be echoed."}}
          }},
          {"type", "object"},
    {"required", {"message"}}}
    }}}
  }}
};