#pragma once

#include <string>

namespace adschain {

const int HTTP_DEFAULT_TIMEOUT = 12000;

std::string HttpPostJsonRequest(const std::string &url, const std::string &body, long msTimeout = HTTP_DEFAULT_TIMEOUT);
std::string HttpGetRequest(const std::string &url,
                           const std::string &queryString = "",
                           long msTimeout = HTTP_DEFAULT_TIMEOUT);

}
