#include <stdexcept>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/pem.h>

#include "FileWrapper.h"
#include "adschain/crypto/PrivateKey.h"

namespace adschain {

class PrivateKeyImpl : public PrivateKey {
 public:
  INJECT(PrivateKeyImpl(FileWrapper * fileWrapper)) : PrivateKeyImpl(*fileWrapper) {}
  explicit PrivateKeyImpl(FILE *file) {
    pKeyPtr = EVP_PKEY_new();
    if (!PEM_read_PrivateKey(file, &pKeyPtr, 0, NULL)) {
      if (pKeyPtr) {
        EVP_PKEY_free(pKeyPtr);
        pKeyPtr = nullptr;
      }
      throw std::runtime_error("Error creating private key from file: " + std::string(std::strerror(errno)));
    }
  }
};

fruit::Component<PrivateKey> getPrivateKeyComponent(const char *filePath) {
  return fruit::createComponent()
      .bind<PrivateKey, PrivateKeyImpl>()
      .bindInstance(*filePath)
      .registerProvider([](const char *filePath) { return new FileWrapper(filePath, "r"); });
}

}  // namespace adschain