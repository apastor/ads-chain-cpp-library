#pragma once

#include <fruit/component.h>

#include "adschain/crypto/PublicKey.h"
#include "adschain/network/PublicKeyDao.h"

namespace adschain {

class PublicKeyService {
 public:
  virtual std::shared_ptr<PublicKey> getDomainPublicKey(const std::string &domain) = 0;
};

fruit::Component<PublicKeyService> getPublicKeyServiceComponent(const char *certsDir);

}  // namespace adschain
