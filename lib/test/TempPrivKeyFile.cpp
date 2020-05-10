#include <cstdio>

#include "TempPrivKeyFile.h"

TempPrivKeyFile::TempPrivKeyFile() :
    fileName(std::tmpnam(nullptr)),
    file(fileName, "w") {
  file << "-----BEGIN EC PARAMETERS-----" << '\n';
  file << "BggqhkjOPQMBBw==" << '\n';
  file << "-----END EC PARAMETERS-----" << '\n';
  file << "-----BEGIN EC PRIVATE KEY-----" << '\n';
  file << "MHcCAQEEIE4lZm6toHaabSxG6v9sCXT2E8IrYDx0QDYb67n5Ld/2oAoGCCqGSM49" << '\n';
  file << "AwEHoUQDQgAEWXQk8rlGgf9prSod+VQf6CMpZmAO6b6Mqjmo4GUQfmowbiMr9l6/" << '\n';
  file << "yfDW3AmdF75yY+4TCO8kgsqIRKEnq3Oj8Q==" << '\n';
  file << "-----END EC PRIVATE KEY-----" << '\n';
};

TempPrivKeyFile::~TempPrivKeyFile() { std::remove(fileName.c_str()); };