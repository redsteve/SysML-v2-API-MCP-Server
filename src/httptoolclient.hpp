#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <map>
#include <memory>
#include <string>

using json = nlohmann::json;
using Headers = std::map<std::string, std::string>;

/// @brief HTTP client for external tool calls
class HttpToolClient final {
public:
    /// @brief This method requests a representation of the specified resource (HTTP GET).
    /// @param url identifies the target resource of the request.
    /// @param headers 
    /// @return The response from the resource.
    json httpGet(const std::string& url, const Headers& headers = {});

    /// @brief This method sends data to the resource/server (HTTP POST).
    /// @param url identifies the target resource of the request.
    /// @param body 
    /// @param headers 
    /// @return The response from the resource.
    json httpPost(const std::string& url, const json& body, const Headers& headers = {});

    /// @brief This method creates a new resource or replaces a representation of the target
    /// resource with the request (HTTP PUT).
    /// @param url identifies the target resource of the request.
    /// @param body 
    /// @param headers 
    /// @return The response from the resource.
    json httpPut(const std::string& url, const json& body, const Headers& headers = {});

    /// @brief This method asks the server to delete a specified resource (HTTP DELETE).
    /// @param url identifies the target resource of the request.
    /// @param headers 
    /// @return The response from the server.
    json httpDelete(const std::string& url, const Headers& headers = {});

    ~HttpToolClient();

private:
  json performHttpRequest(const std::string& method, const std::string& url,
                          const std::string& body, const Headers& headers);
  std::string extractBaseUrl(const std::string& fullUrl) const;
  std::string extractPathFromUrl(const std::string& fullUrl) const;
  httplib::Client *retrieveClient(const std::string& baseUrl);
  void tryToParseResultAsJson(const httplib::Result& result, json& response) const;

  std::map<std::string, std::unique_ptr<httplib::Client>> clients_;
  int timeoutInSeconds_{30};

  static const char* const JSON_MIME_TYPE;
};