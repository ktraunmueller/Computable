/* id_rtrans_subr_6.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_random_transf00_(doublereal *x, doublereal *y, 
	integer *n, doublereal *albetas, integer *ixs)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal a, b;
    static integer i__, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform */
/*       required by routine idd_random_transf0 (please see the latter). */

/*        implement the permutation */

    /* Parameter adjustments */
    --ixs;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

	j = ixs[i__];
	y[i__] = x[j];
/* L1600: */
    }

/*        implement 2 \times 2 matrices */

    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {

	alpha = albetas[(i__ << 1) + 1];
	beta = albetas[(i__ << 1) + 2];

	a = y[i__];
	b = y[i__ + 1];

	y[i__] = alpha * a + beta * b;
	y[i__ + 1] = -beta * a + alpha * b;
/* L1800: */
    }

    return 0;
} /* idd_random_transf00__ */

