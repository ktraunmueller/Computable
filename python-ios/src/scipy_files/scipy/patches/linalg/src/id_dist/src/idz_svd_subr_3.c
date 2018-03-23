/* idz_svd_subr_3.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_retriever_(integer *m, integer *n, doublecomplex *a,
	 integer *krank, doublecomplex *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, r_dim1, r_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       extracts R in the QR decomposition specified by the output a */
/*       of the routine idzr_qrpiv or idzp_qrpiv */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a and r */
/*       a -- output of routine idzr_qrpiv or idzp_qrpiv */
/*       krank -- rank specified to routine idzr_qrpiv, */
/*                or output by routine idzp_qrpiv */

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
} /* idz_retriever__ */





/* Subroutine */ int idz_adjer_(integer *m, integer *n, doublecomplex *a, 
	doublecomplex *aa)
{
    /* System generated locals */
    integer a_dim1, a_offset, aa_dim1, aa_offset, i__1, i__2, i__3;
    doublecomplex z__1;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer j, k;


/*       forms the adjoint aa of a. */

/*       input: */
/*       m -- first dimension of a and second dimension of aa */
/*       n -- second dimension of a and first dimension of aa */
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
} /* idz_adjer__ */

