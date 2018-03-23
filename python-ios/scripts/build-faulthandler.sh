#!/bin/bash

. $(dirname $0)/environment.sh

FAULTHANDLER_VERSION=2.3

if [ ! -f $CACHEROOT/faulthandler-$FAULTHANDLER_VERSION.tar.gz ]; then
	try curl -L http://pypi.python.org/packages/source/f/faulthandler/faulthandler-$FAULTHANDLER_VERSION.tar.gz > $CACHEROOT/faulthandler-$FAULTHANDLER_VERSION.tar.gz
fi

if [ ! -d $TMPROOT/faulthandler-$FAULTHANDLER_VERSION ]; then
	try tar xvf $CACHEROOT/faulthandler-$FAULTHANDLER_VERSION.tar.gz
	try mv faulthandler-$FAULTHANDLER_VERSION $TMPROOT
fi

pushd $TMPROOT/faulthandler-$FAULTHANDLER_VERSION

OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LD="$LD"
OLD_AR="$AR"
OLD_LDSHARED="$LDSHARED"

export CC="$ARM_CC"
export CFLAGS="$ARM_CFLAGS"
export LDFLAGS="$ARM_LDFLAGS"
export LD="$ARM_LD"
export AR="$ARM_AR"
export LDSHARED="$PRJROOT/tools/liblink"

echo "pwd         : `pwd`"
echo "host python : $HOSTPYTHON"
echo "CC          : $CC"
echo "CFLAGS      : $CFLAGS"
echo "LDFLAGS     : $LDFLAGS"
echo "LDSHARED    : $LDSHARED"

echo "setup.py install run..."
try $HOSTPYTHON setup.py install -O2 --root iosbuild

try $AR r libfaulthandler.a build/lib.macosx-10.9-${CPU_ARCHITECTURE}-2.7/faulthandler.so.o

# copy to buildroot
cp libfaulthandler.a $BUILDROOT/lib

popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export LDFLAGS="$OLD_LDFLAGS"
export LD="$OLD_LD"
export AR="$OLD_AR"
export LDSHARED="$OLD_LDSHARED"