#include "adschain/Logger.h"

namespace adschain {

Poco::Logger &logger = Poco::Logger::get("AdauditingLogger");
Poco::LogStream logStream(logger);

}