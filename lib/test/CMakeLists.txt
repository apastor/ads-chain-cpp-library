cmake_minimum_required(VERSION 3.13)

add_executable(adschain-tests "")
target_link_libraries(adschain-tests adschain ${CONAN_LIBS})
# We run our test from the source directory, so we can consistently
# load files with known paths.
# I also like color in my verbose output, so I put in a --force-colour flag.
add_test(NAME adschain-tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/bin/adschain-tests --force-colour
    )

target_sources(adschain-tests
    PRIVATE
    catch-main.cpp
    TempPrivKeyFile.h
    TempPrivKeyFile.cpp
    PrivateKeyTests.cpp
    SignerTests.cpp
    VerifierTests.cpp
    PublicKeyServiceTests.cpp
    QueryStringUtilsTests.cpp
    HttpRequestsTests.cpp
    StopwatchTests.cpp
    )
