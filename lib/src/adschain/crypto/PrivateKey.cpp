#include "adschain/crypto/PrivateKey.h"

namespace adschain {

PublicKey PrivateKey::getPublicKey() {
  BIO *memBio = BIO_new(BIO_s_mem());
  if (!pKeyPtr) {
    throw std::runtime_error("private key EVP_PKEY* is null");
  }
  if (i2d_PUBKEY_bio(memBio, pKeyPtr) != 1) {
    BIO_free(memBio);
    throw std::runtime_error("Error writing public Key to bio");
  }
  EVP_PKEY *_pkey = d2i_PUBKEY_bio(memBio, NULL);
  if (!_pkey) {
    BIO_free(memBio);
    throw std::runtime_error("Error writing public Key to new EVP_PKEY");
  }

  PublicKey publicKey(_pkey);

  BIO_free(memBio);
  EVP_PKEY_free(_pkey);

  return publicKey;
}

}  // namespace adschain