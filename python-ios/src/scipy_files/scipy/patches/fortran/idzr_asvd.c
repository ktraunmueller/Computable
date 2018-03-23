/* idzr_asvd.f -- translated by f2c (version 20100827).
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


/*       routine idzr_aid computes the SVD, to a specified rank, */
/*       of an arbitrary matrix. This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idzr_asvd_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *w, doublecomplex *u, doublecomplex *v, 
	doublereal *s, integer *ier)
{
    /* System generated locals */
    integer a_dim1, a_offset, u_dim1, u_offset, v_dim1, v_offset, i_1;

    /* Local variables */
    extern /* Subroutine */ int idzr_asvd0_(integer *m, integer *n, doublecomplex *a,
                                             integer *krank, doublecomplex *winit, doublecomplex *u, doublecomplex
                                             *v, doublereal *s, integer *ier, integer *list, doublecomplex *proj,
                                             doublecomplex *col, doublecomplex *work);
    static integer lw, icol, lcol, iproj, ilist, lproj, llist, iwork, lwork, 
	    iwinit, lwinit;


/*       constructs a rank-krank SVD  u diag(s) v^*  approximating a, */
/*       where u is an m x krank matrix whose columns are orthonormal, */
/*       v is an n x krank matrix whose columns are orthonormal, */
/*       and diag(s) is a diagonal krank x krank matrix whose entries */
/*       are all nonnegative. This routine uses a randomized algorithm. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       a -- matrix to be decomposed; the present routine does not */
/*            alter a */
/*       krank -- rank of the SVD being constructed */
/*       w -- initialization array that routine idzr_aidi */
/*            has constructed (for use in the present routine, */
/*            w must be at least */
/*            (2*krank+22)*m+(6*krank+21)*n+8*krank**2+10*krank+90 */
/*            complex*16 elements long) */

/*       output: */
/*       u -- matrix of orthonormal left singular vectors of a */
/*       v -- matrix of orthonormal right singular vectors of a */
/*       s -- array of singular values of a */
/*       ier -- 0 when the routine terminates successfully; */
/*              nonzero otherwise */

/*       _N.B._: The algorithm used by this routine is randomized. */



/*       Allocate memory in w. */

/* Computing 2nd power */
    i_1 = *krank;
    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    --w;

    /* Function Body */
    lw = 0;

    iwinit = lw + 1;
    lwinit = ((*krank << 1) + 17) * *n + *m * 21 + 80;
    lw += lwinit;

    ilist = lw + 1;
    llist = *n;
    lw += llist;

    iproj = lw + 1;
    lproj = *krank * (*n - *krank);
    lw += lproj;

    icol = lw + 1;
    lcol = *m * *krank;
    lw += lcol;

    iwork = lw + 1;
/* Computing 2nd power */
    i_1 = *krank;
    lwork = (*krank + 1) * (*m + *n * 3 + 10) + i_1 * i_1 * 9;
    lw += lwork;


    idzr_asvd0_(m, n, &a[a_offset], krank, &w[iwinit], &u[u_offset], &v[
	    v_offset], &s[1], ier, &w[ilist], &w[iproj], &w[icol], &w[iwork]);


    return 0;
} /* idzr_asvd_ */





/* Subroutine */ int idzr_asvd0_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *winit, doublecomplex *u, doublecomplex 
	*v, doublereal *s, integer *ier, integer *list, doublecomplex *proj, 
	doublecomplex *col, doublecomplex *work)
{
    /* System generated locals */
    integer a_dim1, a_offset, u_dim1, u_offset, v_dim1, v_offset, proj_dim1, 
	    proj_offset, i_1;

    /* Local variables */
    extern /* Subroutine */ int idzr_aid_(integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, 
	    doublecomplex *), idz_id2svd_(integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublereal *, integer *, 
	    doublecomplex *), idz_copycols_(integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublecomplex *);


/*       routine idzr_asvd serves as a memory wrapper */
/*       for the present routine (please see routine idzr_asvd */
/*       for further documentation). */



/*       ID a. */

/* Computing 2nd power */
    i_1 = *krank;
    /* Parameter adjustments */
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --work;
    --col;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    --winit;

    /* Function Body */
    idzr_aid_(m, n, &a[a_offset], krank, &winit[1], &list[1], &proj[
	    proj_offset]);


/*       Collect together the columns of a indexed by list into col. */

    idz_copycols_(m, n, &a[a_offset], krank, &list[1], &col[1]);


/*       Convert the ID to an SVD. */

    idz_id2svd_(m, krank, &col[1], n, &list[1], &proj[proj_offset], &u[
	    u_offset], &v[v_offset], &s[1], ier, &work[1]);


    return 0;
} /* idzr_asvd0_ */

