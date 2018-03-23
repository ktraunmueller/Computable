/* id_rtrans_subr_11.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_random_transf_init00_(integer *n, doublereal *
	albetas, integer *ixs)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer ifrepeat;
    static doublereal d__;
    static integer i__;
    extern /* Subroutine */ int id_randperm_(integer *, integer *);


/*       constructs one stage of the random transform */
/*       initialized by routine idd_random_transf_init0 */
/*       (please see the latter). */

/*        construct the random permutation */

    /* Parameter adjustments */
    --ixs;
    albetas -= 3;

    /* Function Body */
    ifrepeat = 0;
    id_randperm_(n, &ixs[1]);

/*        construct the random variables */

    i__1 = *n << 1;
    id_srand_(&i__1, &albetas[3]);

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {

	albetas[(i__ << 1) + 1] = albetas[(i__ << 1) + 1] * 2 - 1;
	albetas[(i__ << 1) + 2] = albetas[(i__ << 1) + 2] * 2 - 1;
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
    return 0;
} /* idd_random_transf_init00__ */

