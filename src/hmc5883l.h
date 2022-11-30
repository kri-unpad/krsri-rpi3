// Currently unused

#pragma once

#include <cstdint>

#define HMC5883L_I2C_ADDRESS 0x1E

namespace hmc5883l {

static const int GAUSS_TO_MTESLA = 100;

enum Register : uint8_t {
  CRA = 0x00,
  CRB = 0x01,
  MODE = 0x02,
  DATA_X_MSB = 0x03,
  DATA_X_LSB = 0x04,
  DATA_Z_MSB = 0x05,
  DATA_Z_LSB = 0x06,
  DATA_Y_MSB = 0x07,
  DATA_Y_LSB = 0x08,
  STATUS = 0x09,
  IRA = 0x0A,
  IRB = 0x0B,
  IRC = 0x0C,
};

enum Gain : uint8_t {
  G1 = 0x20,
  G2 = 0x40,
  G3 = 0x60,
  G4 = 0x80,
  G5 = 0xA0,
  G6 = 0xC0,
  G7 = 0xE0
};

enum Mode : uint8_t {
  CONTINUOUS = 0x00,
  SINGLE = 0x01
};

enum Status : uint8_t {
  READY = 0x01,
  LOCK = 0x02
};

struct SensorData {
  int16_t x, y, z;
  float xScaled, yScaled, zScaled;
  float orientationDeg, orientationRad;
  struct {
    float x, y, z;
  } magnetic;
};

class HMC5883L {
public:
  HMC5883L() = delete;
  HMC5883L(uint8_t address, Gain gain);
  Status readStatus();
  SensorData read();
  void setGain(Gain gain);
  void selfTest();

private:
  int _fd,
      _minDelay,
      _maxValue,
      _minValue;
  float _resolution,
      _declinationAngle,
      _scale;
  Status _status;
  Gain _gain;
  float _gaussLSBXY, _gaussLSBZ;
};

}


