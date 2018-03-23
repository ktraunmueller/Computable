#!/bin/bash

echo "== Building numpy ============================="

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/numpy-$NUMPY_VERSION.tar.gz ] ; then
    try curl -L http://sourceforge.net/projects/numpy/files/NumPy/$NUMPY_VERSION/numpy-$NUMPY_VERSION.tar.gz > $CACHEROOT/numpy-$NUMPY_VERSION.tar.gz
fi

# prepare sources
if [ ! -d $TMPROOT/numpy-$NUMPY_VERSION ]; then
    try tar xvf $CACHEROOT/numpy-$NUMPY_VERSION.tar.gz
    try mv numpy-$NUMPY_VERSION $TMPROOT
fi

cp $PRJROOT/src/numpy_files/numpy/setup.py $TMPROOT/numpy-$NUMPY_VERSION/numpy/setup.py
cp $PRJROOT/src/numpy_files/numpy/core/include/numpy/npy_endian.h $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/include/numpy/npy_endian.h
cp $PRJROOT/src/numpy_files/numpy/distutils/system_info.py $TMPROOT/numpy-$NUMPY_VERSION/numpy/distutils/system_info.py

pushd $TMPROOT/numpy-$NUMPY_VERSION

# build numpy
OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_LD="$LD"
OLD_AR="$AR"
OLD_LDFLAGS="$LDFLAGS"
OLD_LDSHARED="$LDSHARED"

export CC="$ARM_CC -I$BUILDROOT/include $ARM_CFLAGS -fno-builtin"
export CFLAGS="$ARM_CFLAGS"
export CFLAGS="$CFLAGS -I$BUILDROOT/python/include/python2.7"
export CFLAGS="$CFLAGS -I$TMPROOT/numpy-$NUMPY_VERSION/sources/src/private"
export CFLAGS="$CFLAGS -I$TMPROOT/numpy-$NUMPY_VERSION/sources/include"
export CFLAGS="$CFLAGS -iquote$TMPROOT/numpy-$NUMPY_VERSION/sources/numpy/core/include/numpy"
export LD="$ARM_LD $ARM_LDFLAGS"
export AR="$ARM_AR"
export LDFLAGS="$ARM_LDFLAGS"
export LDSHARED="$PRJROOT/tools/liblink"

export BLAS=None
export LAPACK=None
export ATLAS=None

echo "pwd         : `pwd`"
echo "host python : $HOSTPYTHON"
echo "CC          : $CC"
echo "CFLAGS      : $CFLAGS"
echo "LDFLAGS     : $LDFLAGS"
echo "LDSHARED    : $LDSHARED"

#echo "setup.py build run..."
#try $HOSTPYTHON setup.py build --fcompiler=fake
#echo "setup.py install run..."
#try $HOSTPYTHON setup.py install -O2 --root iosbuild

popd

echo "compiling sources..."
rm -rf $TMPROOT/numpy-$NUMPY_VERSION/sources
try mkdir -p $TMPROOT/numpy-$NUMPY_VERSION/sources
try pushd $TMPROOT/numpy-$NUMPY_VERSION/sources

export NPY_ENABLE_SEPARATE_COMPILATION=1
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/private include
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/multiarray/*.h include
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/npymath/*.h include
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/npysort/*.h include
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/umath/*.h include
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/build/src.macosx-10.9-${CPU_ARCHITECTURE}-2.7/numpy .

try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/multiarray/*.c ./numpy/core/src/multiarray
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/npymath/*.c ./numpy/core/src/npymath
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/src/umath/*.c ./numpy/core/src/umath

try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/blasdot ./numpy/core/blasdot
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/fft ./numpy/fft
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/lib/src ./numpy/lib
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/linalg ./numpy/linalg
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/numarray ./numpy/numarray
try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/random/mtrand ./numpy/mtrand

try cp -f $PRJROOT/src/numpy_files/make_Makefile Makefile
try make

echo "creating libnumpy.a..."
try cp -f $PRJROOT/src/numpy_files/link_Makefile Makefile
try make

try cp -r $TMPROOT/numpy-$NUMPY_VERSION/numpy/core/include $BUILDROOT/include
try cp libnumpy.a $BUILDROOT/lib
try popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export LD="$OLD_LD"
export AR="$OLD_AR"
export LDFLAGS="$OLD_LDFLAGS"
export LDSHARED="$OLD_LDSHARED"