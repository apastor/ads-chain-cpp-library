#pragma once

#include <stdexcept>
#include <iostream>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include <fruit/fruit.h>

namespace adschain {

class PublicKey {
  typedef EVP_PKEY *pkey_ptr;
 protected:
  pkey_ptr pKeyPtr = nullptr;
 public:
  PublicKey(const PublicKey &old_obj) : pKeyPtr(old_obj) { EVP_PKEY_up_ref(pKeyPtr); }
  explicit PublicKey(EVP_PKEY *pKeyPtr) : pKeyPtr(pKeyPtr) {
    if (pKeyPtr) EVP_PKEY_up_ref(pKeyPtr);
    else pKeyPtr = EVP_PKEY_new();
  }

  friend std::ostream &operator<<(std::ostream &outs, const PublicKey &pubKey) {
    BIO *bmem;
    bmem = BIO_new(BIO_s_mem());
    if (PEM_write_bio_PUBKEY(bmem, pubKey) == 0) {
      BIO_free(bmem);
      return outs << "NULL";
    }

    char *buff;
    int length = BIO_get_mem_data(bmem, &buff);
    char pemKey[length + 1];
    strncpy(pemKey, buff, length);
    pemKey[length] = '\0';
    BIO_free_all(bmem);
//    free(buff);
    return outs << pemKey;
  }

  virtual ~PublicKey() {
    if (pKeyPtr)
      EVP_PKEY_free(pKeyPtr);
  }

  operator pkey_ptr() const { return pKeyPtr; }
};

}  // namespace adschain
