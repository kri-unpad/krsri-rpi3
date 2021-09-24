#include <wiringPiI2C.h>
#include <thread>
#include <cmath>
#include <sstream>
#include "hmc5883l.h"
#include "../vector3.h"

namespace hmc5883l {

using namespace std::chrono_literals;

HMC5883L::HMC5883L(uint8_t address, Gain gain) {
  _fd = wiringPiI2CSetup(address);

}

Status HMC5883L::readStatus() {
  const uint8_t regValue = wiringPiI2CReadReg8(_fd, Register::STATUS) & 0x03;
  if (regValue & 1) {
    _status = Status::READY;
  }
  if ((regValue >> 1) & 1) {
    _status = Status::LOCK;
  }
  return _status;
}

SensorData HMC5883L::read() {
  while (readStatus() != Status::READY) {
    std::this_thread::sleep_for(67ms);
  }

  // Read data registers
  uint8_t x0, x1, y0, y1, z0, z1;
  x0 = wiringPiI2CReadReg8(_fd, Register::DATA_X_LSB);
  x1 = wiringPiI2CReadReg8(_fd, Register::DATA_X_MSB);
  y0 = wiringPiI2CReadReg8(_fd, Register::DATA_Y_LSB);
  y1 = wiringPiI2CReadReg8(_fd, Register::DATA_Y_MSB);
  z0 = wiringPiI2CReadReg8(_fd, Register::DATA_Z_LSB);
  z1 = wiringPiI2CReadReg8(_fd, Register::DATA_Z_MSB);

  SensorData data = {};

  data.x = static_cast<int16_t>((x1 << 8) | x0);
  data.y = static_cast<int16_t>((y1 << 8) | y0);
  data.z = static_cast<int16_t>((z1 << 8) | z0);

  Vector3 rawVec(
      static_cast<float>((x1 << 8) | x0),
      static_cast<float>((y1 << 8) | y0),
      static_cast<float>((z1 << 8) | z0)
  );
  Vector3 scaledVec = _scale * rawVec;

  data.xScaled = _scale * static_cast<float>(data.x);
  data.yScaled = _scale * static_cast<float>(data.y);
  data.zScaled = _scale * static_cast<float>(data.z);

  data.magnetic.x = static_cast<float>(data.x) / _gaussLSBXY * GAUSS_TO_MTESLA;
  data.magnetic.y = static_cast<float>(data.y) / _gaussLSBXY * GAUSS_TO_MTESLA;
  data.magnetic.z = static_cast<float>(data.z) / _gaussLSBZ * GAUSS_TO_MTESLA;

  float heading = std::atan2(data.magnetic.y, data.magnetic.x);
  if (_declinationAngle > 0) {
    heading += _declinationAngle;
  }

  if (heading < 0) {
    heading += 2 * M_PI;
  } else if (heading > 2 * M_PI) {
    heading -= 2 * M_PI;
  }

  data.orientationRad = heading;
  data.orientationDeg = heading * 180.0f / static_cast<float>(M_PI);

  return data;
}

void HMC5883L::setGain(const Gain gain) {
  wiringPiI2CWriteReg8(_fd, Register::CRB, gain);
  _gain = gain;
  switch (gain) {
    case G1:
      _gaussLSBXY = 1090.0f;
      _gaussLSBZ = 980.0f;
      _scale = 0.92f;
      break;
    case G2:
      _gaussLSBXY = 820.0f;
      _gaussLSBZ = 760.0f;
      _scale = 1.22f;
      break;
    case G3:
      _gaussLSBXY = 660.0f;
      _gaussLSBZ = 600.0f;
      _scale = 1.52f;
      break;
    case G4:
      _gaussLSBXY = 440.0f;
      _gaussLSBZ = 400.0f;
      _scale = 2.27f;
      break;
    case G5:
      _gaussLSBXY = 390.0f;
      _gaussLSBZ = 255.0f;
      _scale = 2.56f;
      break;
    case G6:
      _gaussLSBXY = 330.0f;
      _gaussLSBZ = 295.0f;
      _scale = 3.03f;
      break;
    case G7:
      _gaussLSBXY = 230.0f;
      _gaussLSBZ = 205.0f;
      _scale = 4.35f;
      break;
  }
}

void HMC5883L::selfTest() {
  uint16_t limitLow = 243, limitHigh = 575;

  while (true) {
    wiringPiI2CWriteReg8(_fd, Register::CRA, 0x71);
    setGain(Gain::G5);
    wiringPiI2CWriteReg8(_fd, Register::MODE, Mode::CONTINUOUS);

    SensorData data = read();
    if (data.x < limitLow || data.x > limitHigh ||
        data.y < limitLow || data.y > limitHigh ||
        data.z < limitLow || data.z > limitHigh) {
      switch (_gain) {
        case G1:
          setGain(Gain::G2);
          continue;
        case G2:
          setGain(Gain::G3);
          continue;
        case G3:
          setGain(Gain::G4);
          continue;
        case G4:
          setGain(Gain::G5);
          continue;
        case G5:
          setGain(Gain::G6);
          continue;
        case G6:
          setGain(Gain::G7);
          continue;
        case G7:
          std::stringstream err;
          err << "Self test error: invalid gain level: " << std::hex << std::showbase << _gain;
          throw std::runtime_error(err.str());
      }
    }

    return;
  }
}

} // namespace hmc5883l