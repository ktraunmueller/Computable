#!/bin/bash

. $(dirname $0)/environment.sh

if [ ! -f $CACHEROOT/png-$PNG_VERSION.tar.gz ]; then
	try curl -L http://prdownloads.sourceforge.net/libpng/libpng-$PNG_VERSION.tar.gz?download > $CACHEROOT/png-$PNG_VERSION.tar.gz
fi

if [ ! -d $TMPROOT/libpng-$PNG_VERSION ]; then
	try tar xvf $CACHEROOT/png-$PNG_VERSION.tar.gz
	try mv libpng-$PNG_VERSION $TMPROOT
fi

pushd $TMPROOT/libpng-$PNG_VERSION
try ./configure --prefix=$DESTROOT \
	--host=arm-apple-darwin \
	--enable-static=yes \
	--enable-shared=no \
	CC="$ARM_CC" AR="$ARM_AR" \
	LDFLAGS="$ARM_LDFLAGS" CFLAGS="$ARM_CFLAGS"
#try make clean
try make
try make install

# copy to buildroot
cp $DESTROOT/lib/libpng16.a $BUILDROOT/lib
cp -a $DESTROOT/include/libpng16 $BUILDROOT/include

popd
