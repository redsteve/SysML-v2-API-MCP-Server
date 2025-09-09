#include "httpmcptransport.hpp"

using namespace std;

void HttpMcpTransport::start([[maybe_unused]] function<json(const json&)> requestHandler) {

}

void HttpMcpTransport::stop() {

}

bool HttpMcpTransport::isRunning() const noexcept {
  return false;
}

HttpMcpTransport::~HttpMcpTransport() {

}