#pragma once

#include <string>

#include <openssl/evp.h>

#include "adschain/crypto/PrivateKey.h"

namespace adschain {

class Signer {
 public:
  virtual std::string signB64(std::string str) = 0;
};

fruit::Component<PrivateKey, Signer> getSignerComponent();
fruit::Component<PrivateKey, Signer> getSignerComponentFromFile(const char *filePath);

}  // namespace adschain
