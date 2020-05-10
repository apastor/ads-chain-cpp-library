#include <catch2/catch.hpp>

#include <chrono>
#include <unistd.h>

#include "adschain/tools/Stopwatch.h"

TEST_CASE("AutoStopwatch tests", "[unit]") {
  TimeLogger timeLogger("stopwatch-test-log.txt");

  SECTION("AutoStopwatch objects have internal timestamp") {
    AutoStopwatch sw(timeLogger.getLogger());
    sleep(2);
    CHECK(sw.getStart() < std::chrono::steady_clock::now());
  }

  SECTION("AutoStopwatch appends to the same logger") {
    AutoStopwatch sw2(timeLogger.getLogger());
    sleep(1);
    CHECK(sw2.getStart() < std::chrono::steady_clock::now());
  }

  SECTION("AutoStopwatch sets signature protocol state and number of ads") {
    AutoStopwatch sw3(timeLogger.getLogger());
    sw3.setNumberOfAds(4);
    sw3.setSignatureState(true);
    sw3.setTestID("4,100");
    sleep(1);
    CHECK(sw3.getStart() < std::chrono::steady_clock::now());
  }
}

TEST_CASE("Stopwatch tests", "[unit]") {
  TimeLogger timeLogger("stopwatch-test-log.txt");

  SECTION("Stopwatch objects have internal timestamp") {
    Stopwatch sw(timeLogger.getLogger());
    sleep(2);
    CHECK(sw.getStart() < std::chrono::steady_clock::now());
  }

  SECTION("Stopwatch appends to the same logger") {
    Stopwatch sw2(timeLogger.getLogger());
    sleep(1);
    CHECK(sw2.getStart() < std::chrono::steady_clock::now());
  }

  SECTION("AutoStopwatch sets signature protocol state and number of ads") {
    Stopwatch sw3(timeLogger.getLogger());
    sw3.stop("ssp-test");
    sleep(1);
    sw3.stop("ssp-test-after-sleep");
    CHECK(sw3.getStart() < std::chrono::steady_clock::now());
  }
}