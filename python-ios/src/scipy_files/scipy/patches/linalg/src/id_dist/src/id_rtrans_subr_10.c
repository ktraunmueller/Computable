/* id_rtrans_subr_10.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_random_transf_init0_(integer *nsteps, integer *n, 
	doublereal *albetas, integer *ixs)
{
    /* System generated locals */
    integer albetas_dim2, albetas_offset, ixs_dim1, ixs_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int idd_random_transf_init00_(integer *, 
	    doublereal *, integer *);
    static integer ijk;


/*       routine idd_random_transf_init serves as a memory wrapper */
/*       for the present routine; please see routine */
/*       idd_random_transf_init for documentation. */

    /* Parameter adjustments */
    ixs_dim1 = *n;
    ixs_offset = 1 + ixs_dim1;
    ixs -= ixs_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;

    /* Function Body */
    i__1 = *nsteps;
    for (ijk = 1; ijk <= i__1; ++ijk) {

	idd_random_transf_init00_(n, &albetas[(ijk * albetas_dim2 + 1 << 1) 
		+ 1], &ixs[ijk * ixs_dim1 + 1]);
/* L2000: */
    }
    return 0;
} /* idd_random_transf_init0__ */

