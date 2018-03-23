/* idzr_rsvd.f -- translated by f2c (version 20100827).
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


/*       routine idzr_rsvd computes the SVD, to a specified rank, */
/*       of a matrix specified by routines for applying the matrix */
/*       and its adjoint to arbitrary vectors. */
/*       This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idzr_rsvd_(integer *m, integer *n, U_fp matveca, 
	doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, 
	doublecomplex *p4t, U_fp matvec, doublecomplex *p1, doublecomplex *p2,
	 doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
	u, doublecomplex *v, doublereal *s, integer *ier, doublecomplex *w)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, i_1;

    /* Local variables */
    static integer lw;
    extern /* Subroutine */ int idzr_rsvd0_(integer *m, integer *n, U_fp matveca,
                                             doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t,
                                             doublecomplex *p4t, U_fp matvec, doublecomplex *p1, doublecomplex *p2,
                                             doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
                                             u, doublecomplex *v, doublereal *s, integer *ier, integer *list, 
                                             doublecomplex *proj, doublecomplex *col, doublecomplex *work);
    static integer icol, lcol, iproj, ilist, lproj, llist, iwork, lwork;


/*       constructs a rank-krank SVD  u diag(s) v^*  approximating a, */
/*       where matveca is a routine which applies a^* */
/*       to an arbitrary vector, and matvec is a routine */
/*       which applies a to an arbitrary vector; */
/*       u is an m x krank matrix whose columns are orthonormal, */
/*       v is an n x krank matrix whose columns are orthonormal, */
/*       and diag(s) is a diagonal krank x krank matrix whose entries */
/*       are all nonnegative. This routine uses a randomized algorithm. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       matveca -- routine which applies the adjoint */
/*                  of the matrix to be SVD'd */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matveca(m,x,n,y,p1t,p2t,p3t,p4t), */

/*                  where m is the length of x, */
/*                  x is the vector to which the adjoint */
/*                  of the matrix is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the adjoint of the matrix and x, */
/*                  and p1t, p2t, p3t, and p4t are user-specified */
/*                  parameters */
/*       p1t -- parameter to be passed to routine matveca */
/*       p2t -- parameter to be passed to routine matveca */
/*       p3t -- parameter to be passed to routine matveca */
/*       p4t -- parameter to be passed to routine matveca */
/*       matvec -- routine which applies the matrix to be SVD'd */
/*                 to an arbitrary vector; this routine must have */
/*                 a calling sequence of the form */

/*                 matvec(n,x,m,y,p1,p2,p3,p4), */

/*                 where n is the length of x, */
/*                 x is the vector to which the matrix is to be applied, */
/*                 m is the length of y, */
/*                 y is the product of the matrix and x, */
/*                 and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matvec */
/*       p2 -- parameter to be passed to routine matvec */
/*       p3 -- parameter to be passed to routine matvec */
/*       p4 -- parameter to be passed to routine matvec */
/*       krank -- rank of the SVD being constructed */

/*       output: */
/*       u -- matrix of orthonormal left singular vectors of a */
/*       v -- matrix of orthonormal right singular vectors of a */
/*       s -- array of singular values of a */
/*       ier -- 0 when the routine terminates successfully; */
/*              nonzero otherwise */

/*       work: */
/*       w -- must be at least (krank+1)*(2*m+4*n+10)+8*krank**2 */
/*            complex*16 elements long */

/*       _N.B._: The algorithm used by this routine is randomized. */



/*       Allocate memory in w. */

/* Computing 2nd power */
    i_1 = *krank;
    /* Parameter adjustments */
    --w;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;

    /* Function Body */
    lw = 0;

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


    idzr_rsvd0_(m, n, (U_fp)matveca, p1t, p2t, p3t, p4t, (U_fp)matvec, p1, 
	    p2, p3, p4, krank, &u[u_offset], &v[v_offset], &s[1], ier, &w[
	    ilist], &w[iproj], &w[icol], &w[iwork]);


    return 0;
} /* idzr_rsvd_ */





/* Subroutine */ int idzr_rsvd0_(integer *m, integer *n, U_fp matveca, 
	doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, 
	doublecomplex *p4t, U_fp matvec, doublecomplex *p1, doublecomplex *p2,
	 doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
	u, doublecomplex *v, doublereal *s, integer *ier, integer *list, 
	doublecomplex *proj, doublecomplex *col, doublecomplex *work)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, i_1, i_2, i_3;

    /* Local variables */
    extern /* Subroutine */ int idzr_rid_(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, integer *, doublecomplex *);
    static integer k;
    extern /* Subroutine */ int idz_id2svd_(integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublereal *, integer *, 
	    doublecomplex *), idz_getcols_(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, integer *, doublecomplex *, doublecomplex *);


/*       routine idzr_rsvd serves as a memory wrapper */
/*       for the present routine (please see routine idzr_rsvd */
/*       for further documentation). */



/*       ID a. */

/* Computing 2nd power */
    i_1 = *krank;
    /* Parameter adjustments */
    --list;
    --work;
    --col;
    --proj;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;

    /* Function Body */
    idzr_rid_(m, n, (U_fp)matveca, p1t, p2t, p3t, p4t, krank, &list[1], &
	    work[1]);


/*       Retrieve proj from work. */

    i_1 = *krank * (*n - *krank);
    for (k = 1; k <= i_1; ++k) {
	i_2 = k;
	i_3 = k;
	proj[i_2].r = work[i_3].r, proj[i_2].i = work[i_3].i;
    }


/*       Collect together the columns of a indexed by list into col. */

/* k */
    idz_getcols_(m, n, (U_fp)matvec, p1, p2, p3, p4, krank, &list[1], &col[1]
	    , &work[1]);


/*       Convert the ID to an SVD. */

    idz_id2svd_(m, krank, &col[1], n, &list[1], &proj[1], &u[u_offset], &v[
	    v_offset], &s[1], ier, &work[1]);


    return 0;
} /* idzr_rsvd0_ */

