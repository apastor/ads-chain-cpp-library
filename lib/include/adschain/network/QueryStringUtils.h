#pragma once

#include <memory>

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace adschain {
std::unique_ptr<rapidjson::Document> QueryString2RapidjsonDocument(const std::string &string);
std::string RapidJsonDocument2QueryString(const rapidjson::Document &document);
std::string GetSignatureAtIndex(const rapidjson::Document &document, const std::string &level);

class AdInfoValue : public rapidjson::Value {

};

// // TODO: AdInfo class as an abstraction layer from rapidjson
//class AdInfo {
//  std::unique_ptr<rapidjson::Document> documentPtr;
//  rapidjson::Document &document = *documentPtr;
// public:
//  explicit AdInfo() : documentPtr(new rapidjson::Document), document(*documentPtr) {
//    document.SetObject();
//  }
//  ~AdInfo() { documentPtr.reset(); }
//
//  static std::unique_ptr<AdInfo> FromQueryString(const std::string &string);
//
//};

}  // namespace adschain
