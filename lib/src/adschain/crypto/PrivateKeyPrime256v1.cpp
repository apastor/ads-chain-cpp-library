#include <iostream>

#include <openssl/evp.h>
#include <openssl/ec.h>

#include "adschain/crypto/PrivateKey.h"

namespace adschain {

class PrivateKeyPrime256v1 : public PrivateKey {
 public:
  INJECT(PrivateKeyPrime256v1()) {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (!key) {
      throw std::runtime_error("EC_KEY initialization error");
    }
    EC_KEY_set_asn1_flag(key, OPENSSL_EC_NAMED_CURVE);
    if (EC_KEY_generate_key(key) != 1) {
      EC_KEY_free(key);
      throw std::runtime_error("error generating key pair");
    }
    if (EC_KEY_check_key(key) != 1) {
      EC_KEY_free(key);
      throw std::runtime_error("error validating ec_key");
    }
    pKeyPtr = EVP_PKEY_new();
    if (EVP_PKEY_assign_EC_KEY(pKeyPtr, key) != 1) {
      EC_KEY_free(key);
      throw std::runtime_error("error assigning key to EVP_PKEY struct");
    }
  }
};

fruit::Component<PrivateKey> getPrivateKeyPrime256v1Component() {
  return fruit::createComponent()
      .bind<PrivateKey, PrivateKeyPrime256v1>();
}

}  // namespace adschain