#include <iostream>
#include <thread>
#include <wiringPi.h>

#include "serial.h"

#define SSC32U_SERIAL "/dev/ttyUSB0"
#define SSC32U_BAUD 115200

using namespace std::chrono_literals;

#include "vector3.h"

int main() {
  Vector3 a(1, 1, 1);
  Vector3 b(2, 0, -1);

  std::cout << "a:\t" << a << std::endl
            << "b:\t" << b << std::endl
            << "a.x:\t" << a.x << std::endl
            << "add:\t" << a + b << std::endl
            << "sub:\t" << a - b << std::endl
            << "dot:\t" << a.dot(b) << std::endl
            << "smult:\t" << 2 * b << std::endl;
  return 0;
}

//int main() {
//  std::cout << "KRSRI Start!\n";
//  wiringPiSetup();
//
//  Serial ssc32(SSC32U_SERIAL, SSC32U_BAUD);
//
//  for (int lCoxa = 17, rCoxa = 1; lCoxa <= 21 && rCoxa <= 5; lCoxa += 2, rCoxa += 2) {
//    ssc32.writef("#%d P1500 \r", lCoxa);
//    ssc32.writef("#%d P1500 \r", rCoxa);
//  }
//
//  for (int lFemur = 16, rFemur = 0; lFemur <= 20 && rFemur <= 4; lFemur += 2, rFemur += 2) {
//    ssc32.writef("#%d P1500 \r", lFemur);
//    ssc32.writef("#%d P1500 \r", rFemur);
//  }
//
//  for (int tibia = 8; tibia <= 13; ++tibia) {
//    ssc32.writef("#%d P1500 \r", tibia);
//  }
//
//  ssc32.writef("LH1200 LM1400 LL1800 RH1800 RM1600 RL1200 VS2000 \r");
//  ssc32.writef("LF1600 LR1400 RF1400 RR1600 HT150 \r");
//  ssc32.writef("XL100 XR100 XS200 \r");
//  std::this_thread::sleep_for(10s);
//  ssc32.writef("XL100 XR-100 XS100 \r");
//  std::this_thread::sleep_for(10s);
//  ssc32.writef("XL100 XR100 XS200 \r");
//  std::this_thread::sleep_for(10s);
//  ssc32.writef("XSTOP \r");
//
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "EndlessLoop"
////  while (true) {
////  }
//#pragma clang diagnostic pop
//}
