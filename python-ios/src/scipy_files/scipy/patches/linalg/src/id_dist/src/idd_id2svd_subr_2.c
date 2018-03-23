/* idd_id2svd_subr_2.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_mattrans_(integer *m, integer *n, doublereal *a, 
	doublereal *at)
{
    /* System generated locals */
    integer a_dim1, a_offset, at_dim1, at_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;


/*       transposes a to obtain at. */

/*       input: */
/*       m -- first dimension of a, and second dimension of at */
/*       n -- second dimension of a, and first dimension of at */
/*       a -- matrix to be transposed */

/*       output: */
/*       at -- transpose of a */



    /* Parameter adjustments */
    at_dim1 = *n;
    at_offset = 1 + at_dim1;
    at -= at_offset;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    at[k + j * at_dim1] = a[j + k * a_dim1];
	}
/* j */
    }


/* k */
    return 0;
} /* idd_mattrans__ */





/* Subroutine */ int idd_matmultt_(integer *l, integer *m, doublereal *a, 
	integer *n, doublereal *b, doublereal *c__)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, 
	    i__3;

    /* Local variables */
    static integer i__, j, k;
    static doublereal sum;


/*       multiplies a and b^T to obtain c. */

/*       input: */
/*       l -- first dimension of a and c */
/*       m -- second dimension of a and b */
/*       a -- leftmost matrix in the product c = a b^T */
/*       n -- first dimension of b and second dimension of c */
/*       b -- rightmost matrix in the product c = a b^T */

/*       output: */
/*       c -- product of a and b^T */



    /* Parameter adjustments */
    a_dim1 = *l;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    c_dim1 = *l;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    b_dim1 = *n;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    i__1 = *l;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {

	    sum = 0.;

	    i__3 = *m;
	    for (j = 1; j <= i__3; ++j) {
		sum += a[i__ + j * a_dim1] * b[k + j * b_dim1];
	    }

/* j */
	    c__[i__ + k * c_dim1] = sum;

	}
/* k */
    }


/* i */
    return 0;
} /* idd_matmultt__ */





/* Subroutine */ int idd_rearr_(integer *krank, integer *ind, integer *m, 
	integer *n, doublereal *a)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static integer j, k;
    static doublereal rswap;


/*       rearranges a according to ind obtained */
/*       from routines iddr_qrpiv or iddp_qrpiv, */
/*       assuming that a = q r, where q and r are from iddr_qrpiv */
/*       or iddp_qrpiv. */

/*       input: */
/*       krank -- rank obtained from routine iddp_qrpiv, */
/*                or provided to routine iddr_qrpiv */
/*       ind -- indexing array obtained from routine iddr_qrpiv */
/*              or iddp_qrpiv */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       a -- matrix to be rearranged */

/*       output: */
/*       a -- rearranged matrix */



    /* Parameter adjustments */
    --ind;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    for (k = *krank; k >= 1; --k) {
	i__1 = *m;
	for (j = 1; j <= i__1; ++j) {

	    rswap = a[j + k * a_dim1];
	    a[j + k * a_dim1] = a[j + ind[k] * a_dim1];
	    a[j + ind[k] * a_dim1] = rswap;

	}
/* j */
    }


/* k */
    return 0;
} /* idd_rearr__ */





/* Subroutine */ int idd_rinqr_(integer *m, integer *n, doublereal *a, 
	integer *krank, doublereal *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, r_dim1, r_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;


/*       extracts R in the QR decomposition specified by the output a */
/*       of the routine iddr_qrpiv or iddp_qrpiv. */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a and r */
/*       a -- output of routine iddr_qrpiv or iddp_qrpiv */
/*       krank -- rank output by routine iddp_qrpiv (or specified */
/*                to routine iddr_qrpiv) */

/*       output: */
/*       r -- triangular factor in the QR decomposition specified */
/*            by the output a of the routine iddr_qrpiv or iddp_qrpiv */



/*       Copy a into r and zero out the appropriate */
/*       Householder vectors that are stored in one triangle of a. */

    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    r_dim1 = *krank;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *krank;
	for (j = 1; j <= i__2; ++j) {
	    r__[j + k * r_dim1] = a[j + k * a_dim1];
	}
/* j */
    }

/* k */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (k < *krank) {
	    i__2 = *krank;
	    for (j = k + 1; j <= i__2; ++j) {
		r__[j + k * r_dim1] = 0.;
	    }
/* j */
	}
    }


/* k */
    return 0;
} /* idd_rinqr__ */

