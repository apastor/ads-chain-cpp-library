#include "adschain/crypto/Verifier.h"

namespace adschain {

class VerifierImpl : public Verifier {
 public:
  INJECT(VerifierImpl()) {}
  bool verifyB64(const PublicKey *publicKey, std::string &signatureB64, std::string message) override {
    bool validSignature;
    int binarySignLen = calcDecodeLength(signatureB64);
    unsigned char binarySignature[binarySignLen];
    BIO *bioB64 = BIO_new(BIO_f_base64());
    BIO_set_flags(bioB64, BIO_FLAGS_BASE64_NO_NL);
    BIO *bioMemBuffer = BIO_new_mem_buf(signatureB64.c_str(), signatureB64.length());
    BIO_push(bioB64, bioMemBuffer);

    int length = BIO_read(bioB64, &binarySignature, binarySignLen);
    assert(length == binarySignLen);
    EVP_MD_CTX *mdctx = NULL;
    mdctx = EVP_MD_CTX_create();
    if (!(mdctx)) {
      BIO_free_all(bioB64);
      throw std::runtime_error("EVP_MD_CTX initialization error");
    }

    if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, *publicKey) != 1) {
      BIO_free_all(bioB64);
      EVP_MD_CTX_destroy(mdctx);
      throw std::runtime_error("Error calling EVP_DigestVerifyInit");
    }

    validSignature = EVP_DigestVerify(mdctx, binarySignature, binarySignLen,
                                      reinterpret_cast<const unsigned char *>(message.c_str()), message.length()) == 1;
    BIO_free_all(bioB64);
    EVP_MD_CTX_destroy(mdctx);

    return validSignature;
  }
 private:
  static int calcDecodeLength(std::string b64input) {
    //TODO: implement with padding errors checking
    int len = b64input.length();
    int padding = 0;
    if (b64input[len - 1] == '=' && b64input[len - 2] == '=') //last two chars are =
      padding = 2;
    else if (b64input[len - 1] == '=') //last char is =
      padding = 1;
    return (len * 3) / 4 - padding;
  }
};

fruit::Component<Verifier> getVerifierComponent() {
  return fruit::createComponent()
      .bind<Verifier, VerifierImpl>();
}

}  // namespace adschain