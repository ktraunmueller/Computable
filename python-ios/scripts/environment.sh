#!/bin/bash

try () {
	"$@" || exit -1
}

# version of packages
export IOS_PYTHON_VERSION=2.7.1
export FFI_VERSION=3.0.13
export PYZMQ_VERSION=14.0.1
export NUMPY_VERSION=1.8.0
export SCIPY_VERSION=0.13.2
export PANDAS_VERSION=0.13.1
export FT_VERSION=2.5.3
export PNG_VERSION=1.6.9
export MATPLOTLIB_VERSION=1.3.1
export BZ2_VERSION=1.0.6

# Xcode doesn't include /usr/local/bin
export PATH="$PATH:/usr/local/bin"

# ensure byte-compiling is working
export PYTHONDONTWRITEBYTECODE=

# set TARGET_SDK to iphoneos by default if not specified
if [ "X$TARGET_SDK" == "X" ]; then
        export TARGET_SDK="iphoneos"
fi

# set architechuture based on which target we compiling for
export CPU_ARCHITECTURE="armv7"
if [ "X$TARGET_SDK" == "Xiphonesimulator" ]; then
        export CPU_ARCHITECTURE="i386"
fi

# where our build is located
export PRJROOT="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )/../" && pwd )"
export BUILDROOT="$PRJROOT/build/$CPU_ARCHITECTURE"
export TMPROOT="$PRJROOT/tmp/$CPU_ARCHITECTURE"
export DESTROOT="$PRJROOT/tmp/root/$CPU_ARCHITECTURE"
export CACHEROOT="$PRJROOT/.cache/$CPU_ARCHITECTURE"

# iOS SDK Environmnent (don't use name "SDKROOT"!!! it will break the compilation)
export IOSSDKVER=`xcrun --sdk $TARGET_SDK --show-sdk-version`
export IOSSDKROOT=`xcrun --sdk $TARGET_SDK --show-sdk-path`
export MACSDKVER=`xcrun --sdk macosx --show-sdk-version`
export MACSDKROOT=`xcrun --sdk macosx --show-sdk-path`

#find the right compiler and linker
export ARM_CC=$(xcrun -find -sdk $TARGET_SDK clang)
export ARM_CXX=$(xcrun -find -sdk $TARGET_SDK clang++)
export ARM_CPP=$(xcrun -find -sdk $TARGET_SDK clang++)
export ARM_AR=$(xcrun -find -sdk $TARGET_SDK ar)
export ARM_LD=$(xcrun -find -sdk $TARGET_SDK ld)

export MAC_CC=$(xcrun -find -sdk macosx clang)
export MAC_CXX=$(xcrun -find -sdk macosx clang++)
export MAC_CPP=$(xcrun -find -sdk macosx clang++)
export MAC_AR=$(xcrun -find -sdk macosx ar)
export MAC_LD=$(xcrun -find -sdk macosx ld)

export CCACHE=$(which ccache)

# compiler flags
export ARM_CFLAGS="-arch $CPU_ARCHITECTURE"
export ARM_CFLAGS="$ARM_CFLAGS -pipe -no-cpp-precomp"
export ARM_CFLAGS="$ARM_CFLAGS -isysroot $IOSSDKROOT"
export ARM_CFLAGS="$ARM_CFLAGS -miphoneos-version-min=$IOSSDKVER"
export ARM_CFLAGS="$ARM_CFLAGS -O3"
#export ARM_CFLAGS="$ARM_CFLAGS -O0 -g"
export ARM_CXXFLAGS="$ARM_CFLAGS"
export ARM_CPPFLAGS="$ARM_CXXFLAGS"

export MAC_CFLAGS="-arch x86_64"
export MAC_CFLAGS="$MAC_CFLAGS -pipe -no-cpp-precomp"
export MAC_CFLAGS="$MAC_CFLAGS -isysroot $MACSDKROOT"
export MAC_CFLAGS="$MAC_CFLAGS -mmacosx-version-min=$MACSDKVER"
export MAC_CFLAGS="$MAC_CFLAGS -O3"
export MAC_CXXFLAGS="$MAC_CFLAGS"
export MAC_CPPFLAGS="$MAC_CXXFLAGS"

# linker flags
export ARM_LDFLAGS="-arch $CPU_ARCHITECTURE -isysroot $IOSSDKROOT"
export ARM_LDFLAGS="$ARM_LDFLAGS -miphoneos-version-min=$IOSSDKVER"

export MAC_LDFLAGS="-arch x86_64 -isysroot $MACSDKROOT"
export MAC_LDFLAGS="$MAC_LDFLAGS -mmacosx-version-min=$MACSDKVER"

export MACOSX_DEPLOYMENT_TARGET=

# general xcode build settings
export XCODEBUILD_FLAGS="ONLY_ACTIVE_ARCH=NO ARCHS=$CPU_ARCHITECTURE -configuration Release -sdk ${TARGET_SDK} --arch=$CPU_ARCHITECTURE"

# Python & Cython
export HOSTPYTHON="$TMPROOT/Python-$IOS_PYTHON_VERSION/hostpython"
export USR_PYTHON=$(which python)
export USR_PYTHON_SITE_PACKAGES="/usr/local/lib/python2.7/site-packages"

for fn in cython-2.7 cython; do
	export CYTHON=$(which $fn)
	if [ "X$CYTHON" != "X" ]; then
		break
	fi
done
if [ "X$CYTHON" == "X" ]; then
	echo
	echo "Cython not found !"
	echo "Ensure your PATH contain access to 'cython' or 'cython-2.7'"
	echo
	echo "Current PATH: $PATH"
	echo
fi

# check basic tools
CONFIGURATION_OK=1
for tool in pkg-config autoconf automake libtool hg; do
	if [ "X$(which $tool)" == "X" ]; then
		echo "Missing requirement: $tool is not installed !"
		CONFIGURATION_OK=0
	fi
done
if [ $CONFIGURATION_OK -eq 0 ]; then
	echo "Install those requirements first, then restart the script."
	exit 1
fi

# now do some setup...

# pkg-config for SDL and futures
try mkdir -p $BUILDROOT/pkgconfig
export PKG_CONFIG_PATH="$BUILDROOT/pkgconfig:$PKG_CONFIG_PATH"

# create build directory if not found
try mkdir -p $BUILDROOT
try mkdir -p $BUILDROOT/include
try mkdir -p $BUILDROOT/lib
try mkdir -p $CACHEROOT
try mkdir -p $TMPROOT
try mkdir -p $DESTROOT

# one method to deduplicate some symbol in libraries
function deduplicate() {
	fn=$(basename $1)
	echo "== Trying to remove duplicate symbol in $1"
	try mkdir ddp
	try cd ddp
	try ar x $1
	try ar rc $fn *.o
	try ranlib $fn
	try mv -f $fn $1
	try cd ..
	try rm -rf ddp
}
