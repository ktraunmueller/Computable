/* idd_id2svd_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_id2svd0_(integer *m, integer *krank, doublereal *b, 
	integer *n, integer *list, doublereal *proj, doublereal *u, 
	doublereal *v, doublereal *s, integer *ier, doublereal *work, 
	doublereal *p, doublereal *t, doublereal *r__, doublereal *r2, 
	doublereal *r3, integer *ind, integer *indt)
{
    /* System generated locals */
    integer b_dim1, b_offset, proj_dim1, proj_offset, p_dim1, p_offset, 
	    r_dim1, r_offset, r2_dim1, r2_offset, t_dim1, t_offset, r3_dim1, 
	    r3_offset, u_dim1, u_offset, v_dim1, v_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int idd_rearr_(integer *, integer *, integer *, 
	    integer *, doublereal *), idd_rinqr_(integer *, integer *, 
	    doublereal *, integer *, doublereal *), iddr_qrpiv_(integer *, 
	    integer *, doublereal *, integer *, integer *, doublereal *);
    static integer ldr, ldu;
    extern /* Subroutine */ int idd_qmatmat_(integer *, integer *, integer *,
	     doublereal *, integer *, integer *, doublereal *, doublereal *);
    static integer iftranspose, info;
    static char jobz[1];
    static integer ldvt;
    extern /* Subroutine */ int idd_reconint_(integer *, integer *, integer *
	    , doublereal *, doublereal *), idd_mattrans_(integer *, integer *
	    , doublereal *, doublereal *), idd_matmultt_(integer *, integer *
	    , doublereal *, integer *, doublereal *, doublereal *);
    static integer lwork;
    extern /* Subroutine */ int dgesdd_(char *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, integer *, doublereal *, 
	    integer *, ftnlen);


/*       routine idd_id2svd serves as a memory wrapper */
/*       for the present routine (please see routine idd_id2svd */
/*       for further documentation). */





/* Computing 2nd power */
    i__1 = *krank;
    /* Parameter adjustments */
    --indt;
    r3_dim1 = *krank;
    r3_offset = 1 + r3_dim1;
    r3 -= r3_offset;
    r2_dim1 = *krank;
    r2_offset = 1 + r2_dim1;
    r2 -= r2_offset;
    --s;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    b_dim1 = *m;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --ind;
    r_dim1 = *krank;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    t_dim1 = *n;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    p_dim1 = *krank;
    p_offset = 1 + p_dim1;
    p -= p_offset;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --list;
    --work;

    /* Function Body */
    *ier = 0;



/*       Construct the projection matrix p from the ID. */

    idd_reconint_(n, &list[1], krank, &proj[proj_offset], &p[p_offset]);



/*       Compute a pivoted QR decomposition of b. */

    iddr_qrpiv_(m, krank, &b[b_offset], krank, &ind[1], &r__[r_offset]);


/*       Extract r from the QR decomposition. */

    idd_rinqr_(m, krank, &b[b_offset], krank, &r__[r_offset]);


/*       Rearrange r according to ind. */

    idd_rearr_(krank, &ind[1], krank, krank, &r__[r_offset]);



/*       Transpose p to obtain t. */

    idd_mattrans_(krank, n, &p[p_offset], &t[t_offset]);


/*       Compute a pivoted QR decomposition of t. */

    iddr_qrpiv_(n, krank, &t[t_offset], krank, &indt[1], &r2[r2_offset]);


/*       Extract r2 from the QR decomposition. */

    idd_rinqr_(n, krank, &t[t_offset], krank, &r2[r2_offset]);


/*       Rearrange r2 according to indt. */

    idd_rearr_(krank, &indt[1], krank, krank, &r2[r2_offset]);



/*       Multiply r and r2^T to obtain r3. */

    idd_matmultt_(krank, krank, &r__[r_offset], krank, &r2[r2_offset], &r3[
	    r3_offset]);



/*       Use LAPACK to SVD r3. */

    *(unsigned char *)jobz = 'S';
    ldr = *krank;
/* Computing 2nd power */
    i__1 = *krank;
/* Computing 2nd power */
    i__2 = *krank;
    lwork = i__1 * i__1 * 25 - i__2 * i__2 - (*krank << 2);
    ldu = *krank;
    ldvt = *krank;

/* Computing 2nd power */
    i__1 = *krank;
/* Computing 2nd power */
    i__2 = *krank;
    dgesdd_(jobz, krank, krank, &r3[r3_offset], &ldr, &s[1], &work[1], &ldu, &
	    r__[r_offset], &ldvt, &work[i__1 * i__1 + (*krank << 2) + 1], &
	    lwork, &work[i__2 * i__2 + 1], &info, (ftnlen)1);

    if (info != 0) {
	*ier = info;
	return 0;
    }



/*       Multiply the u from r3 from the left by the q from b */
/*       to obtain the u for a. */

    i__1 = *krank;
    for (k = 1; k <= i__1; ++k) {

	i__2 = *krank;
	for (j = 1; j <= i__2; ++j) {
	    u[j + k * u_dim1] = work[j + *krank * (k - 1)];
	}

/* j */
	i__2 = *m;
	for (j = *krank + 1; j <= i__2; ++j) {
	    u[j + k * u_dim1] = 0.;
	}

/* j */
    }

/* k */
    iftranspose = 0;
    idd_qmatmat_(&iftranspose, m, krank, &b[b_offset], krank, krank, &u[
	    u_offset], &r2[r2_offset]);



/*       Transpose r to obtain r2. */

    idd_mattrans_(krank, krank, &r__[r_offset], &r2[r2_offset]);


/*       Multiply the v from r3 from the left by the q from p^T */
/*       to obtain the v for a. */

    i__1 = *krank;
    for (k = 1; k <= i__1; ++k) {

	i__2 = *krank;
	for (j = 1; j <= i__2; ++j) {
	    v[j + k * v_dim1] = r2[j + k * r2_dim1];
	}

/* j */
	i__2 = *n;
	for (j = *krank + 1; j <= i__2; ++j) {
	    v[j + k * v_dim1] = 0.;
	}

/* j */
    }

/* k */
    iftranspose = 0;
    idd_qmatmat_(&iftranspose, n, krank, &t[t_offset], krank, krank, &v[
	    v_offset], &r2[r2_offset]);


    return 0;
} /* idd_id2svd0__ */

