/* dfft_subr_8.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int dfftb1_(integer *n, doublereal *c__, doublereal *ch, 
	doublereal *wa, integer *ifac)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k1, l1, l2, na, nf, ip, iw, ix2, ix3, ix4, ido, idl1;
    extern /* Subroutine */ int dradb2_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *), dradb3_(integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dradb4_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dradb5_(integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dradbg_(integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --ifac;
    --wa;
    --ch;
    --c__;

    /* Function Body */
    nf = ifac[2];
    na = 0;
    l1 = 1;
    iw = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	l2 = ip * l1;
	ido = *n / l2;
	idl1 = ido * l1;
	if (ip != 4) {
	    goto L103;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	if (na != 0) {
	    goto L101;
	}
	dradb4_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3]);
	goto L102;
L101:
	dradb4_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3]);
L102:
	na = 1 - na;
	goto L115;
L103:
	if (ip != 2) {
	    goto L106;
	}
	if (na != 0) {
	    goto L104;
	}
	dradb2_(&ido, &l1, &c__[1], &ch[1], &wa[iw]);
	goto L105;
L104:
	dradb2_(&ido, &l1, &ch[1], &c__[1], &wa[iw]);
L105:
	na = 1 - na;
	goto L115;
L106:
	if (ip != 3) {
	    goto L109;
	}
	ix2 = iw + ido;
	if (na != 0) {
	    goto L107;
	}
	dradb3_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2]);
	goto L108;
L107:
	dradb3_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2]);
L108:
	na = 1 - na;
	goto L115;
L109:
	if (ip != 5) {
	    goto L112;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	ix4 = ix3 + ido;
	if (na != 0) {
	    goto L110;
	}
	dradb5_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3], &wa[
		ix4]);
	goto L111;
L110:
	dradb5_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3], &wa[
		ix4]);
L111:
	na = 1 - na;
	goto L115;
L112:
	if (na != 0) {
	    goto L113;
	}
	dradbg_(&ido, &ip, &l1, &idl1, &c__[1], &c__[1], &c__[1], &ch[1], &ch[
		1], &wa[iw]);
	goto L114;
L113:
	dradbg_(&ido, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__[1], &c__[
		1], &wa[iw]);
L114:
	if (ido == 1) {
	    na = 1 - na;
	}
L115:
	l1 = l2;
	iw += (ip - 1) * ido;
/* L116: */
    }
    if (na == 0) {
	return 0;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = ch[i__];
/* L117: */
    }
    return 0;
} /* dfftb1_ */

