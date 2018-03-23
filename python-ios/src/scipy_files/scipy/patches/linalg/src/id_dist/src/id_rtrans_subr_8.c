/* id_rtrans_subr_8.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_random_transf_init00_(integer *n, doublereal *
	albetas, doublereal *gammas, integer *ixs)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer ifrepeat;
    static doublereal d__;
    static integer i__;
    extern /* Subroutine */ int id_randperm_(integer *, integer *);
    static doublereal done, twopi;


/*       constructs one stage of the random transform */
/*       initialized by routine idz_random_transf_init0 */
/*       (please see the latter). */

    /* Parameter adjustments */
    --ixs;
    --gammas;
    albetas -= 3;

    /* Function Body */
    done = 1.;
    twopi = atan(done) * 8;

/*        construct the random permutation */

    ifrepeat = 0;
    id_randperm_(n, &ixs[1]);

/*        construct the random variables */

    i__1 = *n << 1;
    id_srand_(&i__1, &albetas[3]);
    i__1 = *n << 1;
    id_srand_(&i__1, &gammas[1]);

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

	albetas[(i__ << 1) + 1] = albetas[(i__ << 1) + 1] * 2 - 1;
	albetas[(i__ << 1) + 2] = albetas[(i__ << 1) + 2] * 2 - 1;
	gammas[(i__ << 1) - 1] = gammas[(i__ << 1) - 1] * 2 - 1;
	gammas[i__ * 2] = gammas[i__ * 2] * 2 - 1;
/* L1300: */
    }

/*        construct the random 2 \times 2 transformations */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

/* Computing 2nd power */
	d__1 = albetas[(i__ << 1) + 1];
/* Computing 2nd power */
	d__2 = albetas[(i__ << 1) + 2];
	d__ = d__1 * d__1 + d__2 * d__2;
	d__ = 1 / sqrt(d__);
	albetas[(i__ << 1) + 1] *= d__;
	albetas[(i__ << 1) + 2] *= d__;
/* L1400: */
    }

/*        construct the random multipliers on the unit circle */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

/* Computing 2nd power */
	d__1 = gammas[(i__ << 1) - 1];
/* Computing 2nd power */
	d__2 = gammas[i__ * 2];
	d__ = d__1 * d__1 + d__2 * d__2;
	d__ = 1 / sqrt(d__);

/*        fill the real part */

	gammas[(i__ << 1) - 1] *= d__;

/*        fill the imaginary part */

	gammas[i__ * 2] *= d__;
/* L1500: */
    }

    return 0;
} /* idz_random_transf_init00__ */

