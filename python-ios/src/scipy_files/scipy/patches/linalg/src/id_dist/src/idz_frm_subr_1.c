/* idz_frm_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_permute__(integer *n, integer *ind, doublecomplex *x,
	 doublecomplex *y)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer k;


/*       copy the entries of x into y, rearranged according */
/*       to the permutation specified by ind. */

/*       input: */
/*       n -- length of ind, x, and y */
/*       ind -- permutation of n objects */
/*       x -- vector to be permuted */

/*       output: */
/*       y -- permutation of x */



    /* Parameter adjustments */
    --y;
    --x;
    --ind;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = ind[k];
	y[i__2].r = x[i__3].r, y[i__2].i = x[i__3].i;
    }


/* k */
    return 0;
} /* idz_permute__ */

