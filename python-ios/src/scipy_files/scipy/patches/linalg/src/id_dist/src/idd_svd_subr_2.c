/* idd_svd_subr_2.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_retriever_(integer *m, integer *n, doublereal *a, 
	integer *krank, doublereal *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, r_dim1, r_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;


/*       extracts R in the QR decomposition specified by the output a */
/*       of the routine iddr_qrpiv or iddp_qrpiv */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a and r */
/*       a -- output of routine iddr_qrpiv or iddp_qrpiv */
/*       krank -- rank specified to routine iddr_qrpiv, */
/*                or output by routine iddp_qrpiv */

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
} /* idd_retriever__ */





/* Subroutine */ int idd_transer_(integer *m, integer *n, doublereal *a, 
	doublereal *at)
{
    /* System generated locals */
    integer a_dim1, a_offset, at_dim1, at_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;


/*       forms the transpose at of a. */

/*       input: */
/*       m -- first dimension of a and second dimension of at */
/*       n -- second dimension of a and first dimension of at */
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
} /* idd_transer__ */

