#include "serial.h"
#include "hexapod.h"

#define SSC32U_SERIAL "/dev/ttyUSB0"
#define SSC32U_BAUD 115200

using namespace std::chrono_literals;

void operation() {
  Serial ssc32(SSC32U_SERIAL, SSC32U_BAUD);

  Hexapod hexapod(ssc32);
  hexapod.standStill();
}

int main() {
  operation();
}
