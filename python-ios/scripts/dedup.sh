#!/bin/bash

try () {
	"$@" || exit -1
}

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

deduplicate ~/Documents/sandbox/ios/python-ios/build/armv7/lib/libnumpy.a
deduplicate ~/Documents/sandbox/ios/python-ios/build/i386/lib/libnumpy.a

lipo -create \
    "~/Documents/sandbox/ios/python-ios/build/armv7/lib/libnumpy.a" \
    "~/Documents/sandbox/ios/python-ios/build/i386/lib/libnumpy.a" \
    -output "~/Documents/sandbox/ios/python-ios/build/universal/lib/libnumpy.a"
echo "Checking fat output file"
lipo -info ~/Documents/sandbox/ios/python-ios/build/universal/lib/libnumpy.a