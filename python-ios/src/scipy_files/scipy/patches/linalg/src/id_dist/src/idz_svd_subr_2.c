/* idz_svd_subr_2.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_permuter_(integer *krank, integer *ind, integer *m, 
	integer *n, doublecomplex *a)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer j, k;
    static doublecomplex cswap;


/*       permutes the columns of a according to ind obtained */
/*       from routine idzr_qrpiv or idzp_qrpiv, assuming that */
/*       a = q r from idzr_qrpiv or idzp_qrpiv. */

/*       input: */
/*       krank -- rank specified to routine idzr_qrpiv */
/*                or obtained from routine idzp_qrpiv */
/*       ind -- indexing array obtained from routine idzr_qrpiv */
/*              or idzp_qrpiv */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       a -- matrix to be rearranged */

/*       output: */
/*       a -- rearranged matrix */



    /* Parameter adjustments */
    --ind;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    for (k = *krank; k >= 1; --k) {
	i__1 = *m;
	for (j = 1; j <= i__1; ++j) {

	    i__2 = j + k * a_dim1;
	    cswap.r = a[i__2].r, cswap.i = a[i__2].i;
	    i__2 = j + k * a_dim1;
	    i__3 = j + ind[k] * a_dim1;
	    a[i__2].r = a[i__3].r, a[i__2].i = a[i__3].i;
	    i__2 = j + ind[k] * a_dim1;
	    a[i__2].r = cswap.r, a[i__2].i = cswap.i;

	}
/* j */
    }


/* k */
    return 0;
} /* idz_permuter__ */

