#!/bin/bash

echo "== Building pandas ============================="

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/pandas-$PANDAS_VERSION.tar.gz ] ; then
    try curl -L https://github.com/pydata/pandas/releases/download/v$PANDAS_VERSION/pandas-$PANDAS_VERSION.tar.gz > $CACHEROOT/pandas-$PANDAS_VERSION.tar.gz
fi

# prepare sources
if [ ! -d $TMPROOT/pandas-$PANDAS_VERSION ]; then
    try tar xvf $CACHEROOT/pandas-$PANDAS_VERSION.tar.gz
    try mv pandas-$PANDAS_VERSION $TMPROOT
fi

pushd $TMPROOT/pandas-$PANDAS_VERSION

# build pandas
OLD_CC="$CC"
OLD_CXX="$CXX"
OLD_CPP="$CPP"
OLD_CFLAGS="$CFLAGS"
OLD_CXXFLAGS="$CXXFLAGS"
OLD_CPPFLAGS="$CPPFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LDSHARED="$LDSHARED"

# ~~~~ generate C sources ~~~~

export CC="$MAC_CC"
export CXX="$MAC_CXX"
export CFLAGS="$MAC_CFLAGS"
export CFLAGS="$CFLAGS -I$BUILDROOT/python/include/python2.7"
export CFLAGS="$CFLAGS -I$BUILDROOT/python/lib/python2.7/site-packages/numpy/core/include"
export CXXFLAGS="$MAC_CXXFLAGS"
export LD="$MAC_LD"
export AR="$MAC_AR"
export LDFLAGS="$MAC_LDFLAGS"
export MACOSX_DEPLOYMENT_TARGET=$MACSDKVER

try cp -f $PRJROOT/src/pandas_files/setup.py .
rm -f pandas/parser.c
mv -f pandas/parser.pyx pandas/pdparser.pyx

# create C sources
echo "setup.py build run..."
try python setup.py build

# ~~~~ compile and link everything (libpandas.a) ~~~~

export CC="$ARM_CC -I$BUILDROOT/include $ARM_CFLAGS -fno-builtin"
export CFLAGS="$ARM_CFLAGS"
export CFLAGS="$CFLAGS -I$BUILDROOT/python/include/python2.7"
export CFLAGS="$CFLAGS -I$TMPROOT/pandas-$PANDAS_VERSION/sources/pandas/src/datetime"
export CFLAGS="$CFLAGS -I$TMPROOT/pandas-$PANDAS_VERSION/sources/pandas/src/ujson/lib"
export CFLAGS="$CFLAGS -I$TMPROOT/pandas-$PANDAS_VERSION/sources/pandas/src"
export CFLAGS="$CFLAGS -I$BUILDROOT/include"
export CFLAGS="$CFLAGS -iquote$BUILDROOT/python/include/python2.7"
export CFLAGS="$CFLAGS -iquote$TMPROOT/pandas-$PANDAS_VERSION/sources/pandas/src"
export CFLAGS="$CFLAGS -iquote$TMPROOT/pandas-$PANDAS_VERSION/sources/pandas/src/klib"
export CFLAGS="$CFLAGS -iquote$BUILDROOT/include"
export CXX="$ARM_CXX"
export CPP="$ARM_CPP"
export CPPFLAGS="$CFLAGS"
export LD="$ARM_LD $ARM_LDFLAGS"
export AR="$ARM_AR"
export LDFLAGS="$ARM_LDFLAGS"
export LDSHARED="$PRJROOT/tools/liblink"

echo "pwd         : `pwd`"
echo "host python : $HOSTPYTHON"
echo "CC          : $CC"
echo "CFLAGS      : $CFLAGS"
echo "LDFLAGS     : $LDFLAGS"
echo "LDSHARED    : $LDSHARED"

echo "compiling C sources..."
try mkdir -p sources
try pushd sources

try cp -r $TMPROOT/pandas-$PANDAS_VERSION/pandas .

try cp -f $PRJROOT/src/pandas_files/sources/helper.h pandas/src/helper.h
try cp -f $PRJROOT/src/pandas_files/sources/numpy_helper.h pandas/src/numpy_helper.h
try cp -f $PRJROOT/src/pandas_files/sources/numpy_helper.c pandas/src/numpy_helper.c
try cp -f $PRJROOT/src/pandas_files/sources/datetime_helper.h pandas/src/datetime_helper.h
try cp -f $PRJROOT/src/pandas_files/sources/datetime_helper.c pandas/src/datetime_helper.c
try cp -f $PRJROOT/src/pandas_files/sources/np_datetime.h pandas/src/datetime/np_datetime.h
try cp -f $PRJROOT/src/pandas_files/sources/np_datetime.c pandas/src/datetime/np_datetime.c
try cp -f $PRJROOT/src/pandas_files/sources/khash_python.h pandas/src/klib/khash_python.h
try cp -f $PRJROOT/src/pandas_files/sources/khash_python.c pandas/src/klib/khash_python.c
try cp -f $PRJROOT/src/pandas_files/sources/ultrajson.h pandas/src/ujson/lib/ultrajson.h
try cp -f $PRJROOT/src/pandas_files/sources/parse_helper.h pandas/src/parse_helper.h

try cp -f $PRJROOT/src/pandas_files/make_Makefile Makefile
try make

echo "creating libpandas.a..."
try cp -f $PRJROOT/src/pandas_files/link_Makefile Makefile
try make

try cp libpandas.a $BUILDROOT/lib
try popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export LD="$OLD_LD"
export AR="$OLD_AR"
export LDFLAGS="$OLD_LDFLAGS"
export LDSHARED="$OLD_LDSHARED"