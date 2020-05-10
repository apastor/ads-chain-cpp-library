#pragma once

#include <iostream>
#include <cstring>

class FileWrapper {
  typedef FILE *file_ptr;
  std::string fileName;
  file_ptr wrapped_file;
 public:
  explicit FileWrapper(const char *name, const char *mode = "r") :
      fileName(std::string(name)), wrapped_file(fopen(name, mode)) {}
  explicit FileWrapper(std::string name, const char *mode = "r") :
      fileName(name), wrapped_file(fopen(name.c_str(), mode)) {}
  operator file_ptr() const { return wrapped_file; }
  ~FileWrapper() {
    if (wrapped_file) fclose(wrapped_file);
  }
  friend FileWrapper &operator<<(FileWrapper &fw, const char *data) {
    std::fputs(data, fw);
    return fw;
  }
  friend FileWrapper &operator<<(FileWrapper &fw, const char data) {
    std::fputc(data, fw);
    std::fflush(fw);
    return fw;
  }
  friend std::ostream &operator<<(std::ostream &output, const FileWrapper &fw) {
    const int bufferSize = 1024;
    char buffer[bufferSize] = {};
    std::fgets(buffer, bufferSize, fw);
    output << buffer;
    return output;
  }

  int fseek(long int offset, int whence) { return std::fseek(wrapped_file, offset, whence); }
  std::string getFileName() { return fileName; }

};
