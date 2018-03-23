/* idzp_aid_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzp_aid0_(doublereal *eps, integer *m, integer *n, 
	doublecomplex *a, integer *krank, integer *list, doublecomplex *proj, 
	doublereal *rnorms)
{
    /* System generated locals */
    integer a_dim1, a_offset, proj_dim1, proj_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int idzp_id_(doublereal *, integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublereal *);


/*       uses routine idzp_id to ID a without modifying its entries */
/*       (in contrast to the usual behavior of idzp_id). */

/*       input: */
/*       eps -- precision of the decomposition to be constructed */
/*       m -- first dimension of a */
/*       n -- second dimension of a */

/*       output: */
/*       krank -- numerical rank of the ID */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns in a; */
/*               proj doubles as a work array in the present routine, so */
/*               must be at least m*n complex*16 elements long */

/*       work: */
/*       rnorms -- must be at least n real*8 elements long */

/*       _N.B._: proj must be at least m*n complex*16 elements long */



/*       Copy a into proj. */

    /* Parameter adjustments */
    --rnorms;
    proj_dim1 = *m;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = j + k * proj_dim1;
	    i__4 = j + k * a_dim1;
	    proj[i__3].r = a[i__4].r, proj[i__3].i = a[i__4].i;
	}
/* j */
    }


/*       ID proj. */

/* k */
    idzp_id_(eps, m, n, &proj[proj_offset], krank, &list[1], &rnorms[1]);


    return 0;
} /* idzp_aid0__ */

