/* dfft_subr_13.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int dffti_(integer *n, doublereal *wsave)
{
    extern /* Subroutine */ int dffti1_(integer *, doublereal *, doublereal *)
	    ;

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    dffti1_(n, &wsave[*n + 1], &wsave[(*n << 1) + 1]);
    return 0;
} /* dffti_ */

/* Subroutine */ int dsinqb_(integer *n, doublereal *x, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k, kc, ns2;
    static doublereal xhold;
    extern /* Subroutine */ int dcosqb_(integer *, doublereal *, doublereal *)
	    ;

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    if (*n > 1) {
	goto L101;
    }
    x[1] *= 4.;
    return 0;
L101:
    ns2 = *n / 2;
    i__1 = *n;
    for (k = 2; k <= i__1; k += 2) {
	x[k] = -x[k];
/* L102: */
    }
    dcosqb_(n, &x[1], &wsave[1]);
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = *n - k;
	xhold = x[k];
	x[k] = x[kc + 1];
	x[kc + 1] = xhold;
/* L103: */
    }
    return 0;
} /* dsinqb_ */

/* Subroutine */ int dsinqf_(integer *n, doublereal *x, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k, kc, ns2;
    static doublereal xhold;
    extern /* Subroutine */ int dcosqf_(integer *, doublereal *, doublereal *)
	    ;

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    ns2 = *n / 2;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = *n - k;
	xhold = x[k];
	x[k] = x[kc + 1];
	x[kc + 1] = xhold;
/* L101: */
    }
    dcosqf_(n, &x[1], &wsave[1]);
    i__1 = *n;
    for (k = 2; k <= i__1; k += 2) {
	x[k] = -x[k];
/* L102: */
    }
    return 0;
} /* dsinqf_ */

/* Subroutine */ int dsinqi_(integer *n, doublereal *wsave)
{
    extern /* Subroutine */ int dcosqi_(integer *, doublereal *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    dcosqi_(n, &wsave[1]);
    return 0;
} /* dsinqi_ */

