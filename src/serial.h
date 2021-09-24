#pragma once

class Serial {
public:
  Serial() = delete;
  Serial(const char *dev, int baudRate);
  ~Serial();
  [[nodiscard]] int fd() const;
  void write(const char *message) const;
  void writef(const char *format, ...) const;
  void flush() const;

private:
  int _fd;
};

