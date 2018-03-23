/* CGSREVCOM.f -- translated by f2c (version 20100827).
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
static integer c__2 = 2;

/*  -*- fortran -*- */
/* Subroutine */ int scgsrevcom_(integer *n, real *b, real *x, real *work, 
	integer *ldw, integer *iter, real *resid, integer *info, integer *
	ndx1, integer *ndx2, real *sclr1, real *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    real r__1, r__2;

    /* Local variables */
    static integer p, q, r__, u;
    extern doublereal sgetbreak_(void);
    static real rho, tol, rho1, beta;
    static integer phat, qhat, rlbl, uhat, vhat, rtld, need1, need2;
    static real bnrm2, alpha;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    static integer maxit;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *);
    extern doublereal wsdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);
    extern doublereal wsnrm2_(integer *, real *, integer *);
    static real tmpval, rhotol;


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     Eijkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGS solves the linear system Ax = b using the */
/*  Conjugate Gradient Squared iterative method with preconditioning. */

/*  Convergence test: ( norm( b - A*x ) / norm( b ) ) .ls.  TOL. */
/*  For other measures, see the above reference. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess. This is commonly set to */
/*          the zero vector. The user should be warned that for */
/*          this particular algorithm, an initial guess close to */
/*          the actual solution can result in divergence. */
/*          On exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors PHAT and QHAT, and UHAT and VHAT share */
/*          the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On ouput, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. */
/*          .gt.   0: Convergence not achieved. This will be set */
/*                to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occured */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If RHO become smaller than some tolerance, */
/*                   the program will terminate. Here we check */
/*                   against tolerance BREAKTOL. */

/*                   -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                        orthogonal. */

/*  NDX1    (input/output) INTEGER. */
/*  NDX2    On entry in INIT call contain indices required by interface */
/*          level for stopping test. */
/*          All other times, used as output, to indicate indices into */
/*          WORK[] for the MATVEC, PSOLVE done by the interface level. */

/*  SCLR1   (output) DOUBLE PRECISION. */
/*  SCLR2   Used to pass the scalars used in MATVEC. Scalars are reqd because */
/*          original routines use dgemv. */

/*  IJOB    (input/output) INTEGER. */
/*          Used to communicate job code between the two levels. */

/*  BLAS CALLS:    DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================= */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;

    /* Function Body */
    if (*ijob == 1) {
	goto L1;
    } else if (*ijob == 2) {
/*        here we do resumption handling */
	if (rlbl == 2) {
	    goto L2;
	}
	if (rlbl == 3) {
	    goto L3;
	}
	if (rlbl == 4) {
	    goto L4;
	}
	if (rlbl == 5) {
	    goto L5;
	}
	if (rlbl == 6) {
	    goto L6;
	}
	if (rlbl == 7) {
	    goto L7;
	}
/*        if neither of these, then error */
	*info = -6;
	goto L20;
    }


/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    p = 3;
    phat = 4;
    q = 5;
    qhat = 6;
    u = 6;
    uhat = 7;
    vhat = 7;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (qhat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (u - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (uhat - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (rtld - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (qhat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (u - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (uhat - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerance parameter. */

    rhotol = sgetbreak_();

/*     Set initial residual. */

    scopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wsnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using RTLD[] as temp. storage. */
/* ********CALL sCOPY(N, X, 1, WORK(1,RTLD), 1) */
	*sclr1 = -1.f;
	*sclr2 = 1.f;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 3;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (wsnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) <= tol) {
	goto L30;
    }

    bnrm2 = wsnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

/*     Choose RTLD such that initially, (R,RTLD) = RHO is not equal to 0. */
/*     Here we choose RTLD = R. */

    scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    *iter = 0;

L10:

/*     Perform Conjugate Gradient Squared iteration. */

    ++(*iter);

    rho = wsdot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * work_dim1 
	    + 1], &c__1);
    if (dabs(rho) < rhotol) {
	goto L25;
    }

/*        Compute direction vectors U and P. */

    if (*iter > 1) {

/*           Compute U. */

	beta = rho / rho1;
	scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	saxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[u * work_dim1 
		+ 1], &c__1);

/*           Compute P. */

/* Computing 2nd power */
	r__2 = beta;
	r__1 = r__2 * r__2;
	sscal_(n, &r__1, &work[p * work_dim1 + 1], &c__1);
	saxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	tmpval = 1.f;
	saxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	scopy_(n, &work[u * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1], &
		c__1);
    }

/*        Compute direction adjusting scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,VHAT) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (vhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.f;
    *sclr2 = 0.f;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    alpha = rho / wsdot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[vhat * 
	    work_dim1 + 1], &c__1);

    scopy_(n, &work[u * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1], &
	    c__1);
    r__1 = -alpha;
    saxpy_(n, &r__1, &work[vhat * work_dim1 + 1], &c__1, &work[q * work_dim1 
	    + 1], &c__1);

/*        Compute direction adjusting vectORT UHAT. */
/*        PHAT is being used as temporary storage here. */

    scopy_(n, &work[q * work_dim1 + 1], &c__1, &work[phat * work_dim1 + 1], &
	    c__1);
    tmpval = 1.f;
    saxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[phat * 
	    work_dim1 + 1], &c__1);
/* ********CALL PSOLVE( WORK(1,UHAT), WORK(1,PHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (phat - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*        Compute new solution approximation vector X. */

    saxpy_(n, &alpha, &work[uhat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual R and check for tolerance. */

/* ********CALL MATVEC( ONE, WORK(1,UHAT), ZERO, WORK(1,QHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (qhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.f;
    *sclr2 = 0.f;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    r__1 = -alpha;
    saxpy_(n, &r__1, &work[qhat * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);

/* ********RESID = wsNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 4;
    return 0;

/* **************** */
L7:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    rho1 = rho;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (dabs(rho) < rhotol) {
	*info = -10;
    }

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of CGSREVCOM */

} /* scgsrevcom_ */

/*     END SUBROUTINE sCGSREVCOM */
/* Subroutine */ int dcgsrevcom_(integer *n, doublereal *b, doublereal *x, 
	doublereal *work, integer *ldw, integer *iter, doublereal *resid, 
	integer *info, integer *ndx1, integer *ndx2, doublereal *sclr1, 
	doublereal *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublereal d__1, d__2;

    /* Local variables */
    static integer p, q, r__, u;
    extern doublereal dgetbreak_(void);
    static doublereal rho, tol, rho1, beta;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);
    static integer phat, qhat, rlbl, uhat, vhat, rtld, need1, need2;
    static doublereal bnrm2;
    extern doublereal dnrm2_(integer *, doublereal *, integer *);
    static doublereal alpha;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *), dcopy_(integer *, doublereal *, integer *, doublereal 
	    *, integer *);
    static integer maxit;
    extern /* Subroutine */ int daxpy_(integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *);
    static doublereal tmpval, rhotol;


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     Eijkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGS solves the linear system Ax = b using the */
/*  Conjugate Gradient Squared iterative method with preconditioning. */

/*  Convergence test: ( norm( b - A*x ) / norm( b ) ) .ls.  TOL. */
/*  For other measures, see the above reference. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess. This is commonly set to */
/*          the zero vector. The user should be warned that for */
/*          this particular algorithm, an initial guess close to */
/*          the actual solution can result in divergence. */
/*          On exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors PHAT and QHAT, and UHAT and VHAT share */
/*          the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On ouput, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. */
/*          .gt.   0: Convergence not achieved. This will be set */
/*                to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occured */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If RHO become smaller than some tolerance, */
/*                   the program will terminate. Here we check */
/*                   against tolerance BREAKTOL. */

/*                   -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                        orthogonal. */

/*  NDX1    (input/output) INTEGER. */
/*  NDX2    On entry in INIT call contain indices required by interface */
/*          level for stopping test. */
/*          All other times, used as output, to indicate indices into */
/*          WORK[] for the MATVEC, PSOLVE done by the interface level. */

/*  SCLR1   (output) DOUBLE PRECISION. */
/*  SCLR2   Used to pass the scalars used in MATVEC. Scalars are reqd because */
/*          original routines use dgemv. */

/*  IJOB    (input/output) INTEGER. */
/*          Used to communicate job code between the two levels. */

/*  BLAS CALLS:    DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================= */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;

    /* Function Body */
    if (*ijob == 1) {
	goto L1;
    } else if (*ijob == 2) {
/*        here we do resumption handling */
	if (rlbl == 2) {
	    goto L2;
	}
	if (rlbl == 3) {
	    goto L3;
	}
	if (rlbl == 4) {
	    goto L4;
	}
	if (rlbl == 5) {
	    goto L5;
	}
	if (rlbl == 6) {
	    goto L6;
	}
	if (rlbl == 7) {
	    goto L7;
	}
/*        if neither of these, then error */
	*info = -6;
	goto L20;
    }


/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    p = 3;
    phat = 4;
    q = 5;
    qhat = 6;
    u = 6;
    uhat = 7;
    vhat = 7;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (qhat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (u - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (uhat - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (rtld - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (qhat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (u - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (uhat - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerance parameter. */

    rhotol = dgetbreak_();

/*     Set initial residual. */

    dcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dnrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using RTLD[] as temp. storage. */
/* ********CALL dCOPY(N, X, 1, WORK(1,RTLD), 1) */
	*sclr1 = -1.;
	*sclr2 = 1.;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 3;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (dnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) <= tol) {
	goto L30;
    }

    bnrm2 = dnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

/*     Choose RTLD such that initially, (R,RTLD) = RHO is not equal to 0. */
/*     Here we choose RTLD = R. */

    dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    *iter = 0;

L10:

/*     Perform Conjugate Gradient Squared iteration. */

    ++(*iter);

    rho = ddot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * work_dim1 
	    + 1], &c__1);
    if (abs(rho) < rhotol) {
	goto L25;
    }

/*        Compute direction vectors U and P. */

    if (*iter > 1) {

/*           Compute U. */

	beta = rho / rho1;
	dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	daxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[u * work_dim1 
		+ 1], &c__1);

/*           Compute P. */

/* Computing 2nd power */
	d__2 = beta;
	d__1 = d__2 * d__2;
	dscal_(n, &d__1, &work[p * work_dim1 + 1], &c__1);
	daxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	tmpval = 1.;
	daxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	dcopy_(n, &work[u * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1], &
		c__1);
    }

/*        Compute direction adjusting scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,VHAT) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (vhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.;
    *sclr2 = 0.;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    alpha = rho / ddot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[vhat * 
	    work_dim1 + 1], &c__1);

    dcopy_(n, &work[u * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1], &
	    c__1);
    d__1 = -alpha;
    daxpy_(n, &d__1, &work[vhat * work_dim1 + 1], &c__1, &work[q * work_dim1 
	    + 1], &c__1);

/*        Compute direction adjusting vectORT UHAT. */
/*        PHAT is being used as temporary storage here. */

    dcopy_(n, &work[q * work_dim1 + 1], &c__1, &work[phat * work_dim1 + 1], &
	    c__1);
    tmpval = 1.;
    daxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[phat * 
	    work_dim1 + 1], &c__1);
/* ********CALL PSOLVE( WORK(1,UHAT), WORK(1,PHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (phat - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*        Compute new solution approximation vector X. */

    daxpy_(n, &alpha, &work[uhat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual R and check for tolerance. */

/* ********CALL MATVEC( ONE, WORK(1,UHAT), ZERO, WORK(1,QHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (qhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.;
    *sclr2 = 0.;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    d__1 = -alpha;
    daxpy_(n, &d__1, &work[qhat * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);

/* ********RESID = dNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 4;
    return 0;

/* **************** */
L7:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    rho1 = rho;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (abs(rho) < rhotol) {
	*info = -10;
    }

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of CGSREVCOM */

} /* dcgsrevcom_ */

/*     END SUBROUTINE dCGSREVCOM */
/* Subroutine */ int ccgsrevcom_(integer *n, complex *b, complex *x, complex *
	work, integer *ldw, integer *iter, real *resid, integer *info, 
	integer *ndx1, integer *ndx2, complex *sclr1, complex *sclr2, integer 
	*ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    complex q__1, q__2;

    /* Builtin functions */
    double c_abs(complex *);
    void c_div(complex *, complex *, complex *), pow_ci(complex *, complex *, 
	    integer *);

    /* Local variables */
    static integer p, q, r__, u;
    extern doublereal sgetbreak_(void);
    static complex rho;
    static real tol;
    static complex rho1, beta;
    static integer phat, qhat, rlbl, uhat, vhat, rtld, need1, need2;
    static real bnrm2;
    static complex alpha;
    extern /* Subroutine */ int cscal_(integer *, complex *, complex *, 
	    integer *), ccopy_(integer *, complex *, integer *, complex *, 
	    integer *);
    static integer maxit;
    extern /* Subroutine */ int caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *);
    extern /* Complex */ VOID wcdotc_(complex *, integer *, complex *, 
	    integer *, complex *, integer *);
    static complex tmpval;
    static real rhotol;
    extern doublereal wscnrm2_(integer *, complex *, integer *);


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     Eijkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGS solves the linear system Ax = b using the */
/*  Conjugate Gradient Squared iterative method with preconditioning. */

/*  Convergence test: ( norm( b - A*x ) / norm( b ) ) .ls.  TOL. */
/*  For other measures, see the above reference. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess. This is commonly set to */
/*          the zero vector. The user should be warned that for */
/*          this particular algorithm, an initial guess close to */
/*          the actual solution can result in divergence. */
/*          On exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors PHAT and QHAT, and UHAT and VHAT share */
/*          the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On ouput, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. */
/*          .gt.   0: Convergence not achieved. This will be set */
/*                to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occured */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If RHO become smaller than some tolerance, */
/*                   the program will terminate. Here we check */
/*                   against tolerance BREAKTOL. */

/*                   -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                        orthogonal. */

/*  NDX1    (input/output) INTEGER. */
/*  NDX2    On entry in INIT call contain indices required by interface */
/*          level for stopping test. */
/*          All other times, used as output, to indicate indices into */
/*          WORK[] for the MATVEC, PSOLVE done by the interface level. */

/*  SCLR1   (output) DOUBLE PRECISION. */
/*  SCLR2   Used to pass the scalars used in MATVEC. Scalars are reqd because */
/*          original routines use dgemv. */

/*  IJOB    (input/output) INTEGER. */
/*          Used to communicate job code between the two levels. */

/*  BLAS CALLS:    DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================= */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;

    /* Function Body */
    if (*ijob == 1) {
	goto L1;
    } else if (*ijob == 2) {
/*        here we do resumption handling */
	if (rlbl == 2) {
	    goto L2;
	}
	if (rlbl == 3) {
	    goto L3;
	}
	if (rlbl == 4) {
	    goto L4;
	}
	if (rlbl == 5) {
	    goto L5;
	}
	if (rlbl == 6) {
	    goto L6;
	}
	if (rlbl == 7) {
	    goto L7;
	}
/*        if neither of these, then error */
	*info = -6;
	goto L20;
    }


/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    p = 3;
    phat = 4;
    q = 5;
    qhat = 6;
    u = 6;
    uhat = 7;
    vhat = 7;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (qhat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (u - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (uhat - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (rtld - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (qhat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (u - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (uhat - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerance parameter. */

    rhotol = sgetbreak_();

/*     Set initial residual. */

    ccopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wscnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using RTLD[] as temp. storage. */
/* ********CALL cCOPY(N, X, 1, WORK(1,RTLD), 1) */
	sclr1->r = -1.f, sclr1->i = 0.f;
	sclr2->r = 1.f, sclr2->i = 0.f;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 3;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (wscnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) <= tol) {
	goto L30;
    }

    bnrm2 = wscnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

/*     Choose RTLD such that initially, (R,RTLD) = RHO is not equal to 0. */
/*     Here we choose RTLD = R. */

    ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    *iter = 0;

L10:

/*     Perform Conjugate Gradient Squared iteration. */

    ++(*iter);

    wcdotc_(&q__1, n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);
    rho.r = q__1.r, rho.i = q__1.i;
    if (c_abs(&rho) < rhotol) {
	goto L25;
    }

/*        Compute direction vectors U and P. */

    if (*iter > 1) {

/*           Compute U. */

	c_div(&q__1, &rho, &rho1);
	beta.r = q__1.r, beta.i = q__1.i;
	ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	caxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[u * work_dim1 
		+ 1], &c__1);

/*           Compute P. */

	pow_ci(&q__1, &beta, &c__2);
	cscal_(n, &q__1, &work[p * work_dim1 + 1], &c__1);
	caxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	tmpval.r = 1.f, tmpval.i = 0.f;
	caxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	ccopy_(n, &work[u * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1], &
		c__1);
    }

/*        Compute direction adjusting scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,VHAT) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (vhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    wcdotc_(&q__2, n, &work[rtld * work_dim1 + 1], &c__1, &work[vhat * 
	    work_dim1 + 1], &c__1);
    c_div(&q__1, &rho, &q__2);
    alpha.r = q__1.r, alpha.i = q__1.i;

    ccopy_(n, &work[u * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1], &
	    c__1);
    q__1.r = -alpha.r, q__1.i = -alpha.i;
    caxpy_(n, &q__1, &work[vhat * work_dim1 + 1], &c__1, &work[q * work_dim1 
	    + 1], &c__1);

/*        Compute direction adjusting vectORT UHAT. */
/*        PHAT is being used as temporary storage here. */

    ccopy_(n, &work[q * work_dim1 + 1], &c__1, &work[phat * work_dim1 + 1], &
	    c__1);
    tmpval.r = 1.f, tmpval.i = 0.f;
    caxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[phat * 
	    work_dim1 + 1], &c__1);
/* ********CALL PSOLVE( WORK(1,UHAT), WORK(1,PHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (phat - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*        Compute new solution approximation vector X. */

    caxpy_(n, &alpha, &work[uhat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual R and check for tolerance. */

/* ********CALL MATVEC( ONE, WORK(1,UHAT), ZERO, WORK(1,QHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (qhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    q__1.r = -alpha.r, q__1.i = -alpha.i;
    caxpy_(n, &q__1, &work[qhat * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);

/* ********RESID = wscNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 4;
    return 0;

/* **************** */
L7:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    rho1.r = rho.r, rho1.i = rho.i;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (c_abs(&rho) < rhotol) {
	*info = -10;
    }

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of CGSREVCOM */

} /* ccgsrevcom_ */

/*     END SUBROUTINE cCGSREVCOM */
/* Subroutine */ int zcgsrevcom_(integer *n, doublecomplex *b, doublecomplex *
	x, doublecomplex *work, integer *ldw, integer *iter, doublereal *
	resid, integer *info, integer *ndx1, integer *ndx2, doublecomplex *
	sclr1, doublecomplex *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublecomplex z__1, z__2;

    /* Builtin functions */
    double z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *), pow_zi(
	    doublecomplex *, doublecomplex *, integer *);

    /* Local variables */
    static integer p, q, r__, u;
    extern doublereal dgetbreak_(void);
    static doublecomplex rho;
    static doublereal tol;
    static doublecomplex rho1, beta;
    static integer phat, qhat, rlbl, uhat, vhat, rtld, need1, need2;
    static doublereal bnrm2;
    static doublecomplex alpha;
    extern /* Subroutine */ int zscal_(integer *, doublecomplex *, 
	    doublecomplex *, integer *);
    static integer maxit;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), zaxpy_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);
    static doublecomplex tmpval;
    static doublereal rhotol;
    extern /* Double Complex */ VOID wzdotc_(doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     Eijkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGS solves the linear system Ax = b using the */
/*  Conjugate Gradient Squared iterative method with preconditioning. */

/*  Convergence test: ( norm( b - A*x ) / norm( b ) ) .ls.  TOL. */
/*  For other measures, see the above reference. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess. This is commonly set to */
/*          the zero vector. The user should be warned that for */
/*          this particular algorithm, an initial guess close to */
/*          the actual solution can result in divergence. */
/*          On exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors PHAT and QHAT, and UHAT and VHAT share */
/*          the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On ouput, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. */
/*          .gt.   0: Convergence not achieved. This will be set */
/*                to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occured */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If RHO become smaller than some tolerance, */
/*                   the program will terminate. Here we check */
/*                   against tolerance BREAKTOL. */

/*                   -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                        orthogonal. */

/*  NDX1    (input/output) INTEGER. */
/*  NDX2    On entry in INIT call contain indices required by interface */
/*          level for stopping test. */
/*          All other times, used as output, to indicate indices into */
/*          WORK[] for the MATVEC, PSOLVE done by the interface level. */

/*  SCLR1   (output) DOUBLE PRECISION. */
/*  SCLR2   Used to pass the scalars used in MATVEC. Scalars are reqd because */
/*          original routines use dgemv. */

/*  IJOB    (input/output) INTEGER. */
/*          Used to communicate job code between the two levels. */

/*  BLAS CALLS:    DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================= */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;

    /* Function Body */
    if (*ijob == 1) {
	goto L1;
    } else if (*ijob == 2) {
/*        here we do resumption handling */
	if (rlbl == 2) {
	    goto L2;
	}
	if (rlbl == 3) {
	    goto L3;
	}
	if (rlbl == 4) {
	    goto L4;
	}
	if (rlbl == 5) {
	    goto L5;
	}
	if (rlbl == 6) {
	    goto L6;
	}
	if (rlbl == 7) {
	    goto L7;
	}
/*        if neither of these, then error */
	*info = -6;
	goto L20;
    }


/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    p = 3;
    phat = 4;
    q = 5;
    qhat = 6;
    u = 6;
    uhat = 7;
    vhat = 7;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (qhat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (u - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (uhat - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (rtld - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (qhat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (u - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (uhat - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (vhat - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerance parameter. */

    rhotol = dgetbreak_();

/*     Set initial residual. */

    zcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dznrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using RTLD[] as temp. storage. */
/* ********CALL zCOPY(N, X, 1, WORK(1,RTLD), 1) */
	sclr1->r = -1., sclr1->i = 0.;
	sclr2->r = 1., sclr2->i = 0.;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 3;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (dznrm2_(n, &work[r__ * work_dim1 + 1], &c__1) <= tol) {
	goto L30;
    }

    bnrm2 = dznrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

/*     Choose RTLD such that initially, (R,RTLD) = RHO is not equal to 0. */
/*     Here we choose RTLD = R. */

    zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    *iter = 0;

L10:

/*     Perform Conjugate Gradient Squared iteration. */

    ++(*iter);

    wzdotc_(&z__1, n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);
    rho.r = z__1.r, rho.i = z__1.i;
    if (z_abs(&rho) < rhotol) {
	goto L25;
    }

/*        Compute direction vectors U and P. */

    if (*iter > 1) {

/*           Compute U. */

	z_div(&z__1, &rho, &rho1);
	beta.r = z__1.r, beta.i = z__1.i;
	zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	zaxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[u * work_dim1 
		+ 1], &c__1);

/*           Compute P. */

	pow_zi(&z__1, &beta, &c__2);
	zscal_(n, &z__1, &work[p * work_dim1 + 1], &c__1);
	zaxpy_(n, &beta, &work[q * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	tmpval.r = 1., tmpval.i = 0.;
	zaxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[u * work_dim1 + 1],
		 &c__1);
	zcopy_(n, &work[u * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1], &
		c__1);
    }

/*        Compute direction adjusting scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,VHAT) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (vhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    wzdotc_(&z__2, n, &work[rtld * work_dim1 + 1], &c__1, &work[vhat * 
	    work_dim1 + 1], &c__1);
    z_div(&z__1, &rho, &z__2);
    alpha.r = z__1.r, alpha.i = z__1.i;

    zcopy_(n, &work[u * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1], &
	    c__1);
    z__1.r = -alpha.r, z__1.i = -alpha.i;
    zaxpy_(n, &z__1, &work[vhat * work_dim1 + 1], &c__1, &work[q * work_dim1 
	    + 1], &c__1);

/*        Compute direction adjusting vectORT UHAT. */
/*        PHAT is being used as temporary storage here. */

    zcopy_(n, &work[q * work_dim1 + 1], &c__1, &work[phat * work_dim1 + 1], &
	    c__1);
    tmpval.r = 1., tmpval.i = 0.;
    zaxpy_(n, &tmpval, &work[u * work_dim1 + 1], &c__1, &work[phat * 
	    work_dim1 + 1], &c__1);
/* ********CALL PSOLVE( WORK(1,UHAT), WORK(1,PHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (phat - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*        Compute new solution approximation vector X. */

    zaxpy_(n, &alpha, &work[uhat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual R and check for tolerance. */

/* ********CALL MATVEC( ONE, WORK(1,UHAT), ZERO, WORK(1,QHAT) ) */

    *ndx1 = (uhat - 1) * *ldw + 1;
    *ndx2 = (qhat - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    z__1.r = -alpha.r, z__1.i = -alpha.i;
    zaxpy_(n, &z__1, &work[qhat * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);

/* ********RESID = dzNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 4;
    return 0;

/* **************** */
L7:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    rho1.r = rho.r, rho1.i = rho.i;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (z_abs(&rho) < rhotol) {
	*info = -10;
    }

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of CGSREVCOM */

} /* zcgsrevcom_ */

