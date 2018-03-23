#!/bin/bash

echo "== Building f2c ============================="

. $(dirname $0)/environment.sh

if [ ! -f $PRJROOT/.cache/f2c.tar ] ; then
    try curl -L http://netlib.sandia.gov/cgi-bin/netlib/netlibfiles.tar?filename=netlib/f2c > $PRJROOT/.cache/f2c.tar

    try mkdir $PRJROOT/.cache/f2c-src
    files=( cds.c data.c defines.h defs.h equiv.c error.c exec.c expr.c f2c.1 f2c.1t f2c.h format.c format.h formatdata.c ftypes.h gram.c gram.dcl gram.exec gram.expr gram.head gram.io init.c intr.c io.c iob.h lex.c machdefs.h main.c makefile.u makefile.vc malloc.c mem.c memset.c misc.c mkfile.plan9 names.c names.h niceprintf.c niceprintf.h notice output.c output.h p1defs.h p1output.c parse.h parse_args.c pccdefs.h pread.c proc.c put.c putpcc.c sysdep.c sysdep.h sysdeptest.c tokens tokdefs.h usignal.h vax.c version.c xsum.c xsum0.out Notice README readme )
    for f in "${files[@]}"
    do
        try curl -L http://www.netlib.org/f2c/src/$f > $PRJROOT/.cache/f2c-src/$f
    done
fi

# TODO remove
rm -rf $TMPROOT/f2c

# prepare sources
if [ ! -d $TMPROOT/f2c ]; then
    try tar xvf $PRJROOT/.cache/f2c.tar
    try mv f2c $TMPROOT
    try cp -R $PRJROOT/.cache/f2c-src $TMPROOT/f2c/src

    try mkdir $TMPROOT/f2c/libf2c
    try cp $TMPROOT/f2c/libf2c.zip $TMPROOT/f2c/libf2c
    pushd $TMPROOT/f2c/libf2c
    try unzip libf2c.zip
    try cp $PRJROOT/src/f2c_files/makefile.u .
    try cp $PRJROOT/src/f2c_files/arith.h .
    popd
fi

INSTALL=/usr/local

pushd $TMPROOT/f2c

# ===== host tool (f2c) =====

# libf2c (host)
pushd libf2c
try rm -rf include
try rm -rf lib
try cp makefile.u Makefile
try make clean
try make
try mkdir include
try mkdir lib
try mv f2c.h include
try mv libf2c.a lib
popd

OLD_CFLAGS="$CFLAGS"
OLD_LDFLAGS="$LDFLAGS"
export CFLAGS="-I$TMPROOT/f2c/libf2c/include -Wno-parentheses"
export LDFLAGS="-L$TMPROOT/f2c/libf2c/lib"

# f2c
pushd src
try cp makefile.u Makefile
try make
try mkdir -p $INSTALL/share/man/man1
try cp f2c.1t $INSTALL/share/man/man1
try cp f2c $INSTALL/bin/f77
chmod +x $INSTALL/bin/f77
popd

export CFLAGS="$OLD_CFLAGS"
export LDFLAGS="$OLD_LDFLAGS"

# ===== target support library (libf2c) =====

OLD_CC="$CC"
OLD_CFLAGS="$CFLAGS"
OLD_LD="$LD"
OLD_AR="$AR"
OLD_LDFLAGS="$LDFLAGS"
OLD_LDSHARED="$LDSHARED"

export CC="$ARM_CC"
export CFLAGS="$ARM_CFLAGS -Wno-parentheses"
export AR="$ARM_AR"
export LD="$ARM_LD"
export LDFLAGS="-arch $CPU_ARCHITECTURE"
export LDSHARED="$PRJROOT/tools/liblink"

# libf2c (target)
rm -rf libf2c
try mkdir libf2c
try cp libf2c.zip libf2c
pushd libf2c
try unzip libf2c.zip
try cp $PRJROOT/src/f2c_files/makefile.u .
# TODO arith.h might need tweaks (run arithchk.c on targets?)
try cp $PRJROOT/src/f2c_files/arith.h .
try cp makefile.u Makefile
try make
try mkdir -p $BUILDROOT/include/f2c
try cp f2c.h $BUILDROOT/include/f2c/
try cp libf2c.a $BUILDROOT/lib/
lipo -info $BUILDROOT/lib/libf2c.a
popd

popd

export CC="$OLD_CC"
export CFLAGS="$OLD_CFLAGS"
export AR="$OLD_AR"
export LDFLAGS="$OLD_LDFLAGS"
export LDSHARED="$OLD_LDSHARED"