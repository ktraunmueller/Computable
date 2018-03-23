/* iddr_rsvd_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int iddr_rsvd_(integer *m, integer *n, U_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	U_fp matvec, doublereal *p1, doublereal *p2, doublereal *p3, 
	doublereal *p4, integer *krank, doublereal *u, doublereal *v, 
	doublereal *s, integer *ier, doublereal *w)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int iddr_rsvd0_(integer *, integer *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *, doublereal *, integer *, doublereal *
	    , doublereal *, doublereal *, doublereal *);
    static integer lw, icol, lcol, iproj, ilist, lproj, llist, iwork, lwork;


/*       constructs a rank-krank SVD  u diag(s) v^T  approximating a, */
/*       where matvect is a routine which applies a^T */
/*       to an arbitrary vector, and matvec is a routine */
/*       which applies a to an arbitrary vector; */
/*       u is an m x krank matrix whose columns are orthonormal, */
/*       v is an n x krank matrix whose columns are orthonormal, */
/*       and diag(s) is a diagonal krank x krank matrix whose entries */
/*       are all nonnegative. This routine uses a randomized algorithm. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       matvect -- routine which applies the transpose */
/*                  of the matrix to be SVD'd */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matvect(m,x,n,y,p1t,p2t,p3t,p4t), */

/*                  where m is the length of x, */
/*                  x is the vector to which the transpose */
/*                  of the matrix is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the transposed matrix and x, */
/*                  and p1t, p2t, p3t, and p4t are user-specified */
/*                  parameters */
/*       p1t -- parameter to be passed to routine matvect */
/*       p2t -- parameter to be passed to routine matvect */
/*       p3t -- parameter to be passed to routine matvect */
/*       p4t -- parameter to be passed to routine matvect */
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
/*       w -- must be at least (krank+1)*(2*m+4*n)+25*krank**2 */
/*            real*8 elements long */

/*       _N.B._: The algorithm used by this routine is randomized. */



/*       Allocate memory in w. */

/* Computing 2nd power */
    i__1 = *krank;
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
    i__1 = *krank;
    lwork = (*krank + 1) * (*m + *n * 3) + i__1 * i__1 * 26;
    lw += lwork;


    iddr_rsvd0_(m, n, (U_fp)matvect, p1t, p2t, p3t, p4t, (U_fp)matvec, p1, 
	    p2, p3, p4, krank, &u[u_offset], &v[v_offset], &s[1], ier, &w[
	    ilist], &w[iproj], &w[icol], &w[iwork]);


    return 0;
} /* iddr_rsvd__ */

