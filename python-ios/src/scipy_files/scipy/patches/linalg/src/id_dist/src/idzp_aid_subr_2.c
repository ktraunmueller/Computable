/* idzp_aid_subr_2.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzp_aid1_(doublereal *eps, integer *n2, integer *n, 
	integer *kranki, doublecomplex *proj, integer *krank, integer *list, 
	doublereal *rnorms)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int idzp_id_(doublereal *, integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublereal *);


/*       IDs the uppermost kranki x n block of the n2 x n matrix */
/*       input as proj. */

/*       input: */
/*       eps -- precision of the decomposition to be constructed */
/*       n2 -- first dimension of proj as input */
/*       n -- second dimension of proj as input */
/*       kranki -- number of rows to extract from proj */
/*       proj -- matrix containing the kranki x n block to be ID'd */

/*       output: */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd */
/*       krank -- numerical rank of the ID */
/*       list -- indices of the columns in the ID */

/*       work: */
/*       rnorms -- must be at least n real*8 elements long */



/*       Move the uppermost kranki x n block of the n2 x n matrix proj */
/*       to the beginning of proj. */

    /* Parameter adjustments */
    --rnorms;
    --list;
    --proj;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *kranki;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = j + *kranki * (k - 1);
	    i__4 = j + *n2 * (k - 1);
	    proj[i__3].r = proj[i__4].r, proj[i__3].i = proj[i__4].i;
	}
/* j */
    }


/*       ID proj. */

/* k */
    idzp_id_(eps, kranki, n, &proj[1], krank, &list[1], &rnorms[1]);


    return 0;
} /* idzp_aid1__ */

