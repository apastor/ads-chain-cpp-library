#include <iostream>

#include <Poco/SharedPtr.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>

#include "adschain/network/HttpRequests.h"
#include "adschain/Logger.h"

using namespace Poco::Net;

namespace adschain {

class SSLInitializer {
 public:
  SSLInitializer() { Poco::Net::initializeSSL(); }

  ~SSLInitializer() { Poco::Net::uninitializeSSL(); }
};

std::string HttpPostJsonRequest(const std::string &url, const std::string &body, long msTimeout) {
  SSLInitializer sslInitializer;
  Poco::SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false);
  Context::Ptr ptrContext = new Context(Context::CLIENT_USE,
                                        "",
                                        "",
                                        "/etc/ssl/certs/rootCA.pem",
                                        Context::VERIFY_STRICT,
                                        9,
                                        false,
                                        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  SSLManager::instance().initializeClient(nullptr, ptrCert, ptrContext);

  Poco::URI uri(url);
  HTTPSClientSession session(uri.getHost(), uri.getPort(), ptrContext);
  session.setTimeout(Poco::Timespan(msTimeout * 1000));
  std::string path(uri.getPathAndQuery());
  if (path.empty()) {
    path = "/";
  }
  HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
  req.setContentType("application/json");
  req.setContentLength(body.length());
  std::ostream &os = session.sendRequest(req);
  os << body;
  if (logger.debug()) {
    req.write(logStream.debug());
  }

  HTTPResponse response;
//  std::cout << response.getStatus() << " " << response.getReason() << std::endl;
  std::string output;
  if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
    std::istream &is = session.receiveResponse(response);
    Poco::StreamCopier::copyToString(is, output);
  }
  return output;
}

std::string HttpGetRequest(const std::string &url, const std::string &queryString, long msTimeout) {
  SSLInitializer sslInitializer;
  Poco::SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false);
  Context::Ptr ptrContext = new Context(Context::CLIENT_USE,
                                        "",
                                        "",
                                        "/etc/ssl/certs/rootCA.pem",
                                        Context::VERIFY_STRICT,
                                        9,
                                        false,
                                        "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
  SSLManager::instance().initializeClient(nullptr, ptrCert, ptrContext);

  Poco::URI uri(url);
  HTTPSClientSession session(uri.getHost(), uri.getPort(), ptrContext);
  //session.setTimeout(Poco::Timespan(msTimeout * 90000));
  std::string path(uri.getPath());
  if (path.empty()) {
    path = "/";
  }
  if (!queryString.empty()) {
    path.append("?");
    path.append(queryString);
  }
  HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
//  std::cout << "***DEBUG HTTPGET*** created request to "<< uri.getHost() <<":" << uri.getPort() << std::endl;
  session.sendRequest(req);
//  std::cout << "***DEBUG HTTPGET*** request sent" << std::endl;

  HTTPResponse response;
//  std::cout << response.getStatus() << " " << response.getReason() << std::endl;
  std::string output;
  if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
    std::istream &is = session.receiveResponse(response);
    Poco::StreamCopier::copyToString(is, output);
  }
  return output;
}

}  // namespace adschain


