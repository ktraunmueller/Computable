/* iddr_rsvd_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int iddr_rsvd0_(integer *m, integer *n, U_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	U_fp matvec, doublereal *p1, doublereal *p2, doublereal *p3, 
	doublereal *p4, integer *krank, doublereal *u, doublereal *v, 
	doublereal *s, integer *ier, integer *list, doublereal *proj, 
	doublereal *col, doublereal *work)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int iddr_rid_(integer *, integer *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, doublereal *);
    static integer k;
    extern /* Subroutine */ int idd_id2svd_(integer *, integer *, doublereal 
	    *, integer *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *, doublereal *), idd_getcols_(integer *, 
	    integer *, U_fp, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *);


/*       routine iddr_rsvd serves as a memory wrapper */
/*       for the present routine (please see routine iddr_rsvd */
/*       for further documentation). */



/*       ID a. */

/* Computing 2nd power */
    i__1 = *krank;
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
    iddr_rid_(m, n, (U_fp)matvect, p1t, p2t, p3t, p4t, krank, &list[1], &
	    work[1]);


/*       Retrieve proj from work. */

    i__1 = *krank * (*n - *krank);
    for (k = 1; k <= i__1; ++k) {
	proj[k] = work[k];
    }


/*       Collect together the columns of a indexed by list into col. */

/* k */
    idd_getcols_(m, n, (U_fp)matvec, p1, p2, p3, p4, krank, &list[1], &col[1]
	    , &work[1]);


/*       Convert the ID to an SVD. */

    idd_id2svd_(m, krank, &col[1], n, &list[1], &proj[1], &u[u_offset], &v[
	    v_offset], &s[1], ier, &work[1]);


    return 0;
} /* iddr_rsvd0__ */

