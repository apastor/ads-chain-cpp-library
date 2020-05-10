#include <regex>
#include <cstdlib>

#include <boost/tokenizer.hpp>
#include <iostream>

#include "adschain/Logger.h"
#include "adschain/network/QueryStringUtils.h"

using rapidjson::Value;

namespace adschain {

std::unique_ptr<rapidjson::Document> QueryString2RapidjsonDocument(const std::string &string) {
  std::unique_ptr<rapidjson::Document> documentPtr(new rapidjson::Document);
  rapidjson::Document &document = *documentPtr;
  document.SetObject();
  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
  boost::char_separator<char> sep{"&"};
  boost::tokenizer<boost::char_separator<char>> tokenizer{string, sep};
  for (const auto &t : tokenizer) {
    const std::string &token(t);
    size_t pos = token.find('=');
    if (pos == std::string::npos) {
      document.AddMember(Value(token.c_str(), allocator).Move(), true, allocator);
    } else {
      std::string key = token.substr(0, pos);
      std::string value = token.substr(pos + 1);
      if (key.back() == ']') {
        size_t openingBrace = key.find_last_of('[', key.length() - 1);
        if (openingBrace > 0) {
          const char *arrayKey = key.substr(0, openingBrace).c_str();
          if (openingBrace == key.length() - 2) {
            if (!document.HasMember(arrayKey)) {
              document.AddMember(Value(arrayKey, allocator).Move(), Value(rapidjson::kArrayType).Move(), allocator);
            }
            rapidjson::Value &arrVal = document[arrayKey];
            if (arrVal.IsArray()) {
              arrVal.PushBack(Value(value.c_str(), allocator).Move(), allocator);
            } else {
              logger.warning("[adschain/network] trying to append value to non array element");
            }
          } else {
            if (!document.HasMember(arrayKey)) {
              document.AddMember(Value(arrayKey, allocator).Move(), Value(rapidjson::kObjectType).Move(), allocator);
            }
            rapidjson::Value &objVal = document[arrayKey];
            std::string arrayIndex = key.substr(openingBrace + 1, key.length() - openingBrace - 2);
            if (objVal.IsObject()) {
              objVal.AddMember(Value(arrayIndex, allocator).Move(), value, allocator);
            } else {
              logger.warning("[adschain/network] trying to insert value to non array element");
            }
          }
        } else {
          logger.warning("[adschain/network] no matching '[' found for query string element ending with ']'");
        }
      } else {
        document.AddMember(Value(key.c_str(), allocator).Move(), Value(value.c_str(), allocator).Move(), allocator);
      }
    }
  }
  return documentPtr;
}

std::string RapidJsonDocument2QueryString(const rapidjson::Document &document) {
  std::ostringstream queryStringStream;
  for (Value::ConstMemberIterator docItr = document.MemberBegin(); docItr != document.MemberEnd(); ++docItr) {
    if (docItr != document.MemberBegin()) {
      queryStringStream << "&";
    }
    if (docItr->value.IsBool()) {
      queryStringStream << docItr->name.GetString();
    } else if (docItr->value.IsString() || docItr->value.IsNumber()) {
      queryStringStream << docItr->name.GetString() << "=" << docItr->value.GetString();
    } else if (docItr->value.IsArray()) {
      for (Value::ConstValueIterator arrItr = docItr->value.Begin(); arrItr != docItr->value.End(); ++arrItr) {
        if (arrItr != docItr->value.Begin()) {
          queryStringStream << "&";
        }
        queryStringStream << docItr->name.GetString() << "[]=" << arrItr->GetString();
      }
    } else if (docItr->value.IsObject()) {
      for (Value::ConstMemberIterator objItr = docItr->value.MemberBegin(); objItr != docItr->value.MemberEnd();
           ++objItr) {
        if (objItr != docItr->value.MemberBegin()) {
          queryStringStream << "&";
        }
        queryStringStream << docItr->name.GetString() << "[" << objItr->name.GetString() << "]="
                          << objItr->value.GetString();
      }
    }
  }
  return queryStringStream.str();
}

std::string GetSignatureAtIndex(const rapidjson::Document &document, const std::string &level) {
  std::stringstream signatureValues;

  std::string signatureFieldsStr(document["keys-string"][level].GetString());
  boost::char_separator<char> sep{";"};
  boost::tokenizer<boost::char_separator<char>> tok{signatureFieldsStr, sep};
  for (boost::tokenizer<boost::char_separator<char>>::iterator it = tok.begin(); it != tok.end(); ++it) {
    if (it != tok.begin())
      signatureValues << ";";
    if (it->back() == ']') {
      size_t openingBrace = it->find_last_of('[');
      const char *arrayKey = it->substr(0, openingBrace).c_str();
      const char *arrayIndex = it->substr(openingBrace + 1, it->length() - openingBrace - 2).c_str();
      signatureValues << document[arrayKey][arrayIndex].GetString();
    } else {
      signatureValues << document[it->c_str()].GetString();
    }
  }
  return signatureValues.str();
}

}  // namespace adschain
