#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char* const SERVER_NAME = "SysMLv2MCPServer4Testing";
const char* const SERVER_VERSION = "0.0.0";

const json requestWithInvalidJsonRpcVersion = {
  {"jsonrpc", "1.0"}
};

const json expectedJsonRpcVersionErrorResponse = {
  {"jsonrpc", "2.0"},
  {"id", json()},
  {"error" , { {"code", -1},
    {"message", "Missing or invalid JSON-RPC version -- must be version 2.0!"}}
  }
};

const json initServerRequest = {
  {"jsonrpc", "2.0"},
  {"id", json()},
  {"method", "initialize"},
  {"params", {
    {"protocolVersion", "2024-11-05"},
    {"clientInfo", {
      {"name", "MCPHostApplication"},
      {"version", "0.0.0"}
      }}
    }}
};

const json expectedInitServerResponse = {
  {"id" , json()},
  {"jsonrpc", "2.0"},
  {"result", {
    {"capabilities", {
      { "logging" , json::object() },
      { "prompts" , json::object() },
      { "resources", json::object() },
      { "tools", json::object() } }
    },
    {"protocolVersion", "2024-11-05"},
    {"serverInfo",
      { {"name", "SysMLv2MCPServer4Testing"},
      {"version", "0.0.0"} }
    }}
}};

const json callEchoToolRequest = {
  {"jsonrpc", "2.0"},
  {"id", json()},
  {"method", "tools/call"},
  {"params", {
    {"name", "echo"},
    {"arguments", {
      {"message", "The big brown fox jumps over the lazy dog."}
      }}
    }}
};