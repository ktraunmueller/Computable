/* QMRREVCOM.f -- translated by f2c (version 20100827).
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
static complex c_b180 = {1.f,0.f};
static integer c__2 = 2;
static doublecomplex c_b263 = {1.,0.};

/* -*- fortran -*- */
/* Subroutine */ int sqmrrevcom_(integer *n, real *b, real *x, real *work, 
	integer *ldw, integer *iter, real *resid, integer *info, integer *
	ndx1, integer *ndx2, real *sclr1, real *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static real gammatol, deltatol;
    static integer d__, p, q, r__, s, v, w, y, z__;
    static real c1;
    extern doublereal sgetbreak_(void);
    static real xi, eta, eps, rho, tol, toz, rho1, beta;
    static integer rlbl, ptld, vtld, wtld, ytld, ztld, need1, need2;
    static real bnrm2, gamma, delta, theta;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    static integer maxit;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *);
    static real xitol;
    extern doublereal wsdot_(integer *, real *, integer *, real *, integer *);
    static real gamma1;
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);
    static real theta1;
    extern doublereal wsnrm2_(integer *, real *, integer *);
    static real tmpval, epstol, rhotol, betatol;



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

/*  QMR Method solves the linear system Ax = b using the */
/*  Quasi-Minimal Residual iterative method with preconditioning. */

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


/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,11). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that W and WTLD, Y and YTLD, and Z and ZTLD share */
/*          workspace. */

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
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO   .ls.  BREAKTOL: RHO and RTLD have become */
/*                                         orthogonal. */
/*                  -11: BETA  .ls.  BREAKTOL: EPS too small in relation to DELT */
/*                                         Convergence has stalled. */
/*                  -12: GAMMA .ls.  BREAKTOL: THETA too large. */
/*                                         Convergence has stalled. */
/*                  -13: DELTA .ls.  BREAKTOL: Y and Z have become */
/*                                         orthogonal. */
/*                  -14: EPS   .ls.  BREAKTOL: Q and PTLD have become */
/*                                         orthogonal. */
/*                  -15: XI    .ls.  BREAKTOL: Z too small. */
/*                                   Convergence has stalled. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */

/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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
	if (rlbl == 8) {
	    goto L8;
	}
	if (rlbl == 9) {
	    goto L9;
	}
	if (rlbl == 10) {
	    goto L10;
	}
	if (rlbl == 11) {
	    goto L11;
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
    d__ = 2;
    p = 3;
    ptld = 4;
    q = 5;
    s = 6;
    v = 7;
    vtld = 8;
    w = 9;
    wtld = 9;
    y = 10;
    ytld = 10;
    z__ = 11;
    ztld = 11;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (d__ - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (vtld - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 10) {
	    need1 = (wtld - 1) * *ldw + 1;
	} else if (*ndx1 == 11) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 12) {
	    need1 = (ytld - 1) * *ldw + 1;
	} else if (*ndx1 == 13) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 14) {
	    need1 = (ztld - 1) * *ldw + 1;
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
	    need2 = (d__ - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (vtld - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 10) {
	    need2 = (wtld - 1) * *ldw + 1;
	} else if (*ndx2 == 11) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 12) {
	    need2 = (ytld - 1) * *ldw + 1;
	} else if (*ndx2 == 13) {
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 14) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerances. */

    rhotol = sgetbreak_();
    betatol = sgetbreak_();
    gammatol = sgetbreak_();
    deltatol = sgetbreak_();
    epstol = sgetbreak_();
    xitol = sgetbreak_();

/*     Set initial residual. */

    scopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wsnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        Note: using D as temp */
/* ********CALL sCOPY( N, X, 1, WORK(1,D), 1 ) */
	*sclr1 = -1.f;
	*sclr2 = 0.f;
	*ndx1 = (d__ - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 7;
	return 0;
    }
/* **************** */
L2:
/* **************** */

    if (wsnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L30;
    }

    bnrm2 = wsnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

    scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

    rho = wsnrm2_(n, &work[y * work_dim1 + 1], &c__1);

    scopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 6;
    return 0;
/* **************** */
L4:
/* **************** */

    xi = wsnrm2_(n, &work[z__ * work_dim1 + 1], &c__1);

    gamma = 1.f;
    eta = -1.f;
    theta = 0.f;

    *iter = 0;

L40:

/*     Perform Preconditioned QMR iteration. */

    ++(*iter);

    if (dabs(rho) < rhotol || dabs(xi) < xitol) {
	goto L25;
    }

    scopy_(n, &work[vtld * work_dim1 + 1], &c__1, &work[v * work_dim1 + 1], &
	    c__1);
    tmpval = 1.f / rho;
    sscal_(n, &tmpval, &work[v * work_dim1 + 1], &c__1);
    sscal_(n, &tmpval, &work[y * work_dim1 + 1], &c__1);

    tmpval = 1.f / xi;
    scopy_(n, &work[wtld * work_dim1 + 1], &c__1, &work[w * work_dim1 + 1], &
	    c__1);
    sscal_(n, &tmpval, &work[w * work_dim1 + 1], &c__1);
    sscal_(n, &tmpval, &work[z__ * work_dim1 + 1], &c__1);

    delta = wsdot_(n, &work[z__ * work_dim1 + 1], &c__1, &work[y * work_dim1 
	    + 1], &c__1);
    if (dabs(delta) < deltatol) {
	goto L25;
    }

/* ********CALL PSOLVEQ( WORK(1,YTLD), WORK(1,Y), 'RIGHT' ) */

    *ndx1 = (ytld - 1) * *ldw + 1;
    *ndx2 = (y - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 4;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,ZTLD), WORK(1,Z), 'LEFT' ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (z__ - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 5;
    return 0;
/* **************** */
L6:
/* **************** */


    if (*iter > 1) {
	c1 = -(xi * delta / eps);
	saxpy_(n, &c1, &work[p * work_dim1 + 1], &c__1, &work[ytld * 
		work_dim1 + 1], &c__1);
	scopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	r__1 = -(rho * (delta / eps));
	saxpy_(n, &r__1, &work[q * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	scopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    } else {
	scopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	scopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,PTLD) ) */

    *sclr1 = 1.f;
    *sclr2 = 0.f;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (ptld - 1) * *ldw + 1;
    rlbl = 7;
    *ijob = 1;
    return 0;
/* **************** */
L7:
/* **************** */


    eps = wsdot_(n, &work[q * work_dim1 + 1], &c__1, &work[ptld * work_dim1 + 
	    1], &c__1);
    if (dabs(eps) < epstol) {
	goto L25;
    }

    beta = eps / delta;
    if (dabs(beta) < betatol) {
	goto L25;
    }

    scopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1],
	     &c__1);
    r__1 = -beta;
    saxpy_(n, &r__1, &work[v * work_dim1 + 1], &c__1, &work[vtld * work_dim1 
	    + 1], &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 8;
    *ijob = 3;
    return 0;

/* **************** */
L8:
/* **************** */
    rho1 = rho;
    rho = wsnrm2_(n, &work[y * work_dim1 + 1], &c__1);

    scopy_(n, &work[w * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], &
	    c__1);
/* ********CALL MATVECTRANS( ONE, WORK(1,Q), -BETA, WORK(1,WTLD) ) */

    *sclr1 = 1.f;
    *sclr2 = -beta;
    *ndx1 = (q - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 9;
    *ijob = 2;
    return 0;
/* **************** */
L9:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 10;
    *ijob = 6;
    return 0;
/* **************** */
L10:
/* **************** */


    xi = wsnrm2_(n, &work[z__ * work_dim1 + 1], &c__1);

    gamma1 = gamma;
    theta1 = theta;

    theta = rho / (gamma1 * dabs(beta));
/* Computing 2nd power */
    r__1 = theta;
    gamma = 1.f / sqrt(r__1 * r__1 + 1.f);
    if (dabs(gamma) < gammatol) {
	goto L25;
    }

/* Computing 2nd power */
    r__1 = gamma;
/* Computing 2nd power */
    r__2 = gamma1;
    eta = -eta * rho1 * (r__1 * r__1) / (beta * (r__2 * r__2));

    if (*iter > 1) {
/* Computing 2nd power */
	r__2 = theta1 * gamma;
	r__1 = r__2 * r__2;
	sscal_(n, &r__1, &work[d__ * work_dim1 + 1], &c__1);
	saxpy_(n, &eta, &work[p * work_dim1 + 1], &c__1, &work[d__ * 
		work_dim1 + 1], &c__1);
/* Computing 2nd power */
	r__2 = theta1 * gamma;
	r__1 = r__2 * r__2;
	sscal_(n, &r__1, &work[s * work_dim1 + 1], &c__1);
	saxpy_(n, &eta, &work[ptld * work_dim1 + 1], &c__1, &work[s * 
		work_dim1 + 1], &c__1);
    } else {
	scopy_(n, &work[p * work_dim1 + 1], &c__1, &work[d__ * work_dim1 + 1],
		 &c__1);
	sscal_(n, &eta, &work[d__ * work_dim1 + 1], &c__1);
	scopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1]
		, &c__1);
	sscal_(n, &eta, &work[s * work_dim1 + 1], &c__1);
    }

/*        Compute current solution vector x. */

    tmpval = 1.f;
    saxpy_(n, &tmpval, &work[d__ * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    toz = 1.f;
    r__1 = -toz;
    saxpy_(n, &r__1, &work[s * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = wsNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID .LE. TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 11;
    *ijob = 8;
    return 0;

/* **************** */
L11:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    goto L40;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;

    return 0;

L25:

/*     Method breakdown. */

    if (dabs(rho) < rhotol) {
	*info = -10;
    } else if (dabs(beta) < betatol) {
	*info = -11;
    } else if (dabs(gamma) < gammatol) {
	*info = -12;
    } else if (dabs(delta) < deltatol) {
	*info = -13;
    } else if (dabs(eps) < epstol) {
	*info = -14;
    } else if (dabs(xi) < xitol) {
	*info = -15;
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

/*     End of QMRREVCOM */

} /* sqmrrevcom_ */

/*     END SUBROUTINE sQMRREVCOM */
/* Subroutine */ int dqmrrevcom_(integer *n, doublereal *b, doublereal *x, 
	doublereal *work, integer *ldw, integer *iter, doublereal *resid, 
	integer *info, integer *ndx1, integer *ndx2, doublereal *sclr1, 
	doublereal *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal gammatol, deltatol;
    static integer d__, p, q, r__, s, v, w, y, z__;
    extern doublereal dgetbreak_(void);
    static doublereal c1, xi, eta, eps, rho, tol, toz, rho1, beta;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);
    static integer rlbl, ptld, vtld, wtld, ytld, ztld, need1, need2;
    static doublereal bnrm2;
    extern doublereal dnrm2_(integer *, doublereal *, integer *);
    static doublereal gamma;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    static doublereal delta, theta;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
    static integer maxit;
    extern /* Subroutine */ int daxpy_(integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *);
    static doublereal xitol, gamma1, theta1, tmpval, epstol, rhotol, betatol;



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

/*  QMR Method solves the linear system Ax = b using the */
/*  Quasi-Minimal Residual iterative method with preconditioning. */

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


/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,11). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that W and WTLD, Y and YTLD, and Z and ZTLD share */
/*          workspace. */

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
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO   .ls.  BREAKTOL: RHO and RTLD have become */
/*                                         orthogonal. */
/*                  -11: BETA  .ls.  BREAKTOL: EPS too small in relation to DELT */
/*                                         Convergence has stalled. */
/*                  -12: GAMMA .ls.  BREAKTOL: THETA too large. */
/*                                         Convergence has stalled. */
/*                  -13: DELTA .ls.  BREAKTOL: Y and Z have become */
/*                                         orthogonal. */
/*                  -14: EPS   .ls.  BREAKTOL: Q and PTLD have become */
/*                                         orthogonal. */
/*                  -15: XI    .ls.  BREAKTOL: Z too small. */
/*                                   Convergence has stalled. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */

/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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
	if (rlbl == 8) {
	    goto L8;
	}
	if (rlbl == 9) {
	    goto L9;
	}
	if (rlbl == 10) {
	    goto L10;
	}
	if (rlbl == 11) {
	    goto L11;
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
    d__ = 2;
    p = 3;
    ptld = 4;
    q = 5;
    s = 6;
    v = 7;
    vtld = 8;
    w = 9;
    wtld = 9;
    y = 10;
    ytld = 10;
    z__ = 11;
    ztld = 11;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (d__ - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (vtld - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 10) {
	    need1 = (wtld - 1) * *ldw + 1;
	} else if (*ndx1 == 11) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 12) {
	    need1 = (ytld - 1) * *ldw + 1;
	} else if (*ndx1 == 13) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 14) {
	    need1 = (ztld - 1) * *ldw + 1;
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
	    need2 = (d__ - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (vtld - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 10) {
	    need2 = (wtld - 1) * *ldw + 1;
	} else if (*ndx2 == 11) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 12) {
	    need2 = (ytld - 1) * *ldw + 1;
	} else if (*ndx2 == 13) {
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 14) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerances. */

    rhotol = dgetbreak_();
    betatol = dgetbreak_();
    gammatol = dgetbreak_();
    deltatol = dgetbreak_();
    epstol = dgetbreak_();
    xitol = dgetbreak_();

/*     Set initial residual. */

    dcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dnrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        Note: using D as temp */
/* ********CALL dCOPY( N, X, 1, WORK(1,D), 1 ) */
	*sclr1 = -1.;
	*sclr2 = 0.;
	*ndx1 = (d__ - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 7;
	return 0;
    }
/* **************** */
L2:
/* **************** */

    if (dnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L30;
    }

    bnrm2 = dnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

    dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

    rho = dnrm2_(n, &work[y * work_dim1 + 1], &c__1);

    dcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 6;
    return 0;
/* **************** */
L4:
/* **************** */

    xi = dnrm2_(n, &work[z__ * work_dim1 + 1], &c__1);

    gamma = 1.;
    eta = -1.;
    theta = 0.;

    *iter = 0;

L40:

/*     Perform Preconditioned QMR iteration. */

    ++(*iter);

    if (abs(rho) < rhotol || abs(xi) < xitol) {
	goto L25;
    }

    dcopy_(n, &work[vtld * work_dim1 + 1], &c__1, &work[v * work_dim1 + 1], &
	    c__1);
    tmpval = 1. / rho;
    dscal_(n, &tmpval, &work[v * work_dim1 + 1], &c__1);
    dscal_(n, &tmpval, &work[y * work_dim1 + 1], &c__1);

    tmpval = 1. / xi;
    dcopy_(n, &work[wtld * work_dim1 + 1], &c__1, &work[w * work_dim1 + 1], &
	    c__1);
    dscal_(n, &tmpval, &work[w * work_dim1 + 1], &c__1);
    dscal_(n, &tmpval, &work[z__ * work_dim1 + 1], &c__1);

    delta = ddot_(n, &work[z__ * work_dim1 + 1], &c__1, &work[y * work_dim1 + 
	    1], &c__1);
    if (abs(delta) < deltatol) {
	goto L25;
    }

/* ********CALL PSOLVEQ( WORK(1,YTLD), WORK(1,Y), 'RIGHT' ) */

    *ndx1 = (ytld - 1) * *ldw + 1;
    *ndx2 = (y - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 4;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,ZTLD), WORK(1,Z), 'LEFT' ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (z__ - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 5;
    return 0;
/* **************** */
L6:
/* **************** */


    if (*iter > 1) {
	c1 = -(xi * delta / eps);
	daxpy_(n, &c1, &work[p * work_dim1 + 1], &c__1, &work[ytld * 
		work_dim1 + 1], &c__1);
	dcopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	d__1 = -(rho * (delta / eps));
	daxpy_(n, &d__1, &work[q * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	dcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    } else {
	dcopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	dcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,PTLD) ) */

    *sclr1 = 1.;
    *sclr2 = 0.;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (ptld - 1) * *ldw + 1;
    rlbl = 7;
    *ijob = 1;
    return 0;
/* **************** */
L7:
/* **************** */


    eps = ddot_(n, &work[q * work_dim1 + 1], &c__1, &work[ptld * work_dim1 + 
	    1], &c__1);
    if (abs(eps) < epstol) {
	goto L25;
    }

    beta = eps / delta;
    if (abs(beta) < betatol) {
	goto L25;
    }

    dcopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1],
	     &c__1);
    d__1 = -beta;
    daxpy_(n, &d__1, &work[v * work_dim1 + 1], &c__1, &work[vtld * work_dim1 
	    + 1], &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 8;
    *ijob = 3;
    return 0;

/* **************** */
L8:
/* **************** */
    rho1 = rho;
    rho = dnrm2_(n, &work[y * work_dim1 + 1], &c__1);

    dcopy_(n, &work[w * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], &
	    c__1);
/* ********CALL MATVECTRANS( ONE, WORK(1,Q), -BETA, WORK(1,WTLD) ) */

    *sclr1 = 1.;
    *sclr2 = -beta;
    *ndx1 = (q - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 9;
    *ijob = 2;
    return 0;
/* **************** */
L9:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 10;
    *ijob = 6;
    return 0;
/* **************** */
L10:
/* **************** */


    xi = dnrm2_(n, &work[z__ * work_dim1 + 1], &c__1);

    gamma1 = gamma;
    theta1 = theta;

    theta = rho / (gamma1 * abs(beta));
/* Computing 2nd power */
    d__1 = theta;
    gamma = 1. / sqrt(d__1 * d__1 + 1.);
    if (abs(gamma) < gammatol) {
	goto L25;
    }

/* Computing 2nd power */
    d__1 = gamma;
/* Computing 2nd power */
    d__2 = gamma1;
    eta = -eta * rho1 * (d__1 * d__1) / (beta * (d__2 * d__2));

    if (*iter > 1) {
/* Computing 2nd power */
	d__2 = theta1 * gamma;
	d__1 = d__2 * d__2;
	dscal_(n, &d__1, &work[d__ * work_dim1 + 1], &c__1);
	daxpy_(n, &eta, &work[p * work_dim1 + 1], &c__1, &work[d__ * 
		work_dim1 + 1], &c__1);
/* Computing 2nd power */
	d__2 = theta1 * gamma;
	d__1 = d__2 * d__2;
	dscal_(n, &d__1, &work[s * work_dim1 + 1], &c__1);
	daxpy_(n, &eta, &work[ptld * work_dim1 + 1], &c__1, &work[s * 
		work_dim1 + 1], &c__1);
    } else {
	dcopy_(n, &work[p * work_dim1 + 1], &c__1, &work[d__ * work_dim1 + 1],
		 &c__1);
	dscal_(n, &eta, &work[d__ * work_dim1 + 1], &c__1);
	dcopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1]
		, &c__1);
	dscal_(n, &eta, &work[s * work_dim1 + 1], &c__1);
    }

/*        Compute current solution vector x. */

    tmpval = 1.;
    daxpy_(n, &tmpval, &work[d__ * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    toz = 1.;
    d__1 = -toz;
    daxpy_(n, &d__1, &work[s * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = dNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID .LE. TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 11;
    *ijob = 8;
    return 0;

/* **************** */
L11:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    goto L40;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;

    return 0;

L25:

/*     Method breakdown. */

    if (abs(rho) < rhotol) {
	*info = -10;
    } else if (abs(beta) < betatol) {
	*info = -11;
    } else if (abs(gamma) < gammatol) {
	*info = -12;
    } else if (abs(delta) < deltatol) {
	*info = -13;
    } else if (abs(eps) < epstol) {
	*info = -14;
    } else if (abs(xi) < xitol) {
	*info = -15;
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

/*     End of QMRREVCOM */

} /* dqmrrevcom_ */

/*     END SUBROUTINE dQMRREVCOM */
/* Subroutine */ int cqmrrevcom_(integer *n, complex *b, complex *x, complex *
	work, integer *ldw, integer *iter, real *resid, integer *info, 
	integer *ndx1, integer *ndx2, complex *sclr1, complex *sclr2, integer 
	*ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    real r__1;
    complex q__1, q__2, q__3, q__4, q__5, q__6, q__7;

    /* Builtin functions */
    double c_abs(complex *);
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *),
	     pow_ci(complex *, complex *, integer *), c_sqrt(complex *, 
	    complex *);

    /* Local variables */
    static real gammatol, deltatol;
    static integer d__, p, q, r__, s, v, w, y, z__;
    static complex c1;
    extern doublereal sgetbreak_(void);
    static complex xi, eta, eps, rho;
    static real tol;
    static complex toz, rho1, beta;
    static integer rlbl, ptld, vtld, wtld, ytld, ztld, need1, need2;
    static real bnrm2;
    static complex gamma;
    extern /* Subroutine */ int cscal_(integer *, complex *, complex *, 
	    integer *);
    static complex delta, theta;
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *);
    static integer maxit;
    extern /* Subroutine */ int caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *);
    static real xitol;
    static complex gamma1, theta1;
    extern /* Complex */ VOID wcdotc_(complex *, integer *, complex *, 
	    integer *, complex *, integer *);
    static complex tmpval;
    static real epstol, rhotol;
    extern doublereal wscnrm2_(integer *, complex *, integer *);
    static real betatol;



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

/*  QMR Method solves the linear system Ax = b using the */
/*  Quasi-Minimal Residual iterative method with preconditioning. */

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


/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,11). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that W and WTLD, Y and YTLD, and Z and ZTLD share */
/*          workspace. */

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
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO   .ls.  BREAKTOL: RHO and RTLD have become */
/*                                         orthogonal. */
/*                  -11: BETA  .ls.  BREAKTOL: EPS too small in relation to DELT */
/*                                         Convergence has stalled. */
/*                  -12: GAMMA .ls.  BREAKTOL: THETA too large. */
/*                                         Convergence has stalled. */
/*                  -13: DELTA .ls.  BREAKTOL: Y and Z have become */
/*                                         orthogonal. */
/*                  -14: EPS   .ls.  BREAKTOL: Q and PTLD have become */
/*                                         orthogonal. */
/*                  -15: XI    .ls.  BREAKTOL: Z too small. */
/*                                   Convergence has stalled. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */

/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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
	if (rlbl == 8) {
	    goto L8;
	}
	if (rlbl == 9) {
	    goto L9;
	}
	if (rlbl == 10) {
	    goto L10;
	}
	if (rlbl == 11) {
	    goto L11;
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
    d__ = 2;
    p = 3;
    ptld = 4;
    q = 5;
    s = 6;
    v = 7;
    vtld = 8;
    w = 9;
    wtld = 9;
    y = 10;
    ytld = 10;
    z__ = 11;
    ztld = 11;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (d__ - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (vtld - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 10) {
	    need1 = (wtld - 1) * *ldw + 1;
	} else if (*ndx1 == 11) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 12) {
	    need1 = (ytld - 1) * *ldw + 1;
	} else if (*ndx1 == 13) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 14) {
	    need1 = (ztld - 1) * *ldw + 1;
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
	    need2 = (d__ - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (vtld - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 10) {
	    need2 = (wtld - 1) * *ldw + 1;
	} else if (*ndx2 == 11) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 12) {
	    need2 = (ytld - 1) * *ldw + 1;
	} else if (*ndx2 == 13) {
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 14) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerances. */

    rhotol = sgetbreak_();
    betatol = sgetbreak_();
    gammatol = sgetbreak_();
    deltatol = sgetbreak_();
    epstol = sgetbreak_();
    xitol = sgetbreak_();

/*     Set initial residual. */

    ccopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wscnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        Note: using D as temp */
/* ********CALL cCOPY( N, X, 1, WORK(1,D), 1 ) */
	sclr1->r = -1.f, sclr1->i = 0.f;
	sclr2->r = 0.f, sclr2->i = 0.f;
	*ndx1 = (d__ - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 7;
	return 0;
    }
/* **************** */
L2:
/* **************** */

    if (wscnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L30;
    }

    bnrm2 = wscnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

    ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

    r__1 = wscnrm2_(n, &work[y * work_dim1 + 1], &c__1);
    rho.r = r__1, rho.i = 0.f;

    ccopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 6;
    return 0;
/* **************** */
L4:
/* **************** */

    r__1 = wscnrm2_(n, &work[z__ * work_dim1 + 1], &c__1);
    xi.r = r__1, xi.i = 0.f;

    gamma.r = 1.f, gamma.i = 0.f;
    eta.r = -1.f, eta.i = 0.f;
    theta.r = 0.f, theta.i = 0.f;

    *iter = 0;

L40:

/*     Perform Preconditioned QMR iteration. */

    ++(*iter);

    if (c_abs(&rho) < rhotol || c_abs(&xi) < xitol) {
	goto L25;
    }

    ccopy_(n, &work[vtld * work_dim1 + 1], &c__1, &work[v * work_dim1 + 1], &
	    c__1);
    c_div(&q__1, &c_b180, &rho);
    tmpval.r = q__1.r, tmpval.i = q__1.i;
    cscal_(n, &tmpval, &work[v * work_dim1 + 1], &c__1);
    cscal_(n, &tmpval, &work[y * work_dim1 + 1], &c__1);

    c_div(&q__1, &c_b180, &xi);
    tmpval.r = q__1.r, tmpval.i = q__1.i;
    ccopy_(n, &work[wtld * work_dim1 + 1], &c__1, &work[w * work_dim1 + 1], &
	    c__1);
    cscal_(n, &tmpval, &work[w * work_dim1 + 1], &c__1);
    cscal_(n, &tmpval, &work[z__ * work_dim1 + 1], &c__1);

    wcdotc_(&q__1, n, &work[z__ * work_dim1 + 1], &c__1, &work[y * work_dim1 
	    + 1], &c__1);
    delta.r = q__1.r, delta.i = q__1.i;
    if (c_abs(&delta) < deltatol) {
	goto L25;
    }

/* ********CALL PSOLVEQ( WORK(1,YTLD), WORK(1,Y), 'RIGHT' ) */

    *ndx1 = (ytld - 1) * *ldw + 1;
    *ndx2 = (y - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 4;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,ZTLD), WORK(1,Z), 'LEFT' ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (z__ - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 5;
    return 0;
/* **************** */
L6:
/* **************** */


    if (*iter > 1) {
	q__3.r = xi.r * delta.r - xi.i * delta.i, q__3.i = xi.r * delta.i + 
		xi.i * delta.r;
	c_div(&q__2, &q__3, &eps);
	q__1.r = -q__2.r, q__1.i = -q__2.i;
	c1.r = q__1.r, c1.i = q__1.i;
	caxpy_(n, &c1, &work[p * work_dim1 + 1], &c__1, &work[ytld * 
		work_dim1 + 1], &c__1);
	ccopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	c_div(&q__4, &delta, &eps);
	r_cnjg(&q__3, &q__4);
	q__2.r = rho.r * q__3.r - rho.i * q__3.i, q__2.i = rho.r * q__3.i + 
		rho.i * q__3.r;
	q__1.r = -q__2.r, q__1.i = -q__2.i;
	caxpy_(n, &q__1, &work[q * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	ccopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    } else {
	ccopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	ccopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,PTLD) ) */

    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (ptld - 1) * *ldw + 1;
    rlbl = 7;
    *ijob = 1;
    return 0;
/* **************** */
L7:
/* **************** */


    wcdotc_(&q__1, n, &work[q * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
	    + 1], &c__1);
    eps.r = q__1.r, eps.i = q__1.i;
    if (c_abs(&eps) < epstol) {
	goto L25;
    }

    c_div(&q__1, &eps, &delta);
    beta.r = q__1.r, beta.i = q__1.i;
    if (c_abs(&beta) < betatol) {
	goto L25;
    }

    ccopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1],
	     &c__1);
    q__1.r = -beta.r, q__1.i = -beta.i;
    caxpy_(n, &q__1, &work[v * work_dim1 + 1], &c__1, &work[vtld * work_dim1 
	    + 1], &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 8;
    *ijob = 3;
    return 0;

/* **************** */
L8:
/* **************** */
    rho1.r = rho.r, rho1.i = rho.i;
    r__1 = wscnrm2_(n, &work[y * work_dim1 + 1], &c__1);
    rho.r = r__1, rho.i = 0.f;

    ccopy_(n, &work[w * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], &
	    c__1);
/* ********CALL MATVECTRANS( ONE, WORK(1,Q), -BETA, WORK(1,WTLD) ) */

    sclr1->r = 1.f, sclr1->i = 0.f;
    r_cnjg(&q__2, &beta);
    q__1.r = -q__2.r, q__1.i = -q__2.i;
    sclr2->r = q__1.r, sclr2->i = q__1.i;
    *ndx1 = (q - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 9;
    *ijob = 2;
    return 0;
/* **************** */
L9:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 10;
    *ijob = 6;
    return 0;
/* **************** */
L10:
/* **************** */


    r__1 = wscnrm2_(n, &work[z__ * work_dim1 + 1], &c__1);
    xi.r = r__1, xi.i = 0.f;

    gamma1.r = gamma.r, gamma1.i = gamma.i;
    theta1.r = theta.r, theta1.i = theta.i;

    r__1 = c_abs(&beta);
    q__2.r = r__1 * gamma1.r, q__2.i = r__1 * gamma1.i;
    c_div(&q__1, &rho, &q__2);
    theta.r = q__1.r, theta.i = q__1.i;
    pow_ci(&q__4, &theta, &c__2);
    q__3.r = q__4.r + 1.f, q__3.i = q__4.i;
    c_sqrt(&q__2, &q__3);
    c_div(&q__1, &c_b180, &q__2);
    gamma.r = q__1.r, gamma.i = q__1.i;
    if (c_abs(&gamma) < gammatol) {
	goto L25;
    }

    q__4.r = -eta.r, q__4.i = -eta.i;
    q__3.r = q__4.r * rho1.r - q__4.i * rho1.i, q__3.i = q__4.r * rho1.i + 
	    q__4.i * rho1.r;
    pow_ci(&q__5, &gamma, &c__2);
    q__2.r = q__3.r * q__5.r - q__3.i * q__5.i, q__2.i = q__3.r * q__5.i + 
	    q__3.i * q__5.r;
    pow_ci(&q__7, &gamma1, &c__2);
    q__6.r = beta.r * q__7.r - beta.i * q__7.i, q__6.i = beta.r * q__7.i + 
	    beta.i * q__7.r;
    c_div(&q__1, &q__2, &q__6);
    eta.r = q__1.r, eta.i = q__1.i;

    if (*iter > 1) {
	q__2.r = theta1.r * gamma.r - theta1.i * gamma.i, q__2.i = theta1.r * 
		gamma.i + theta1.i * gamma.r;
	pow_ci(&q__1, &q__2, &c__2);
	cscal_(n, &q__1, &work[d__ * work_dim1 + 1], &c__1);
	caxpy_(n, &eta, &work[p * work_dim1 + 1], &c__1, &work[d__ * 
		work_dim1 + 1], &c__1);
	q__2.r = theta1.r * gamma.r - theta1.i * gamma.i, q__2.i = theta1.r * 
		gamma.i + theta1.i * gamma.r;
	pow_ci(&q__1, &q__2, &c__2);
	cscal_(n, &q__1, &work[s * work_dim1 + 1], &c__1);
	caxpy_(n, &eta, &work[ptld * work_dim1 + 1], &c__1, &work[s * 
		work_dim1 + 1], &c__1);
    } else {
	ccopy_(n, &work[p * work_dim1 + 1], &c__1, &work[d__ * work_dim1 + 1],
		 &c__1);
	cscal_(n, &eta, &work[d__ * work_dim1 + 1], &c__1);
	ccopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1]
		, &c__1);
	cscal_(n, &eta, &work[s * work_dim1 + 1], &c__1);
    }

/*        Compute current solution vector x. */

    tmpval.r = 1.f, tmpval.i = 0.f;
    caxpy_(n, &tmpval, &work[d__ * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    toz.r = 1.f, toz.i = 0.f;
    q__1.r = -toz.r, q__1.i = -toz.i;
    caxpy_(n, &q__1, &work[s * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = wscNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID .LE. TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 11;
    *ijob = 8;
    return 0;

/* **************** */
L11:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    goto L40;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;

    return 0;

L25:

/*     Method breakdown. */

    if (c_abs(&rho) < rhotol) {
	*info = -10;
    } else if (c_abs(&beta) < betatol) {
	*info = -11;
    } else if (c_abs(&gamma) < gammatol) {
	*info = -12;
    } else if (c_abs(&delta) < deltatol) {
	*info = -13;
    } else if (c_abs(&eps) < epstol) {
	*info = -14;
    } else if (c_abs(&xi) < xitol) {
	*info = -15;
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

/*     End of QMRREVCOM */

} /* cqmrrevcom_ */

/*     END SUBROUTINE cQMRREVCOM */
/* Subroutine */ int zqmrrevcom_(integer *n, doublecomplex *b, doublecomplex *
	x, doublecomplex *work, integer *ldw, integer *iter, doublereal *
	resid, integer *info, integer *ndx1, integer *ndx2, doublecomplex *
	sclr1, doublecomplex *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset;
    doublereal d__1;
    doublecomplex z__1, z__2, z__3, z__4, z__5, z__6, z__7;

    /* Builtin functions */
    double z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *), d_cnjg(
	    doublecomplex *, doublecomplex *), pow_zi(doublecomplex *, 
	    doublecomplex *, integer *), z_sqrt(doublecomplex *, 
	    doublecomplex *);

    /* Local variables */
    static doublereal gammatol, deltatol;
    static integer d__, p, q, r__, s, v, w, y, z__;
    extern doublereal dgetbreak_(void);
    static doublecomplex c1, xi, eta, eps, rho;
    static doublereal tol;
    static doublecomplex toz, rho1, beta;
    static integer rlbl, ptld, vtld, wtld, ytld, ztld, need1, need2;
    static doublereal bnrm2;
    static doublecomplex gamma, delta, theta;
    extern /* Subroutine */ int zscal_(integer *, doublecomplex *, 
	    doublecomplex *, integer *);
    static integer maxit;
    static doublereal xitol;
    static doublecomplex gamma1;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), zaxpy_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    static doublecomplex theta1;
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);
    static doublecomplex tmpval;
    static doublereal epstol, rhotol;
    extern /* Double Complex */ VOID wzdotc_(doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);
    static doublereal betatol;



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

/*  QMR Method solves the linear system Ax = b using the */
/*  Quasi-Minimal Residual iterative method with preconditioning. */

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


/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,11). */
/*          Workspace for residual, direction vector, etc. */
/*          Note that W and WTLD, Y and YTLD, and Z and ZTLD share */
/*          workspace. */

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
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

/*          .gt.   0: Convergence to tolerance not achieved. This will be */
/*                set to the number of iterations performed. */

/*          .ls.   0: Illegal input parameter, or breakdown occurred */
/*                during iteration. */

/*                Illegal parameter: */

/*                   -1: matrix dimension N .ls.  0 */
/*                   -2: LDW .ls.  N */
/*                   -3: Maximum number of iterations ITER .ls. = 0. */

/*                BREAKDOWN: If parameters RHO or OMEGA become smaller */
/*                   than some tolerance, the program will terminate. */
/*                   Here we check against tolerance BREAKTOL. */

/*                  -10: RHO   .ls.  BREAKTOL: RHO and RTLD have become */
/*                                         orthogonal. */
/*                  -11: BETA  .ls.  BREAKTOL: EPS too small in relation to DELT */
/*                                         Convergence has stalled. */
/*                  -12: GAMMA .ls.  BREAKTOL: THETA too large. */
/*                                         Convergence has stalled. */
/*                  -13: DELTA .ls.  BREAKTOL: Y and Z have become */
/*                                         orthogonal. */
/*                  -14: EPS   .ls.  BREAKTOL: Q and PTLD have become */
/*                                         orthogonal. */
/*                  -15: XI    .ls.  BREAKTOL: Z too small. */
/*                                   Convergence has stalled. */

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

/*  BLAS CALLS:   DAXPY, DCOPY, DDOT, DNRM2, DSCAL */
/*  ============================================================== */

/*     .. Parameters .. */

/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
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
	if (rlbl == 8) {
	    goto L8;
	}
	if (rlbl == 9) {
	    goto L9;
	}
	if (rlbl == 10) {
	    goto L10;
	}
	if (rlbl == 11) {
	    goto L11;
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
    d__ = 2;
    p = 3;
    ptld = 4;
    q = 5;
    s = 6;
    v = 7;
    vtld = 8;
    w = 9;
    wtld = 9;
    y = 10;
    ytld = 10;
    z__ = 11;
    ztld = 11;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (d__ - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (p - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (ptld - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (q - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 7) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 == 8) {
	    need1 = (vtld - 1) * *ldw + 1;
	} else if (*ndx1 == 9) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 10) {
	    need1 = (wtld - 1) * *ldw + 1;
	} else if (*ndx1 == 11) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 12) {
	    need1 = (ytld - 1) * *ldw + 1;
	} else if (*ndx1 == 13) {
	    need1 = (z__ - 1) * *ldw + 1;
	} else if (*ndx1 == 14) {
	    need1 = (ztld - 1) * *ldw + 1;
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
	    need2 = (d__ - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (p - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (ptld - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (q - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 7) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 == 8) {
	    need2 = (vtld - 1) * *ldw + 1;
	} else if (*ndx2 == 9) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 10) {
	    need2 = (wtld - 1) * *ldw + 1;
	} else if (*ndx2 == 11) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 12) {
	    need2 = (ytld - 1) * *ldw + 1;
	} else if (*ndx2 == 13) {
	    need2 = (z__ - 1) * *ldw + 1;
	} else if (*ndx2 == 14) {
	    need2 = (ztld - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L20;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set breakdown tolerances. */

    rhotol = dgetbreak_();
    betatol = dgetbreak_();
    gammatol = dgetbreak_();
    deltatol = dgetbreak_();
    epstol = dgetbreak_();
    xitol = dgetbreak_();

/*     Set initial residual. */

    zcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dznrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ZERO, WORK(1,R) ) */
/*        Note: using D as temp */
/* ********CALL zCOPY( N, X, 1, WORK(1,D), 1 ) */
	sclr1->r = -1., sclr1->i = 0.;
	sclr2->r = 0., sclr2->i = 0.;
	*ndx1 = (d__ - 1) * *ldw + 1;
	*ndx2 = (r__ - 1) * *ldw + 1;
	rlbl = 2;
	*ijob = 7;
	return 0;
    }
/* **************** */
L2:
/* **************** */

    if (dznrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L30;
    }

    bnrm2 = dznrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

    zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 3;
    *ijob = 3;
    return 0;
/* **************** */
L3:
/* **************** */

    d__1 = dznrm2_(n, &work[y * work_dim1 + 1], &c__1);
    rho.r = d__1, rho.i = 0.;

    zcopy_(n, &work[r__ * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], 
	    &c__1);
/* *****CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 4;
    *ijob = 6;
    return 0;
/* **************** */
L4:
/* **************** */

    d__1 = dznrm2_(n, &work[z__ * work_dim1 + 1], &c__1);
    xi.r = d__1, xi.i = 0.;

    gamma.r = 1., gamma.i = 0.;
    eta.r = -1., eta.i = 0.;
    theta.r = 0., theta.i = 0.;

    *iter = 0;

L40:

/*     Perform Preconditioned QMR iteration. */

    ++(*iter);

    if (z_abs(&rho) < rhotol || z_abs(&xi) < xitol) {
	goto L25;
    }

    zcopy_(n, &work[vtld * work_dim1 + 1], &c__1, &work[v * work_dim1 + 1], &
	    c__1);
    z_div(&z__1, &c_b263, &rho);
    tmpval.r = z__1.r, tmpval.i = z__1.i;
    zscal_(n, &tmpval, &work[v * work_dim1 + 1], &c__1);
    zscal_(n, &tmpval, &work[y * work_dim1 + 1], &c__1);

    z_div(&z__1, &c_b263, &xi);
    tmpval.r = z__1.r, tmpval.i = z__1.i;
    zcopy_(n, &work[wtld * work_dim1 + 1], &c__1, &work[w * work_dim1 + 1], &
	    c__1);
    zscal_(n, &tmpval, &work[w * work_dim1 + 1], &c__1);
    zscal_(n, &tmpval, &work[z__ * work_dim1 + 1], &c__1);

    wzdotc_(&z__1, n, &work[z__ * work_dim1 + 1], &c__1, &work[y * work_dim1 
	    + 1], &c__1);
    delta.r = z__1.r, delta.i = z__1.i;
    if (z_abs(&delta) < deltatol) {
	goto L25;
    }

/* ********CALL PSOLVEQ( WORK(1,YTLD), WORK(1,Y), 'RIGHT' ) */

    *ndx1 = (ytld - 1) * *ldw + 1;
    *ndx2 = (y - 1) * *ldw + 1;
    rlbl = 5;
    *ijob = 4;
    return 0;
/* **************** */
L5:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,ZTLD), WORK(1,Z), 'LEFT' ) */

    *ndx1 = (ztld - 1) * *ldw + 1;
    *ndx2 = (z__ - 1) * *ldw + 1;
    rlbl = 6;
    *ijob = 5;
    return 0;
/* **************** */
L6:
/* **************** */


    if (*iter > 1) {
	z__3.r = xi.r * delta.r - xi.i * delta.i, z__3.i = xi.r * delta.i + 
		xi.i * delta.r;
	z_div(&z__2, &z__3, &eps);
	z__1.r = -z__2.r, z__1.i = -z__2.i;
	c1.r = z__1.r, c1.i = z__1.i;
	zaxpy_(n, &c1, &work[p * work_dim1 + 1], &c__1, &work[ytld * 
		work_dim1 + 1], &c__1);
	zcopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	z_div(&z__4, &delta, &eps);
	d_cnjg(&z__3, &z__4);
	z__2.r = rho.r * z__3.r - rho.i * z__3.i, z__2.i = rho.r * z__3.i + 
		rho.i * z__3.r;
	z__1.r = -z__2.r, z__1.i = -z__2.i;
	zaxpy_(n, &z__1, &work[q * work_dim1 + 1], &c__1, &work[ztld * 
		work_dim1 + 1], &c__1);
	zcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    } else {
	zcopy_(n, &work[ytld * work_dim1 + 1], &c__1, &work[p * work_dim1 + 1]
		, &c__1);
	zcopy_(n, &work[ztld * work_dim1 + 1], &c__1, &work[q * work_dim1 + 1]
		, &c__1);
    }

/* ********CALL MATVEC( ONE, WORK(1,P), ZERO, WORK(1,PTLD) ) */

    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    *ndx1 = (p - 1) * *ldw + 1;
    *ndx2 = (ptld - 1) * *ldw + 1;
    rlbl = 7;
    *ijob = 1;
    return 0;
/* **************** */
L7:
/* **************** */


    wzdotc_(&z__1, n, &work[q * work_dim1 + 1], &c__1, &work[ptld * work_dim1 
	    + 1], &c__1);
    eps.r = z__1.r, eps.i = z__1.i;
    if (z_abs(&eps) < epstol) {
	goto L25;
    }

    z_div(&z__1, &eps, &delta);
    beta.r = z__1.r, beta.i = z__1.i;
    if (z_abs(&beta) < betatol) {
	goto L25;
    }

    zcopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[vtld * work_dim1 + 1],
	     &c__1);
    z__1.r = -beta.r, z__1.i = -beta.i;
    zaxpy_(n, &z__1, &work[v * work_dim1 + 1], &c__1, &work[vtld * work_dim1 
	    + 1], &c__1);
/* *****CALL PSOLVEQ( WORK(1,Y), WORK(1,VTLD), 'LEFT' ) */

    *ndx1 = (y - 1) * *ldw + 1;
    *ndx2 = (vtld - 1) * *ldw + 1;
    rlbl = 8;
    *ijob = 3;
    return 0;

/* **************** */
L8:
/* **************** */
    rho1.r = rho.r, rho1.i = rho.i;
    d__1 = dznrm2_(n, &work[y * work_dim1 + 1], &c__1);
    rho.r = d__1, rho.i = 0.;

    zcopy_(n, &work[w * work_dim1 + 1], &c__1, &work[wtld * work_dim1 + 1], &
	    c__1);
/* ********CALL MATVECTRANS( ONE, WORK(1,Q), -BETA, WORK(1,WTLD) ) */

    sclr1->r = 1., sclr1->i = 0.;
    d_cnjg(&z__2, &beta);
    z__1.r = -z__2.r, z__1.i = -z__2.i;
    sclr2->r = z__1.r, sclr2->i = z__1.i;
    *ndx1 = (q - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 9;
    *ijob = 2;
    return 0;
/* **************** */
L9:
/* **************** */

/* ********CALL PSOLVETRANSQ( WORK(1,Z), WORK(1,WTLD), 'RIGHT' ) */

    *ndx1 = (z__ - 1) * *ldw + 1;
    *ndx2 = (wtld - 1) * *ldw + 1;
    rlbl = 10;
    *ijob = 6;
    return 0;
/* **************** */
L10:
/* **************** */


    d__1 = dznrm2_(n, &work[z__ * work_dim1 + 1], &c__1);
    xi.r = d__1, xi.i = 0.;

    gamma1.r = gamma.r, gamma1.i = gamma.i;
    theta1.r = theta.r, theta1.i = theta.i;

    d__1 = z_abs(&beta);
    z__2.r = d__1 * gamma1.r, z__2.i = d__1 * gamma1.i;
    z_div(&z__1, &rho, &z__2);
    theta.r = z__1.r, theta.i = z__1.i;
    pow_zi(&z__4, &theta, &c__2);
    z__3.r = z__4.r + 1., z__3.i = z__4.i;
    z_sqrt(&z__2, &z__3);
    z_div(&z__1, &c_b263, &z__2);
    gamma.r = z__1.r, gamma.i = z__1.i;
    if (z_abs(&gamma) < gammatol) {
	goto L25;
    }

    z__4.r = -eta.r, z__4.i = -eta.i;
    z__3.r = z__4.r * rho1.r - z__4.i * rho1.i, z__3.i = z__4.r * rho1.i + 
	    z__4.i * rho1.r;
    pow_zi(&z__5, &gamma, &c__2);
    z__2.r = z__3.r * z__5.r - z__3.i * z__5.i, z__2.i = z__3.r * z__5.i + 
	    z__3.i * z__5.r;
    pow_zi(&z__7, &gamma1, &c__2);
    z__6.r = beta.r * z__7.r - beta.i * z__7.i, z__6.i = beta.r * z__7.i + 
	    beta.i * z__7.r;
    z_div(&z__1, &z__2, &z__6);
    eta.r = z__1.r, eta.i = z__1.i;

    if (*iter > 1) {
	z__2.r = theta1.r * gamma.r - theta1.i * gamma.i, z__2.i = theta1.r * 
		gamma.i + theta1.i * gamma.r;
	pow_zi(&z__1, &z__2, &c__2);
	zscal_(n, &z__1, &work[d__ * work_dim1 + 1], &c__1);
	zaxpy_(n, &eta, &work[p * work_dim1 + 1], &c__1, &work[d__ * 
		work_dim1 + 1], &c__1);
	z__2.r = theta1.r * gamma.r - theta1.i * gamma.i, z__2.i = theta1.r * 
		gamma.i + theta1.i * gamma.r;
	pow_zi(&z__1, &z__2, &c__2);
	zscal_(n, &z__1, &work[s * work_dim1 + 1], &c__1);
	zaxpy_(n, &eta, &work[ptld * work_dim1 + 1], &c__1, &work[s * 
		work_dim1 + 1], &c__1);
    } else {
	zcopy_(n, &work[p * work_dim1 + 1], &c__1, &work[d__ * work_dim1 + 1],
		 &c__1);
	zscal_(n, &eta, &work[d__ * work_dim1 + 1], &c__1);
	zcopy_(n, &work[ptld * work_dim1 + 1], &c__1, &work[s * work_dim1 + 1]
		, &c__1);
	zscal_(n, &eta, &work[s * work_dim1 + 1], &c__1);
    }

/*        Compute current solution vector x. */

    tmpval.r = 1., tmpval.i = 0.;
    zaxpy_(n, &tmpval, &work[d__ * work_dim1 + 1], &c__1, &x[1], &c__1);

/*        Compute residual vector rk, find norm, */
/*        then check for tolerance. */

    toz.r = 1., toz.i = 0.;
    z__1.r = -toz.r, z__1.i = -toz.i;
    zaxpy_(n, &z__1, &work[s * work_dim1 + 1], &c__1, &work[r__ * work_dim1 + 
	    1], &c__1);

/* ********RESID = dzNRM2( N, WORK(1,R), 1 ) / BNRM2 */
/* ********IF ( RESID .LE. TOL  ) GO TO 30 */

    *ndx1 = need1;
    *ndx2 = need2;
/*        Prepare for resumption & return */
    rlbl = 11;
    *ijob = 8;
    return 0;

/* **************** */
L11:
/* **************** */
    if (*info == 1) {
	goto L30;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L20;
    }

    goto L40;

L20:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;

    return 0;

L25:

/*     Method breakdown. */

    if (z_abs(&rho) < rhotol) {
	*info = -10;
    } else if (z_abs(&beta) < betatol) {
	*info = -11;
    } else if (z_abs(&gamma) < gammatol) {
	*info = -12;
    } else if (z_abs(&delta) < deltatol) {
	*info = -13;
    } else if (z_abs(&eps) < epstol) {
	*info = -14;
    } else if (z_abs(&xi) < xitol) {
	*info = -15;
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

/*     End of QMRREVCOM */

} /* zqmrrevcom_ */

