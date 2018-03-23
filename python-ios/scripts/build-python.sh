#!/bin/bash

. $(dirname $0)/environment.sh

# credit to:
# http://randomsplat.com/id5-cross-compiling-python-for-embedded-linux.html
# http://latenitesoft.blogspot.com/2008/10/iphone-programming-tips-building-unix.html

# download python and patch if they aren't there
if [ ! -f $CACHEROOT/Python-$IOS_PYTHON_VERSION.tar.bz2 ]; then
    curl http://www.python.org/ftp/python/$IOS_PYTHON_VERSION/Python-$IOS_PYTHON_VERSION.tar.bz2 > $CACHEROOT/Python-$IOS_PYTHON_VERSION.tar.bz2
fi

# get rid of old build
rm -rf $TMPROOT/Python-$IOS_PYTHON_VERSION
try tar -xjf $CACHEROOT/Python-$IOS_PYTHON_VERSION.tar.bz2
try mv Python-$IOS_PYTHON_VERSION $TMPROOT
try pushd $TMPROOT/Python-$IOS_PYTHON_VERSION

# Patch Python for temporary reduce PY_SSIZE_T_MAX otherzise, splitting string doesn't work
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-ssize-t-max.patch
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-dynload.patch
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-static-_sqlite3.patch

# Copy our setup for modules
try cp $PRJROOT/src/python_files/ModulesSetup Modules/Setup.local
try cp $PRJROOT/src/python_files/_scproxy.py Lib/_scproxy.py

echo "Building for native machine ============================================"

try ./configure CC="clang -Qunused-arguments -fcolor-diagnostics" LDFLAGS="-lsqlite3" CFLAGS="--sysroot=$MACSDKROOT"
try make -j4 python.exe Parser/pgen
try mv python.exe hostpython
try mv Parser/pgen Parser/hostpgen
try make distclean

echo "Building for iOS ======================================================="

# patch python to cross-compile
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-xcompile.patch
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-setuppath.patch

# set up environment variables for cross compilation
export CPP="$CCACHE /usr/bin/cpp $CPPFLAGS"

# Copy our setup for modules
try cp $PRJROOT/src/python_files/ModulesSetup Modules/Setup.local
try cat $PRJROOT/src/python_files/ModulesSetup-$CPU_ARCHITECTURE.mobile >> Modules/Setup.local
try cp $PRJROOT/src/python_files/_scproxy.py Lib/_scproxy.py

try ./configure CC="$ARM_CC" LD="$ARM_LD" \
    CFLAGS="$ARM_CFLAGS" \
    LDFLAGS="$ARM_LDFLAGS -Lextralibs/ -lsqlite3 -L$BUILDROOT/lib -undefined dynamic_lookup" \
    --without-pymalloc \
    --disable-toolbox-glue \
    --host=armv7-apple-darwin \
    --prefix=/python \
    --without-doc-strings

# with undefined lookup, checks in configure just failed :(
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-pyconfig.patch
try patch -p1 < $PRJROOT/src/python_files/Python-$IOS_PYTHON_VERSION-ctypes_duplicate.${CPU_ARCHITECTURE}.patch

try make -j4 HOSTPYTHON=./hostpython HOSTPGEN=./Parser/hostpgen \
     CROSS_COMPILE_TARGET=yes

try make install HOSTPYTHON=./hostpython CROSS_COMPILE_TARGET=yes \
    prefix="$BUILDROOT/python"

try mv -f $BUILDROOT/python/lib/libpython2.7.a $BUILDROOT/lib/

deduplicate $BUILDROOT/lib/libpython2.7.a
