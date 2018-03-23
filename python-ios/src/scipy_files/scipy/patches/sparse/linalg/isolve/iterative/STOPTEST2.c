/* STOPTEST2.f -- translated by f2c (version 20100827).
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

/* Table of constant values */

static integer c__1 = 1;

/* -*- fortran -*- */
/*     STOPTEST2 */
/*  Purpose */
/*  ======= */

/*  Computes the stopping criterion 2. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  INFO    (output) INTEGER */
/*          On exit, 1/0 depending on whether stopping criterion */
/*          was met or not. */

/*  BNRM2   (input/output) DOUBLE PRECISION. */
/*          On first time entry, will be -1.0. */
/*          On first time exit will contain norm2(B) */
/*          On all subsequent entry/exit's unchanged. */

/*  RESID   (output) DOUBLE PRECISION. */
/*          On exit, the computed stopping measure. */

/*  TOL     (input) DOUBLE PRECISION. */
/*          On input, the allowable convergence measure. */

/*  R       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, the residual. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  BLAS CALLS:   DNRM2 */
/*  ============================================================ */

/* Subroutine */ int sstoptest2_(integer *n, real *r__, real *b, real *bnrm2, 
	real *resid, real *tol, integer *info)
{
    extern doublereal wsnrm2_(integer *, real *, integer *);

    /* Parameter adjustments */
    --b;
    --r__;

    /* Function Body */
    if (*info == -1) {
	*bnrm2 = wsnrm2_(n, &b[1], &c__1);
	if (*bnrm2 == 0.f) {
	    *bnrm2 = 1.f;
	}
    }
    *resid = wsnrm2_(n, &r__[1], &c__1) / *bnrm2;
    *info = 0;
    if (*resid <= *tol) {
	*info = 1;
    }
    return 0;
} /* sstoptest2_ */

/*     END SUBROUTINE sSTOPTEST2 */
/* Subroutine */ int dstoptest2_(integer *n, doublereal *r__, doublereal *b, 
	doublereal *bnrm2, doublereal *resid, doublereal *tol, integer *info)
{
    extern doublereal dnrm2_(integer *, doublereal *, integer *);

    /* Parameter adjustments */
    --b;
    --r__;

    /* Function Body */
    if (*info == -1) {
	*bnrm2 = dnrm2_(n, &b[1], &c__1);
	if (*bnrm2 == 0.) {
	    *bnrm2 = 1.;
	}
    }
    *resid = dnrm2_(n, &r__[1], &c__1) / *bnrm2;
    *info = 0;
    if (*resid <= *tol) {
	*info = 1;
    }
    return 0;
} /* dstoptest2_ */

/*     END SUBROUTINE dSTOPTEST2 */
/* Subroutine */ int cstoptest2_(integer *n, complex *r__, complex *b, real *
	bnrm2, real *resid, real *tol, integer *info)
{
    extern doublereal wscnrm2_(integer *, complex *, integer *);

    /* Parameter adjustments */
    --b;
    --r__;

    /* Function Body */
    if (*info == -1) {
	*bnrm2 = wscnrm2_(n, &b[1], &c__1);
	if (*bnrm2 == 0.f) {
	    *bnrm2 = 1.f;
	}
    }
    *resid = wscnrm2_(n, &r__[1], &c__1) / *bnrm2;
    *info = 0;
    if (*resid <= *tol) {
	*info = 1;
    }
    return 0;
} /* cstoptest2_ */

/*     END SUBROUTINE cSTOPTEST2 */
/* Subroutine */ int zstoptest2_(integer *n, doublecomplex *r__, 
	doublecomplex *b, doublereal *bnrm2, doublereal *resid, doublereal *
	tol, integer *info)
{
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);

    /* Parameter adjustments */
    --b;
    --r__;

    /* Function Body */
    if (*info == -1) {
	*bnrm2 = dznrm2_(n, &b[1], &c__1);
	if (*bnrm2 == 0.) {
	    *bnrm2 = 1.;
	}
    }
    *resid = dznrm2_(n, &r__[1], &c__1) / *bnrm2;
    *info = 0;
    if (*resid <= *tol) {
	*info = 1;
    }
    return 0;
} /* zstoptest2_ */

