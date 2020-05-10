#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/throw_exception.hpp>
#include <chrono>
#include <netinet/in.h>

typedef std::chrono::duration<long int, std::ratio<1l, 10000000l>> hundredns;
namespace boost {
namespace uuids {

class unixtime_generator {
  uint16_t clk_seq;
  uint64_t nodeid;
  std::chrono::nanoseconds boot_epoch;
  uint64_t last_tstamp = 0;
  bool not_first;

 public:
  explicit unixtime_generator(uint16_t _clk_seq, uint64_t _nodeid) :
      clk_seq(_clk_seq),
      nodeid(_nodeid & 0x0000FFFFFFFFFFFF),
      not_first(false) { //TODO exception if nodeid MSB is not zero
    auto sys_time = std::chrono::system_clock::now();
    auto wall_time = std::chrono::steady_clock::now();
    boot_epoch = sys_time.time_since_epoch() - wall_time.time_since_epoch();
  };
  uuid generate() {
    uuid u;
    uint64_t tstamp;
    do {
      tstamp = std::chrono::duration_cast<hundredns>(
          std::chrono::steady_clock::now().time_since_epoch() + boot_epoch).count();
    } while (tstamp == last_tstamp);
    last_tstamp = tstamp;
    uint32_t subsvar = htonl((uint32_t) tstamp);
    tstamp >>= 32;
    int i = 0;
    for (uuid::iterator it_byte = u.begin(); it_byte != u.end(); ++it_byte, ++i) {
      switch (i) {
        case 4:subsvar = htons((uint16_t) tstamp);
          tstamp >>= 16;
          break;
        case 6:subsvar = htons(tstamp);
          subsvar &= 0xFF0F;
          subsvar |= 0x0010;
          break;
        case 8:
          if (not_first) {
            return u;
          }
          subsvar = htons(clk_seq);
          subsvar &= 0xFF3F;
          subsvar |= 0x0080;
          break;
        case 10:subsvar = htonl(nodeid >> 32) >> 16;
          break;
        case 12:subsvar = htonl(nodeid);
          break;
      }

      *it_byte = subsvar;
      subsvar >>= 8;
    }
    not_first = true;
    return u;
  }
};

}
} // namespace boost::uuids

