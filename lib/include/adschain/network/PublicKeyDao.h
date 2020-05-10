#pragma once

#include <fruit/component.h>

#include "adschain/crypto/PublicKey.h"

namespace adschain {

class PublicKeyDao {
 public:
  virtual std::shared_ptr<PublicKey> getDomainPublicKey(const std::string &domain) = 0;
};

fruit::Component<PublicKeyDao> getPublicKeyDaoComponent(const char *certsDir);

}  // namespace adschain
