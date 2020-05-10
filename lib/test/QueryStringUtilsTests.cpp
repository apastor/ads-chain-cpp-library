#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "adschain/network/QueryStringUtils.h"

using namespace adschain;

TEST_CASE("URLs query string can be parsed to rapidjson Objects", "[unit]") {
  SECTION("query string with dictionary") {
    std::string queryString("param1&param2=hello&param3[0]=value0&param3[1]=value1&index=1");
    std::unique_ptr<rapidjson::Document> documentPtr = QueryString2RapidjsonDocument(queryString);
    const rapidjson::Document &document = *documentPtr;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
//    std::cout << buffer.GetString() << std::endl;
    REQUIRE(document["param1"].GetBool());
    REQUIRE(document["param2"] == "hello");
    REQUIRE(document["param3"]["0"] == "value0");
    REQUIRE(document["param3"]["1"] == "value1");
//    std::cout << std::to_string(document["index"].GetString()[0]-'1') << std::endl;
    REQUIRE(document["param3"][std::to_string(document["index"].GetString()[0] - '1').c_str()] == "value0");
  }
  SECTION("query string with array") {
    std::string queryString("param1&param2=hello&param3[]=value0&param3[]=value1");
    std::unique_ptr<rapidjson::Document> documentPtr = QueryString2RapidjsonDocument(queryString);
    const rapidjson::Document &document = *documentPtr;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
//    std::cout << buffer.GetString() << std::endl;
    REQUIRE(document["param1"].GetBool());
    REQUIRE(document["param2"] == "hello");
    REQUIRE(document["param3"][0] == "value0");
    REQUIRE(document["param3"][1] == "value1");
  }
}

TEST_CASE("rapidjson Documents can be codified to URL query strings", "[unit]") {
  SECTION("query string with dictionary") {
    std::string inputQueryString("param1&param2=hello&param3[0]=value0&param3[1]=value1");
    std::unique_ptr<rapidjson::Document> documentPtr = QueryString2RapidjsonDocument(inputQueryString);
    std::string outputQueryString = RapidJsonDocument2QueryString(*documentPtr);
//    std::cout << outputQueryString << std::endl;
    REQUIRE(inputQueryString == outputQueryString);
  }
  SECTION("query string with array") {
    std::string inputQueryString("param1&param2=hello&param3[]=value0&param3[]=value1");
    std::unique_ptr<rapidjson::Document> documentPtr = QueryString2RapidjsonDocument(inputQueryString);
    std::string outputQueryString = RapidJsonDocument2QueryString(*documentPtr);
//    std::cout << outputQueryString << std::endl;
    REQUIRE(inputQueryString == outputQueryString);
  }
}

TEST_CASE("Extract signature string at the given level", "[unit]") {
  SECTION("signature extraction") {
    std::stringstream ad_tag, msgStr;
    auto client_ip = "a.b.c.d";
    auto custody0 = "example.com";
    auto custody1 = "ssp.com";
    msgStr << client_ip << ";" << custody0 << ";" << custody1;
    auto signString = "user-ip;custody[0];custody[1]";

    ad_tag << "user-ip=" << client_ip
           << "&custody[0]=" << custody0
           << "&custody[1]=" << custody1
           << "&custody-index=" << "1"
           << "&keys-string[0]=" << signString;

    std::unique_ptr<rapidjson::Document> documentPtr = QueryString2RapidjsonDocument(ad_tag.str());
    const rapidjson::Document &adInfo = *documentPtr;
    std::string prevCustodyIndex = std::to_string(adInfo["custody-index"].GetString()[0] - '1');
    REQUIRE(ad_tag.str() == RapidJsonDocument2QueryString(adInfo));
    std::string expectedMsgStr = GetSignatureAtIndex(adInfo, prevCustodyIndex);
    REQUIRE(expectedMsgStr == msgStr.str());
  }
}
