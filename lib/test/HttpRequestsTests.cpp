#include <catch2/catch.hpp>

#include <iostream>

#include "adschain/network/HttpRequests.h"

using namespace adschain;

TEST_CASE("Https get request tests", "[network]") {

  SECTION("retrieve the publisher website") {
    try {
      std::string publisherResponse = HttpGetRequest("https://dsp.com/bid", "?hello-there");
//      std::cout << publisherResponse << std::endl;
    } catch (std::exception &e) {
      std::cerr << " an exception was caught, with message '" << e.what() << "'\n";
    }
//    REQUIRE(! publisherResponse.empty());
  }
}

TEST_CASE("Https post request tests", "[network]") {

  SECTION("retrieve the publisher website") {
    try {
      std::string jsonString =
          R"({"ad-size":"300x250","ad-uuid":"c5645019-fc2e-1037-a035-ecad1a7f0000","user-ip":"163.117.140.170","ad-custody":{"0":"publisher.com","1":"ssp.com","2":"adx.com","3":"AUCTION"},"custody-index":"2","sign":{"0":"MEQCIDI0/6PibDhqZ5aVOYj0+p7npZBp9/+yiWQNm+tsvqZNAiB1+RqNE3TYqz9cnSIaSQKA7jZjONgO6qmhLwQNxDfISA==","1":"MEUCIBIsUy3pihpqm13hwZY3IZEW8gWdOHaalFnK9LOG9jg6AiEAqjfGuIbBEHg3BMsXAILXw5WvyjgL17pYXMA8EuMcfNc=","2":"MEQCID/mzLhKz9lUPLcuMp/mHQTMYLcgSyCovB+M/+pJ1f1rAiAXAVRjuOKAswjejCFK+nJ4ZPRTptydvzAIVhteqAzk6w=="},"sign-string":{"0":"ad-custody[0];ad-custody[1];ad-uuid;user-ip;ad-size","1":"sign[0];ad-custody[2]","2":"sign[1];ad-custody[3]"}})";
      std::string publisherResponse = HttpPostJsonRequest("https://dsp.com/bid", jsonString);
//      std::cout << publisherResponse << std::endl;
    } catch (std::exception &e) {
      std::cerr << " an exception was caught, with message '" << e.what() << "'\n";
    }
//    REQUIRE(! publisherResponse.empty());
  }
}