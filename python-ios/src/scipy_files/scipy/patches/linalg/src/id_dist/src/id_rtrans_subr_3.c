/* id_rtrans_subr_3.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_random_transf0_inv_(integer *nsteps, doublecomplex *
	x, doublecomplex *y, integer *n, doublecomplex *w2, doublereal *
	albetas, doublecomplex *gammas, integer *iixs)
{
    /* System generated locals */
    integer gammas_dim1, gammas_offset, albetas_dim2, albetas_offset, 
	    iixs_dim1, iixs_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, ijk;
    extern /* Subroutine */ int idz_random_transf00_inv_(doublecomplex *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *, 
	    integer *);


/*       routine idz_random_transf_inverse serves as a memory wrapper */
/*       for the present routine; please see routine */
/*       idz_random_transf_inverse for documentation. */

    /* Parameter adjustments */
    --x;
    --y;
    iixs_dim1 = *n;
    iixs_offset = 1 + iixs_dim1;
    iixs -= iixs_offset;
    gammas_dim1 = *n;
    gammas_offset = 1 + gammas_dim1;
    gammas -= gammas_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;
    --w2;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

	i__2 = i__;
	i__3 = i__;
	w2[i__2].r = x[i__3].r, w2[i__2].i = x[i__3].i;
/* L1200: */
    }

    for (ijk = *nsteps; ijk >= 1; --ijk) {

	idz_random_transf00_inv_(&w2[1], &y[1], n, &albetas[(ijk * 
		albetas_dim2 + 1 << 1) + 1], &gammas[ijk * gammas_dim1 + 1], &
		iixs[ijk * iixs_dim1 + 1]);

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {

	    i__2 = j;
	    i__3 = j;
	    w2[i__2].r = y[i__3].r, w2[i__2].i = y[i__3].i;
/* L1400: */
	}
/* L2000: */
    }

    return 0;
} /* idz_random_transf0_inv__ */

