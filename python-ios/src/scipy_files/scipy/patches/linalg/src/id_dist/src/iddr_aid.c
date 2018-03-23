/* iddr_aid.f -- translated by f2c (version 20100827).
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

/*       this file contains the following user-callable routines: */


/*       routine iddr_aid computes the ID, to a specified rank, */
/*       of an arbitrary matrix. This routine is randomized. */

/*       routine iddr_aidi initializes routine iddr_aid. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int iddr_aid_(integer *m, integer *n, doublereal *a, 
	integer *krank, doublereal *w, integer *list, doublereal *proj)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int iddr_aid0_(integer *, integer *, doublereal *
	    , integer *, doublereal *, integer *, doublereal *, doublereal *);
    static integer ir, lr, iw, lw, lw2;


/*       computes the ID of the matrix a, i.e., lists in list */
/*       the indices of krank columns of a such that */

/*       a(j,list(k))  =  a(j,list(k)) */

/*       for all j = 1, ..., m; k = 1, ..., krank, and */

/*                       min(m,n,krank) */
/*       a(j,list(k))  =     Sigma      a(j,list(l)) * proj(l,k-krank)(*) */
/*                            l=1 */

/*                     +  epsilon(j,k-krank) */

/*       for all j = 1, ..., m; k = krank+1, ..., n, */

/*       for some matrix epsilon, dimensioned epsilon(m,n-krank), */
/*       whose norm is (hopefully) minimized by the pivoting procedure. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       a -- matrix to be ID'd; the present routine does not alter a */
/*       krank -- rank of the ID to be constructed */
/*       w -- initialization array that routine iddr_aidi */
/*            has constructed */

/*       output: */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd */

/*       _N.B._: The algorithm used by this routine is randomized. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Allocate memory in w. */

    /* Parameter adjustments */
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --proj;
    --w;

    /* Function Body */
    lw = 0;

    iw = lw + 1;
    lw2 = *m * 27 + 100 + *n;
    lw += lw2;

    ir = lw + 1;
    lr = (*krank + 8 << 1) * *n;
    lw += lr;


    iddr_aid0_(m, n, &a[a_offset], krank, &w[iw], &list[1], &proj[1], &w[ir])
	    ;


    return 0;
} /* iddr_aid__ */





/* Subroutine */ int iddr_aid0_(integer *m, integer *n, doublereal *a, 
	integer *krank, doublereal *w, integer *list, doublereal *proj, 
	doublereal *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, r_dim1, r_offset, proj_dim1, proj_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int idd_sfrm_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *);
    static integer k, l, n2, mn, lproj;
    extern /* Subroutine */ int iddr_copydarr_(integer *, doublereal *, 
	    doublereal *), iddr_id_(integer *, integer *, doublereal *, 
	    integer *, integer *, doublereal *);


/*       routine iddr_aid serves as a memory wrapper */
/*       for the present routine */
/*       (see iddr_aid for further documentation). */


/*       Please note that the second dimension of r is 2*n */
/*       (instead of n) so that if krank+8 >= m/2, then */
/*       we can copy the whole of a into r. */


/*       Retrieve the number of random test vectors */
/*       and the greatest integer less than m that is */
/*       a positive integer power of two. */

    /* Parameter adjustments */
    --list;
    --w;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    r_dim1 = *krank + 8;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;

    /* Function Body */
    l = (integer) w[1];
    n2 = (integer) w[2];


    if (l < n2 && l <= *m) {

/*         Apply the random matrix. */

	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    idd_sfrm_(&l, m, &n2, &w[11], &a[k * a_dim1 + 1], &r__[k * 
		    r_dim1 + 1]);
	}

/*         ID r. */

/* k */
	iddr_id_(&l, n, &r__[r_offset], krank, &list[1], &w[*m * 26 + 101]);

/*         Retrieve proj from r. */

	lproj = *krank * (*n - *krank);
	iddr_copydarr_(&lproj, &r__[r_offset], &proj[proj_offset]);

    }


    if (l >= n2 || l > *m) {

/*         ID a directly. */

	mn = *m * *n;
	iddr_copydarr_(&mn, &a[a_offset], &r__[r_offset]);
	iddr_id_(m, n, &r__[r_offset], krank, &list[1], &w[*m * 26 + 101]);

/*         Retrieve proj from r. */

	lproj = *krank * (*n - *krank);
	iddr_copydarr_(&lproj, &r__[r_offset], &proj[proj_offset]);

    }


    return 0;
} /* iddr_aid0__ */





/* Subroutine */ int iddr_copydarr_(integer *n, doublereal *a, doublereal *b)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;


/*       copies a into b. */

/*       input: */
/*       n -- length of a and b */
/*       a -- array to copy into b */

/*       output: */
/*       b -- copy of a */



    /* Parameter adjustments */
    --b;
    --a;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	b[k] = a[k];
    }


/* k */
    return 0;
} /* iddr_copydarr__ */





/* Subroutine */ int iddr_aidi_(integer *m, integer *n, integer *krank, 
	doublereal *w)
{
    static integer l;
    extern /* Subroutine */ int idd_sfrmi_(integer *, integer *, integer *, 
	    doublereal *);
    static integer n2;


/*       initializes the array w for using routine iddr_aid. */

/*       input: */
/*       m -- number of rows in the matrix to be ID'd */
/*       n -- number of columns in the matrix to be ID'd */
/*       krank -- rank of the ID to be constructed */

/*       output: */
/*       w -- initialization array for using routine iddr_aid */



/*       Set the number of random test vectors to 8 more than the rank. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    l = *krank + 8;
    w[1] = (doublereal) l;


/*       Initialize the rest of the array w. */

    n2 = 0;
    if (l <= *m) {
	idd_sfrmi_(&l, m, &n2, &w[11]);
    }
    w[2] = (doublereal) n2;


    return 0;
} /* iddr_aidi__ */

