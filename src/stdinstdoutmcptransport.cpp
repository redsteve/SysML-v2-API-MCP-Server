#include "stdinstdoutmcptransport.hpp"
#include <iostream>

using namespace std;

void StdinStdoutMcpTransport::start(function<json(const json&)> requestHandler) {
  if (running_)
    return;
  
  running_ = true;
  thread_ = thread([this, requestHandler]() {
    string line;
    while (running_ && std::getline(std::cin, line)) {
      try {
        if (line.empty()) continue;
        
        json request = json::parse(line);
        json response = requestHandler(request);
        
        cout << response.dump() << endl;
        cout.flush();
      } catch (const exception& ex) {
        const json errorResponse = {
          {"jsonrpc", "2.0"},
          {"id", nullptr},
          {"error", {
            {"code", -32700},
            {"message", "Error while parsing received JSON text: " + std::string(ex.what())}
          }}
        };
        std::cout << errorResponse.dump() << std::endl;
      }
    }
  });
}

void StdinStdoutMcpTransport::stop() {
  running_ = false;
  if (thread_.joinable()) {
    thread_.join();
  }
}

bool StdinStdoutMcpTransport::isRunning() const noexcept {
  return running_;
}

StdinStdoutMcpTransport::~StdinStdoutMcpTransport() {
  stop();
}