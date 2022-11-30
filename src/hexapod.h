#pragma once

#include "serial.h"

class Hexapod {
public:
  Hexapod() = delete;
  explicit Hexapod(const Serial &ssc32) : _ssc32(ssc32), _status(Status::Idle) {
    _ssc32.printf("LH1200 LM1400 LL1800 RH1800 RM1600 RL1200 VS2000 \r");
    _ssc32.printf("LF1600 LR1400 RF1400 RR1600 HT150 \r");
  }
  ~Hexapod() {
    if (_status != Status::Idle) {
      _ssc32.printf("XSTOP \r");
    }
  }

  enum Status {
    Idle,
    WalkingForward,
    RotatingRight,
    RotatingLeft
  };

  void standStill() {
    if (_status != Status::Idle) {
      _ssc32.printf("XSTOP \r");
    }
    for (int lCoxa = 17, rCoxa = 1; lCoxa <= 21 && rCoxa <= 5; lCoxa += 2, rCoxa += 2) {
      _ssc32.printf("#%d P1500 \r", lCoxa);
      _ssc32.printf("#%d P1500 \r", rCoxa);
    }

    for (int lFemur = 16, rFemur = 0; lFemur <= 20 && rFemur <= 4; lFemur += 2, rFemur += 2) {
      _ssc32.printf("#%d P1500 \r", lFemur);
      _ssc32.printf("#%d P1500 \r", rFemur);
    }

    for (int tibia = 8; tibia <= 13; ++tibia) {
      _ssc32.printf("#%d P1500 \r", tibia);
    }

    _status = Status::Idle;
  }

  void forward() {
    if (_status == Status::WalkingForward) {
      return;
    }
    _status = Status::WalkingForward;
    _ssc32.printf("XL100 XR100 XS200 \r");
  }

  void rotateRight() {
    if (_status == Status::RotatingRight) {
      return;
    }
    _status = Status::RotatingRight;
    _ssc32.printf("XL100 XR-100 XS100 \r");
  }

  void rotateLeft() {
    if (_status == Status::RotatingLeft) {
      return;
    }
    _status = Status::RotatingLeft;
    _ssc32.printf("XL-100 XR100 XS100 \r");
  }

  Status status() const {
    return _status;
  }

private:
  Serial _ssc32;
  Status _status;
};