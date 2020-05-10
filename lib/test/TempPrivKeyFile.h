#pragma once

#include <FileWrapper.h>

class TempPrivKeyFile {
 public:
  std::string fileName;
 private:
  FileWrapper file;
 public:
  TempPrivKeyFile();
  virtual ~TempPrivKeyFile();
};
