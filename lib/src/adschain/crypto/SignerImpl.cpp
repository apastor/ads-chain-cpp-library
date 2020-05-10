#include "adschain/crypto/Signer.h"

namespace adschain {

class SignerImpl : public Signer {
 private:
  PrivateKey *privateKey;
 public:
  INJECT(SignerImpl(PrivateKey * privateKey)) : privateKey(privateKey) {}
  std::string signB64(std::string message) override {
//    std::cout << message << std::endl;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
    if (!(mdctx)) {
      throw std::runtime_error("EVP_MD_CTX initialization error");
    }
    if (EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, *privateKey) != 1) {
      EVP_MD_CTX_destroy(mdctx);
      throw std::runtime_error("Error calling EVP_DigestSignInit");
    }
    if (EVP_DigestSignUpdate(mdctx, message.c_str(), message.length()) != 1) {
      EVP_MD_CTX_destroy(mdctx);
      throw std::runtime_error("Error calling EVP_DigestSignUpdate");
    }

    size_t binarySignatureLength = 0;
    if (EVP_DigestSignFinal(mdctx, NULL, &binarySignatureLength) != 1) {
      EVP_MD_CTX_destroy(mdctx);
      throw std::runtime_error("Error calling EVP_DigestSignFinal to obtain the signature length");
    }
    unsigned char *binarySignature = NULL;
    binarySignature = (unsigned char *) OPENSSL_malloc(sizeof(unsigned char) * (binarySignatureLength));
    if (EVP_DigestSignFinal(mdctx, binarySignature, &binarySignatureLength) != 1) {
      EVP_MD_CTX_destroy(mdctx);
      if (binarySignature) OPENSSL_free(binarySignature);
      throw std::runtime_error("Error calling EVP_DigestSignFinal to sign the message");
    }
    EVP_MD_CTX_destroy(mdctx);

    BIO *bioB64 = BIO_new(BIO_f_base64());
    BIO_set_flags(bioB64, BIO_FLAGS_BASE64_NO_NL);
    BIO *bioMem = BIO_new(BIO_s_mem());
    BIO_push(bioB64, bioMem);
    BIO_write(bioB64, binarySignature, binarySignatureLength);
    BIO_flush(bioB64);
    BUF_MEM *buffer = NULL;
    BIO_get_mem_ptr(bioMem, &buffer);
    // char *signature = (char *) calloc(1+buffer->length, sizeof(char));
    std::string dom_sign(buffer->data, buffer->length);
    OPENSSL_free(binarySignature);
    BIO_free_all(bioB64);
    return dom_sign;
  }
};

fruit::Component<fruit::Required<PrivateKey>, Signer> getSignerImplComponent() {
  return fruit::createComponent()
      .bind<Signer, SignerImpl>();
}

fruit::Component<PrivateKey, Signer> getSignerComponent() {
  return fruit::createComponent()
      .install(getPrivateKeyPrime256v1Component)
      .install(getSignerImplComponent);
}

fruit::Component<PrivateKey, Signer> getSignerComponentFromFile(const char *filePath) {
  return fruit::createComponent()
      .install(getPrivateKeyComponent, filePath)
      .install(getSignerImplComponent);
}

}  // namespace adschain