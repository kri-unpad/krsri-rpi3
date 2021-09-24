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

void Serial::write(const char *message) const {
  serialPuts(_fd, message);
}

void Serial::writef(const char *format, ...) const {
  va_list argp;
  va_start (argp, format);
  serialPrintf(_fd, format, argp);
  va_end(argp);
}

void Serial::flush() const {
  serialFlush(_fd);
}

