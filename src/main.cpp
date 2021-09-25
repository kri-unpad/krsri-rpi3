#include <iostream>
#include <thread>
#include <wiringPi.h>
#include <atomic>
#include <functional>
#include <random>
#include "serial.h"
#include "navigator.h"

#define START_BTN_PIN 7
#define LED_PIN 0
#define SSC32U_SERIAL "/dev/ttyUSB0"
#define SSC32U_BAUD 115200
#define NANONAV_SERIAL "/dev/ttyUSB1"
#define NANONAV_BAUD 9600

using namespace std::chrono_literals;

std::atomic<bool> running = true;

void waitForButtonPress(int pin) {
  pinMode(pin, INPUT);
  pullUpDnControl(pin, PUD_DOWN);
  constexpr auto now = []() { return std::chrono::system_clock::now().time_since_epoch(); };
  auto lastDebounce = now();
  const std::chrono::duration debounceDelay = 250ms;
  while (true) {
    if (now() - lastDebounce > debounceDelay) {
      while (digitalRead(pin) == LOW) {
      }
      while (digitalRead(pin) == HIGH) {
      }
      return;
    }
  }
}

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

//  void bismillahRandom() {
//    _status = Status::WalkingForward;
//    _ssc32.printf("XL100 XR100 XS200 \r");
//    std::this_thread::sleep_for(5s);
//    std::this_thread::sleep_for(10s);
//    _ssc32.printf("XL100 XR-100 XS100 \r");
//    std::this_thread::sleep_for(10s);
//    _ssc32.printf("XL100 XR100 XS200 \r");
//    std::this_thread::sleep_for(10s);
//  }

private:
  Serial _ssc32;
  Status _status;
};

void setup() {
  wiringPiSetup();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void operation() {
  Serial ssc32(SSC32U_SERIAL, SSC32U_BAUD);
  Serial nano(NANONAV_SERIAL, NANONAV_BAUD);

  Hexapod hexapod(ssc32);
  Navigator nav(nano);

  hexapod.standStill();
  std::this_thread::sleep_for(2s);

  auto genRandBool = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());

  while (running) {
    std::cout << "[OPERATION] operating...\n";
    nav.readDistances();
    if (nav.getFrontDistance() < 15
        && (hexapod.status() != Hexapod::RotatingLeft || hexapod.status() != Hexapod::RotatingRight)) {
      if (genRandBool()) {
        hexapod.rotateLeft();
      } else {
        hexapod.rotateRight();
      }
      while (nav.getFrontDistance() < 50) {
        nav.readDistances();
        std::this_thread::sleep_for(100ms);
      }
    }
    hexapod.forward();
    std::this_thread::sleep_for(100ms);
  }

  std::cout << "[OPERATION] operation stopped!\n";
  hexapod.standStill();
}

int main() {
  std::cout << "[MAIN] KRSRI Start!\n";

  setup();
  std::cout << "[MAIN] setup complete.\n";

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
  while (true) {
    std::cout << "[MAIN] waiting the start button.\n";
    waitForButtonPress(START_BTN_PIN);

    std::thread tOperation(operation);
    running = true;
    digitalWrite(LED_PIN, HIGH);

    waitForButtonPress(START_BTN_PIN);
    running = false;
    tOperation.join();
    digitalWrite(LED_PIN, LOW);
    std::cout << "[MAIN] stopped.\n";
  }
#pragma clang diagnostic pop
}
