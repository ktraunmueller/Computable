/* idzp_aid_subr_5.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_transposer_(integer *m, integer *n, doublecomplex *
	a, doublecomplex *at)
{
    /* System generated locals */
    integer a_dim1, a_offset, at_dim1, at_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       transposes a to obtain at. */

/*       input: */
/*       m -- first dimension of a, and second dimension of at */
/*       n -- second dimension of a, and first dimension of at */
/*       a -- matrix to be transposed */

/*       output: */
/*       at -- transpose of a */



    /* Parameter adjustments */
    at_dim1 = *n;
    at_offset = 1 + at_dim1;
    at -= at_offset;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {

	    i__3 = k + j * at_dim1;
	    i__4 = j + k * a_dim1;
	    at[i__3].r = a[i__4].r, at[i__3].i = a[i__4].i;

	}
/* j */
    }


/* k */
    return 0;
} /* idz_transposer__ */

