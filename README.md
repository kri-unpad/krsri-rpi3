# krsri-rpi3

## Building

```shell
# Clone repository
git clone git@github.com:kri-unpad/krsri-rpi3.git

# Get submodules
git submodule update --init

# Get toolchains
./get-toolchains.sh

# Build (Cross Compile for RPI3 32 bit)
mkdir build
cd build
cmake \
  -DCMAKE_TOOLCHAIN_FILE=../cmake/armv8-rpi3-linux-gnueabihf.cmake \
  ..
make
```
