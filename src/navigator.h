#pragma once

#include <tuple>
#include <regex>
#include "serial.h"

class Navigator {
public:
  using DistancesType = std::tuple<uint16_t, uint16_t, uint16_t>;

  Navigator() = delete;
  explicit Navigator(const Serial &nano) : _nano(nano) {}

  std::string readRawDistances();
  DistancesType readDistances();

  uint16_t getFrontDistance() {
    return std::get<0>(_distances);
  }

  uint16_t getRightDistance() {
    return std::get<1>(_distances);
  }

  uint16_t getLeftDistance() {
    return std::get<2>(_distances);
  }

private:
  Serial _nano;
  std::string _rawDistances;
  DistancesType _distances;
};

