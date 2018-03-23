#!/bin/bash

echo "== Building vtparse ============================="

. $(dirname $0)/environment.sh

# https://github.com/haberman/vtparse

# TODO remove
rm -rf $TMPROOT/vtparse

# prepare sources
if [ ! -d $TMPROOT/vtparse ]; then
    try cp -R $PRJROOT/src/vtparse_files/vtparse $TMPROOT/vtparse
fi

pushd $TMPROOT/vtparse

OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_LD="$LD"
OLD_AR="$AR"

export CC="$ARM_CC"
export CFLAGS="$ARM_CFLAGS -Wno-parentheses"
export AR="$ARM_AR"

try make
try cp libvtparse.a $BUILDROOT/lib/
try mkdir $BUILDROOT/include/vtparse
try cp *.h $BUILDROOT/include/vtparse
lipo -info $BUILDROOT/lib/libvtparse.a

popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export AR="$OLD_AR"
