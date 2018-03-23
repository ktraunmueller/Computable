/* GMRESREVCOM.f -- translated by f2c (version 20100827).
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
static complex c_b136 = {1.f,0.f};
static doublecomplex c_b195 = {1.,0.};

/*  -*- fortran -*- */
/* Subroutine */ int sgmresrevcom_(integer *n, real *b, real *x, integer *
	restrt, real *work, integer *ldw, real *work2, integer *ldw2, integer 
	*iter, real *resid, integer *info, integer *ndx1, integer *ndx2, real 
	*sclr1, real *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset, work2_dim1, work2_offset;

    /* Local variables */
    extern /* Subroutine */ int selemvec_(integer *, integer *, real *, real *
	    );
    static integer h__, i__, r__, s, v, w, y, av, giv;
    static real tol, toz;
    extern doublereal wsapproxres_(integer *, real *, real *, real *, integer 
	    *);
    static integer rlbl, need1, need2;
    static real bnrm2;
    extern /* Subroutine */ int sapplygivens_(integer *, real *, real *, 
	    integer *), sscal_(integer *, real *, real *, integer *);
    static integer maxit;
    static real rnorm;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *);
    extern doublereal wsnrm2_(integer *, real *, integer *);
    static real tmpval;
    extern /* Subroutine */ int supdate_(integer *, integer *, real *, real *,
	     integer *, real *, real *, real *, integer *), sorthoh_(integer *
	    , integer *, real *, real *, integer *, real *);


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     EiITERkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  GMRES solves the linear system Ax = b using the */
/*  Generalized Minimal Residual iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  RESTRT  (input) INTEGER */
/*          Restart parameter, .ls. = N. This parameter controls the amount */
/*          of memory required for matrix WORK2. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6+restrt). */
/*          Note that if the initial guess is the zero vector, then */
/*          storing the initial residual is not necessary. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  WORK2   (workspace) DOUBLE PRECISION array, dimension (LDW2,2*RESTRT+2). */
/*          This workspace is used for constructing and storing the */
/*          upper Hessenberg matrix. The two extra columns are used to */
/*          store the Givens rotation matrices. */

/*  LDW2    (input) INTEGER */
/*          The leading dimension of the array WORK2. */
/*          LDW2 .gt. = max(2,RESTRT+1). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable error tolerance. */
/*          On ouput, the norm of the residual vector if solution */
/*          approximated to tolerance, otherwise reset to input */
/*          tolerance. */

/*  INFO    (output) INTEGER */
/*          =  0:  successful exit */
/*          =  1:  maximum number of iterations performed; */
/*                 convergence not achieved. */
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

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

/*  ============================================================ */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
/*     .. */
/*     .. Executable Statements .. */

/* Entry point, so test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;
    work2_dim1 = *ldw2;
    work2_offset = 1 + work2_dim1;
    work2 -= work2_offset;

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
	goto L200;
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
    s = 2;
    w = 3;
    y = 4;
    av = 5;
    v = 6;

    h__ = 1;
    giv = h__ + *restrt;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (av - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 > v * 1000 && *ndx1 <= v * 1000 + *restrt) {
	    need1 = (*ndx1 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx1 > giv * 1000 && *ndx1 <= giv * 1000 + *restrt) {
	    need1 = (*ndx1 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (av - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 > v * 1000 && *ndx2 <= v * 1000 + *restrt) {
	    need2 = (*ndx2 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx2 > giv * 1000 && *ndx2 <= giv * 1000 + *restrt) {
	    need2 = (*ndx2 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set initial residual. */

    scopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wsnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using X directly */
	*sclr1 = -1.f;
	*sclr2 = 1.f;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 1;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (wsnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L200;
    }
    bnrm2 = wsnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

    *iter = 0;
L10:

    ++(*iter);

/*        Construct the first column of V, and initialize S to the */
/*        elementary vector E1 scaled by RNORM. */

/* ********CALL PSOLVE( WORK( 1,V ), WORK( 1,R ) ) */

    *ndx1 = (v - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

    rnorm = wsnrm2_(n, &work[v * work_dim1 + 1], &c__1);
    toz = 1.f / rnorm;
    sscal_(n, &toz, &work[v * work_dim1 + 1], &c__1);
    tmpval = rnorm;
    selemvec_(&c__1, n, &tmpval, &work[s * work_dim1 + 1]);

/*         DO 50 I = 1, RESTRT */
    i__ = 1;
L49:
    if (i__ > *restrt) {
	goto L50;
    }
/* ***********CALL MATVEC( ONE, WORK( 1,V+I-1 ), ZERO, WORK( 1,AV ) ) */

    *ndx1 = (v + i__ - 2) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.f;
    *sclr2 = 0.f;
    rlbl = 4;
    *ijob = 3;
    return 0;

/* **************** */
L4:
/* **************** */

/* ********CALL PSOLVE( WORK( 1,W ), WORK( 1,AV ) ) */

    *ndx1 = (w - 1) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*           Construct I-th column of H so that it is orthnormal to */
/*           the previous I-1 columns. */

    sorthoh_(&i__, n, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work[v * 
	    work_dim1 + 1], ldw, &work[w * work_dim1 + 1]);

    if (i__ > 0) {
	sapplygivens_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work2[
		giv * work2_dim1 + 1], ldw2);
    }

/*              Apply Givens rotations to the I-th column of H. This */
/*              effectively reduces the Hessenberg matrix to upper */
/*              triangular form during the RESTRT iterations. */


/*           Approxiyate residual norm. Check tolerance. If okay, compute */
/*           final approximation vector X and quit. */

    *resid = wsapproxres_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &
	    work[s * work_dim1 + 1], &work2[giv * work2_dim1 + 1], ldw2) / 
	    bnrm2;
    if (*resid <= tol) {
	supdate_(&i__, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y *
		 work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * 
		work_dim1 + 1], ldw);
	goto L200;
    }
    ++i__;
    goto L49;
L50:
    i__ = *restrt;

/*        Compute current solution vector X. */

    supdate_(restrt, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y * 
	    work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * work_dim1 + 1]
	    , ldw);

/*        Compute residual vector R, find norm, */
/*        then check for tolerance. */

    scopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
/* ********CALL MATVEC( -ONE, X, ONE, WORK( 1,R ) ) */

    *ndx1 = -1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = -1.f;
    *sclr2 = 1.f;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    work[i__ + 1 + s * work_dim1] = wsnrm2_(n, &work[r__ * work_dim1 + 1], &
	    c__1);

/* ********RESID = WORK( I+1,S ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 200 */

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
	goto L200;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L100;
    }

    goto L10;

L100:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L200:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of GMRESREVCOM */

} /* sgmresrevcom_ */

/*     END SUBROUTINE sGMRESREVCOM */

/*     ========================================================= */
/* Subroutine */ int sorthoh_(integer *i__, integer *n, real *h__, real *v, 
	integer *ldv, real *w)
{
    /* System generated locals */
    integer v_dim1, v_offset, i__1;
    real r__1;

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *), 
	    scopy_(integer *, real *, integer *, real *, integer *);
    extern doublereal wsdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);
    extern doublereal wsnrm2_(integer *, real *, integer *);
    static real tmpval;



/*     Construct the I-th column of the upper Hessenberg matrix H */
/*     using the Gram-Schmidt process on V and W. */


    /* Parameter adjustments */
    --h__;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    --w;

    /* Function Body */
    i__1 = *i__;
    for (k = 1; k <= i__1; ++k) {
	h__[k] = wsdot_(n, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
	r__1 = -h__[k];
	saxpy_(n, &r__1, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
/* L10: */
    }
    h__[*i__ + 1] = wsnrm2_(n, &w[1], &c__1);
    scopy_(n, &w[1], &c__1, &v[(*i__ + 1) * v_dim1 + 1], &c__1);
    tmpval = 1.f / h__[*i__ + 1];
    sscal_(n, &tmpval, &v[(*i__ + 1) * v_dim1 + 1], &c__1);

    return 0;

} /* sorthoh_ */

/*     END SUBROUTINE sORTHOH */
/*     ========================================================= */
/* Subroutine */ int sapplygivens_(integer *i__, real *h__, real *givens, 
	integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int sgetgiv_(real *, real *, real *, real *), 
	    srotvec_(real *, real *, real *, real *);



/*     This routine applies a sequence of I-1 Givens rotations to */
/*     the I-th column of H. The Givens parameters are stored, so that */
/*     the first I-2 Givens rotation matrices are known. The I-1st */
/*     Givens rotation is computed using BLAS 1 routine DROTG. Each */
/*     rotation is applied to the 2x1 vector [H( J ), H( J+1 )]', */
/*     which results in H( J+1 ) = 0. */

/*      DOUBLE PRECISION   TEMP */

/*     .. Executable Statements .. */

/*     Construct I-1st rotation matrix. */

/*     CALL sROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL sGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */

/*     Apply 1,...,I-1st rotation matrices to the I-th column of H. */

    /* Parameter adjustments */
    --h__;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    i__1 = *i__ - 1;
    for (j = 1; j <= i__1; ++j) {
	srotvec_(&h__[j], &h__[j + 1], &givens[j + givens_dim1], &givens[j + (
		givens_dim1 << 1)]);
/*        TEMP     =  GIVENS( J,1 ) * H( J ) + GIVENS( J,2 ) * H( J+1 ) */
/*        H( J+1 ) = -GIVENS( J,2 ) * H( J ) + GIVENS( J,1 ) * H( J+1 ) */
/*        H( J ) = TEMP */
/* L10: */
    }
    sgetgiv_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);
    srotvec_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);

    return 0;

} /* sapplygivens_ */

/*     END SUBROUTINE sAPPLYGIVENS */

/*     =============================================================== */
doublereal wsapproxres_(integer *i__, real *h__, real *s, real *givens, 
	integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset;
    real ret_val, r__1;

    /* Local variables */
    extern /* Subroutine */ int srotvec_(real *, real *, real *, real *);



/*     This func allows the user to approximate the residual */
/*     using an updating scheme involving Givens rotations. The */
/*     rotation matrix is formed using [H( I ),H( I+1 )]' with the */
/*     intent of zeroing H( I+1 ), but here is applied to the 2x1 */
/*     vector [S(I), S(I+1)]'. */


/*     .. Executable Statements .. */

/*     CALL sROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL sGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
    /* Parameter adjustments */
    --h__;
    --s;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    srotvec_(&s[*i__], &s[*i__ + 1], &givens[*i__ + givens_dim1], &givens[*
	    i__ + (givens_dim1 << 1)]);

    ret_val = (r__1 = s[*i__ + 1], dabs(r__1));

    return ret_val;

} /* wsapproxres_ */

/*     END FUNCTION wsAPPROXRES */
/*     =============================================================== */
/* Subroutine */ int supdate_(integer *i__, integer *n, real *x, real *h__, 
	integer *ldh, real *y, real *s, real *v, integer *ldv)
{
    /* System generated locals */
    integer h_dim1, h_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int scopy_(integer *, real *, integer *, real *, 
	    integer *), saxpy_(integer *, real *, real *, integer *, real *, 
	    integer *), strsv_(char *, char *, char *, integer *, real *, 
	    integer *, real *, integer *, ftnlen, ftnlen, ftnlen);



/*     Solve H*y = s for upper triangualar H. */

    /* Parameter adjustments */
    --x;
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    --y;
    --s;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;

    /* Function Body */
    scopy_(i__, &s[1], &c__1, &y[1], &c__1);
    strsv_("UPPER", "NOTRANS", "NONUNIT", i__, &h__[h_offset], ldh, &y[1], &
	    c__1, (ftnlen)5, (ftnlen)7, (ftnlen)7);

/*     Compute current solution vector X. */

    i__1 = *i__;
    for (j = 1; j <= i__1; ++j) {
	saxpy_(n, &y[j], &v[j * v_dim1 + 1], &c__1, &x[1], &c__1);
/* L10: */
    }

    return 0;

} /* supdate_ */

/*     END SUBROUTINE sUPDATE */

/*     =============================================================== */
/* Subroutine */ int sgetgiv_(real *a, real *b, real *c__, real *s)
{
    /* System generated locals */
    real r__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static real temp;



    if (dabs(*b) == 0.f) {
	*c__ = 1.f;
	*s = 0.f;
    } else if (dabs(*b) > dabs(*a)) {
	temp = -(*a) / *b;
/* Computing 2nd power */
	r__1 = dabs(temp);
	*s = 1.f / sqrt(r__1 * r__1 + 1.f);
	*c__ = temp * *s;
/*         S = b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = -a / SQRT( abs(a)**2 + abs(b)**2 ) */
    } else {
	temp = -(*b) / *a;
/* Computing 2nd power */
	r__1 = dabs(temp);
	*c__ = 1.f / sqrt(r__1 * r__1 + 1.f);
	*s = temp * *c__;
/*         S = -b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = a / SQRT( abs(a)**2 + abs(b)**2 ) */
    }

    return 0;

} /* sgetgiv_ */

/*     END SUBROUTINE sGETGIV */

/*     ================================================================ */
/* Subroutine */ int srotvec_(real *x, real *y, real *c__, real *s)
{
    static real temp;



    temp = *c__ * *x - *s * *y;
    *y = *s * *x + *c__ * *y;
    *x = temp;

    return 0;

} /* srotvec_ */

/*     END SUBROUTINE sROTVEC */

/*     =============================================================== */
/* Subroutine */ int selemvec_(integer *i__, integer *n, real *alpha, real *e)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer j;


/*     Construct the I-th elementary vector E, scaled by ALPHA. */


/*     .. Parameters .. */

    /* Parameter adjustments */
    --e;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	e[j] = 0.f;
/* L10: */
    }
    e[*i__] = *alpha;

    return 0;

} /* selemvec_ */

/*     END SUBROUTINE sELEMVEC */
/* Subroutine */ int dgmresrevcom_(integer *n, doublereal *b, doublereal *x, 
	integer *restrt, doublereal *work, integer *ldw, doublereal *work2, 
	integer *ldw2, integer *iter, doublereal *resid, integer *info, 
	integer *ndx1, integer *ndx2, doublereal *sclr1, doublereal *sclr2, 
	integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset, work2_dim1, work2_offset;

    /* Local variables */
    extern /* Subroutine */ int delemvec_(integer *, integer *, doublereal *, 
	    doublereal *);
    static integer h__, i__, r__, s, v, w, y, av;
    extern doublereal dapproxres_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    static integer giv;
    static doublereal tol, toz;
    static integer rlbl, need1, need2;
    static doublereal bnrm2;
    extern doublereal dnrm2_(integer *, doublereal *, integer *);
    extern /* Subroutine */ int dapplygivens_(integer *, doublereal *, 
	    doublereal *, integer *), dscal_(integer *, doublereal *, 
	    doublereal *, integer *), dcopy_(integer *, doublereal *, integer 
	    *, doublereal *, integer *);
    static integer maxit;
    static doublereal rnorm, tmpval;
    extern /* Subroutine */ int dupdate_(integer *, integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     integer *), dorthoh_(integer *, integer *, doublereal *, 
	    doublereal *, integer *, doublereal *);


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     EiITERkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  GMRES solves the linear system Ax = b using the */
/*  Generalized Minimal Residual iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  RESTRT  (input) INTEGER */
/*          Restart parameter, .ls. = N. This parameter controls the amount */
/*          of memory required for matrix WORK2. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6+restrt). */
/*          Note that if the initial guess is the zero vector, then */
/*          storing the initial residual is not necessary. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  WORK2   (workspace) DOUBLE PRECISION array, dimension (LDW2,2*RESTRT+2). */
/*          This workspace is used for constructing and storing the */
/*          upper Hessenberg matrix. The two extra columns are used to */
/*          store the Givens rotation matrices. */

/*  LDW2    (input) INTEGER */
/*          The leading dimension of the array WORK2. */
/*          LDW2 .gt. = max(2,RESTRT+1). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable error tolerance. */
/*          On ouput, the norm of the residual vector if solution */
/*          approximated to tolerance, otherwise reset to input */
/*          tolerance. */

/*  INFO    (output) INTEGER */
/*          =  0:  successful exit */
/*          =  1:  maximum number of iterations performed; */
/*                 convergence not achieved. */
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

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

/*  ============================================================ */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
/*     .. */
/*     .. Executable Statements .. */

/* Entry point, so test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;
    work2_dim1 = *ldw2;
    work2_offset = 1 + work2_dim1;
    work2 -= work2_offset;

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
	goto L200;
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
    s = 2;
    w = 3;
    y = 4;
    av = 5;
    v = 6;

    h__ = 1;
    giv = h__ + *restrt;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (av - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 > v * 1000 && *ndx1 <= v * 1000 + *restrt) {
	    need1 = (*ndx1 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx1 > giv * 1000 && *ndx1 <= giv * 1000 + *restrt) {
	    need1 = (*ndx1 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (av - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 > v * 1000 && *ndx2 <= v * 1000 + *restrt) {
	    need2 = (*ndx2 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx2 > giv * 1000 && *ndx2 <= giv * 1000 + *restrt) {
	    need2 = (*ndx2 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set initial residual. */

    dcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dnrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using X directly */
	*sclr1 = -1.;
	*sclr2 = 1.;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 1;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (dnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L200;
    }
    bnrm2 = dnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

    *iter = 0;
L10:

    ++(*iter);

/*        Construct the first column of V, and initialize S to the */
/*        elementary vector E1 scaled by RNORM. */

/* ********CALL PSOLVE( WORK( 1,V ), WORK( 1,R ) ) */

    *ndx1 = (v - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

    rnorm = dnrm2_(n, &work[v * work_dim1 + 1], &c__1);
    toz = 1. / rnorm;
    dscal_(n, &toz, &work[v * work_dim1 + 1], &c__1);
    tmpval = rnorm;
    delemvec_(&c__1, n, &tmpval, &work[s * work_dim1 + 1]);

/*         DO 50 I = 1, RESTRT */
    i__ = 1;
L49:
    if (i__ > *restrt) {
	goto L50;
    }
/* ***********CALL MATVEC( ONE, WORK( 1,V+I-1 ), ZERO, WORK( 1,AV ) ) */

    *ndx1 = (v + i__ - 2) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = 1.;
    *sclr2 = 0.;
    rlbl = 4;
    *ijob = 3;
    return 0;

/* **************** */
L4:
/* **************** */

/* ********CALL PSOLVE( WORK( 1,W ), WORK( 1,AV ) ) */

    *ndx1 = (w - 1) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*           Construct I-th column of H so that it is orthnormal to */
/*           the previous I-1 columns. */

    dorthoh_(&i__, n, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work[v * 
	    work_dim1 + 1], ldw, &work[w * work_dim1 + 1]);

    if (i__ > 0) {
	dapplygivens_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work2[
		giv * work2_dim1 + 1], ldw2);
    }

/*              Apply Givens rotations to the I-th column of H. This */
/*              effectively reduces the Hessenberg matrix to upper */
/*              triangular form during the RESTRT iterations. */


/*           Approxiyate residual norm. Check tolerance. If okay, compute */
/*           final approximation vector X and quit. */

    *resid = dapproxres_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &
	    work[s * work_dim1 + 1], &work2[giv * work2_dim1 + 1], ldw2) / 
	    bnrm2;
    if (*resid <= tol) {
	dupdate_(&i__, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y *
		 work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * 
		work_dim1 + 1], ldw);
	goto L200;
    }
    ++i__;
    goto L49;
L50:
    i__ = *restrt;

/*        Compute current solution vector X. */

    dupdate_(restrt, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y * 
	    work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * work_dim1 + 1]
	    , ldw);

/*        Compute residual vector R, find norm, */
/*        then check for tolerance. */

    dcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
/* ********CALL MATVEC( -ONE, X, ONE, WORK( 1,R ) ) */

    *ndx1 = -1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    *sclr1 = -1.;
    *sclr2 = 1.;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    work[i__ + 1 + s * work_dim1] = dnrm2_(n, &work[r__ * work_dim1 + 1], &
	    c__1);

/* ********RESID = WORK( I+1,S ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 200 */

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
	goto L200;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L100;
    }

    goto L10;

L100:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L200:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of GMRESREVCOM */

} /* dgmresrevcom_ */

/*     END SUBROUTINE dGMRESREVCOM */

/*     ========================================================= */
/* Subroutine */ int dorthoh_(integer *i__, integer *n, doublereal *h__, 
	doublereal *v, integer *ldv, doublereal *w)
{
    /* System generated locals */
    integer v_dim1, v_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static integer k;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *), dnrm2_(integer *, doublereal *, integer *);
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *), dcopy_(integer *, doublereal *, integer *, doublereal 
	    *, integer *), daxpy_(integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *);
    static doublereal tmpval;



/*     Construct the I-th column of the upper Hessenberg matrix H */
/*     using the Gram-Schmidt process on V and W. */


    /* Parameter adjustments */
    --h__;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    --w;

    /* Function Body */
    i__1 = *i__;
    for (k = 1; k <= i__1; ++k) {
	h__[k] = ddot_(n, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
	d__1 = -h__[k];
	daxpy_(n, &d__1, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
/* L10: */
    }
    h__[*i__ + 1] = dnrm2_(n, &w[1], &c__1);
    dcopy_(n, &w[1], &c__1, &v[(*i__ + 1) * v_dim1 + 1], &c__1);
    tmpval = 1. / h__[*i__ + 1];
    dscal_(n, &tmpval, &v[(*i__ + 1) * v_dim1 + 1], &c__1);

    return 0;

} /* dorthoh_ */

/*     END SUBROUTINE dORTHOH */
/*     ========================================================= */
/* Subroutine */ int dapplygivens_(integer *i__, doublereal *h__, doublereal *
	givens, integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int dgetgiv_(doublereal *, doublereal *, 
	    doublereal *, doublereal *), drotvec_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);



/*     This routine applies a sequence of I-1 Givens rotations to */
/*     the I-th column of H. The Givens parameters are stored, so that */
/*     the first I-2 Givens rotation matrices are known. The I-1st */
/*     Givens rotation is computed using BLAS 1 routine DROTG. Each */
/*     rotation is applied to the 2x1 vector [H( J ), H( J+1 )]', */
/*     which results in H( J+1 ) = 0. */

/*      DOUBLE PRECISION   TEMP */

/*     .. Executable Statements .. */

/*     Construct I-1st rotation matrix. */

/*     CALL dROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL dGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */

/*     Apply 1,...,I-1st rotation matrices to the I-th column of H. */

    /* Parameter adjustments */
    --h__;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    i__1 = *i__ - 1;
    for (j = 1; j <= i__1; ++j) {
	drotvec_(&h__[j], &h__[j + 1], &givens[j + givens_dim1], &givens[j + (
		givens_dim1 << 1)]);
/*        TEMP     =  GIVENS( J,1 ) * H( J ) + GIVENS( J,2 ) * H( J+1 ) */
/*        H( J+1 ) = -GIVENS( J,2 ) * H( J ) + GIVENS( J,1 ) * H( J+1 ) */
/*        H( J ) = TEMP */
/* L10: */
    }
    dgetgiv_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);
    drotvec_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);

    return 0;

} /* dapplygivens_ */

/*     END SUBROUTINE dAPPLYGIVENS */

/*     =============================================================== */
doublereal dapproxres_(integer *i__, doublereal *h__, doublereal *s, 
	doublereal *givens, integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset;
    doublereal ret_val, d__1;

    /* Local variables */
    extern /* Subroutine */ int drotvec_(doublereal *, doublereal *, 
	    doublereal *, doublereal *);



/*     This func allows the user to approximate the residual */
/*     using an updating scheme involving Givens rotations. The */
/*     rotation matrix is formed using [H( I ),H( I+1 )]' with the */
/*     intent of zeroing H( I+1 ), but here is applied to the 2x1 */
/*     vector [S(I), S(I+1)]'. */


/*     .. Executable Statements .. */

/*     CALL dROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL dGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
    /* Parameter adjustments */
    --h__;
    --s;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    drotvec_(&s[*i__], &s[*i__ + 1], &givens[*i__ + givens_dim1], &givens[*
	    i__ + (givens_dim1 << 1)]);

    ret_val = (d__1 = s[*i__ + 1], abs(d__1));

    return ret_val;

} /* dapproxres_ */

/*     END FUNCTION dAPPROXRES */
/*     =============================================================== */
/* Subroutine */ int dupdate_(integer *i__, integer *n, doublereal *x, 
	doublereal *h__, integer *ldh, doublereal *y, doublereal *s, 
	doublereal *v, integer *ldv)
{
    /* System generated locals */
    integer h_dim1, h_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *), daxpy_(integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *), dtrsv_(char *, 
	    char *, char *, integer *, doublereal *, integer *, doublereal *, 
	    integer *, ftnlen, ftnlen, ftnlen);



/*     Solve H*y = s for upper triangualar H. */

    /* Parameter adjustments */
    --x;
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    --y;
    --s;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;

    /* Function Body */
    dcopy_(i__, &s[1], &c__1, &y[1], &c__1);
    dtrsv_("UPPER", "NOTRANS", "NONUNIT", i__, &h__[h_offset], ldh, &y[1], &
	    c__1, (ftnlen)5, (ftnlen)7, (ftnlen)7);

/*     Compute current solution vector X. */

    i__1 = *i__;
    for (j = 1; j <= i__1; ++j) {
	daxpy_(n, &y[j], &v[j * v_dim1 + 1], &c__1, &x[1], &c__1);
/* L10: */
    }

    return 0;

} /* dupdate_ */

/*     END SUBROUTINE dUPDATE */

/*     =============================================================== */
/* Subroutine */ int dgetgiv_(doublereal *a, doublereal *b, doublereal *c__, 
	doublereal *s)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal temp;



    if (abs(*b) == 0.) {
	*c__ = 1.;
	*s = 0.;
    } else if (abs(*b) > abs(*a)) {
	temp = -(*a) / *b;
/* Computing 2nd power */
	d__1 = abs(temp);
	*s = 1. / sqrt(d__1 * d__1 + 1.);
	*c__ = temp * *s;
/*         S = b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = -a / SQRT( abs(a)**2 + abs(b)**2 ) */
    } else {
	temp = -(*b) / *a;
/* Computing 2nd power */
	d__1 = abs(temp);
	*c__ = 1. / sqrt(d__1 * d__1 + 1.);
	*s = temp * *c__;
/*         S = -b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = a / SQRT( abs(a)**2 + abs(b)**2 ) */
    }

    return 0;

} /* dgetgiv_ */

/*     END SUBROUTINE dGETGIV */

/*     ================================================================ */
/* Subroutine */ int drotvec_(doublereal *x, doublereal *y, doublereal *c__, 
	doublereal *s)
{
    static doublereal temp;



    temp = *c__ * *x - *s * *y;
    *y = *s * *x + *c__ * *y;
    *x = temp;

    return 0;

} /* drotvec_ */

/*     END SUBROUTINE dROTVEC */

/*     =============================================================== */
/* Subroutine */ int delemvec_(integer *i__, integer *n, doublereal *alpha, 
	doublereal *e)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer j;


/*     Construct the I-th elementary vector E, scaled by ALPHA. */


/*     .. Parameters .. */

    /* Parameter adjustments */
    --e;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	e[j] = 0.;
/* L10: */
    }
    e[*i__] = *alpha;

    return 0;

} /* delemvec_ */

/*     END SUBROUTINE dELEMVEC */
/* Subroutine */ int cgmresrevcom_(integer *n, complex *b, complex *x, 
	integer *restrt, complex *work, integer *ldw, complex *work2, integer 
	*ldw2, integer *iter, real *resid, integer *info, integer *ndx1, 
	integer *ndx2, complex *sclr1, complex *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset, work2_dim1, work2_offset, i__1;
    real r__1;

    /* Local variables */
    extern /* Subroutine */ int celemvec_(integer *, integer *, complex *, 
	    complex *);
    static integer h__, i__, r__, s, v, w, y, av, giv;
    static real tol;
    static complex toz;
    static integer rlbl, need1, need2;
    static real bnrm2;
    extern /* Subroutine */ int capplygivens_(integer *, complex *, complex *,
	     integer *), cscal_(integer *, complex *, complex *, integer *);
    extern doublereal wscapproxres_(integer *, complex *, complex *, complex *
	    , integer *);
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *);
    static integer maxit;
    static real rnorm;
    static complex tmpval;
    extern doublereal wscnrm2_(integer *, complex *, integer *);
    extern /* Subroutine */ int cupdate_(integer *, integer *, complex *, 
	    complex *, integer *, complex *, complex *, complex *, integer *),
	     corthoh_(integer *, integer *, complex *, complex *, integer *, 
	    complex *);


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     EiITERkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  GMRES solves the linear system Ax = b using the */
/*  Generalized Minimal Residual iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  RESTRT  (input) INTEGER */
/*          Restart parameter, .ls. = N. This parameter controls the amount */
/*          of memory required for matrix WORK2. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6+restrt). */
/*          Note that if the initial guess is the zero vector, then */
/*          storing the initial residual is not necessary. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  WORK2   (workspace) DOUBLE PRECISION array, dimension (LDW2,2*RESTRT+2). */
/*          This workspace is used for constructing and storing the */
/*          upper Hessenberg matrix. The two extra columns are used to */
/*          store the Givens rotation matrices. */

/*  LDW2    (input) INTEGER */
/*          The leading dimension of the array WORK2. */
/*          LDW2 .gt. = max(2,RESTRT+1). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable error tolerance. */
/*          On ouput, the norm of the residual vector if solution */
/*          approximated to tolerance, otherwise reset to input */
/*          tolerance. */

/*  INFO    (output) INTEGER */
/*          =  0:  successful exit */
/*          =  1:  maximum number of iterations performed; */
/*                 convergence not achieved. */
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

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

/*  ============================================================ */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
/*     .. */
/*     .. Executable Statements .. */

/* Entry point, so test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;
    work2_dim1 = *ldw2;
    work2_offset = 1 + work2_dim1;
    work2 -= work2_offset;

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
	goto L200;
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
    s = 2;
    w = 3;
    y = 4;
    av = 5;
    v = 6;

    h__ = 1;
    giv = h__ + *restrt;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (av - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 > v * 1000 && *ndx1 <= v * 1000 + *restrt) {
	    need1 = (*ndx1 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx1 > giv * 1000 && *ndx1 <= giv * 1000 + *restrt) {
	    need1 = (*ndx1 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (av - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 > v * 1000 && *ndx2 <= v * 1000 + *restrt) {
	    need2 = (*ndx2 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx2 > giv * 1000 && *ndx2 <= giv * 1000 + *restrt) {
	    need2 = (*ndx2 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set initial residual. */

    ccopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (wscnrm2_(n, &x[1], &c__1) != 0.f) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using X directly */
	sclr1->r = -1.f, sclr1->i = 0.f;
	sclr2->r = 1.f, sclr2->i = 0.f;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 1;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (wscnrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L200;
    }
    bnrm2 = wscnrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.f) {
	bnrm2 = 1.f;
    }

    *iter = 0;
L10:

    ++(*iter);

/*        Construct the first column of V, and initialize S to the */
/*        elementary vector E1 scaled by RNORM. */

/* ********CALL PSOLVE( WORK( 1,V ), WORK( 1,R ) ) */

    *ndx1 = (v - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

    rnorm = wscnrm2_(n, &work[v * work_dim1 + 1], &c__1);
    r__1 = 1.f / rnorm;
    toz.r = r__1, toz.i = 0.f;
    cscal_(n, &toz, &work[v * work_dim1 + 1], &c__1);
    tmpval.r = rnorm, tmpval.i = 0.f;
    celemvec_(&c__1, n, &tmpval, &work[s * work_dim1 + 1]);

/*         DO 50 I = 1, RESTRT */
    i__ = 1;
L49:
    if (i__ > *restrt) {
	goto L50;
    }
/* ***********CALL MATVEC( ONE, WORK( 1,V+I-1 ), ZERO, WORK( 1,AV ) ) */

    *ndx1 = (v + i__ - 2) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1.f, sclr1->i = 0.f;
    sclr2->r = 0.f, sclr2->i = 0.f;
    rlbl = 4;
    *ijob = 3;
    return 0;

/* **************** */
L4:
/* **************** */

/* ********CALL PSOLVE( WORK( 1,W ), WORK( 1,AV ) ) */

    *ndx1 = (w - 1) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*           Construct I-th column of H so that it is orthnormal to */
/*           the previous I-1 columns. */

    corthoh_(&i__, n, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work[v * 
	    work_dim1 + 1], ldw, &work[w * work_dim1 + 1]);

    if (i__ > 0) {
	capplygivens_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work2[
		giv * work2_dim1 + 1], ldw2);
    }

/*              Apply Givens rotations to the I-th column of H. This */
/*              effectively reduces the Hessenberg matrix to upper */
/*              triangular form during the RESTRT iterations. */


/*           Approxiyate residual norm. Check tolerance. If okay, compute */
/*           final approximation vector X and quit. */

    *resid = wscapproxres_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &
	    work[s * work_dim1 + 1], &work2[giv * work2_dim1 + 1], ldw2) / 
	    bnrm2;
    if (*resid <= tol) {
	cupdate_(&i__, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y *
		 work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * 
		work_dim1 + 1], ldw);
	goto L200;
    }
    ++i__;
    goto L49;
L50:
    i__ = *restrt;

/*        Compute current solution vector X. */

    cupdate_(restrt, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y * 
	    work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * work_dim1 + 1]
	    , ldw);

/*        Compute residual vector R, find norm, */
/*        then check for tolerance. */

    ccopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
/* ********CALL MATVEC( -ONE, X, ONE, WORK( 1,R ) ) */

    *ndx1 = -1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = -1.f, sclr1->i = 0.f;
    sclr2->r = 1.f, sclr2->i = 0.f;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    i__1 = i__ + 1 + s * work_dim1;
    r__1 = wscnrm2_(n, &work[r__ * work_dim1 + 1], &c__1);
    work[i__1].r = r__1, work[i__1].i = 0.f;

/* ********RESID = WORK( I+1,S ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 200 */

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
	goto L200;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L100;
    }

    goto L10;

L100:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L200:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of GMRESREVCOM */

} /* cgmresrevcom_ */

/*     END SUBROUTINE cGMRESREVCOM */

/*     ========================================================= */
/* Subroutine */ int corthoh_(integer *i__, integer *n, complex *h__, complex 
	*v, integer *ldv, complex *w)
{
    /* System generated locals */
    integer v_dim1, v_offset, i__1, i__2;
    real r__1;
    complex q__1;

    /* Builtin functions */
    void c_div(complex *, complex *, complex *);

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int cscal_(integer *, complex *, complex *, 
	    integer *), ccopy_(integer *, complex *, integer *, complex *, 
	    integer *), caxpy_(integer *, complex *, complex *, integer *, 
	    complex *, integer *);
    extern /* Complex */ VOID wcdotc_(complex *, integer *, complex *, 
	    integer *, complex *, integer *);
    static complex tmpval;
    extern doublereal wscnrm2_(integer *, complex *, integer *);



/*     Construct the I-th column of the upper Hessenberg matrix H */
/*     using the Gram-Schmidt process on V and W. */


    /* Parameter adjustments */
    --h__;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    --w;

    /* Function Body */
    i__1 = *i__;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	wcdotc_(&q__1, n, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
	h__[i__2].r = q__1.r, h__[i__2].i = q__1.i;
	i__2 = k;
	q__1.r = -h__[i__2].r, q__1.i = -h__[i__2].i;
	caxpy_(n, &q__1, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
/* L10: */
    }
    i__1 = *i__ + 1;
    r__1 = wscnrm2_(n, &w[1], &c__1);
    h__[i__1].r = r__1, h__[i__1].i = 0.f;
    ccopy_(n, &w[1], &c__1, &v[(*i__ + 1) * v_dim1 + 1], &c__1);
    c_div(&q__1, &c_b136, &h__[*i__ + 1]);
    tmpval.r = q__1.r, tmpval.i = q__1.i;
    cscal_(n, &tmpval, &v[(*i__ + 1) * v_dim1 + 1], &c__1);

    return 0;

} /* corthoh_ */

/*     END SUBROUTINE cORTHOH */
/*     ========================================================= */
/* Subroutine */ int capplygivens_(integer *i__, complex *h__, complex *
	givens, integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int cgetgiv_(complex *, complex *, complex *, 
	    complex *), crotvec_(complex *, complex *, complex *, complex *);



/*     This routine applies a sequence of I-1 Givens rotations to */
/*     the I-th column of H. The Givens parameters are stored, so that */
/*     the first I-2 Givens rotation matrices are known. The I-1st */
/*     Givens rotation is computed using BLAS 1 routine DROTG. Each */
/*     rotation is applied to the 2x1 vector [H( J ), H( J+1 )]', */
/*     which results in H( J+1 ) = 0. */

/*      DOUBLE PRECISION   TEMP */

/*     .. Executable Statements .. */

/*     Construct I-1st rotation matrix. */

/*     CALL cROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL cGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */

/*     Apply 1,...,I-1st rotation matrices to the I-th column of H. */

    /* Parameter adjustments */
    --h__;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    i__1 = *i__ - 1;
    for (j = 1; j <= i__1; ++j) {
	crotvec_(&h__[j], &h__[j + 1], &givens[j + givens_dim1], &givens[j + (
		givens_dim1 << 1)]);
/*        TEMP     =  GIVENS( J,1 ) * H( J ) + GIVENS( J,2 ) * H( J+1 ) */
/*        H( J+1 ) = -GIVENS( J,2 ) * H( J ) + GIVENS( J,1 ) * H( J+1 ) */
/*        H( J ) = TEMP */
/* L10: */
    }
    cgetgiv_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);
    crotvec_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);

    return 0;

} /* capplygivens_ */

/*     END SUBROUTINE cAPPLYGIVENS */

/*     =============================================================== */
doublereal wscapproxres_(integer *i__, complex *h__, complex *s, complex *
	givens, integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset;
    real ret_val;

    /* Builtin functions */
    double c_abs(complex *);

    /* Local variables */
    extern /* Subroutine */ int crotvec_(complex *, complex *, complex *, 
	    complex *);



/*     This func allows the user to approximate the residual */
/*     using an updating scheme involving Givens rotations. The */
/*     rotation matrix is formed using [H( I ),H( I+1 )]' with the */
/*     intent of zeroing H( I+1 ), but here is applied to the 2x1 */
/*     vector [S(I), S(I+1)]'. */


/*     .. Executable Statements .. */

/*     CALL cROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL cGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
    /* Parameter adjustments */
    --h__;
    --s;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    crotvec_(&s[*i__], &s[*i__ + 1], &givens[*i__ + givens_dim1], &givens[*
	    i__ + (givens_dim1 << 1)]);

    ret_val = c_abs(&s[*i__ + 1]);

    return ret_val;

} /* wscapproxres_ */

/*     END FUNCTION wscAPPROXRES */
/*     =============================================================== */
/* Subroutine */ int cupdate_(integer *i__, integer *n, complex *x, complex *
	h__, integer *ldh, complex *y, complex *s, complex *v, integer *ldv)
{
    /* System generated locals */
    integer h_dim1, h_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *), caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *), ctrsv_(char *, char *, char *, 
	    integer *, complex *, integer *, complex *, integer *, ftnlen, 
	    ftnlen, ftnlen);



/*     Solve H*y = s for upper triangualar H. */

    /* Parameter adjustments */
    --x;
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    --y;
    --s;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;

    /* Function Body */
    ccopy_(i__, &s[1], &c__1, &y[1], &c__1);
    ctrsv_("UPPER", "NOTRANS", "NONUNIT", i__, &h__[h_offset], ldh, &y[1], &
	    c__1, (ftnlen)5, (ftnlen)7, (ftnlen)7);

/*     Compute current solution vector X. */

    i__1 = *i__;
    for (j = 1; j <= i__1; ++j) {
	caxpy_(n, &y[j], &v[j * v_dim1 + 1], &c__1, &x[1], &c__1);
/* L10: */
    }

    return 0;

} /* cupdate_ */

/*     END SUBROUTINE cUPDATE */

/*     =============================================================== */
/* Subroutine */ int cgetgiv_(complex *a, complex *b, complex *c__, complex *
	s)
{
    /* System generated locals */
    real r__1, r__2;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    double c_abs(complex *);
    void c_div(complex *, complex *, complex *), c_sqrt(complex *, complex *);

    /* Local variables */
    static complex temp;



    r__1 = c_abs(b);
    if (r__1 == 0.f) {
	c__->r = 1.f, c__->i = 0.f;
	s->r = 0.f, s->i = 0.f;
    } else if (c_abs(b) > c_abs(a)) {
	q__2.r = -a->r, q__2.i = -a->i;
	c_div(&q__1, &q__2, b);
	temp.r = q__1.r, temp.i = q__1.i;
/* Computing 2nd power */
	r__2 = c_abs(&temp);
	r__1 = r__2 * r__2;
	q__3.r = r__1 + 1.f, q__3.i = 0.f;
	c_sqrt(&q__2, &q__3);
	c_div(&q__1, &c_b136, &q__2);
	s->r = q__1.r, s->i = q__1.i;
	q__1.r = temp.r * s->r - temp.i * s->i, q__1.i = temp.r * s->i + 
		temp.i * s->r;
	c__->r = q__1.r, c__->i = q__1.i;
/*         S = b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = -a / SQRT( abs(a)**2 + abs(b)**2 ) */
    } else {
	q__2.r = -b->r, q__2.i = -b->i;
	c_div(&q__1, &q__2, a);
	temp.r = q__1.r, temp.i = q__1.i;
/* Computing 2nd power */
	r__2 = c_abs(&temp);
	r__1 = r__2 * r__2;
	q__3.r = r__1 + 1.f, q__3.i = 0.f;
	c_sqrt(&q__2, &q__3);
	c_div(&q__1, &c_b136, &q__2);
	c__->r = q__1.r, c__->i = q__1.i;
	q__1.r = temp.r * c__->r - temp.i * c__->i, q__1.i = temp.r * c__->i 
		+ temp.i * c__->r;
	s->r = q__1.r, s->i = q__1.i;
/*         S = -b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = a / SQRT( abs(a)**2 + abs(b)**2 ) */
    }

    return 0;

} /* cgetgiv_ */

/*     END SUBROUTINE cGETGIV */

/*     ================================================================ */
/* Subroutine */ int crotvec_(complex *x, complex *y, complex *c__, complex *
	s)
{
    /* System generated locals */
    complex q__1, q__2, q__3, q__4, q__5;

    /* Builtin functions */
    void r_cnjg(complex *, complex *);

    /* Local variables */
    static complex temp;



    r_cnjg(&q__3, c__);
    q__2.r = q__3.r * x->r - q__3.i * x->i, q__2.i = q__3.r * x->i + q__3.i * 
	    x->r;
    r_cnjg(&q__5, s);
    q__4.r = q__5.r * y->r - q__5.i * y->i, q__4.i = q__5.r * y->i + q__5.i * 
	    y->r;
    q__1.r = q__2.r - q__4.r, q__1.i = q__2.i - q__4.i;
    temp.r = q__1.r, temp.i = q__1.i;
    q__2.r = s->r * x->r - s->i * x->i, q__2.i = s->r * x->i + s->i * x->r;
    q__3.r = c__->r * y->r - c__->i * y->i, q__3.i = c__->r * y->i + c__->i * 
	    y->r;
    q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
    y->r = q__1.r, y->i = q__1.i;
    x->r = temp.r, x->i = temp.i;

    return 0;

} /* crotvec_ */

/*     END SUBROUTINE cROTVEC */

/*     =============================================================== */
/* Subroutine */ int celemvec_(integer *i__, integer *n, complex *alpha, 
	complex *e)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer j;


/*     Construct the I-th elementary vector E, scaled by ALPHA. */


/*     .. Parameters .. */

    /* Parameter adjustments */
    --e;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = j;
	e[i__2].r = 0.f, e[i__2].i = 0.f;
/* L10: */
    }
    i__1 = *i__;
    e[i__1].r = alpha->r, e[i__1].i = alpha->i;

    return 0;

} /* celemvec_ */

/*     END SUBROUTINE cELEMVEC */
/* Subroutine */ int zgmresrevcom_(integer *n, doublecomplex *b, 
	doublecomplex *x, integer *restrt, doublecomplex *work, integer *ldw, 
	doublecomplex *work2, integer *ldw2, integer *iter, doublereal *resid,
	 integer *info, integer *ndx1, integer *ndx2, doublecomplex *sclr1, 
	doublecomplex *sclr2, integer *ijob)
{
    /* System generated locals */
    integer work_dim1, work_offset, work2_dim1, work2_offset, i__1;
    doublereal d__1;

    /* Local variables */
    extern /* Subroutine */ int zelemvec_(integer *, integer *, doublecomplex 
	    *, doublecomplex *);
    static integer h__, i__, r__, s, v, w, y, av, giv;
    static doublereal tol;
    static doublecomplex toz;
    extern doublereal dzapproxres_(integer *, doublecomplex *, doublecomplex *
	    , doublecomplex *, integer *);
    static integer rlbl, need1, need2;
    static doublereal bnrm2;
    extern /* Subroutine */ int zapplygivens_(integer *, doublecomplex *, 
	    doublecomplex *, integer *), zscal_(integer *, doublecomplex *, 
	    doublecomplex *, integer *);
    static integer maxit;
    static doublereal rnorm;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *);
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);
    static doublecomplex tmpval;
    extern /* Subroutine */ int zupdate_(integer *, integer *, doublecomplex *
	    , doublecomplex *, integer *, doublecomplex *, doublecomplex *, 
	    doublecomplex *, integer *), zorthoh_(integer *, integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *);


/*  -- Iterative template routine -- */
/*     Univ. of Tennessee and Oak Ridge National Laboratory */
/*     October 1, 1993 */
/*     Details of this algorithm are described in "Templates for the */
/*     Solution of Linear Systems: Building Blocks for Iterative */
/*     Methods", Barrett, Berry, Chan, Demmel, Donato, Dongarra, */
/*     EiITERkhout, Pozo, Romine, and van der Vorst, SIAM Publications, */
/*     1993. (ftp netlib2.cs.utk.edu; cd linalg; get templates.ps). */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  GMRES solves the linear system Ax = b using the */
/*  Generalized Minimal Residual iterative method with preconditioning. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER. */
/*          On entry, the dimension of the matrix. */
/*          Unchanged on exit. */

/*  B       (input) DOUBLE PRECISION array, dimension N. */
/*          On entry, right hand side vector B. */
/*          Unchanged on exit. */

/*  X       (input/output) DOUBLE PRECISION array, dimension N. */
/*          On input, the initial guess; on exit, the iterated solution. */

/*  RESTRT  (input) INTEGER */
/*          Restart parameter, .ls. = N. This parameter controls the amount */
/*          of memory required for matrix WORK2. */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (LDW,6+restrt). */
/*          Note that if the initial guess is the zero vector, then */
/*          storing the initial residual is not necessary. */

/*  LDW     (input) INTEGER */
/*          The leading dimension of the array WORK. LDW .gt. = max(1,N). */

/*  WORK2   (workspace) DOUBLE PRECISION array, dimension (LDW2,2*RESTRT+2). */
/*          This workspace is used for constructing and storing the */
/*          upper Hessenberg matrix. The two extra columns are used to */
/*          store the Givens rotation matrices. */

/*  LDW2    (input) INTEGER */
/*          The leading dimension of the array WORK2. */
/*          LDW2 .gt. = max(2,RESTRT+1). */

/*  ITER    (input/output) INTEGER */
/*          On input, the maximum iterations to be performed. */
/*          On output, actual number of iterations performed. */

/*  RESID   (input/output) DOUBLE PRECISION */
/*          On input, the allowable error tolerance. */
/*          On ouput, the norm of the residual vector if solution */
/*          approximated to tolerance, otherwise reset to input */
/*          tolerance. */

/*  INFO    (output) INTEGER */
/*          =  0:  successful exit */
/*          =  1:  maximum number of iterations performed; */
/*                 convergence not achieved. */
/*            -5: Erroneous NDX1/NDX2 in INIT call. */
/*            -6: Erroneous RLBL. */

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

/*  ============================================================ */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */

/*     indicates where to resume from. Only valid when IJOB = 2! */

/*     saving all. */

/*     .. */
/*     .. External Routines .. */
/*     .. */
/*     .. Executable Statements .. */

/* Entry point, so test IJOB */
    /* Parameter adjustments */
    --b;
    --x;
    work_dim1 = *ldw;
    work_offset = 1 + work_dim1;
    work -= work_offset;
    work2_dim1 = *ldw2;
    work2_offset = 1 + work2_dim1;
    work2 -= work2_offset;

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
	goto L200;
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
    s = 2;
    w = 3;
    y = 4;
    av = 5;
    v = 6;

    h__ = 1;
    giv = h__ + *restrt;

/*     Check if caller will need indexing info. */

    if (*ndx1 != -1) {
	if (*ndx1 == 1) {
	    need1 = (r__ - 1) * *ldw + 1;
	} else if (*ndx1 == 2) {
	    need1 = (s - 1) * *ldw + 1;
	} else if (*ndx1 == 3) {
	    need1 = (w - 1) * *ldw + 1;
	} else if (*ndx1 == 4) {
	    need1 = (y - 1) * *ldw + 1;
	} else if (*ndx1 == 5) {
	    need1 = (av - 1) * *ldw + 1;
	} else if (*ndx1 == 6) {
	    need1 = (v - 1) * *ldw + 1;
	} else if (*ndx1 > v * 1000 && *ndx1 <= v * 1000 + *restrt) {
	    need1 = (*ndx1 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx1 > giv * 1000 && *ndx1 <= giv * 1000 + *restrt) {
	    need1 = (*ndx1 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need1 = *ndx1;
    }

    if (*ndx2 != -1) {
	if (*ndx2 == 1) {
	    need2 = (r__ - 1) * *ldw + 1;
	} else if (*ndx2 == 2) {
	    need2 = (s - 1) * *ldw + 1;
	} else if (*ndx2 == 3) {
	    need2 = (w - 1) * *ldw + 1;
	} else if (*ndx2 == 4) {
	    need2 = (y - 1) * *ldw + 1;
	} else if (*ndx2 == 5) {
	    need2 = (av - 1) * *ldw + 1;
	} else if (*ndx2 == 6) {
	    need2 = (v - 1) * *ldw + 1;
	} else if (*ndx2 > v * 1000 && *ndx2 <= v * 1000 + *restrt) {
	    need2 = (*ndx2 - v * 1000 - 1) * *ldw + 1;
	} else if (*ndx2 > giv * 1000 && *ndx2 <= giv * 1000 + *restrt) {
	    need2 = (*ndx2 - giv * 1000 - 1) * *ldw + 1;
	} else {
/*           report error */
	    *info = -5;
	    goto L100;
	}
    } else {
	need2 = *ndx2;
    }

/*     Set initial residual. */

    zcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
    if (dznrm2_(n, &x[1], &c__1) != 0.) {
/* ********CALL MATVEC( -ONE, X, ONE, WORK(1,R) ) */
/*        Note: using X directly */
	sclr1->r = -1., sclr1->i = 0.;
	sclr2->r = 1., sclr2->i = 0.;
	*ndx1 = -1;
	*ndx2 = (r__ - 1) * *ldw + 1;

/*        Prepare for resumption & return */
	rlbl = 2;
	*ijob = 1;
	return 0;
    }

/* **************** */
L2:
/* **************** */

    if (dznrm2_(n, &work[r__ * work_dim1 + 1], &c__1) < tol) {
	goto L200;
    }
    bnrm2 = dznrm2_(n, &b[1], &c__1);
    if (bnrm2 == 0.) {
	bnrm2 = 1.;
    }

    *iter = 0;
L10:

    ++(*iter);

/*        Construct the first column of V, and initialize S to the */
/*        elementary vector E1 scaled by RNORM. */

/* ********CALL PSOLVE( WORK( 1,V ), WORK( 1,R ) ) */

    *ndx1 = (v - 1) * *ldw + 1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 3;
    *ijob = 2;
    return 0;

/* **************** */
L3:
/* **************** */

    rnorm = dznrm2_(n, &work[v * work_dim1 + 1], &c__1);
    d__1 = 1. / rnorm;
    toz.r = d__1, toz.i = 0.;
    zscal_(n, &toz, &work[v * work_dim1 + 1], &c__1);
    tmpval.r = rnorm, tmpval.i = 0.;
    zelemvec_(&c__1, n, &tmpval, &work[s * work_dim1 + 1]);

/*         DO 50 I = 1, RESTRT */
    i__ = 1;
L49:
    if (i__ > *restrt) {
	goto L50;
    }
/* ***********CALL MATVEC( ONE, WORK( 1,V+I-1 ), ZERO, WORK( 1,AV ) ) */

    *ndx1 = (v + i__ - 2) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = 1., sclr1->i = 0.;
    sclr2->r = 0., sclr2->i = 0.;
    rlbl = 4;
    *ijob = 3;
    return 0;

/* **************** */
L4:
/* **************** */

/* ********CALL PSOLVE( WORK( 1,W ), WORK( 1,AV ) ) */

    *ndx1 = (w - 1) * *ldw + 1;
    *ndx2 = (av - 1) * *ldw + 1;
/*        Prepare for return & return */
    rlbl = 5;
    *ijob = 2;
    return 0;

/* **************** */
L5:
/* **************** */

/*           Construct I-th column of H so that it is orthnormal to */
/*           the previous I-1 columns. */

    zorthoh_(&i__, n, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work[v * 
	    work_dim1 + 1], ldw, &work[w * work_dim1 + 1]);

    if (i__ > 0) {
	zapplygivens_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &work2[
		giv * work2_dim1 + 1], ldw2);
    }

/*              Apply Givens rotations to the I-th column of H. This */
/*              effectively reduces the Hessenberg matrix to upper */
/*              triangular form during the RESTRT iterations. */


/*           Approxiyate residual norm. Check tolerance. If okay, compute */
/*           final approximation vector X and quit. */

    *resid = dzapproxres_(&i__, &work2[(i__ + h__ - 1) * work2_dim1 + 1], &
	    work[s * work_dim1 + 1], &work2[giv * work2_dim1 + 1], ldw2) / 
	    bnrm2;
    if (*resid <= tol) {
	zupdate_(&i__, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y *
		 work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * 
		work_dim1 + 1], ldw);
	goto L200;
    }
    ++i__;
    goto L49;
L50:
    i__ = *restrt;

/*        Compute current solution vector X. */

    zupdate_(restrt, n, &x[1], &work2[h__ * work2_dim1 + 1], ldw2, &work[y * 
	    work_dim1 + 1], &work[s * work_dim1 + 1], &work[v * work_dim1 + 1]
	    , ldw);

/*        Compute residual vector R, find norm, */
/*        then check for tolerance. */

    zcopy_(n, &b[1], &c__1, &work[r__ * work_dim1 + 1], &c__1);
/* ********CALL MATVEC( -ONE, X, ONE, WORK( 1,R ) ) */

    *ndx1 = -1;
    *ndx2 = (r__ - 1) * *ldw + 1;
/*        Prepare for return & return */
    sclr1->r = -1., sclr1->i = 0.;
    sclr2->r = 1., sclr2->i = 0.;
    rlbl = 6;
    *ijob = 1;
    return 0;

/* **************** */
L6:
/* **************** */

    i__1 = i__ + 1 + s * work_dim1;
    d__1 = dznrm2_(n, &work[r__ * work_dim1 + 1], &c__1);
    work[i__1].r = d__1, work[i__1].i = 0.;

/* ********RESID = WORK( I+1,S ) / BNRM2 */
/* ********IF ( RESID.LE.TOL  ) GO TO 200 */

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
	goto L200;
    }

    if (*iter == maxit) {
	*info = 1;
	goto L100;
    }

    goto L10;

L100:

/*     Iteration fails. */

    rlbl = -1;
    *ijob = -1;
    return 0;

L200:

/*     Iteration successful; return. */

    *info = 0;
    rlbl = -1;
    *ijob = -1;
    return 0;

/*     End of GMRESREVCOM */

} /* zgmresrevcom_ */

/*     END SUBROUTINE zGMRESREVCOM */

/*     ========================================================= */
/* Subroutine */ int zorthoh_(integer *i__, integer *n, doublecomplex *h__, 
	doublecomplex *v, integer *ldv, doublecomplex *w)
{
    /* System generated locals */
    integer v_dim1, v_offset, i__1, i__2;
    doublereal d__1;
    doublecomplex z__1;

    /* Builtin functions */
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int zscal_(integer *, doublecomplex *, 
	    doublecomplex *, integer *), zcopy_(integer *, doublecomplex *, 
	    integer *, doublecomplex *, integer *), zaxpy_(integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *);
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);
    static doublecomplex tmpval;
    extern /* Double Complex */ VOID wzdotc_(doublecomplex *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *);



/*     Construct the I-th column of the upper Hessenberg matrix H */
/*     using the Gram-Schmidt process on V and W. */


    /* Parameter adjustments */
    --h__;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    --w;

    /* Function Body */
    i__1 = *i__;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	wzdotc_(&z__1, n, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
	h__[i__2].r = z__1.r, h__[i__2].i = z__1.i;
	i__2 = k;
	z__1.r = -h__[i__2].r, z__1.i = -h__[i__2].i;
	zaxpy_(n, &z__1, &v[k * v_dim1 + 1], &c__1, &w[1], &c__1);
/* L10: */
    }
    i__1 = *i__ + 1;
    d__1 = dznrm2_(n, &w[1], &c__1);
    h__[i__1].r = d__1, h__[i__1].i = 0.;
    zcopy_(n, &w[1], &c__1, &v[(*i__ + 1) * v_dim1 + 1], &c__1);
    z_div(&z__1, &c_b195, &h__[*i__ + 1]);
    tmpval.r = z__1.r, tmpval.i = z__1.i;
    zscal_(n, &tmpval, &v[(*i__ + 1) * v_dim1 + 1], &c__1);

    return 0;

} /* zorthoh_ */

/*     END SUBROUTINE zORTHOH */
/*     ========================================================= */
/* Subroutine */ int zapplygivens_(integer *i__, doublecomplex *h__, 
	doublecomplex *givens, integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int zgetgiv_(doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *), zrotvec_(doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublecomplex *);



/*     This routine applies a sequence of I-1 Givens rotations to */
/*     the I-th column of H. The Givens parameters are stored, so that */
/*     the first I-2 Givens rotation matrices are known. The I-1st */
/*     Givens rotation is computed using BLAS 1 routine DROTG. Each */
/*     rotation is applied to the 2x1 vector [H( J ), H( J+1 )]', */
/*     which results in H( J+1 ) = 0. */

/*      DOUBLE PRECISION   TEMP */

/*     .. Executable Statements .. */

/*     Construct I-1st rotation matrix. */

/*     CALL zROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL zGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */

/*     Apply 1,...,I-1st rotation matrices to the I-th column of H. */

    /* Parameter adjustments */
    --h__;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    i__1 = *i__ - 1;
    for (j = 1; j <= i__1; ++j) {
	zrotvec_(&h__[j], &h__[j + 1], &givens[j + givens_dim1], &givens[j + (
		givens_dim1 << 1)]);
/*        TEMP     =  GIVENS( J,1 ) * H( J ) + GIVENS( J,2 ) * H( J+1 ) */
/*        H( J+1 ) = -GIVENS( J,2 ) * H( J ) + GIVENS( J,1 ) * H( J+1 ) */
/*        H( J ) = TEMP */
/* L10: */
    }
    zgetgiv_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);
    zrotvec_(&h__[*i__], &h__[*i__ + 1], &givens[*i__ + givens_dim1], &givens[
	    *i__ + (givens_dim1 << 1)]);

    return 0;

} /* zapplygivens_ */

/*     END SUBROUTINE zAPPLYGIVENS */

/*     =============================================================== */
doublereal dzapproxres_(integer *i__, doublecomplex *h__, doublecomplex *s, 
	doublecomplex *givens, integer *ldg)
{
    /* System generated locals */
    integer givens_dim1, givens_offset;
    doublereal ret_val;

    /* Builtin functions */
    double z_abs(doublecomplex *);

    /* Local variables */
    extern /* Subroutine */ int zrotvec_(doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *);



/*     This func allows the user to approximate the residual */
/*     using an updating scheme involving Givens rotations. The */
/*     rotation matrix is formed using [H( I ),H( I+1 )]' with the */
/*     intent of zeroing H( I+1 ), but here is applied to the 2x1 */
/*     vector [S(I), S(I+1)]'. */


/*     .. Executable Statements .. */

/*     CALL zROTG( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
/*      CALL zGETGIV( H( I ), H( I+1 ), GIVENS( I,1 ), GIVENS( I,2 ) ) */
    /* Parameter adjustments */
    --h__;
    --s;
    givens_dim1 = *ldg;
    givens_offset = 1 + givens_dim1;
    givens -= givens_offset;

    /* Function Body */
    zrotvec_(&s[*i__], &s[*i__ + 1], &givens[*i__ + givens_dim1], &givens[*
	    i__ + (givens_dim1 << 1)]);

    ret_val = z_abs(&s[*i__ + 1]);

    return ret_val;

} /* dzapproxres_ */

/*     END FUNCTION dzAPPROXRES */
/*     =============================================================== */
/* Subroutine */ int zupdate_(integer *i__, integer *n, doublecomplex *x, 
	doublecomplex *h__, integer *ldh, doublecomplex *y, doublecomplex *s, 
	doublecomplex *v, integer *ldv)
{
    /* System generated locals */
    integer h_dim1, h_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    static integer j;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), zaxpy_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *), ztrsv_(
	    char *, char *, char *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *, ftnlen, ftnlen, ftnlen);



/*     Solve H*y = s for upper triangualar H. */

    /* Parameter adjustments */
    --x;
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    --y;
    --s;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;

    /* Function Body */
    zcopy_(i__, &s[1], &c__1, &y[1], &c__1);
    ztrsv_("UPPER", "NOTRANS", "NONUNIT", i__, &h__[h_offset], ldh, &y[1], &
	    c__1, (ftnlen)5, (ftnlen)7, (ftnlen)7);

/*     Compute current solution vector X. */

    i__1 = *i__;
    for (j = 1; j <= i__1; ++j) {
	zaxpy_(n, &y[j], &v[j * v_dim1 + 1], &c__1, &x[1], &c__1);
/* L10: */
    }

    return 0;

} /* zupdate_ */

/*     END SUBROUTINE zUPDATE */

/*     =============================================================== */
/* Subroutine */ int zgetgiv_(doublecomplex *a, doublecomplex *b, 
	doublecomplex *c__, doublecomplex *s)
{
    /* System generated locals */
    doublereal d__1, d__2;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    double z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *), z_sqrt(
	    doublecomplex *, doublecomplex *);

    /* Local variables */
    static doublecomplex temp;



    d__1 = z_abs(b);
    if (d__1 == 0.) {
	c__->r = 1., c__->i = 0.;
	s->r = 0., s->i = 0.;
    } else if (z_abs(b) > z_abs(a)) {
	z__2.r = -a->r, z__2.i = -a->i;
	z_div(&z__1, &z__2, b);
	temp.r = z__1.r, temp.i = z__1.i;
/* Computing 2nd power */
	d__2 = z_abs(&temp);
	d__1 = d__2 * d__2;
	z__3.r = d__1 + 1., z__3.i = 0.;
	z_sqrt(&z__2, &z__3);
	z_div(&z__1, &c_b195, &z__2);
	s->r = z__1.r, s->i = z__1.i;
	z__1.r = temp.r * s->r - temp.i * s->i, z__1.i = temp.r * s->i + 
		temp.i * s->r;
	c__->r = z__1.r, c__->i = z__1.i;
/*         S = b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = -a / SQRT( abs(a)**2 + abs(b)**2 ) */
    } else {
	z__2.r = -b->r, z__2.i = -b->i;
	z_div(&z__1, &z__2, a);
	temp.r = z__1.r, temp.i = z__1.i;
/* Computing 2nd power */
	d__2 = z_abs(&temp);
	d__1 = d__2 * d__2;
	z__3.r = d__1 + 1., z__3.i = 0.;
	z_sqrt(&z__2, &z__3);
	z_div(&z__1, &c_b195, &z__2);
	c__->r = z__1.r, c__->i = z__1.i;
	z__1.r = temp.r * c__->r - temp.i * c__->i, z__1.i = temp.r * c__->i 
		+ temp.i * c__->r;
	s->r = z__1.r, s->i = z__1.i;
/*         S = -b / SQRT( abs(a)**2 + abs(b)**2 ) */
/*         C = a / SQRT( abs(a)**2 + abs(b)**2 ) */
    }

    return 0;

} /* zgetgiv_ */

/*     END SUBROUTINE zGETGIV */

/*     ================================================================ */
/* Subroutine */ int zrotvec_(doublecomplex *x, doublecomplex *y, 
	doublecomplex *c__, doublecomplex *s)
{
    /* System generated locals */
    doublecomplex z__1, z__2, z__3, z__4, z__5;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static doublecomplex temp;



    d_cnjg(&z__3, c__);
    z__2.r = z__3.r * x->r - z__3.i * x->i, z__2.i = z__3.r * x->i + z__3.i * 
	    x->r;
    d_cnjg(&z__5, s);
    z__4.r = z__5.r * y->r - z__5.i * y->i, z__4.i = z__5.r * y->i + z__5.i * 
	    y->r;
    z__1.r = z__2.r - z__4.r, z__1.i = z__2.i - z__4.i;
    temp.r = z__1.r, temp.i = z__1.i;
    z__2.r = s->r * x->r - s->i * x->i, z__2.i = s->r * x->i + s->i * x->r;
    z__3.r = c__->r * y->r - c__->i * y->i, z__3.i = c__->r * y->i + c__->i * 
	    y->r;
    z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
    y->r = z__1.r, y->i = z__1.i;
    x->r = temp.r, x->i = temp.i;

    return 0;

} /* zrotvec_ */

/*     END SUBROUTINE zROTVEC */

/*     =============================================================== */
/* Subroutine */ int zelemvec_(integer *i__, integer *n, doublecomplex *alpha,
	 doublecomplex *e)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer j;


/*     Construct the I-th elementary vector E, scaled by ALPHA. */


/*     .. Parameters .. */

    /* Parameter adjustments */
    --e;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = j;
	e[i__2].r = 0., e[i__2].i = 0.;
/* L10: */
    }
    i__1 = *i__;
    e[i__1].r = alpha->r, e[i__1].i = alpha->i;

    return 0;

} /* zelemvec_ */

