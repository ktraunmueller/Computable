/* idzp_rsvd_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzp_rsvd0_(integer *m, integer *n, U_fp matveca, 
	doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, 
	doublecomplex *p4t, U_fp matvec, doublecomplex *p1, doublecomplex *p2,
	 doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
	u, doublecomplex *v, doublereal *s, integer *ier, integer *list, 
	doublecomplex *proj, doublecomplex *col, doublecomplex *work)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, proj_dim1, proj_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int idz_id2svd_(integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublereal *, integer *, 
	    doublecomplex *), idz_getcols_(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, integer *, doublecomplex *, doublecomplex *);


/*       routine idzp_rsvd serves as a memory wrapper */
/*       for the present routine (please see routine idzp_rsvd */
/*       for further documentation). */



/*       Collect together the columns of a indexed by list into col. */

/* Computing 2nd power */
    i__1 = *krank;
    /* Parameter adjustments */
    --list;
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

    /* Function Body */
    idz_getcols_(m, n, (U_fp)matvec, p1, p2, p3, p4, krank, &list[1], &col[1]
	    , &work[1]);


/*       Convert the ID to an SVD. */

    idz_id2svd_(m, krank, &col[1], n, &list[1], &proj[proj_offset], &u[
	    u_offset], &v[v_offset], &s[1], ier, &work[1]);


    return 0;
} /* idzp_rsvd0__ */

