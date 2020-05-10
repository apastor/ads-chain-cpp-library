#include <catch2/catch.hpp>

#include <iostream>

#include <openssl/evp.h>

#include "adschain/crypto/PrivateKey.h"
#include "TempPrivKeyFile.h"

using namespace adschain;

TEST_CASE("PrivateKeyPrime256v1 key class", "[unit]") {

  SECTION("can generate a new key pair") {
    fruit::Injector<PrivateKey> injector(getPrivateKeyPrime256v1Component);
    PrivateKey *privateKey(injector);
    CHECK(reinterpret_cast<EVP_PKEY *>(privateKey) != (EVP_PKEY *) NULL);
    CHECK(privateKey->getPublicKey() != nullptr);
  }

}

TEST_CASE_METHOD(TempPrivKeyFile, "PrivateKeyImpl key class", "[integration]") {
  fruit::Injector<PrivateKey> injector(getPrivateKeyComponent, fileName.c_str());
  PrivateKey *privateKey(injector);
  CHECK(reinterpret_cast<EVP_PKEY *>(privateKey) != (EVP_PKEY *) NULL);
  CHECK(privateKey->getPublicKey() != nullptr);
}
