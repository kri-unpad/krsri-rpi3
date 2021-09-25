#include "navigator.h"

std::string Navigator::readRawDistances() {
  std::string raw = _nano.readln();
  _rawDistances = raw;
  return raw;
}

Navigator::DistancesType Navigator::readDistances() {
  std::string raw = readRawDistances();
  std::regex re("DIST ([0-9]+) ([0-9]+) ([0-9]+)");
  std::vector<std::string> tokens;

  std::smatch sm;
  if (regex_search(raw, sm, re)) {
    for (int i = 1; i < sm.size(); ++i) {
      tokens.push_back(sm[i].str());
    }
    _distances = std::make_tuple(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));
    return _distances;
  } else {
    return _distances;
  }
}
