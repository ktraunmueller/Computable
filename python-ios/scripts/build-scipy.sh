#!/bin/bash

echo "== Building scipy ============================="

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/scipy-$SCIPY_VERSION.tar.gz ] ; then
    try curl -L http://sourceforge.net/projects/scipy/files/NumPy/$SCIPY_VERSION/scipy-$SCIPY_VERSION.tar.gz > $CACHEROOT/scipy-$SCIPY_VERSION.tar.gz
fi

FFTW_VERSION=3.3.3
if [ ! -f $CACHEROOT/fftw-$FFTW_VERSION.tar.gz ] ; then
    try curl -L http://www.fftw.org/fftw-$FFTW_VERSION.tar.gz > $CACHEROOT/fftw-$FFTW_VERSION.tar.gz
fi

# prepare sources
if [ ! -d $TMPROOT/scipy-$SCIPY_VERSION ]; then
    try tar xvf $CACHEROOT/scipy-$SCIPY_VERSION.tar.gz
    try mv scipy-$SCIPY_VERSION $TMPROOT
fi

# would be required for fftpack tests
#if [ ! -d $TMPROOT/scipy-$SCIPY_VERSION/fftw-$FFTW_VERSION ]; then
#    try tar xvf $CACHEROOT/fftw-$FFTW_VERSION.tar.gz
#    try mv fftw-$FFTW_VERSION $TMPROOT/scipy-$SCIPY_VERSION
#fi

pushd $TMPROOT/scipy-$SCIPY_VERSION

# How building scipy for iOS works:
# - scipy's setup.py requires numpy's distutils (C compiler, Fortran compiler, etc). For this, we're using a
#   customized numpy host installation.
# - We're adding a custom F2CFCompiler to the host numpy installation, and run the scipy setup
#   with "python setup.py build --fcompiler=f2c"
# - The custom F2CFCompiler converts .f to .c on the fly, compiles the C sources using the target (iOS) compiler,
#   and uses the target linker to create the static libraries :)
# - For this to work, we need the following environment variables (iphoneos/arm7 target settings):
#   CC = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
#   CXX = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
#   CPP = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++
#   CFLAGS = -arch armv7 -pipe -no-cpp-precomp -O3 -isysroot -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk -miphoneos-version-min=7.0
#   LD = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ld
#   LDFLAGS = -arch armv7 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS7.0.sdk -miphoneos-version-min=7.0
#   AR = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar
#
# Setup steps:
#
# 1) check python (2.7.6):
#   $ which python
#   /usr/local/bin/python
#   $ ls -la /usr/local/bin/python
#   lrwxr-xr-x  1 ktraunmueller  admin  33 16 Jan 22:52 /usr/local/bin/python -> ../Cellar/python/2.7.6/bin/python

# 2) install f2c:
#   2.1: build and install f2c with build-f2c.sh (so f2c's f77 stub ends up linked as /usr/local/bin/g77)

# 3) install numpy on the host:
#   3.1: download and unzip numpy (somewhere outside python-ios)
#   3.2: install numpy on the host
#       $ cd numpy
#       $ python setup.py build
#       $ python setup.py install
#   -> numpy is now installed on the host (e.g., /usr/local/lib/python2.7/site-packages/numpy)
#   3.3: extend numpy distutils with our custom F2CFCompiler
#       $ cp scipy_files/f2c.py /usr/local/lib/python2.7/site-packages/numpy/distutils/fcompiler

# 4) build scipy for the target
#   4.1: download and unzip scipy
#   4.2: install scipy
#       $ cd scipy
#       $ python setup.py build --fcompiler=f2c

# 5) compile the module sources into libscipy.a

# patches:
# fix conflicting function declarations
try cp $PRJROOT/src/scipy_files/scipy/patches/linalg/src/id_dist/src/idd_sfft.c $TMPROOT/scipy-$SCIPY_VERSION/scipy/linalg/src/id_dist/src
# fix global p1evl (declared also in polevl.c)
try cp -f $PRJROOT/src/scipy_files/scipy/patches/ndimage/src/ni_fourier.c $TMPROOT/scipy-$SCIPY_VERSION/scipy/ndimage/src
# remove mvndst.f, because it's not F77 (Declaration error for rho: adjustable dimension on non-argument)
#try rm scipy/stats/mvndst.f
try rm -f $TMPROOT/scipy-$SCIPY_VERSION/scipy/stats/mvn.pyf
# remove mvnmodule from setup
try cp $PRJROOT/src/scipy_files/scipy/patches/stats/setup.py $TMPROOT/scipy-$SCIPY_VERSION/scipy/stats
# remove fftpack tests (requires fftw3)
try rm -rf $TMPROOT/scipy-$SCIPY_VERSION/scipy/fftpack/tests
# remove code that fails to compile (example.c:37:27: error: too few arguments to function call, expected 24, have 23)
try rm -f $TMPROOT/scipy-$SCIPY_VERSION/scipy/optimize/tnc/example.c

OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_CXXFLAGS="$CXXFLAGS"
OLD_CPPFLAGS="$CPPFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LDSHARED="$LDSHARED"

export CC="$ARM_CC"
export CXX="$ARM_CXX"
export CPP="$ARM_CPP"
export CFLAGS="$ARM_CFLAGS -I$TMPROOT/f2c/libf2c/include"
export CXXFLAGS="$ARM_CXXFLAGS"
export CPPFLAGS="$ARM_CPPFLAGS"
export LD="$ARM_LD"
export AR="$ARM_AR"
export LDFLAGS="$ARM_LDFLAGS"

export BLAS=None
export LAPACK=None
export ATLAS=None

echo "pwd         : `pwd`"
echo "python      : $USR_PYTHON"
echo "CC          : $CC"
echo "CFLAGS      : $CFLAGS"
echo "LDFLAGS     : $LDFLAGS"
echo "LDSHARED    : $LDSHARED"

# TODO fix compiler warnings? (e.g., "uninitialized use occurs here")

# ~~~~ generate/compile fortran and c sources ~~~~

#echo "setup.py build run..."
#try python setup.py build --fcompiler=f2c

# ~~~~ compile and link everything (libscipy.a) ~~~~

export CFLAGS="$CFLAGS -I$BUILDROOT/python/include/python2.7"
export CFLAGS="$CFLAGS -I$BUILDROOT/python/lib/python2.7/site-packages/numpy/core/include"
export CFLAGS="$CFLAGS -I$PRJROOT/src/scipy_files/modules_src"
export CFLAGS="$CFLAGS -I$BUILDROOT/include/f2c"
export CFLAGS="$CFLAGS -I$TMPROOT/scipy-$SCIPY_VERSION/sources/src.macosx-10.9-x86_64-2.7"
export CFLAGS="$CFLAGS -I$TMPROOT/scipy-$SCIPY_VERSION/scipy/fftpack/src"
export CFLAGS="$CFLAGS -I$TMPROOT/scipy-$SCIPY_VERSION/scipy/signal"
export CFLAGS="$CFLAGS -I$TMPROOT/scipy-$SCIPY_VERSION/scipy/special"
export CFLAGS="$CFLAGS -I$TMPROOT/scipy-$SCIPY_VERSION/scipy/spatial"
export CXXFLAGS="$CFLAGS"

echo "compiling c sources..."
try mkdir -p $TMPROOT/scipy-$SCIPY_VERSION/sources
try pushd $TMPROOT/scipy-$SCIPY_VERSION/sources

try cp $PRJROOT/src/scipy_files/scipy/patches/fortran/*.c .

try cp -r $TMPROOT/scipy-$SCIPY_VERSION/build/src.macosx-10.9-x86_64-2.7 .
try rm -f ./src.macosx-10.9-x86_64-2.7/build/src.macosx-10.9-x86_64-2.7/scipy/sparse/linalg/eigen/arpack/_arpackmodule.c

try mkdir -p special
try pushd special
try cp -r $TMPROOT/scipy-$SCIPY_VERSION/scipy/special/c_misc .
try cp -r $TMPROOT/scipy-$SCIPY_VERSION/scipy/special/cephes .
try mv cephes/struve.c cephes/struve_cephes.c
try cp $TMPROOT/scipy-$SCIPY_VERSION/scipy/special/cephes.h .
try popd

try cp -r $PRJROOT/src/scipy_files/scipy/patches/linalg .
try cp -r $PRJROOT/src/scipy_files/scipy/patches/sparse .
try cp -r $PRJROOT/src/scipy_files/scipy/patches/optimize .
try cp -r $PRJROOT/src/scipy_files/scipy/patches/special .
try cp -r $PRJROOT/src/scipy_files/scipy/patches/stats .

# blas
try cp -r $PRJROOT/src/scipy_files/scipy/blas .

try cp $PRJROOT/src/scipy_files/scipy/make_Makefile Makefile
try make
try popd

echo "copying objects..."
rm -rf $TMPROOT/scipy-$SCIPY_VERSION/lib
try mkdir -p $TMPROOT/scipy-$SCIPY_VERSION/lib
try pushd $TMPROOT/scipy-$SCIPY_VERSION/lib
try cp -r $TMPROOT/scipy-$SCIPY_VERSION/build/temp.macosx-10.9-x86_64-2.7/scipy/ .
try cp -r $TMPROOT/scipy-$SCIPY_VERSION/build/temp.macosx-10.9-x86_64-2.7/build/ .
try cp -r $TMPROOT/scipy-$SCIPY_VERSION/build/temp.macosx-10.9-x86_64-2.7/Users .

try rm -f ./sparse/linalg/dsolve/SuperLU/SRC/dcomplex.o
try rm -f ./sparse/linalg/dsolve/SuperLU/SRC/scomplex.o
try cp -r $TMPROOT/scipy-$SCIPY_VERSION/sources .

try rm -f ./fftpack/src/dfftpack/dcosqb.o
try rm -f ./fftpack/src/dfftpack/dcosqf.o
try rm -f ./fftpack/src/dfftpack/dcosqi.o
try rm -f ./fftpack/src/dfftpack/dcost.o
try rm -f ./fftpack/src/dfftpack/dcosti.o

try rm -f ./special/_ufuncs_cxx.o

try rm -f ./optimize/minpack2/dcsrch.o
try rm -f ./optimize/minpack2/dcstep.o

try rm -f ./src.macosx-10.9-x86_64-2.7/build/src.macosx-10.9-x86_64-2.7/scipy/sparse/linalg/eigen/arpack/_arpackmodule.o
#try rm -rf ./src.macosx-10.9-x86_64-2.7/build/src.macosx-10.9-x86_64-2.7/scipy/lib
#try rm -rf ./sources/src.macosx-10.9-x86_64-2.7/build/src.macosx-10.9-x86_64-2.7/scipy/lib

try mv special/cephes/struve.o special/cephes/struve_cephes.o

echo "creating libscipy.a..."
try cp $PRJROOT/src/scipy_files/scipy/link_Makefile Makefile
try make
try cp libscipy.a $BUILDROOT/lib
deduplicate $BUILDROOT/lib/libscipy.a
try popd

# ~~~~ f2py ~~~~

popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export CXXFLAGS="$OLD_CXXFLAGS"
export CPPFLAGS="$OLD_CPPFLAGS"
export LDFLAGS="$OLD_LDFLAGS"
export LDSHARED="$OLD_LDSHARED"