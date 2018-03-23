Welcome
=======

libzmq iOS Universal (fat) static library compilation script.

This script creates a static library for libzmq (ZeroMQ) for iOS development,
by creating a single universal (fat) static library supporting armv7, armv7s and
i386 architectures.


Requirements
============

- libzmq 3.2.2 or later, plus libzmq required libraries
  listed on [ZeroMQ download page](http://www.zeromq.org/area:download)
- [Xcode 4.3.3](https://developer.apple.com/xcode/) or later, to discover your 
  version type; "xcodebuild -version"


Usage
=====
To get help using this shell script, run the script with the help flag;

```
./compile_libzmq_ios_universal -h
```

Example
=======

These instructions will create an iOS Universal (fat) static library for libzmq.

Clone this repository to a location a Mac OS X system with Xcode 4.3.3 or later
installed.

```
git clone https://github.com/samsoir/libzmq-ios-universal.git
```

```cd``` into the directory that libzmq-ios-universal was cloned to.

Obtain the _libzmq_ stable release for 3.2 or later, available from the
[ZeroMQ download page](http://www.zeromq.org/area:download). Ensure you download
POSIX tarball version.

To download libzmq 3.2.2 and untar it, use the following command;

```wget http://download.zeromq.org/zeromq-3.2.2.tar.gz && tar xfz zeromq-3.2.2.tar.gz```

Compile the unversal (fat) static library using the following command;

```
./compile_libzmq_ios_universal
```

Once compilation is complete, instructions on where the compiled binary and
header files are printed to the screen.

If there is an error, check the build logs located in build/build.log


Credits
=======
Credits can be found in the CREDITS file


License
=======
This project is licensed with the ISC License. Full terms and conditions are
available in the LICENSE file.
