/* dfft_subr_4.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Subroutine */ int zffti1_(integer *n, doublereal *wa, integer *ifac)
{
    /* Initialized data */

    static integer ntryh[4] = { 3,4,2,5 };

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, i1, k1, l1, l2, ib;
    static doublereal fi;
    static integer ld, ii, nf, ip, nl, nq, nr;
    static doublereal arg;
    static integer ido, ipm;
    static doublereal tpi, argh;
    static integer idot, ntry;
    static doublereal argld;

    /* Parameter adjustments */
    --ifac;
    --wa;

    /* Function Body */
    nl = *n;
    nf = 0;
    j = 0;
L101:
    ++j;
    if (j - 4 <= 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    ntry = ntryh[j - 1];
    goto L104;
L103:
    ntry += 2;
L104:
    nq = nl / ntry;
    nr = nl - ntry * nq;
    if (nr != 0) {
	goto L101;
    } else {
	goto L105;
    }
L105:
    ++nf;
    ifac[nf + 2] = ntry;
    nl = nq;
    if (ntry != 2) {
	goto L107;
    }
    if (nf == 1) {
	goto L107;
    }
    i__1 = nf;
    for (i__ = 2; i__ <= i__1; ++i__) {
	ib = nf - i__ + 2;
	ifac[ib + 2] = ifac[ib + 1];
/* L106: */
    }
    ifac[3] = 2;
L107:
    if (nl != 1) {
	goto L104;
    }
    ifac[1] = *n;
    ifac[2] = nf;
    tpi = 6.2831853071795864769252867665590057;
    argh = tpi / (doublereal) (*n);
    i__ = 2;
    l1 = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	ld = 0;
	l2 = l1 * ip;
	ido = *n / l2;
	idot = ido + ido + 2;
	ipm = ip - 1;
	i__2 = ipm;
	for (j = 1; j <= i__2; ++j) {
	    i1 = i__;
	    wa[i__ - 1] = 1.;
	    wa[i__] = 0.;
	    ld += l1;
	    fi = 0.;
	    argld = (doublereal) ld * argh;
	    i__3 = idot;
	    for (ii = 4; ii <= i__3; ii += 2) {
		i__ += 2;
		fi += 1.;
		arg = fi * argld;
		wa[i__ - 1] = cos(arg);
		wa[i__] = sin(arg);
/* L108: */
	    }
	    if (ip <= 5) {
		goto L109;
	    }
	    wa[i1 - 1] = wa[i__ - 1];
	    wa[i1] = wa[i__];
L109:
	    ;
	}
	l1 = l2;
/* L110: */
    }
    return 0;
} /* zffti1_ */

