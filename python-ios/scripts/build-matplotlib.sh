#!/bin/bash

echo "== Building matplotlib ============================="

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/matplotlib-$MATPLOTLIB_VERSION.tar.gz ] ; then
    try curl -L http://netcologne.dl.sourceforge.net/project/matplotlib/matplotlib/matplotlib-$MATPLOTLIB_VERSION/matplotlib-$MATPLOTLIB_VERSION.tar.gz > $CACHEROOT/matplotlib-$MATPLOTLIB_VERSION.tar.gz
fi

# TODO remove
#rm -rf $TMPROOT/matplotlib-$MATPLOTLIB_VERSION

# prepare sources
if [ ! -d $TMPROOT/matplotlib-$MATPLOTLIB_VERSION ]; then
    try tar xvf $CACHEROOT/matplotlib-$MATPLOTLIB_VERSION.tar.gz
    try mv matplotlib-$MATPLOTLIB_VERSION $TMPROOT
fi

cp $PRJROOT/src/matplotlib_files/setup.cfg $TMPROOT/matplotlib-$MATPLOTLIB_VERSION
cp $PRJROOT/src/matplotlib_files/setup.py $TMPROOT/matplotlib-$MATPLOTLIB_VERSION
cp $PRJROOT/src/matplotlib_files/setupext.py $TMPROOT/matplotlib-$MATPLOTLIB_VERSION

pushd $TMPROOT/matplotlib-$MATPLOTLIB_VERSION

# build matplotlib
OLD_CC="$CC"
OLD_CXX="$CXX"
OLD_CPP="$CPP"
OLD_CFLAGS="$CFLAGS"
OLD_CXXFLAGS="$CXXFLAGS"
OLD_CPPFLAGS="$CPPFLAGS"
OLD_LDFLAGS="$LDFLAGS"
OLD_LDSHARED="$LDSHARED"

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

# build distribution structure
#echo "setup.py build run..."
#try python setup.py build

export CC="$ARM_CC"
export CXX="$ARM_CXX"
export CPP="$ARM_CPP"
export CFLAGS="$ARM_CFLAGS"
export CFLAGS="$CFLAGS -I$BUILDROOT/python/include/python2.7"
#export CFLAGS="$CFLAGS -I$BUILDROOT/include/numpy"
export CFLAGS="$CFLAGS -I$BUILDROOT/include/freetype"
export CFLAGS="$CFLAGS -I$BUILDROOT/include/libpng16"
export CFLAGS="$CFLAGS -iquote$BUILDROOT/include"
export CFLAGS="$CFLAGS -iquote$TMPROOT/matplotlib-$MATPLOTLIB_VERSION/sources"
export CFLAGS="$CFLAGS -iquote$TMPROOT/matplotlib-$MATPLOTLIB_VERSION/sources/agg24/include"
export CXXFLAGS="$CFLAGS"
export CPPFLAGS="$CFLAGS"
export LD="$ARM_LD"
export AR="$ARM_AR"
export LDFLAGS="$ARM_LDFLAGS"

echo "compiling sources..."
rm -rf $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/sources
try mkdir -p $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/sources
try pushd $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/sources

try cp -r $PRJROOT/src/matplotlib_files/sources/* .

#try cp -r $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/src .
#try rm src/_backend_gdk.c
#try rm src/_gtkagg.cpp
#try rm src/_tkagg.cpp
#try rm src/_windowing.cpp
#try cp -r $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/agg24 .
#try rm -rf agg24/src/platform
#try cp -r $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/ttconv .
#try cp -r $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/CXX .
#try rm -rf CXX/Python3
#try cp -r $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/lib/matplotlib/delaunay .
#try cp -r $TMPROOT/matplotlib-$MATPLOTLIB_VERSION/lib/matplotlib/tri .

try cp -f $PRJROOT/src/matplotlib_files/make_Makefile Makefile
try make

echo "creating libmatplotlib.a..."
try cp -f $PRJROOT/src/matplotlib_files/link_Makefile Makefile
try make
try cp libmatplotlib.a $BUILDROOT/lib
try popd

export CC="$OLD_CC"
export CXX="$OLD_CXX"
export CPP="$OLD_CPP"
export CFLAGS="$OLD_CFLAGS"
export CXXFLAGS="$OLD_CXXFLAGS"
export CPPFLAGS="$OLD_CPPFLAGS"
export LDFLAGS="$OLD_LDFLAGS"
export LDSHARED="$OLD_LDSHARED"
