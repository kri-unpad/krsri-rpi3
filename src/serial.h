#pragma once
#include <string>

class Serial {
public:
  Serial() = delete;
  Serial(const char *dev, int baudRate);
  ~Serial();
  [[nodiscard]] int fd() const;
  void print(const char *message) const;
  void printf(const char *format, ...) const;
  [[nodiscard]] std::string readln() const;
  void flush() const;

private:
  int _fd;
};

