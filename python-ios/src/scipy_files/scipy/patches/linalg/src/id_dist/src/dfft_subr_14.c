/* dfft_subr_14.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int dsint1_(integer *n, doublereal *war, doublereal *was, 
	doublereal *xh, doublereal *x, integer *ifac)
{
    /* Initialized data */

    static doublereal sqrt3 = 1.7320508075688772935274463415058723;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k;
    static doublereal t1, t2;
    static integer kc, np1, ns2, modn;
    static doublereal xhold;
    extern /* Subroutine */ int dfftf1_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *);

    /* Parameter adjustments */
    --ifac;
    --x;
    --xh;
    --was;
    --war;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xh[i__] = war[i__];
	war[i__] = x[i__];
/* L100: */
    }
    if ((i__1 = *n - 2) < 0) {
	goto L101;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L101:
    xh[1] += xh[1];
    goto L106;
L102:
    xhold = sqrt3 * (xh[1] + xh[2]);
    xh[2] = sqrt3 * (xh[1] - xh[2]);
    xh[1] = xhold;
    goto L106;
L103:
    np1 = *n + 1;
    ns2 = *n / 2;
    x[1] = 0.;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = np1 - k;
	t1 = xh[k] - xh[kc];
	t2 = was[k] * (xh[k] + xh[kc]);
	x[k + 1] = t1 + t2;
	x[kc + 1] = t2 - t1;
/* L104: */
    }
    modn = *n % 2;
    if (modn != 0) {
	x[ns2 + 2] = xh[ns2 + 1] * 4.;
    }
    dfftf1_(&np1, &x[1], &xh[1], &war[1], &ifac[1]);
    xh[1] = x[1] * .5;
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	xh[i__ - 1] = -x[i__];
	xh[i__] = xh[i__ - 2] + x[i__ - 1];
/* L105: */
    }
    if (modn != 0) {
	goto L106;
    }
    xh[*n] = -x[*n + 1];
L106:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	x[i__] = war[i__];
	war[i__] = xh[i__];
/* L107: */
    }
    return 0;
} /* dsint1_ */

