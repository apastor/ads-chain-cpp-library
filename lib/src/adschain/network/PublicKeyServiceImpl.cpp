#include <openssl/ssl.h>

#include "Poco/ExpireLRUCache.h"
#include "Poco/Timespan.h"

#include "adschain/network/PublicKeyService.h"
#include "adschain/network/PublicKeyDao.h"

namespace adschain {

class PublicKeyServiceImpl : public PublicKeyService {
 private:
  PublicKeyDao *publicKeyDao;
  std::unique_ptr<Poco::ExpireLRUCache < std::string, std::shared_ptr<PublicKey>>>
  cache =
  std::make_unique<Poco::ExpireLRUCache < std::string, std::shared_ptr<PublicKey>> > (100, Poco::Timespan::DAYS / 1000);
 public:
  INJECT(PublicKeyServiceImpl(PublicKeyDao * publicKeyDao)) : publicKeyDao(publicKeyDao) {}
  std::shared_ptr<PublicKey> getDomainPublicKey(const std::string &domain) override {
    std::shared_ptr<PublicKey> pubKeyPtr(nullptr);
    Poco::SharedPtr<std::shared_ptr<PublicKey>> cachedKeyPtrPtr = cache->get(domain);
    if (cachedKeyPtrPtr.isNull()) {
      pubKeyPtr = publicKeyDao->getDomainPublicKey(domain);
      cache->add(domain, pubKeyPtr);
    } else {
      pubKeyPtr = *cachedKeyPtrPtr;
    }
    return pubKeyPtr;
  }
};

fruit::Component<PublicKeyService> getPublicKeyServiceComponent(const char *certsDir) {
  return fruit::createComponent()
      .bind<PublicKeyService, PublicKeyServiceImpl>()
      .install(getPublicKeyDaoComponent, certsDir);
}

}  // namespace adschain
