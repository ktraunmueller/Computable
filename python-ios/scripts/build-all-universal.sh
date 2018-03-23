#!/bin/bash

#rm -rf ../build
#rm -rf ../tmp

export TARGET_SDK="iphoneos"
./build-all.sh

export TARGET_SDK="iphonesimulator"
./build-all.sh

. $(dirname $0)/environment.sh

export UNIVERSAL_DIR="$PRJROOT/build/universal"
rm -rf $UNIVERSAL_DIR
mkdir -p $UNIVERSAL_DIR/include
mkdir -p $UNIVERSAL_DIR/lib

# ffi
lipo -create \
    "${PRJROOT}/build/armv7/lib/libffi.a" \
    "${PRJROOT}/build/i386/lib/libffi.a" \
    -output "${UNIVERSAL_DIR}/lib/libffi.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libffi.a

# python
lipo -create \
    "${PRJROOT}/build/armv7/lib/libpython2.7.a" \
    "${PRJROOT}/build/i386/lib/libpython2.7.a" \
    -output "${UNIVERSAL_DIR}/lib/libpython2.7.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libpython2.7.a

# pyzmq
lipo -create \
    "${PRJROOT}/build/armv7/lib/libpyzmq.a" \
    "${PRJROOT}/build/i386/lib/libpyzmq.a" \
    -output "${UNIVERSAL_DIR}/lib/libpyzmq.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libpyzmq.a

# numpy
lipo -create \
    "${PRJROOT}/build/armv7/lib/libnumpy.a" \
    "${PRJROOT}/build/i386/lib/libnumpy.a" \
    -output "${UNIVERSAL_DIR}/lib/libnumpy.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libnumpy.a

# f2c
lipo -create \
    "${PRJROOT}/build/armv7/lib/libf2c.a" \
    "${PRJROOT}/build/i386/lib/libf2c.a" \
    -output "${UNIVERSAL_DIR}/lib/libf2c.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libf2c.a

# scipy
lipo -create \
    "${PRJROOT}/build/armv7/lib/libscipy.a" \
    "${PRJROOT}/build/i386/lib/libscipy.a" \
    -output "${UNIVERSAL_DIR}/lib/libscipy.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libscipy.a

# matplotlib
lipo -create \
    "${PRJROOT}/build/armv7/lib/libmatplotlib.a" \
    "${PRJROOT}/build/i386/lib/libmatplotlib.a" \
    -output "${UNIVERSAL_DIR}/lib/libmatplotlib.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libmatplotlib.a

# pandas
lipo -create \
    "${PRJROOT}/build/armv7/lib/libpandas.a" \
    "${PRJROOT}/build/i386/lib/libpandas.a" \
    -output "${UNIVERSAL_DIR}/lib/libpandas.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libpandas.a

# vtparse
lipo -create \
    "${PRJROOT}/build/armv7/lib/libvtparse.a" \
    "${PRJROOT}/build/i386/lib/libvtparse.a" \
    -output "${UNIVERSAL_DIR}/lib/libvtparse.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libvtparse.a

# freetype
lipo -create \
    "${PRJROOT}/build/armv7/lib/libfreetype.a" \
    "${PRJROOT}/build/i386/lib/libfreetype.a" \
    -output "${UNIVERSAL_DIR}/lib/libfreetype.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libfreetype.a

# png
lipo -create \
    "${PRJROOT}/build/armv7/lib/libpng16.a" \
    "${PRJROOT}/build/i386/lib/libpng16.a" \
    -output "${UNIVERSAL_DIR}/lib/libpng16.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libpng16.a

# bz2
lipo -create \
    "${PRJROOT}/build/armv7/lib/libbz2.a" \
    "${PRJROOT}/build/i386/lib/libbz2.a" \
    -output "${UNIVERSAL_DIR}/lib/libbz2.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libbz2.a

# faulthandler
lipo -create \
    "${PRJROOT}/build/armv7/lib/libfaulthandler.a" \
    "${PRJROOT}/build/i386/lib/libfaulthandler.a" \
    -output "${UNIVERSAL_DIR}/lib/libfaulthandler.a"
echo "Checking fat output file"
lipo -info ${UNIVERSAL_DIR}/lib/libfaulthandler.a