/* BiCGREVCOM.f -- translated by f2c (version 20100827).
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

/*  -*- fortran -*- */
/* Subroutine */ int sbicgrevcom_(integer *n, real *b, real *x, real *work, 
	integer *ldw, integer *iter, real *resid, integer *info, integer *
	ndx1, integer *ndx2, real *sclr1, real *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    real r__1;

    /* Local variables */
    static integer p, q, r__, z__;
    extern doublereal sgetbreak_(void);
    static real rho, tol, rho1, beta;
    static integer rlbl, ptld, rtld, qtld, ztld, need1, need2;
    static real bnrm2, alpha;
    static integer maxit;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *);
    extern doublereal wsdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);
    extern doublereal wsnrm2_(integer *, real *, integer *);
    static real rhotol;



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

/*  BiCG solves the linear system Ax = b using the */
/*  BiConjugate Gradient iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X      (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that Z and Q, and ZTLD and QTLD share workspace. */

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

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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

/* init. */
/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    z__ = 3;
    ztld = 4;
    p = 5;
    ptld = 6;
    q = 3;
    qtld = 4;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ztld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (qtld - 1) * *ldw + 1;
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
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (qtld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown parameters. */

    rhotol = sgetbreak_();

/*     Set initial residual. */

    scopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wsnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        using WORK[RTLD] as temp */
/* ********CALL sCOPY( N, X, 1, WORK(1,RTLD), 1 ) */
	*sclr1 = -1.f;
	*sclr2 = 0.f;
	*ndx1 = (rtld - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 5;
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

/*     Perform BiConjugate Gradient iteration. */

    ++(*iter);

/*        Compute direction vectors PK and PTLD. */

/* ********CALL PSOLVE( WORK(1,Z), WORK(1,R) ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

/* ********CALL PSOLVETRANS( WORK(1,ZTLD), WORK(1,RTLD) ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (rtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 4;
    return 0;
/* **************** */
L4:
/* **************** */


/*         RHO = wsdot( N, WORK(1,Z), 1, WORK(1,RTLD), 1 ) */
    rho = wsdot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[z__ * work_dim1 
	    + 1], &c__1);
    if (dabs(rho) < rhotol) {
	goto L25;
    }

    if (*iter > 1) {
	beta = rho / rho1;
	saxpy_(n, &beta, &work[p * work_dim1 + 1], &c__1, &work[z__ * 
		work_dim1 + 1], &c__1);
/*            CALL sAXPY( N, BETA, WORK(1,PTLD), 1, WORK(1,ZTLD), 1 ) */
	r__1 = beta;
	saxpy_(n, &r__1, &work[ptld * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	scopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	scopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    } else {
	scopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	scopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,Q) ) */

    *sclr1 = 1.f;
    *sclr2 = 0.f;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (q - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 1;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL MATVECTRANS( ONE, WORK(1,PTLD), ZERO, WORK(1,QTLD) ) */

    *sclr1 = 1.f;
    *sclr2 = 0.f;
    *ndx1 = (ptld - 1) * *ldw + 1;
    *ndx2 = (qtld - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 2;
    return 0;
/* **************** */
L6:
/* **************** */
    alpha = rho / wsdot_(n, &work[ptld * work_dim1 + 1], &c__1, &work[q * 
	    work_dim1 + 1], &c__1);

/*        Compute current solution vector x. */

    saxpy_(n, &alpha, &work[p * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    r__1 = -alpha;
    saxpy_(n, &r__1, &work[q * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = wsNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 6;
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

/*         CALL sAXPY( N, -ALPHA, WORK(1,QTLD), 1, WORK(1,RTLD), 1 ) */
    r__1 = -alpha;
    saxpy_(n, &r__1, &work[qtld * work_dim1 + 1], &c__1, &work[rtld * 
	    work_dim1 + 1], &c__1);
    rho1 = rho;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    *info = -10;
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGREVCOM */

} /* sbicgrevcom_ */

/*     END SUBROUTINE sBICGREVCOM */
/* Subroutine */ int dbicgrevcom_(integer *n, doublereal *b, doublereal *x, 
	doublereal *work, integer *ldw, integer *iter, doublereal *resid, 
	integer *info, integer *ndx1, integer *ndx2, doublereal *sclr1, 
	doublereal *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublereal d__1;

    /* Local variables */
    static integer p, q, r__, z__;
    extern doublereal dgetbreak_(void);
    static doublereal rho, tol, rho1, beta;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);
    static integer rlbl, ptld, rtld, qtld, ztld, need1, need2;
    static doublereal bnrm2;
    extern doublereal dnrm2_(integer *, doublereal *, integer *);
    static doublereal alpha;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
    static integer maxit;
    extern /* Subroutine */ int daxpy_(integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *);
    static doublereal rhotol;



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

/*  BiCG solves the linear system Ax = b using the */
/*  BiConjugate Gradient iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X      (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that Z and Q, and ZTLD and QTLD share workspace. */

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

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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

/* init. */
/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    z__ = 3;
    ztld = 4;
    p = 5;
    ptld = 6;
    q = 3;
    qtld = 4;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ztld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (qtld - 1) * *ldw + 1;
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
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (qtld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown parameters. */

    rhotol = dgetbreak_();

/*     Set initial residual. */

    dcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dnrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        using WORK[RTLD] as temp */
/* ********CALL dCOPY( N, X, 1, WORK(1,RTLD), 1 ) */
	*sclr1 = -1.;
	*sclr2 = 0.;
	*ndx1 = (rtld - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 5;
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

/*     Perform BiConjugate Gradient iteration. */

    ++(*iter);

/*        Compute direction vectors PK and PTLD. */

/* ********CALL PSOLVE( WORK(1,Z), WORK(1,R) ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

/* ********CALL PSOLVETRANS( WORK(1,ZTLD), WORK(1,RTLD) ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (rtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 4;
    return 0;
/* **************** */
L4:
/* **************** */


/*         RHO = ddot( N, WORK(1,Z), 1, WORK(1,RTLD), 1 ) */
    rho = ddot_(n, &work[rtld * work_dim1 + 1], &c__1, &work[z__ * work_dim1 
	    + 1], &c__1);
    if (abs(rho) < rhotol) {
	goto L25;
    }

    if (*iter > 1) {
	beta = rho / rho1;
	daxpy_(n, &beta, &work[p * work_dim1 + 1], &c__1, &work[z__ * 
		work_dim1 + 1], &c__1);
/*            CALL dAXPY( N, BETA, WORK(1,PTLD), 1, WORK(1,ZTLD), 1 ) */
	d__1 = beta;
	daxpy_(n, &d__1, &work[ptld * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	dcopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	dcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    } else {
	dcopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	dcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,Q) ) */

    *sclr1 = 1.;
    *sclr2 = 0.;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (q - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 1;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL MATVECTRANS( ONE, WORK(1,PTLD), ZERO, WORK(1,QTLD) ) */

    *sclr1 = 1.;
    *sclr2 = 0.;
    *ndx1 = (ptld - 1) * *ldw + 1;
    *ndx2 = (qtld - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 2;
    return 0;
/* **************** */
L6:
/* **************** */
    alpha = rho / ddot_(n, &work[ptld * work_dim1 + 1], &c__1, &work[q * 
	    work_dim1 + 1], &c__1);

/*        Compute current solution vector x. */

    daxpy_(n, &alpha, &work[p * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    d__1 = -alpha;
    daxpy_(n, &d__1, &work[q * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = dNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 6;
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

/*         CALL dAXPY( N, -ALPHA, WORK(1,QTLD), 1, WORK(1,RTLD), 1 ) */
    d__1 = -alpha;
    daxpy_(n, &d__1, &work[qtld * work_dim1 + 1], &c__1, &work[rtld * 
	    work_dim1 + 1], &c__1);
    rho1 = rho;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    *info = -10;
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGREVCOM */

} /* dbicgrevcom_ */

/*     END SUBROUTINE dBICGREVCOM */
/* Subroutine */ int cbicgrevcom_(integer *n, complex *b, complex *x, complex 
	*work, integer *ldw, integer *iter, real *resid, integer *info, 
	integer *ndx1, integer *ndx2, complex *sclr1, complex *sclr2, integer 
	*ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    complex q__1, q__2;

    /* Builtin functions */
    double c_abs(complex *);
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *);

    /* Local variables */
    static integer p, q, r__, z__;
    extern doublereal sgetbreak_(void);
    static complex rho;
    static real tol;
    static complex rho1, beta;
    static integer rlbl, ptld, rtld, qtld, ztld, need1, need2;
    static real bnrm2;
    static complex alpha;
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *);
    static integer maxit;
    extern /* Subroutine */ int caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *);
    extern /* Complex */ VOID wcdotc_(complex *, integer *, complex *, 
	    integer *, complex *, integer *);
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

/*  BiCG solves the linear system Ax = b using the */
/*  BiConjugate Gradient iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X      (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that Z and Q, and ZTLD and QTLD share workspace. */

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

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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

/* init. */
/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    z__ = 3;
    ztld = 4;
    p = 5;
    ptld = 6;
    q = 3;
    qtld = 4;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ztld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (qtld - 1) * *ldw + 1;
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
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (qtld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown parameters. */

    rhotol = sgetbreak_();

/*     Set initial residual. */

    ccopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wscnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        using WORK[RTLD] as temp */
/* ********CALL cCOPY( N, X, 1, WORK(1,RTLD), 1 ) */
	sclr1->r = -1.f, sclr1->i = 0.f;
	sclr2->r = 0.f, sclr2->i = 0.f;
	*ndx1 = (rtld - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 5;
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

/*     Perform BiConjugate Gradient iteration. */

    ++(*iter);

/*        Compute direction vectors PK and PTLD. */

/* ********CALL PSOLVE( WORK(1,Z), WORK(1,R) ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

/* ********CALL PSOLVETRANS( WORK(1,ZTLD), WORK(1,RTLD) ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (rtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 4;
    return 0;
/* **************** */
L4:
/* **************** */


/*         RHO = wcdotc( N, WORK(1,Z), 1, WORK(1,RTLD), 1 ) */
    wcdotc_(&q__1, n, &work[rtld * work_dim1 + 1], &c__1, &work[z__ * 
	    work_dim1 + 1], &c__1);
    rho.r = q__1.r, rho.i = q__1.i;
    if (c_abs(&rho) < rhotol) {
	goto L25;
    }

    if (*iter > 1) {
	c_div(&q__1, &rho, &rho1);
	beta.r = q__1.r, beta.i = q__1.i;
	caxpy_(n, &beta, &work[p * work_dim1 + 1], &c__1, &work[z__ * 
		work_dim1 + 1], &c__1);
/*            CALL cAXPY( N, BETA, WORK(1,PTLD), 1, WORK(1,ZTLD), 1 ) */
	r_cnjg(&q__1, &beta);
	caxpy_(n, &q__1, &work[ptld * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	ccopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	ccopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    } else {
	ccopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	ccopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,Q) ) */

    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (q - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 1;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL MATVECTRANS( ONE, WORK(1,PTLD), ZERO, WORK(1,QTLD) ) */

    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    *ndx1 = (ptld - 1) * *ldw + 1;
    *ndx2 = (qtld - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 2;
    return 0;
/* **************** */
L6:
/* **************** */
    wcdotc_(&q__2, n, &work[ptld * work_dim1 + 1], &c__1, &work[q * work_dim1 
	    + 1], &c__1);
    c_div(&q__1, &rho, &q__2);
    alpha.r = q__1.r, alpha.i = q__1.i;

/*        Compute current solution vector x. */

    caxpy_(n, &alpha, &work[p * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    q__1.r = -alpha.r, q__1.i = -alpha.i;
    caxpy_(n, &q__1, &work[q * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = wscNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 6;
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

/*         CALL cAXPY( N, -ALPHA, WORK(1,QTLD), 1, WORK(1,RTLD), 1 ) */
    r_cnjg(&q__2, &alpha);
    q__1.r = -q__2.r, q__1.i = -q__2.i;
    caxpy_(n, &q__1, &work[qtld * work_dim1 + 1], &c__1, &work[rtld * 
	    work_dim1 + 1], &c__1);
    rho1.r = rho.r, rho1.i = rho.i;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    *info = -10;
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGREVCOM */

} /* cbicgrevcom_ */

/*     END SUBROUTINE cBICGREVCOM */
/* Subroutine */ int zbicgrevcom_(integer *n, doublecomplex *b, doublecomplex 
	*x, doublecomplex *work, integer *ldw, integer *iter, doublereal *
	resid, integer *info, integer *ndx1, integer *ndx2, doublecomplex *
	sclr1, doublecomplex *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublecomplex z__1, z__2;

    /* Builtin functions */
    double z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *), d_cnjg(
	    doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer p, q, r__, z__;
    extern doublereal dgetbreak_(void);
    static doublecomplex rho;
    static doublereal tol;
    static doublecomplex rho1, beta;
    static integer rlbl, ptld, rtld, qtld, ztld, need1, need2;
    static doublereal bnrm2;
    static doublecomplex alpha;
    static integer maxit;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), zaxpy_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);
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

/*  BiCG solves the linear system Ax = b using the */
/*  BiConjugate Gradient iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X      (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that Z and Q, and ZTLD and QTLD share workspace. */

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

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */
/*                   -5: Erroneous NDX1/NDX2 in INIT call. */
/*                   -6: Erroneous RLBL. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO .ls.  BREAKTOL: RHO and RTLD have become */
/*                                       orthogonal. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, */
/*  ============================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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

/* init. */
/* **************** */
L1:
/* **************** */

    *info = 0;
    maxit = *iter;
    tol = *resid;

/*     Alias workspace columns. */

    r__ = 1;
    rtld = 2;
    z__ = 3;
    ztld = 4;
    p = 5;
    ptld = 6;
    q = 3;
    qtld = 4;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (rtld - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ztld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (qtld - 1) * *ldw + 1;
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
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (qtld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown parameters. */

    rhotol = dgetbreak_();

/*     Set initial residual. */

    zcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dznrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        using WORK[RTLD] as temp */
/* ********CALL zCOPY( N, X, 1, WORK(1,RTLD), 1 ) */
	sclr1->r = -1., sclr1->i = 0.;
	sclr2->r = 0., sclr2->i = 0.;
	*ndx1 = (rtld - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 5;
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

/*     Perform BiConjugate Gradient iteration. */

    ++(*iter);

/*        Compute direction vectors PK and PTLD. */

/* ********CALL PSOLVE( WORK(1,Z), WORK(1,R) ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

/* ********CALL PSOLVETRANS( WORK(1,ZTLD), WORK(1,RTLD) ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (rtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 4;
    return 0;
/* **************** */
L4:
/* **************** */


/*         RHO = wzdotc( N, WORK(1,Z), 1, WORK(1,RTLD), 1 ) */
    wzdotc_(&z__1, n, &work[rtld * work_dim1 + 1], &c__1, &work[z__ * 
	    work_dim1 + 1], &c__1);
    rho.r = z__1.r, rho.i = z__1.i;
    if (z_abs(&rho) < rhotol) {
	goto L25;
    }

    if (*iter > 1) {
	z_div(&z__1, &rho, &rho1);
	beta.r = z__1.r, beta.i = z__1.i;
	zaxpy_(n, &beta, &work[p * work_dim1 + 1], &c__1, &work[z__ * 
		work_dim1 + 1], &c__1);
/*            CALL zAXPY( N, BETA, WORK(1,PTLD), 1, WORK(1,ZTLD), 1 ) */
	d_cnjg(&z__1, &beta);
	zaxpy_(n, &z__1, &work[ptld * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	zcopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	zcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    } else {
	zcopy_(n, &work[z__ * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1],
		 &c__1);
	zcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
		+ 1], &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,Q) ) */

    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (q - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 1;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL MATVECTRANS( ONE, WORK(1,PTLD), ZERO, WORK(1,QTLD) ) */

    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    *ndx1 = (ptld - 1) * *ldw + 1;
    *ndx2 = (qtld - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 2;
    return 0;
/* **************** */
L6:
/* **************** */
    wzdotc_(&z__2, n, &work[ptld * work_dim1 + 1], &c__1, &work[q * work_dim1 
	    + 1], &c__1);
    z_div(&z__1, &rho, &z__2);
    alpha.r = z__1.r, alpha.i = z__1.i;

/*        Compute current solution vector x. */

    zaxpy_(n, &alpha, &work[p * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    z__1.r = -alpha.r, z__1.i = -alpha.i;
    zaxpy_(n, &z__1, &work[q * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = dzNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 7;
    *ijob = 6;
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

/*         CALL zAXPY( N, -ALPHA, WORK(1,QTLD), 1, WORK(1,RTLD), 1 ) */
    d_cnjg(&z__2, &alpha);
    z__1.r = -z__2.r, z__1.i = -z__2.i;
    zaxpy_(n, &z__1, &work[qtld * work_dim1 + 1], &c__1, &work[rtld * 
	    work_dim1 + 1], &c__1);
    rho1.r = rho.r, rho1.i = rho.i;

    goto L10;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L25:

/*     Set breakdown flag. */

    *info = -10;
    rlbl = -1;
    *ijob = -1;
    return 0;

L30:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of BICGREVCOM */

} /* zbicgrevcom_ */

