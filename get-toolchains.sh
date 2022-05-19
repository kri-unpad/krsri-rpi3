#/usr/bin/env bash

PROJECT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
TOOLCHAINS_DIR="${PROJECT_DIR}/.toolchains";
TARGET="armv8-rpi3-linux-gnueabihf";
SYSROOT="${TOOLCHAINS_DIR}/sysroot/${TARGET}";
IMAGE_TAG="${TARGET}";
IMAGE="tttapa/docker-arm-cross-build-scripts:${IMAGE_TAG}";

mkdir -p "${TOOLCHAINS_DIR}";

echo "Pulling Docker image ${IMAGE}";
[ ! -z $(docker images -q "${IMAGE}") ] || docker pull "${IMAGE}";

echo "Creating archives";
container=$(docker run -d "${IMAGE}" \
    bash -c "tar cf RPi-staging.tar RPi-staging & \
            tar cf RPi-sysroot.tar RPi-sysroot & \
            tar cf x-tools.tar x-tools & \
            wait");
status=$(docker wait "${container}");
if [ ${status} -ne 0 ]; then
    echo "Error creating toolchain archives"
    exit 1
fi

echo "Copying sysroot";
docker cp \
  "${container}:/home/develop/RPi-sysroot.tar" \
  "${TOOLCHAINS_DIR}/sysroot-${TARGET}.tar";

echo "Copying toolchain";
docker cp \
  "${container}:/home/develop/x-tools.tar" \
  "${TOOLCHAINS_DIR}/x-tools-${TARGET}.tar";
docker rm "${container}" >/dev/null

echo "Extracting sysroot";
chmod -fR u+w "${TOOLCHAINS_DIR}/sysroot/${TARGET}" || :
rm -rf "${TOOLCHAINS_DIR}/sysroot/${TARGET}";
mkdir -p "${TOOLCHAINS_DIR}/sysroot/${TARGET}";
tar xf \
  "${TOOLCHAINS_DIR}/sysroot-${TARGET}.tar" \
  -C "${TOOLCHAINS_DIR}/sysroot/${TARGET}" \
  --strip-components 1;
rm -f "${TOOLCHAINS_DIR}/sysroot-${TARGET}.tar";

echo "Extracting toolchain"
chmod -fR u+w "${TOOLCHAINS_DIR}/x-tools/${TARGET}" || :
rm -rf "${TOOLCHAINS_DIR}/x-tools/${TARGET}";
tar xf \
  "${TOOLCHAINS_DIR}/x-tools-${TARGET}.tar" \
  -C "${TOOLCHAINS_DIR}";
rm -f "${TOOLCHAINS_DIR}/x-tools-${TARGET}.tar";

echo "Building wiringPi";
make \
  -C "${PROJECT_DIR}/external/WiringPi/wiringPi" \
  DESTDIR="${SYSROOT}/usr" \
  CC="${TOOLCHAINS_DIR}/x-tools/${TARGET}/bin/${TARGET}-gcc" \
  EXTRA_CFLAGS="--sysroot='${SYSROOT}'";

echo "Installing wiringPi";
make \
  -C "${PROJECT_DIR}/external/WiringPi/wiringPi" \
  DESTDIR="${SYSROOT}/usr" \
  PREFIX="" \
  LDCONFIG="" \
  install;
