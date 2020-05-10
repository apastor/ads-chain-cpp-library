#include <iostream>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include "Poco/Logger.h"

#include "adschain/network/PublicKeyDao.h"

using Poco::Logger;

namespace adschain {

class PublicKeyDaoImpl : public PublicKeyDao {
 private:
  SSL_CTX *ctx;
  Logger &logger = Logger::get("AdauditingLogger");
 public:
  INJECT(PublicKeyDaoImpl(
             const char *certsDir)) : ctx(SSL_CTX_new(TLS_method())) {
    if (SSL_CTX_load_verify_locations(ctx, NULL, certsDir) != 1) {
      if (ctx) SSL_CTX_free(ctx);
      throw std::runtime_error("Error creating SSL context: " + std::string(std::strerror(errno)));
    }
  }

  virtual ~PublicKeyDaoImpl() {
    if (ctx) SSL_CTX_free(ctx);
  }

  std::shared_ptr<PublicKey> getDomainPublicKey(const std::string &domain) {
    SSL *ssl;
    BIO *bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);

    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    if (BIO_set_conn_hostname(bio, domain.c_str()) != 1) {
      logger.information("[openssl] Can't set domain name");
      ERR_print_errors_fp(stderr);
    }
    if (BIO_set_conn_port(bio, "https") != 1) {
      logger.information("[openssl] Can't set port to https");
      ERR_print_errors_fp(stderr);
    }

    if (BIO_do_handshake(bio) <= 0) {
      logger.information("[openssl] Error establishing SSL connection to %s", domain);
      ERR_print_errors_fp(stderr);
    }

    X509 *cert = SSL_get_peer_certificate(ssl);
    EVP_PKEY *pKey = NULL;

    if (cert && SSL_get_verify_result(ssl) == X509_V_OK) {
      logger.information("[openssl] retrieving certificate from %s", domain);
      pKey = X509_get_pubkey(cert);
      logger.information("[openssl] certificate obtained");
    } else {
      logger.error("[openssl] %s SSL certificate not valid", domain);
    }
    BIO_free_all(bio);
    X509_free(cert);
    std::shared_ptr<PublicKey> publicKey = std::make_shared<PublicKey>(pKey);
    if (pKey)
      EVP_PKEY_free(pKey);
    return publicKey;
  }
};

fruit::Component<PublicKeyDao> getPublicKeyDaoComponent(const char *certsDir) {
  return fruit::createComponent()
      .bind<PublicKeyDao, PublicKeyDaoImpl>()
      .bindInstance(*certsDir)
      .registerProvider([](const char *certsDir) { return new PublicKeyDaoImpl(certsDir); });
}

}  // namespace adschain
