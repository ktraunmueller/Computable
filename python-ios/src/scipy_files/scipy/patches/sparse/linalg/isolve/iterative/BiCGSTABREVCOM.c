/* BiCGSTABREVCOM.f -- translated by f2c (version 20100827).
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
/* Subroutine */ int sbicgstabrevcom_(integer *n, real *b, real *x, real *
	work, integer *ldw, integer *iter, real *resid, integer *info, 
	integer *ndx1, integer *ndx2, real *sclr1, real *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    real r__1;

    /* Local variables */
    static real omegatol;
    static integer p, r__, s, t, v;
    extern doublereal sgetbreak_(void);
    static real rho, tol, rho1, beta;
    static integer phat, rlbl, shat, rtld, need1, need2;
    static real bnrm2, alpha, omega;
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

/*  BICGSTAB solves the linear system A*x = b using the */
/*  BiConjugate Gradient Stabilized iterative method with */
/*  preconditioning. */

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
/*          the zero vector. */
/*          On exit, if INFO = 0, the iterated approximate solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors R and S shared the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On output, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. Iterated approximate solution returned. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*           .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N  .ls.  0 */
/*                   -2: LDW  .ls.  N */
/*                   -3: Maximum number of iterations ITER  .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO  .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */
/*                  -11: OMEGA  .ls.  BREAKTOL: S and T have become */
/*                                         orthogonal relative to T'*T. */

/*                  BREAKTOL is set in func GETBREAK. */

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

/*  BLAS CALLS: DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */
/*     .. */
/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, so test IJOB */
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
    v = 4;
    t = 5;
    phat = 6;
    shat = 7;
    s = 1;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (t - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (shat - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (s - 1) * *ldw + 1;
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
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (t - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (shat - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (s - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set parameter tolerances. */

    rhotol = sgetbreak_();
    omegatol = sgetbreak_();

/*     Set initial residual. */

    scopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wsnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL sMATVEC( -ONE, X, ONE, WORK(1,R) ) */
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
    scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    bnrm2 = wsnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

    *iter = 0;

L10:

/*     Perform BiConjugate Gradient Stabilized iteration. */

    ++(*iter);

    rho = wsdot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * work_dim1 
	    + 1], &c__1);
    if (dabs(rho) < rhotol) {
	goto L25;
    }

/*        Compute vector P. */

    if (*iter > 1) {
	beta = rho / rho1 * (alpha / omega);
	r__1 = -omega;
	saxpy_(n, &r__1, &work[v * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	sscal_(n, &beta, &work[p * work_dim1 + 1], &c__1);
	tmpval = 1.f;
	saxpy_(n, &tmpval, &work[r__ * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
    }

/*        Compute direction adjusting vector PHAT and scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*     Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,V) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (v - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.f;
    *sclr2 = 0.f;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    alpha = rho / wsdot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[v * 
	    work_dim1 + 1], &c__1);

/*        Early check for tolerance. */

    r__1 = -alpha;
    saxpy_(n, &r__1, &work[v * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);
    scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1], &
	    c__1);
    if (wsnrm2_(n, &work[s * work_dim1 + 1], &c__1) <= tol) {
	saxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
	*resid = wsnrm2_(n, &work[s * work_dim1 + 1], &c__1) / bnrm2;
	goto L30;
    } else {

/*           Compute stabilizer vector SHAT and scalar OMEGA. */

/* ***********CALL PSOLVE( WORK(1,SHAT), WORK(1,S) ) */

	*ndx1 = (shat - 1) * *ldw + 1;
	*ndx2 = (s - 1) * *ldw + 1;
/*     Prepare for return & return */
	rlbl = 5;
	*ijob = 2;
	return 0;
    }

/* **************** */
L5:
/* **************** */

/* ***********CALL MATVEC( ONE, WORK(1,SHAT), ZERO, WORK(1,T) ) */

    *ndx1 = (shat - 1) * *ldw + 1;
    *ndx2 = (t - 1) * *ldw + 1;
/*           Prepare for return & return */
    *sclr1 = 1.f;
    *sclr2 = 0.f;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    omega = wsdot_(n, &work[t * work_dim1 + 1], &c__1, &work[s * work_dim1 + 
	    1], &c__1) / wsdot_(n, &work[t * work_dim1 + 1], &c__1, &work[t * 
	    work_dim1 + 1], &c__1);

/*           Compute new solution approximation vector X. */

    saxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
    saxpy_(n, &omega, &work[shat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*     Compute residual R, check for tolerance. */

    r__1 = -omega;
    saxpy_(n, &r__1, &work[t * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ***********RESID = DNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ***********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*     Prepare for resumption & return */
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

    if (dabs(omega) < omegatol) {
	goto L25;
    } else {
	rho1 = rho;
	goto L10;
    }

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (dabs(rho) < rhotol) {
	*info = -10;
    } else if (dabs(omega) < omegatol) {
	*info = -11;
    }
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGSTABREVCOM */

} /* sbicgstabrevcom_ */

/*     END SUBROUTINE sBICGSTABREVCOM */
/* Subroutine */ int dbicgstabrevcom_(integer *n, doublereal *b, doublereal *
	x, doublereal *work, integer *ldw, integer *iter, doublereal *resid, 
	integer *info, integer *ndx1, integer *ndx2, doublereal *sclr1, 
	doublereal *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublereal d__1;

    /* Local variables */
    static doublereal omegatol;
    static integer p, r__, s, t, v;
    extern doublereal dgetbreak_(void);
    static doublereal rho, tol, rho1, beta;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);
    static integer phat, rlbl, shat, rtld, need1, need2;
    static doublereal bnrm2;
    extern doublereal dnrm2_(integer *, doublereal *, integer *);
    static doublereal alpha;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal omega;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
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

/*  BICGSTAB solves the linear system A*x = b using the */
/*  BiConjugate Gradient Stabilized iterative method with */
/*  preconditioning. */

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
/*          the zero vector. */
/*          On exit, if INFO = 0, the iterated approximate solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors R and S shared the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On output, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. Iterated approximate solution returned. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*           .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N  .ls.  0 */
/*                   -2: LDW  .ls.  N */
/*                   -3: Maximum number of iterations ITER  .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO  .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */
/*                  -11: OMEGA  .ls.  BREAKTOL: S and T have become */
/*                                         orthogonal relative to T'*T. */

/*                  BREAKTOL is set in func GETBREAK. */

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

/*  BLAS CALLS: DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */
/*     .. */
/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, so test IJOB */
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
    v = 4;
    t = 5;
    phat = 6;
    shat = 7;
    s = 1;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (t - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (shat - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (s - 1) * *ldw + 1;
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
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (t - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (shat - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (s - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set parameter tolerances. */

    rhotol = dgetbreak_();
    omegatol = dgetbreak_();

/*     Set initial residual. */

    dcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dnrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL dMATVEC( -ONE, X, ONE, WORK(1,R) ) */
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
    dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    bnrm2 = dnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

    *iter = 0;

L10:

/*     Perform BiConjugate Gradient Stabilized iteration. */

    ++(*iter);

    rho = ddot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * work_dim1 
	    + 1], &c__1);
    if (abs(rho) < rhotol) {
	goto L25;
    }

/*        Compute vector P. */

    if (*iter > 1) {
	beta = rho / rho1 * (alpha / omega);
	d__1 = -omega;
	daxpy_(n, &d__1, &work[v * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	dscal_(n, &beta, &work[p * work_dim1 + 1], &c__1);
	tmpval = 1.;
	daxpy_(n, &tmpval, &work[r__ * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
    }

/*        Compute direction adjusting vector PHAT and scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*     Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,V) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (v - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.;
    *sclr2 = 0.;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    alpha = rho / ddot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[v * 
	    work_dim1 + 1], &c__1);

/*        Early check for tolerance. */

    d__1 = -alpha;
    daxpy_(n, &d__1, &work[v * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);
    dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1], &
	    c__1);
    if (dnrm2_(n, &work[s * work_dim1 + 1], &c__1) <= tol) {
	daxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
	*resid = dnrm2_(n, &work[s * work_dim1 + 1], &c__1) / bnrm2;
	goto L30;
    } else {

/*           Compute stabilizer vector SHAT and scalar OMEGA. */

/* ***********CALL PSOLVE( WORK(1,SHAT), WORK(1,S) ) */

	*ndx1 = (shat - 1) * *ldw + 1;
	*ndx2 = (s - 1) * *ldw + 1;
/*     Prepare for return & return */
	rlbl = 5;
	*ijob = 2;
	return 0;
    }

/* **************** */
L5:
/* **************** */

/* ***********CALL MATVEC( ONE, WORK(1,SHAT), ZERO, WORK(1,T) ) */

    *ndx1 = (shat - 1) * *ldw + 1;
    *ndx2 = (t - 1) * *ldw + 1;
/*           Prepare for return & return */
    *sclr1 = 1.;
    *sclr2 = 0.;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    omega = ddot_(n, &work[t * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1]
	    , &c__1) / ddot_(n, &work[t * work_dim1 + 1], &c__1, &work[t * 
	    work_dim1 + 1], &c__1);

/*           Compute new solution approximation vector X. */

    daxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
    daxpy_(n, &omega, &work[shat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*     Compute residual R, check for tolerance. */

    d__1 = -omega;
    daxpy_(n, &d__1, &work[t * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ***********RESID = DNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ***********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*     Prepare for resumption & return */
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

    if (abs(omega) < omegatol) {
	goto L25;
    } else {
	rho1 = rho;
	goto L10;
    }

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (abs(rho) < rhotol) {
	*info = -10;
    } else if (abs(omega) < omegatol) {
	*info = -11;
    }
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGSTABREVCOM */

} /* dbicgstabrevcom_ */

/*     END SUBROUTINE dBICGSTABREVCOM */
/* Subroutine */ int cbicgstabrevcom_(integer *n, complex *b, complex *x, 
	complex *work, integer *ldw, integer *iter, real *resid, integer *
	info, integer *ndx1, integer *ndx2, complex *sclr1, complex *sclr2, 
	integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    double c_abs(complex *);
    void c_div(complex *, complex *, complex *);

    /* Local variables */
    static real omegatol;
    static integer p, r__, s, t, v;
    extern doublereal sgetbreak_(void);
    static complex rho;
    static real tol;
    static complex rho1, beta;
    static integer phat, rlbl, shat, rtld, need1, need2;
    static real bnrm2;
    static complex alpha;
    extern /* Subroutine */ int cscal_(integer *, complex *, complex *, 
	    integer *);
    static complex omega;
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *);
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

/*  BICGSTAB solves the linear system A*x = b using the */
/*  BiConjugate Gradient Stabilized iterative method with */
/*  preconditioning. */

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
/*          the zero vector. */
/*          On exit, if INFO = 0, the iterated approximate solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors R and S shared the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On output, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. Iterated approximate solution returned. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*           .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N  .ls.  0 */
/*                   -2: LDW  .ls.  N */
/*                   -3: Maximum number of iterations ITER  .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO  .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */
/*                  -11: OMEGA  .ls.  BREAKTOL: S and T have become */
/*                                         orthogonal relative to T'*T. */

/*                  BREAKTOL is set in func GETBREAK. */

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

/*  BLAS CALLS: DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */
/*     .. */
/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, so test IJOB */
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
    v = 4;
    t = 5;
    phat = 6;
    shat = 7;
    s = 1;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (t - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (shat - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (s - 1) * *ldw + 1;
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
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (t - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (shat - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (s - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set parameter tolerances. */

    rhotol = sgetbreak_();
    omegatol = sgetbreak_();

/*     Set initial residual. */

    ccopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wscnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL cMATVEC( -ONE, X, ONE, WORK(1,R) ) */
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
    ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    bnrm2 = wscnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

    *iter = 0;

L10:

/*     Perform BiConjugate Gradient Stabilized iteration. */

    ++(*iter);

    wcdotc_(&q__1, n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);
    rho.r = q__1.r, rho.i = q__1.i;
    if (c_abs(&rho) < rhotol) {
	goto L25;
    }

/*        Compute vector P. */

    if (*iter > 1) {
	c_div(&q__2, &rho, &rho1);
	c_div(&q__3, &alpha, &omega);
	q__1.r = q__2.r * q__3.r - q__2.i * q__3.i, q__1.i = q__2.r * q__3.i 
		+ q__2.i * q__3.r;
	beta.r = q__1.r, beta.i = q__1.i;
	q__1.r = -omega.r, q__1.i = -omega.i;
	caxpy_(n, &q__1, &work[v * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	cscal_(n, &beta, &work[p * work_dim1 + 1], &c__1);
	tmpval.r = 1.f, tmpval.i = 0.f;
	caxpy_(n, &tmpval, &work[r__ * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
    }

/*        Compute direction adjusting vector PHAT and scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*     Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,V) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (v - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    wcdotc_(&q__2, n, &work[rtld * work_dim1 + 1], &c__1, &work[v * work_dim1 
	    + 1], &c__1);
    c_div(&q__1, &rho, &q__2);
    alpha.r = q__1.r, alpha.i = q__1.i;

/*        Early check for tolerance. */

    q__1.r = -alpha.r, q__1.i = -alpha.i;
    caxpy_(n, &q__1, &work[v * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);
    ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1], &
	    c__1);
    if (wscnrm2_(n, &work[s * work_dim1 + 1], &c__1) <= tol) {
	caxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
	*resid = wscnrm2_(n, &work[s * work_dim1 + 1], &c__1) / bnrm2;
	goto L30;
    } else {

/*           Compute stabilizer vector SHAT and scalar OMEGA. */

/* ***********CALL PSOLVE( WORK(1,SHAT), WORK(1,S) ) */

	*ndx1 = (shat - 1) * *ldw + 1;
	*ndx2 = (s - 1) * *ldw + 1;
/*     Prepare for return & return */
	rlbl = 5;
	*ijob = 2;
	return 0;
    }

/* **************** */
L5:
/* **************** */

/* ***********CALL MATVEC( ONE, WORK(1,SHAT), ZERO, WORK(1,T) ) */

    *ndx1 = (shat - 1) * *ldw + 1;
    *ndx2 = (t - 1) * *ldw + 1;
/*           Prepare for return & return */
    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    wcdotc_(&q__2, n, &work[t * work_dim1 + 1], &c__1, &work[s * work_dim1 + 
	    1], &c__1);
    wcdotc_(&q__3, n, &work[t * work_dim1 + 1], &c__1, &work[t * work_dim1 + 
	    1], &c__1);
    c_div(&q__1, &q__2, &q__3);
    omega.r = q__1.r, omega.i = q__1.i;

/*           Compute new solution approximation vector X. */

    caxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
    caxpy_(n, &omega, &work[shat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*     Compute residual R, check for tolerance. */

    q__1.r = -omega.r, q__1.i = -omega.i;
    caxpy_(n, &q__1, &work[t * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ***********RESID = DNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ***********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*     Prepare for resumption & return */
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

    if (c_abs(&omega) < omegatol) {
	goto L25;
    } else {
	rho1.r = rho.r, rho1.i = rho.i;
	goto L10;
    }

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (c_abs(&rho) < rhotol) {
	*info = -10;
    } else if (c_abs(&omega) < omegatol) {
	*info = -11;
    }
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGSTABREVCOM */

} /* cbicgstabrevcom_ */

/*     END SUBROUTINE cBICGSTABREVCOM */
/* Subroutine */ int zbicgstabrevcom_(integer *n, doublecomplex *b, 
	doublecomplex *x, doublecomplex *work, integer *ldw, integer *iter, 
	doublereal *resid, integer *info, integer *ndx1, integer *ndx2, 
	doublecomplex *sclr1, doublecomplex *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    double z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *);

    /* Local variables */
    static doublereal omegatol;
    static integer p, r__, s, t, v;
    extern doublereal dgetbreak_(void);
    static doublecomplex rho;
    static doublereal tol;
    static doublecomplex rho1, beta;
    static integer phat, rlbl, shat, rtld, need1, need2;
    static doublereal bnrm2;
    static doublecomplex alpha, omega;
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

/*  BICGSTAB solves the linear system A*x = b using the */
/*  BiConjugate Gradient Stabilized iterative method with */
/*  preconditioning. */

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
/*          the zero vector. */
/*          On exit, if INFO = 0, the iterated approximate solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,7) */
/*          Workspace for residual, direction vector, etc. */
/*          Note that vectors R and S shared the same workspace. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable convergence measure for */
/*          norm( b - A*x ) / norm( b ). */
/*          On output, the final value of this measure. */

/*  INFO    (output) INTEGER */

/*          =  0: Successful exit. Iterated approximate solution returned. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*           .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N  .ls.  0 */
/*                   -2: LDW  .ls.  N */
/*                   -3: Maximum number of iterations ITER  .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO  .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */
/*                  -11: OMEGA  .ls.  BREAKTOL: S and T have become */
/*                                         orthogonal relative to T'*T. */

/*                  BREAKTOL is set in func GETBREAK. */

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

/*  BLAS CALLS: DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */
/*     .. */
/*     .. External Funcs .. */
/*     .. */
/*     .. Intrinsic Funcs .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Entry point, so test IJOB */
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
    v = 4;
    t = 5;
    phat = 6;
    shat = 7;
    s = 1;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (t - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (phat - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (shat - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (s - 1) * *ldw + 1;
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
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (t - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (phat - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (shat - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (s - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set parameter tolerances. */

    rhotol = dgetbreak_();
    omegatol = dgetbreak_();

/*     Set initial residual. */

    zcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dznrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL zMATVEC( -ONE, X, ONE, WORK(1,R) ) */
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
    zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[rtld * work_dim1 + 1], 
	    &c__1);

    bnrm2 = dznrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

    *iter = 0;

L10:

/*     Perform BiConjugate Gradient Stabilized iteration. */

    ++(*iter);

    wzdotc_(&z__1, n, &work[rtld * work_dim1 + 1], &c__1, &work[r__ * 
	    work_dim1 + 1], &c__1);
    rho.r = z__1.r, rho.i = z__1.i;
    if (z_abs(&rho) < rhotol) {
	goto L25;
    }

/*        Compute vector P. */

    if (*iter > 1) {
	z_div(&z__2, &rho, &rho1);
	z_div(&z__3, &alpha, &omega);
	z__1.r = z__2.r * z__3.r - z__2.i * z__3.i, z__1.i = z__2.r * z__3.i 
		+ z__2.i * z__3.r;
	beta.r = z__1.r, beta.i = z__1.i;
	z__1.r = -omega.r, z__1.i = -omega.i;
	zaxpy_(n, &z__1, &work[v * work_dim1 + 1], &c__1, &work[p * work_dim1 
		+ 1], &c__1);
	zscal_(n, &beta, &work[p * work_dim1 + 1], &c__1);
	tmpval.r = 1., tmpval.i = 0.;
	zaxpy_(n, &tmpval, &work[r__ * work_dim1 + 1], &c__1, &work[p * 
		work_dim1 + 1], &c__1);
    } else {
	zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
    }

/*        Compute direction adjusting vector PHAT and scalar ALPHA. */

/* ********CALL PSOLVE( WORK(1,PHAT), WORK(1,P) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (p - 1) * *ldw + 1;
/*     Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

/* ********CALL MATVEC( ONE, WORK(1,PHAT), ZERO, WORK(1,V) ) */

    *ndx1 = (phat - 1) * *ldw + 1;
    *ndx2 = (v - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    rlbl = 4;
    *ijob = 1;
    return 0;

/* **************** */
L4:
/* **************** */

    wzdotc_(&z__2, n, &work[rtld * work_dim1 + 1], &c__1, &work[v * work_dim1 
	    + 1], &c__1);
    z_div(&z__1, &rho, &z__2);
    alpha.r = z__1.r, alpha.i = z__1.i;

/*        Early check for tolerance. */

    z__1.r = -alpha.r, z__1.i = -alpha.i;
    zaxpy_(n, &z__1, &work[v * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);
    zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1], &
	    c__1);
    if (dznrm2_(n, &work[s * work_dim1 + 1], &c__1) <= tol) {
	zaxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
	*resid = dznrm2_(n, &work[s * work_dim1 + 1], &c__1) / bnrm2;
	goto L30;
    } else {

/*           Compute stabilizer vector SHAT and scalar OMEGA. */

/* ***********CALL PSOLVE( WORK(1,SHAT), WORK(1,S) ) */

	*ndx1 = (shat - 1) * *ldw + 1;
	*ndx2 = (s - 1) * *ldw + 1;
/*     Prepare for return & return */
	rlbl = 5;
	*ijob = 2;
	return 0;
    }

/* **************** */
L5:
/* **************** */

/* ***********CALL MATVEC( ONE, WORK(1,SHAT), ZERO, WORK(1,T) ) */

    *ndx1 = (shat - 1) * *ldw + 1;
    *ndx2 = (t - 1) * *ldw + 1;
/*           Prepare for return & return */
    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    wzdotc_(&z__2, n, &work[t * work_dim1 + 1], &c__1, &work[s * work_dim1 + 
	    1], &c__1);
    wzdotc_(&z__3, n, &work[t * work_dim1 + 1], &c__1, &work[t * work_dim1 + 
	    1], &c__1);
    z_div(&z__1, &z__2, &z__3);
    omega.r = z__1.r, omega.i = z__1.i;

/*           Compute new solution approximation vector X. */

    zaxpy_(n, &alpha, &work[phat * work_dim1 + 1], &c__1, &x[1], &c__1);
    zaxpy_(n, &omega, &work[shat * work_dim1 + 1], &c__1, &x[1], &c__1);

/*     Compute residual R, check for tolerance. */

    z__1.r = -omega.r, z__1.i = -omega.i;
    zaxpy_(n, &z__1, &work[t * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ***********RESID = DNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ***********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*     Prepare for resumption & return */
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

    if (z_abs(&omega) < omegatol) {
	goto L25;
    } else {
	rho1.r = rho.r, rho1.i = rho.i;
	goto L10;
    }

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    if (z_abs(&rho) < rhotol) {
	*info = -10;
    } else if (z_abs(&omega) < omegatol) {
	*info = -11;
    }
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGSTABREVCOM */

} /* zbicgstabrevcom_ */

