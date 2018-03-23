/* iddp_rsvd_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int iddp_rsvd0_(integer *m, integer *n, U_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	U_fp matvec, doublereal *p1, doublereal *p2, doublereal *p3, 
	doublereal *p4, integer *krank, doublereal *u, doublereal *v, 
	doublereal *s, integer *ier, integer *list, doublereal *proj, 
	doublereal *col, doublereal *work)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, proj_dim1, proj_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int idd_id2svd_(integer *, integer *, doublereal 
	    *, integer *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *, doublereal *), idd_getcols_(integer *, 
	    integer *, U_fp, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *);


/*       routine iddp_rsvd serves as a memory wrapper */
/*       for the present routine (please see routine iddp_rsvd */
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
    idd_getcols_(m, n, (U_fp)matvec, p1, p2, p3, p4, krank, &list[1], &col[1]
	    , &work[1]);


/*       Convert the ID to an SVD. */

    idd_id2svd_(m, krank, &col[1], n, &list[1], &proj[proj_offset], &u[
	    u_offset], &v[v_offset], &s[1], ier, &work[1]);


    return 0;
} /* iddp_rsvd0__ */

