#pragma once

#include <string>
#include <chrono>

#include "Poco/Logger.h"
#include "Poco/AsyncChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/AutoPtr.h"

using Poco::Logger;
using Poco::AsyncChannel;
using Poco::FileChannel;
using Poco::AutoPtr;

class AutoStopwatch {
 private:
  std::chrono::steady_clock::time_point mStart;
  Logger &mLogger;
  bool usingSignatures_ = false;
  int nAds_ = -1;
  std::string testID_ = "";

 public:
  AutoStopwatch(Logger &logger) : mLogger(logger) {
    mStart = std::chrono::steady_clock::now();
  }

  void setSignatureState(bool usingSignatures) {
    usingSignatures_ = usingSignatures;
  }

  void setNumberOfAds(int nAds) {
    nAds_ = nAds;
  }

  void setTestID(const char *testID) {
    testID_ = testID;
  }

  ~AutoStopwatch() {
    std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - mStart;
    mLogger.information("%s,%b,%i,%.2f", testID_, usingSignatures_, nAds_, elapsed.count());
  }

  std::chrono::steady_clock::time_point &getStart() {
    return mStart;
  }
};

class Stopwatch {
 private:
  std::chrono::steady_clock::time_point mStart;
  Logger &mLogger;

 public:
  Stopwatch(Logger &logger) : mLogger(logger) {
    mStart = std::chrono::steady_clock::now();
  }

  void stop(const char *testID) {
    std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - mStart;
    mLogger.information("%s,%.2f", std::string(testID), elapsed.count());
  }

  std::chrono::steady_clock::time_point &getStart() {
    return mStart;
  }
};

class TimeLogger {
 private:
  Logger &logger;

 public:
  TimeLogger() : TimeLogger("stopwatch.log") {}
  TimeLogger(const char *fileName) : logger(Logger::get("StopwatchFileLogger")) {
    AutoPtr<FileChannel> pChannel(new FileChannel);
    pChannel->setProperty("path", fileName);
    pChannel->setProperty("rotation", "10 M");
    pChannel->setProperty("archive", "timestamp");
    AutoPtr<AsyncChannel> pAsync(new AsyncChannel(pChannel));
    logger.setChannel(pAsync);
    logger.setLevel(Poco::Message::PRIO_INFORMATION);
  }

  Logger &getLogger() {
    return logger;
  }
};