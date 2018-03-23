/* dfft_subr_15.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int dsint_(integer *n, doublereal *x, doublereal *wsave)
{
    static integer np1, iw1, iw2, iw3;
    extern /* Subroutine */ int dsint1_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    np1 = *n + 1;
    iw1 = *n / 2 + 1;
    iw2 = iw1 + np1;
    iw3 = iw2 + np1;
    dsint1_(n, &x[1], &wsave[1], &wsave[iw1], &wsave[iw2], &wsave[iw3]);
    return 0;
} /* dsint_ */

/* Subroutine */ int dsinti_(integer *n, doublereal *wsave)
{
    /* Initialized data */

    static doublereal pi = 3.1415926535897932384626433832795028;

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double sin(doublereal);

    /* Local variables */
    static integer k;
    static doublereal dt;
    static integer np1, ns2;
    extern /* Subroutine */ int dffti_(integer *, doublereal *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n <= 1) {
	return 0;
    }
    ns2 = *n / 2;
    np1 = *n + 1;
    dt = pi / (doublereal) np1;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	wsave[k] = sin(k * dt) * 2.;
/* L101: */
    }
    dffti_(&np1, &wsave[ns2 + 1]);
    return 0;
} /* dsinti_ */

