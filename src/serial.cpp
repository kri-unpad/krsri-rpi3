#include <cstdarg>
#include <stdexcept>
#include <wiringSerial.h>
#include <unordered_map>

#include "serial.h"

Serial::Serial(const char *dev, const int baudRate) {
  _fd = serialOpen(dev, baudRate);
  if (_fd < 0) {
    throw std::runtime_error("unable to open serial device");
  }
}

Serial::~Serial() {
  serialClose(_fd);
}

int Serial::fd() const {
  return _fd;
}

void Serial::print(const char *message) const {
  serialPuts(_fd, message);
}

void Serial::printf(const char *format, ...) const {
  char buffer[1024];
  va_list args;
  va_start (args, format);
  vsnprintf(buffer, 1023, format, args);
  va_end(args);
  print(buffer);
}

void Serial::flush() const {
  serialFlush(_fd);
}
std::string Serial::readln() const {
  std::string buffer;
  while (serialDataAvail(_fd) < 10) {
  }
  if (serialDataAvail(_fd) > 0) {
    char c;
    while ((c = serialGetchar(_fd)) != '\n') {
      buffer.push_back(c);
    }
  }
  return buffer;
}

