/* idz_id2svd_subr_2.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_matadj_(integer *m, integer *n, doublecomplex *a, 
	doublecomplex *aa)
{
    /* System generated locals */
    integer a_dim1, a_offset, aa_dim1, aa_offset, i__1, i__2, i__3;
    doublecomplex z__1;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer j, k;


/*       Takes the adjoint of a to obtain aa. */

/*       input: */
/*       m -- first dimension of a, and second dimension of aa */
/*       n -- second dimension of a, and first dimension of aa */
/*       a -- matrix whose adjoint is to be taken */

/*       output: */
/*       aa -- adjoint of a */



    /* Parameter adjustments */
    aa_dim1 = *n;
    aa_offset = 1 + aa_dim1;
    aa -= aa_offset;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = k + j * aa_dim1;
	    d_cnjg(&z__1, &a[j + k * a_dim1]);
	    aa[i__3].r = z__1.r, aa[i__3].i = z__1.i;
	}
/* j */
    }


/* k */
    return 0;
} /* idz_matadj__ */





/* Subroutine */ int idz_matmulta_(integer *l, integer *m, doublecomplex *a, 
	integer *n, doublecomplex *b, doublecomplex *c__)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset, i__1, i__2, 
	    i__3, i__4;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer i__, j, k;
    static doublecomplex sum;


/*       multiplies a and b^* to obtain c. */

/*       input: */
/*       l -- first dimension of a and c */
/*       m -- second dimension of a and b */
/*       a -- leftmost matrix in the product c = a b^* */
/*       n -- first dimension of b and second dimension of c */
/*       b -- rightmost matrix in the product c = a b^* */

/*       output: */
/*       c -- product of a and b^* */



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

	    sum.r = 0., sum.i = 0.;

	    i__3 = *m;
	    for (j = 1; j <= i__3; ++j) {
		i__4 = i__ + j * a_dim1;
		d_cnjg(&z__3, &b[k + j * b_dim1]);
		z__2.r = a[i__4].r * z__3.r - a[i__4].i * z__3.i, z__2.i = a[
			i__4].r * z__3.i + a[i__4].i * z__3.r;
		z__1.r = sum.r + z__2.r, z__1.i = sum.i + z__2.i;
		sum.r = z__1.r, sum.i = z__1.i;
	    }

/* j */
	    i__3 = i__ + k * c_dim1;
	    c__[i__3].r = sum.r, c__[i__3].i = sum.i;

	}
/* k */
    }


/* i */
    return 0;
} /* idz_matmulta__ */





/* Subroutine */ int idz_rearr_(integer *krank, integer *ind, integer *m, 
	integer *n, doublecomplex *a)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer j, k;
    static doublecomplex cswap;


/*       rearranges a according to ind obtained */
/*       from routines idzr_qrpiv or idzp_qrpiv, */
/*       assuming that a = q r, where q and r are from idzr_qrpiv */
/*       or idzp_qrpiv. */

/*       input: */
/*       krank -- rank obtained from routine idzp_qrpiv, */
/*                or provided to routine idzr_qrpiv */
/*       ind -- indexing array obtained from routine idzr_qrpiv */
/*              or idzp_qrpiv */
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

	    i__2 = j + k * a_dim1;
	    cswap.r = a[i__2].r, cswap.i = a[i__2].i;
	    i__2 = j + k * a_dim1;
	    i__3 = j + ind[k] * a_dim1;
	    a[i__2].r = a[i__3].r, a[i__2].i = a[i__3].i;
	    i__2 = j + ind[k] * a_dim1;
	    a[i__2].r = cswap.r, a[i__2].i = cswap.i;

	}
/* j */
    }


/* k */
    return 0;
} /* idz_rearr__ */





/* Subroutine */ int idz_rinqr_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, r_dim1, r_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       extracts R in the QR decomposition specified by the output a */
/*       of the routine idzr_qrpiv or idzp_qrpiv. */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a and r */
/*       a -- output of routine idzr_qrpiv or idzp_qrpiv */
/*       krank -- rank output by routine idzp_qrpiv (or specified */
/*                to routine idzr_qrpiv) */

/*       output: */
/*       r -- triangular factor in the QR decomposition specified */
/*            by the output a of the routine idzr_qrpiv or idzp_qrpiv */



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
	    i__3 = j + k * r_dim1;
	    i__4 = j + k * a_dim1;
	    r__[i__3].r = a[i__4].r, r__[i__3].i = a[i__4].i;
	}
/* j */
    }

/* k */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (k < *krank) {
	    i__2 = *krank;
	    for (j = k + 1; j <= i__2; ++j) {
		i__3 = j + k * r_dim1;
		r__[i__3].r = 0., r__[i__3].i = 0.;
	    }
/* j */
	}
    }


/* k */
    return 0;
} /* idz_rinqr__ */

