#include "httptoolclient.hpp"
#include <spdlog/spdlog.h>
#include <regex>

using namespace std;

json HttpToolClient::httpGet(const string& url, const Headers& headers) {
  return performHttpRequest("GET", url, "", headers);
}

json HttpToolClient::httpPost(const string& url, const json& body, const Headers& headers) {
  const std::string jsonBody = body.dump();
  auto modifiedHeaders = headers;
  modifiedHeaders["Content-Type"] = JSON_MIME_TYPE;
  
  return performHttpRequest("POST", url, jsonBody, modifiedHeaders);
}

json HttpToolClient::httpPut(const string& url, const json& body, const Headers& headers) {
  const std::string jsonBody = body.dump();
  auto modifiedHeaders = headers;
  modifiedHeaders["Content-Type"] = JSON_MIME_TYPE;
  
  return performHttpRequest("PUT", url, jsonBody, modifiedHeaders);
}

json HttpToolClient::httpDelete(const string& url, const Headers& headers) {
  return performHttpRequest("DELETE", url, "", headers);
}

json HttpToolClient::performHttpRequest(const string& method, const string& url,
  const string& body, const Headers& headers) {
  spdlog::trace("Try to perform HTTP request with method='{0}', url='{1}', body='{2}'.",
    method, url, body);
  try {
    const string baseUrl = extractBaseUrl(url);
    if (baseUrl.empty()) {
      throw std::runtime_error("Invalid URL format: " + url);
    }
    const string path = extractPathFromUrl(url);
    
    httplib::Client* client = retrieveClient(baseUrl);
    
    httplib::Headers httpHeaders;
    for (const auto& [key, value] : headers) {
      httpHeaders.emplace(key, value);
    }

    httplib::Result result;
            
    if (method == "GET") {
      result = client->Get(path, httpHeaders);
    } else if (method == "POST") {
      result = client->Post(path, httpHeaders, body, JSON_MIME_TYPE);
    } else if (method == "PUT") {
      result = client->Put(path, httpHeaders, body, JSON_MIME_TYPE);
    } else if (method == "DELETE") {
      result = client->Delete(path, httpHeaders);
    } else {
      throw std::runtime_error("Unsupported HTTP method: " + method);
    }

    if (! result) {
      throw std::runtime_error("HTTP request failed: " + httplib::to_string(result.error()));
    }

    json response = {
      {"status", result->status},
      {"headers", json::object()},
      {"body", result->body}
    };

    for (const auto& [key, value] : result->headers) {
      response["headers"][key] = value;
    }

    tryToParseResultAsJson(result, response);
    spdlog::trace("HTTP request successful. Response was: '{}'.", response.dump());
    return response;

  } catch (const std::exception& ex) {
    spdlog::error("HTTP request failed! Reason: {}", ex.what());
    return {
      {"error", true},
      {"message", ex.what()},
      {"status", -1}
    };
  }
}

string HttpToolClient::extractBaseUrl(const string& fullUrl) const {
  const regex url_regex(R"((https?://[^/]+))");
  smatch match;
  if (regex_search(fullUrl, match, url_regex) && match.size() > 1) {
    return match.str(1);
  }
  return "";
}

string HttpToolClient::extractPathFromUrl(const string& fullUrl) const {
  const regex path_regex(R"(https?://[^/]+(.*))");
  smatch match;
  if (regex_match(fullUrl, match, path_regex) && match.size() > 1) {
    return match.str(1).empty() ? "/" : match.str(1);
  }
  return "/";
}

httplib::Client* HttpToolClient::retrieveClient(const std::string& baseUrl) {
  if (httpClients_.find(baseUrl) == httpClients_.end()) {
    httpClients_[baseUrl] = std::make_unique<httplib::Client>(baseUrl);
    httpClients_[baseUrl]->set_read_timeout(timeoutInSeconds_);
    httpClients_[baseUrl]->set_write_timeout(timeoutInSeconds_);
    httpClients_[baseUrl]->set_connection_timeout(timeoutInSeconds_);
    spdlog::info("New HTTP client created for server at URL {}.", baseUrl);
  }
  return httpClients_[baseUrl].get();
}

void HttpToolClient::tryToParseResultAsJson(const httplib::Result& result, json& response) const {
  if (! result->body.empty()) {
    try {
      const json parsedBody = json::parse(result->body);
      response["json"] = parsedBody;
    } catch (...) {
      // result->body is not valid JSON: Do nothing and leave the response untouched.
    }
  }
}

const char* const HttpToolClient::JSON_MIME_TYPE = "application/json";