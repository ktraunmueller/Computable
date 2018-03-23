/* id_rtrans_subr_4.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_random_transf00_inv_(doublecomplex *x, 
	doublecomplex *y, integer *n, doublereal *albetas, doublecomplex *
	gammas, integer *ixs)
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static doublecomplex a, b;
    static integer i__, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform */
/*       required by routine idz_random_transf0_inv */
/*       (please see the latter). */

/*        implement 2 \times 2 matrices */

    /* Parameter adjustments */
    --ixs;
    --gammas;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    for (i__ = *n - 1; i__ >= 1; --i__) {

	alpha = albetas[(i__ << 1) + 1];
	beta = albetas[(i__ << 1) + 2];

	i__1 = i__;
	a.r = x[i__1].r, a.i = x[i__1].i;
	i__1 = i__ + 1;
	b.r = x[i__1].r, b.i = x[i__1].i;

	i__1 = i__;
	z__2.r = alpha * a.r, z__2.i = alpha * a.i;
	z__3.r = beta * b.r, z__3.i = beta * b.i;
	z__1.r = z__2.r - z__3.r, z__1.i = z__2.i - z__3.i;
	x[i__1].r = z__1.r, x[i__1].i = z__1.i;
	i__1 = i__ + 1;
	z__2.r = beta * a.r, z__2.i = beta * a.i;
	z__3.r = alpha * b.r, z__3.i = alpha * b.i;
	z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
	x[i__1].r = z__1.r, x[i__1].i = z__1.i;
/* L1600: */
    }

/*        implement the permutation */
/*        and divide by the random numbers on the unit circle */
/*        (or, equivalently, multiply by their conjugates) */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

	j = ixs[i__];
	i__2 = j;
	i__3 = i__;
	d_cnjg(&z__2, &gammas[i__]);
	z__1.r = x[i__3].r * z__2.r - x[i__3].i * z__2.i, z__1.i = x[i__3].r *
		 z__2.i + x[i__3].i * z__2.r;
	y[i__2].r = z__1.r, y[i__2].i = z__1.i;
/* L1800: */
    }

    return 0;
} /* idz_random_transf00_inv__ */

