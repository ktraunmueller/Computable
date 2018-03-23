#!/bin/bash

echo "== Building pyzmq ============================="

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/pyzmq-$PYZMQ_VERSION.tar.gz ] ; then
    try curl -L https://pypi.python.org/packages/source/p/pyzmq/pyzmq-$PYZMQ_VERSION.tar.gz > $CACHEROOT/pyzmq-$PYZMQ_VERSION.tar.gz
fi

# prepare sources
if [ ! -d $TMPROOT/pyzmq-$PYZMQ_VERSION ]; then
    try tar xvf $CACHEROOT/pyzmq-$PYZMQ_VERSION.tar.gz
    try mv pyzmq-$PYZMQ_VERSION $TMPROOT

    cp $PRJROOT/src/pyzmq_files/setup-$CPU_ARCHITECTURE.cfg $TMPROOT/pyzmq-$PYZMQ_VERSION
    cp $PRJROOT/src/pyzmq_files/setup.py $TMPROOT/pyzmq-$PYZMQ_VERSION

    # TODO create patch
    #pushd $TMPROOT/pyzmq-$PYZMQ_VERSION
    #try patch -p1 < $PRJROOT/src/pyzmq_files/pyzmq-$PYZMQ_VERSION.patch
    #popd
fi

pushd $TMPROOT/pyzmq-$PYZMQ_VERSION

# build pyzmq
OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_CXXFLAGS="$CXXFLAGS"
OLD_CPPFLAGS="$CPPFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LDSHARED="$LDSHARED"

export LIBZMQ=/Users/ktraunmueller/Documents/sandbox/ios/libzmq-ios-universal/build/universal
export CC="$ARM_CC -I$BUILDROOT/include -I$LIBZMQ/include"
export CXX="$ARM_CXX"
export CFLAGS="$ARM_CFLAGS"
export CXXFLAGS="$ARM_CXXFLAGS"
export CPPFLAGS="$ARM_CPPFLAGS"
export LDFLAGS="$ARM_LDFLAGS -L$LIBZMQ/lib"
export LDSHARED="$PRJROOT/tools/liblink"

echo "pwd         : `pwd`"
echo "host python : $HOSTPYTHON"
echo "CC          : $CC"
echo "CFLAGS      : $CFLAGS"
echo "LDFLAGS     : $LDFLAGS"
echo "LDSHARED    : $LDSHARED"

echo "setup.py configure run..."
try $HOSTPYTHON setup.py configure --zmq=$LIBZMQ
echo "setup.py install run..."
#try $HOSTPYTHON setup.py install -O2 --root iosbuild
try $HOSTPYTHON setup.py install -O0 --root iosbuild

try find iosbuild | grep -E '.*\.(py|pyc|so\.o|so\.a|so\.libs)$$' | xargs rm
try cp -a iosbuild/usr/local/lib/python2.7/site-packages/zmq "$BUILDROOT/python/lib/python2.7/site-packages"

bd=$TMPROOT/pyzmq-$PYZMQ_VERSION/build/lib.macosx-*/zmq
try $PRJROOT/tools/biglink $BUILDROOT/lib/libpyzmq.a $bd $bd/backend/cython $bd/devices $bd/utils
deduplicate $BUILDROOT/lib/libpyzmq.a

popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export CXXFLAGS="$OLD_CXXFLAGS"
export CPPFLAGS="$OLD_CPPFLAGS"
export LDFLAGS="$OLD_LDFLAGS"
export LDSHARED="$OLD_LDSHARED"