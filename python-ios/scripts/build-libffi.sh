#!/bin/bash

echo "Building libffi ============================="

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/libffi-$FFI_VERSION.tar.gz ]; then
    try curl -L ftp://sourceware.org/pub/libffi/libffi-$FFI_VERSION.tar.gz > $CACHEROOT/libffi-$FFI_VERSION.tar.gz 
fi

if [ ! -d $TMPROOT/libffi-$FFI_VERSION ]; then
    try tar xvf $CACHEROOT/libffi-$FFI_VERSION.tar.gz
    try mv libffi-$FFI_VERSION $TMPROOT

    pushd $TMPROOT/libffi-$FFI_VERSION
    try patch -p1 < $PRJROOT/src/ffi_files/ffi-$FFI_VERSION-sysv.S.patch
    popd
fi

pushd $TMPROOT/libffi-$FFI_VERSION
try xcodebuild  $XCODEBUILD_FLAGS  OTHER_CFLAGS="-no-integrated-as" -project libffi.xcodeproj -target "libffi iOS" 
try cp build/Release-${TARGET_SDK}/libffi.a $BUILDROOT/lib/libffi.a
try cp -a build/Release-${TARGET_SDK}/usr/local/include $BUILDROOT/include/ffi
popd
