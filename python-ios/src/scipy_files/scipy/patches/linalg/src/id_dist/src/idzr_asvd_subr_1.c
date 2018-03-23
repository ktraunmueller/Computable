/* idzr_asvd_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzr_asvd0_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *winit, doublecomplex *u, doublecomplex 
	*v, doublereal *s, integer *ier, integer *list, doublecomplex *proj, 
	doublecomplex *col, doublecomplex *work)
{
    /* System generated locals */
    integer a_dim1, a_offset, u_dim1, u_offset, v_dim1, v_offset, proj_dim1, 
	    proj_offset, i__1;

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
    i__1 = *krank;
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
} /* idzr_asvd0__ */

