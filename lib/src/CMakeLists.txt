add_subdirectory(adschain/crypto)
add_subdirectory(adschain/network)

target_sources(adschain
    PRIVATE
    adschain/Logger.h
    adschain/Logger.cpp
    )