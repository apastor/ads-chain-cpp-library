#pragma once

#include <iostream>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

#include <fruit/fruit.h>

#include "adschain/crypto/PublicKey.h"

namespace adschain {

class PrivateKey {
  typedef EVP_PKEY *pkey_ptr;
 protected:
  pkey_ptr pKeyPtr{};
 public:
  PrivateKey() = default;
  PrivateKey(const PrivateKey &old_obj) : pKeyPtr(old_obj) { EVP_PKEY_up_ref(pKeyPtr); }
  explicit PrivateKey(EVP_PKEY *pKeyPtr) : pKeyPtr(pKeyPtr) { EVP_PKEY_up_ref(pKeyPtr); }
  virtual ~PrivateKey() {
    if (pKeyPtr) {
      EVP_PKEY_free(pKeyPtr);
      pKeyPtr = nullptr;
    }
  }
  operator pkey_ptr() const { return pKeyPtr; }
  PublicKey getPublicKey();
};

fruit::Component<PrivateKey> getPrivateKeyComponent(const char *filePath);
fruit::Component<PrivateKey> getPrivateKeyPrime256v1Component();

}  // namespace adschain
