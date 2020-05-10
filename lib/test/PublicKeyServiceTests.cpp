#include <catch2/catch.hpp>

#include <openssl/evp.h>

#include "gmock/gmock.h"

#include "adschain/network/PublicKeyService.h"
#include "adschain/network/PublicKeyDao.h"
#include "adschain/crypto/PublicKey.h"

using namespace adschain;

class MockPublicKeyDao : public PublicKeyDao {
 public:
  INJECT(MockPublicKeyDao()) = default;
  MOCK_METHOD1(getDomainPublicKey, std::shared_ptr<PublicKey>(
      const std::string &domain));
};

fruit::Component<PublicKeyDao> getMockPublicKeyDaoComponent() {
  return fruit::createComponent()
      .bind<PublicKeyDao, MockPublicKeyDao>();
}

fruit::Component<MockPublicKeyDao, PublicKeyService> getPublicKeyServiceComponentWithMockDao() {
  return fruit::createComponent()
      .replace(getPublicKeyDaoComponent).with(getMockPublicKeyDaoComponent)
      .install(getPublicKeyServiceComponent);
}

TEST_CASE("PublicKeyService tests", "[unit]") {
  fruit::Injector<MockPublicKeyDao, PublicKeyService> injector(getPublicKeyServiceComponentWithMockDao);
  PublicKeyService *publicKeyService(injector);
  MockPublicKeyDao *mockPublicKeyDao(injector);

  SECTION("retrieves the key if the domain is not cached") {
    EXPECT_CALL(*mockPublicKeyDao, getDomainPublicKey("domain.com"))
        .WillOnce(::testing::Return(std::make_shared<PublicKey>(nullptr)));
    std::shared_ptr<PublicKey> pubKey1 = publicKeyService->getDomainPublicKey("domain.com");
    std::shared_ptr<PublicKey> pubKey2 = publicKeyService->getDomainPublicKey("domain.com");
    REQUIRE(pubKey1 == pubKey2);
    REQUIRE(pubKey1 != nullptr);
  }
}
