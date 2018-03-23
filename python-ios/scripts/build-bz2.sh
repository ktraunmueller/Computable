#!/bin/bash

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/bzip2-$BZ2_VERSION.tar.gz ]; then
	try curl -L http://www.bzip.org/$BZ2_VERSION/bzip2-$BZ2_VERSION.tar.gz > $CACHEROOT/bzip2-$BZ2_VERSION.tar.gz
fi

if [ ! -d $TMPROOT/bzip2-$BZ2_VERSION ]; then
	try tar xvf $CACHEROOT/bzip2-$BZ2_VERSION.tar.gz
	try mv bzip2-$BZ2_VERSION $TMPROOT
fi

OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LD="$LD"
OLD_AR="$AR"

export CC="$ARM_CC"
export CFLAGS="$ARM_CFLAGS -Wno-parentheses"
export LDFLAGS="-arch $CPU_ARCHITECTURE"
export LD="$ARM_LD"
export AR="$ARM_AR"

pushd $TMPROOT/bzip2-$BZ2_VERSION

try cp $PRJROOT/src/bz2_files/Makefile Makefile

try make clean
try make
try cp libbz2.a $BUILDROOT/lib/

popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export LDFLAGS="$OLD_LDFLAGS"
export LD="$OLD_LD"
export AR="$OLD_AR"
