#pragma once

#include <string>

#include <openssl/evp.h>

#include "adschain/crypto/PrivateKey.h"

namespace adschain {

class Verifier {
 public:
  virtual bool verifyB64(const PublicKey *publicKey, std::string &signatureB64, std::string message) = 0;
};

fruit::Component<Verifier> getVerifierComponent();

}  // namespace adschain
