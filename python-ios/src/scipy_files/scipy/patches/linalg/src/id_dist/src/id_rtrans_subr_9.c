/* id_rtrans_subr_9.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_random_transf0_(integer *nsteps, doublecomplex *x, 
	doublecomplex *y, integer *n, doublecomplex *w2, doublereal *albetas, 
	doublecomplex *gammas, integer *iixs)
{
    /* System generated locals */
    integer gammas_dim1, gammas_offset, albetas_dim2, albetas_offset, 
	    iixs_dim1, iixs_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int idz_random_transf00_(doublecomplex *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *, 
	    integer *);
    static integer ijk;


/*       routine idz_random_transf serves as a memory wrapper */
/*       for the present routine; please see routine idz_random_transf */
/*       for documentation. */

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

    i__1 = *nsteps;
    for (ijk = 1; ijk <= i__1; ++ijk) {

	idz_random_transf00_(&w2[1], &y[1], n, &albetas[(ijk * albetas_dim2 
		+ 1 << 1) + 1], &gammas[ijk * gammas_dim1 + 1], &iixs[ijk * 
		iixs_dim1 + 1]);
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {

	    i__3 = j;
	    i__4 = j;
	    w2[i__3].r = y[i__4].r, w2[i__3].i = y[i__4].i;
/* L1400: */
	}
/* L2000: */
    }

    return 0;
} /* idz_random_transf0__ */






/* Subroutine */ int idz_random_transf00_(doublecomplex *x, doublecomplex *y,
	 integer *n, doublereal *albetas, doublecomplex *gammas, integer *ixs)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublereal d__1;
    doublecomplex z__1, z__2, z__3;

    /* Local variables */
    static doublecomplex a, b;
    static integer i__, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform */
/*       required by routine idz_random_transf0 (please see the latter). */

/*        implement the permutation */
/*        and multiply by the random numbers */
/*        on the unit circle */

    /* Parameter adjustments */
    --ixs;
    --gammas;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

	j = ixs[i__];
	i__2 = i__;
	i__3 = j;
	i__4 = i__;
	z__1.r = x[i__3].r * gammas[i__4].r - x[i__3].i * gammas[i__4].i, 
		z__1.i = x[i__3].r * gammas[i__4].i + x[i__3].i * gammas[i__4]
		.r;
	y[i__2].r = z__1.r, y[i__2].i = z__1.i;
/* L1600: */
    }

/*        implement 2 \times 2 matrices */

    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {

	alpha = albetas[(i__ << 1) + 1];
	beta = albetas[(i__ << 1) + 2];

	i__2 = i__;
	a.r = y[i__2].r, a.i = y[i__2].i;
	i__2 = i__ + 1;
	b.r = y[i__2].r, b.i = y[i__2].i;

	i__2 = i__;
	z__2.r = alpha * a.r, z__2.i = alpha * a.i;
	z__3.r = beta * b.r, z__3.i = beta * b.i;
	z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
	y[i__2].r = z__1.r, y[i__2].i = z__1.i;
	i__2 = i__ + 1;
	d__1 = -beta;
	z__2.r = d__1 * a.r, z__2.i = d__1 * a.i;
	z__3.r = alpha * b.r, z__3.i = alpha * b.i;
	z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
	y[i__2].r = z__1.r, y[i__2].i = z__1.i;
/* L2600: */
    }

    return 0;
} /* idz_random_transf00__ */

