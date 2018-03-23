/* cobyla2.f -- translated by f2c (version 20100827).
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
static integer c__9 = 9;
static integer c__5 = 5;
static integer c__3 = 3;

/* ------------------------------------------------------------------------ */

/* Subroutine */ int cobyla_(U_fp calcfc, integer *n, integer *m, doublereal *
	x, doublereal *rhobeg, doublereal *rhoend, integer *iprint, integer *
	maxfun, doublereal *w, integer *iact, doublereal *dinfo)
{
    static integer ia, idx, mpp, icon, isim, isigb, idatm, iveta, isimi, 
	    ivsig, iwork;
    extern /* Subroutine */ int cobylb_(U_fp, integer *, integer *, integer *,
	     doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *);


/*     This subroutine minimizes an objective function F(X) subject to M */
/*     inequality constraints on X, where X is a vector of variables that has */
/*     N components. The algorithm employs linear approximations to the */
/*     objective and constraint functions, the approximations being formed by */
/*     linear interpolation at N+1 points in the space of the variables. */
/*     We regard these interpolation points as vertices of a simplex. The */
/*     parameter RHO controls the size of the simplex and it is reduced */
/*     automatically from RHOBEG to RHOEND. For each RHO the subroutine tries */
/*     to achieve a good vector of variables for the current size, and then */
/*     RHO is reduced until the value RHOEND is reached. Therefore RHOBEG and */
/*     RHOEND should be set to reasonable initial changes to and the required */
/*     accuracy in the variables respectively, but this accuracy should be */
/*     viewed as a subject for experimentation because it is not guaranteed. */
/*     The subroutine has an advantage over many of its competitors, however, */
/*     which is that it treats each constraint individually when calculating */
/*     a change to the variables, instead of lumping the constraints together */
/*     into a single penalty function. The name of the subroutine is derived */
/*     from the phrase Constrained Optimization BY Linear Approximations. */

/*     The user must set the values of N, M, RHOBEG and RHOEND, and must */
/*     provide an initial vector of variables in X. Further, the value of */
/*     IPRINT should be set to 0, 1, 2 or 3, which controls the amount of */
/*     printing during the calculation. Specifically, there is no output if */
/*     IPRINT=0 and there is output only at the end of the calculation if */
/*     IPRINT=1. Otherwise each new value of RHO and SIGMA is printed. */
/*     Further, the vector of variables and some function information are */
/*     given either when RHO is reduced or when each new value of F(X) is */
/*     computed in the cases IPRINT=2 or IPRINT=3 respectively. Here SIGMA */
/*     is a penalty parameter, it being assumed that a change to X is an */
/*     improvement if it reduces the merit function */
/*                F(X)+SIGMA*MAX(0.0,-C1(X),-C2(X),...,-CM(X)), */
/*     where C1,C2,...,CM denote the constraint functions that should become */
/*     nonnegative eventually, at least to the precision of RHOEND. In the */
/*     printed output the displayed term that is multiplied by SIGMA is */
/*     called MAXCV, which stands for 'MAXimum Constraint Violation'. The */
/*     argument MAXFUN is an integer variable that must be set by the user to a */
/*     limit on the number of calls of CALCFC, the purpose of this routine being */
/*     given below. The value of MAXFUN will be altered to the number of calls */
/*     of CALCFC that are made. The arguments W and IACT provide real and */
/*     integer arrays that are used as working space. Their lengths must be at */
/*     least N*(3*N+2*M+11)+4*M+6 and M+1 respectively. */

/*     In order to define the objective and constraint functions, we require */
/*     a subroutine that has the name and arguments */
/*                SUBROUTINE CALCFC (N,M,X,F,CON) */
/*                DIMENSION X(*),CON(*)  . */
/*     The values of N and M are fixed and have been defined already, while */
/*     X is now the current vector of variables. The subroutine should return */
/*     the objective and constraint functions at X in F and CON(1),CON(2), */
/*     ...,CON(M). Note that we are trying to adjust X so that F(X) is as */
/*     small as possible subject to the constraint functions being nonnegative. */

/*     Partition the working space array W to provide the storage that is needed */
/*     for the main calculation. */

    /* Parameter adjustments */
    --dinfo;
    --iact;
    --w;
    --x;

    /* Function Body */
    mpp = *m + 2;
    icon = 1;
    isim = icon + mpp;
    isimi = isim + *n * *n + *n;
    idatm = isimi + *n * *n;
    ia = idatm + *n * mpp + mpp;
    ivsig = ia + *m * *n + *n;
    iveta = ivsig + *n;
    isigb = iveta + *n;
    idx = isigb + *n;
    iwork = idx + *n;
    cobylb_((U_fp)calcfc, n, m, &mpp, &x[1], rhobeg, rhoend, iprint, maxfun, &
	    w[icon], &w[isim], &w[isimi], &w[idatm], &w[ia], &w[ivsig], &w[
	    iveta], &w[isigb], &w[idx], &w[iwork], &iact[1], &dinfo[1]);
    return 0;
} /* cobyla_ */

/* ------------------------------------------------------------------------------ */
/* Subroutine */ int cobylb_(S_fp calcfc, integer *n, integer *m, integer *
	mpp, doublereal *x, doublereal *rhobeg, doublereal *rhoend, integer *
	iprint, integer *maxfun, doublereal *con, doublereal *sim, doublereal 
	*simi, doublereal *datmat, doublereal *a, doublereal *vsig, 
	doublereal *veta, doublereal *sigbar, doublereal *dx, doublereal *w, 
	integer *iact, doublereal *dinfo)
{
    /* Format strings */
    static char fmt_10[] = "(/3x,\002The initial value of RHO is\002,1pe13.6"
	    ",2x,\002and PARMU is set to zero.\002)";
    static char fmt_50[] = "(/3x,\002Return from subroutine COBYLA because t"
	    "he \002,\002MAXFUN limit has been reached.\002)";
    static char fmt_70[] = "(/3x,\002NFVALS =\002,i5,3x,\002F =\002,1pe13.6,"
	    "4x,\002MAXCV =\002,1pe13.6/3x,\002X =\002,1pe13.6,1p4e15.6)";
    static char fmt_80[] = "(1pe19.6,1p4e15.6)";
    static char fmt_210[] = "(/3x,\002Return from subroutine COBYLA because"
	    " \002,\002rounding errors are becoming damaging.\002)";
    static char fmt_410[] = "(/3x,\002Increase in PARMU to\002,1pe13.6)";
    static char fmt_580[] = "(/3x,\002Reduction in RHO to\002,1pe13.6,\002  "
	    "and PARMU =\002,1pe13.6)";
    static char fmt_590[] = "(/3x,\002Normal return from subroutine COBYL"
	    "A\002)";

    /* System generated locals */
    integer sim_dim1, sim_offset, simi_dim1, simi_offset, datmat_dim1, 
	    datmat_offset, a_dim1, a_offset, i__1, i__2, i__3;
    doublereal d__1, d__2;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
	     s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double sqrt(doublereal);

    /* Local variables */
    static doublereal f;
    static integer i__, j, k, l, mp, np, iz;
    static doublereal phi, rho, sum, beta, cmin, cmax;
    static integer ivmc;
    static doublereal weta;
    static integer ivmd;
    static doublereal temp, wsig, gamma;
    static integer iflag;
    static doublereal alpha, delta, denom, tempa, barmu;
    static integer nbest, ifull, iptem, jdrop;
    static doublereal ratio, vmold, parmu, error, vmnew;
    static integer ibrnch;
    static doublereal edgmax, pareta, prerec, phimin, parsig;
    static integer isdirn, nfvals;
    static doublereal prerem, cvmaxm;
    static integer itotal;
    static doublereal dxsign;
    static integer iptemp;
    static doublereal resmax, cvmaxp;
    static integer izdota, idxnew;
    static doublereal resnew, trured;
    extern /* Subroutine */ int trstlp_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *);

    /* Fortran I/O blocks */
    static cilist io___30 = { 0, 6, 0, fmt_10, 0 };
    static cilist io___37 = { 0, 6, 0, fmt_50, 0 };
    static cilist io___38 = { 0, 6, 0, 0, 0 };
    static cilist io___39 = { 0, 6, 0, 0, 0 };
    static cilist io___40 = { 0, 6, 0, 0, 0 };
    static cilist io___42 = { 0, 6, 0, 0, 0 };
    static cilist io___45 = { 0, 6, 0, fmt_70, 0 };
    static cilist io___46 = { 0, 6, 0, fmt_80, 0 };
    static cilist io___51 = { 0, 6, 0, fmt_210, 0 };
    static cilist io___55 = { 0, 6, 0, 0, 0 };
    static cilist io___56 = { 0, 6, 0, 0, 0 };
    static cilist io___57 = { 0, 6, 0, 0, 0 };
    static cilist io___58 = { 0, 6, 0, 0, 0 };
    static cilist io___59 = { 0, 6, 0, 0, 0 };
    static cilist io___60 = { 0, 6, 0, 0, 0 };
    static cilist io___61 = { 0, 6, 0, 0, 0 };
    static cilist io___62 = { 0, 6, 0, 0, 0 };
    static cilist io___75 = { 0, 6, 0, fmt_410, 0 };
    static cilist io___84 = { 0, 6, 0, fmt_580, 0 };
    static cilist io___85 = { 0, 6, 0, fmt_70, 0 };
    static cilist io___86 = { 0, 6, 0, fmt_80, 0 };
    static cilist io___87 = { 0, 6, 0, fmt_590, 0 };
    static cilist io___88 = { 0, 6, 0, fmt_70, 0 };
    static cilist io___89 = { 0, 6, 0, fmt_80, 0 };



/*     Set the initial values of some parameters. The last column of SIM holds */
/*     the optimal vertex of the current simplex, and the preceding N columns */
/*     hold the displacements from the optimal vertex to the other vertices. */
/*     Further, SIMI holds the inverse of the matrix that is contained in the */
/*     first N columns of SIM. */

    /* Parameter adjustments */
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    simi_dim1 = *n;
    simi_offset = 1 + simi_dim1;
    simi -= simi_offset;
    sim_dim1 = *n;
    sim_offset = 1 + sim_dim1;
    sim -= sim_offset;
    datmat_dim1 = *mpp;
    datmat_offset = 1 + datmat_dim1;
    datmat -= datmat_offset;
    --x;
    --con;
    --vsig;
    --veta;
    --sigbar;
    --dx;
    --w;
    --iact;
    --dinfo;

    /* Function Body */
    itotal = *n * (*n * 3 + (*m << 1) + 11) + (*m << 2) + 6;
    iptem = min(*n,5);
    iptemp = iptem + 1;
    np = *n + 1;
    mp = *m + 1;
    alpha = .25;
    beta = 2.1;
    gamma = .5;
    delta = 1.1;
    rho = *rhobeg;
    parmu = 0.;
/*     Set the STATUS value of DINFO to 1.0 to start. */
/*     IF an error occurs it will get set to 0.0 */
    dinfo[1] = 1.;
/*     Fix compiler warnings */
    iflag = 1;
    parsig = 0.;
    sum = 0.;
    prerec = 0.;
    prerem = 0.;
    cmin = 0.;
    cmax = 0.;
    if (*iprint >= 2) {
	s_wsfe(&io___30);
	do_fio(&c__1, (char *)&rho, (ftnlen)sizeof(doublereal));
	e_wsfe();
    }
    nfvals = 0;
    temp = 1. / rho;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sim[i__ + np * sim_dim1] = x[i__];
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    sim[i__ + j * sim_dim1] = 0.;
/* L20: */
	    simi[i__ + j * simi_dim1] = 0.;
	}
	sim[i__ + i__ * sim_dim1] = rho;
/* L30: */
	simi[i__ + i__ * simi_dim1] = temp;
    }
    jdrop = np;
    ibrnch = 0;

/*     Make the next call of the user-supplied subroutine CALCFC. These */
/*     instructions are also used for calling CALCFC during the iterations of */
/*     the algorithm. */

L40:
    if (nfvals >= *maxfun && nfvals > 0) {
	if (*iprint >= 1) {
	    s_wsfe(&io___37);
	    e_wsfe();
	}
	dinfo[1] = 2.;
	goto L600;
    }
    ++nfvals;
    if (*iprint == 3) {
	s_wsle(&io___38);
	do_lio(&c__9, &c__1, "  SIM = ", (ftnlen)8);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_lio(&c__5, &c__1, (char *)&sim[j + np * sim_dim1], (ftnlen)
		    sizeof(doublereal));
	}
	e_wsle();
	s_wsle(&io___39);
	do_lio(&c__9, &c__1, "  DX = ", (ftnlen)7);
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&dx[i__], (ftnlen)sizeof(doublereal))
		    ;
	}
	e_wsle();
	s_wsle(&io___40);
	do_lio(&c__9, &c__1, "  BEFORE: ", (ftnlen)10);
	do_lio(&c__3, &c__1, (char *)&(*n), (ftnlen)sizeof(integer));
	do_lio(&c__3, &c__1, (char *)&(*m), (ftnlen)sizeof(integer));
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	}
	do_lio(&c__5, &c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&con[i__], (ftnlen)sizeof(doublereal)
		    );
	}
	e_wsle();
    }
    (*calcfc)(n, m, &x[1], &f, &con[1]);
    if (*iprint == 3) {
	s_wsle(&io___42);
	do_lio(&c__9, &c__1, "  AFTER: ", (ftnlen)9);
	do_lio(&c__3, &c__1, (char *)&(*n), (ftnlen)sizeof(integer));
	do_lio(&c__3, &c__1, (char *)&(*m), (ftnlen)sizeof(integer));
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	}
	do_lio(&c__5, &c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&con[i__], (ftnlen)sizeof(doublereal)
		    );
	}
	e_wsle();
    }
    resmax = 0.;
    if (*m > 0) {
	i__1 = *m;
	for (k = 1; k <= i__1; ++k) {
/* L60: */
/* Computing MAX */
	    d__1 = resmax, d__2 = -con[k];
	    resmax = max(d__1,d__2);
	}
    }
    if (nfvals == *iprint - 1 || *iprint == 3) {
	s_wsfe(&io___45);
	do_fio(&c__1, (char *)&nfvals, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&resmax, (ftnlen)sizeof(doublereal));
	i__1 = iptem;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
	if (iptem < *n) {
	    s_wsfe(&io___46);
	    i__1 = *n;
	    for (i__ = iptemp; i__ <= i__1; ++i__) {
		do_fio(&c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
	}
    }
    con[mp] = f;
    con[*mpp] = resmax;
    if (ibrnch == 1) {
	goto L440;
    }

/*     Set the recently calculated function values in a column of DATMAT. This */
/*     array has a column for each vertex of the current simplex, the entries of */
/*     each column being the values of the constraint functions (if any) */
/*     followed by the objective function and the greatest constraint violation */
/*     at the vertex. */

    i__1 = *mpp;
    for (k = 1; k <= i__1; ++k) {
/* L90: */
	datmat[k + jdrop * datmat_dim1] = con[k];
    }
    if (nfvals > np) {
	goto L130;
    }

/*     Exchange the new vertex of the initial simplex with the optimal vertex if */
/*     necessary. Then, if the initial simplex is not complete, pick its next */
/*     vertex and calculate the function values there. */

    if (jdrop <= *n) {
	if (datmat[mp + np * datmat_dim1] <= f) {
	    x[jdrop] = sim[jdrop + np * sim_dim1];
	} else {
	    sim[jdrop + np * sim_dim1] = x[jdrop];
	    i__1 = *mpp;
	    for (k = 1; k <= i__1; ++k) {
		datmat[k + jdrop * datmat_dim1] = datmat[k + np * datmat_dim1]
			;
/* L100: */
		datmat[k + np * datmat_dim1] = con[k];
	    }
	    i__1 = jdrop;
	    for (k = 1; k <= i__1; ++k) {
		sim[jdrop + k * sim_dim1] = -rho;
		temp = 0.f;
		i__2 = jdrop;
		for (i__ = k; i__ <= i__2; ++i__) {
/* L110: */
		    temp -= simi[i__ + k * simi_dim1];
		}
/* L120: */
		simi[jdrop + k * simi_dim1] = temp;
	    }
	}
    }
    if (nfvals <= *n) {
	jdrop = nfvals;
	x[jdrop] += rho;
	goto L40;
    }
L130:
    ibrnch = 1;

/*     Identify the optimal vertex of the current simplex. */

L140:
    phimin = datmat[mp + np * datmat_dim1] + parmu * datmat[*mpp + np * 
	    datmat_dim1];
    nbest = np;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	temp = datmat[mp + j * datmat_dim1] + parmu * datmat[*mpp + j * 
		datmat_dim1];
	if (temp < phimin) {
	    nbest = j;
	    phimin = temp;
	} else if (temp == phimin && parmu == 0.) {
	    if (datmat[*mpp + j * datmat_dim1] < datmat[*mpp + nbest * 
		    datmat_dim1]) {
		nbest = j;
	    }
	}
/* L150: */
    }

/*     Switch the best vertex into pole position if it is not there already, */
/*     and also update SIM, SIMI and DATMAT. */

    if (nbest <= *n) {
	i__1 = *mpp;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    temp = datmat[i__ + np * datmat_dim1];
	    datmat[i__ + np * datmat_dim1] = datmat[i__ + nbest * datmat_dim1]
		    ;
/* L160: */
	    datmat[i__ + nbest * datmat_dim1] = temp;
	}
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    temp = sim[i__ + nbest * sim_dim1];
	    sim[i__ + nbest * sim_dim1] = 0.;
	    sim[i__ + np * sim_dim1] += temp;
	    tempa = 0.;
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		sim[i__ + k * sim_dim1] -= temp;
/* L170: */
		tempa -= simi[k + i__ * simi_dim1];
	    }
/* L180: */
	    simi[nbest + i__ * simi_dim1] = tempa;
	}
    }

/*     Make an error return if SIGI is a poor approximation to the inverse of */
/*     the leading N by N submatrix of SIG. */

    error = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    temp = 0.;
	    if (i__ == j) {
		temp += -1.;
	    }
	    i__3 = *n;
	    for (k = 1; k <= i__3; ++k) {
/* L190: */
		temp += simi[i__ + k * simi_dim1] * sim[k + j * sim_dim1];
	    }
/* L200: */
/* Computing MAX */
	    d__1 = error, d__2 = abs(temp);
	    error = max(d__1,d__2);
	}
    }
    if (error > .1) {
	if (*iprint >= 1) {
	    s_wsfe(&io___51);
	    e_wsfe();
	}
	dinfo[1] = 3.;
	goto L600;
    }

/*     Calculate the coefficients of the linear approximations to the objective */
/*     and constraint functions, placing minus the objective function gradient */
/*     after the constraint gradients in the array A. The vector W is used for */
/*     working space. */

    i__2 = mp;
    for (k = 1; k <= i__2; ++k) {
	con[k] = -datmat[k + np * datmat_dim1];
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* L220: */
	    w[j] = datmat[k + j * datmat_dim1] + con[k];
	}
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    temp = 0.;
	    i__3 = *n;
	    for (j = 1; j <= i__3; ++j) {
/* L230: */
		temp += w[j] * simi[j + i__ * simi_dim1];
	    }
	    if (k == mp) {
		temp = -temp;
	    }
/* L240: */
	    a[i__ + k * a_dim1] = temp;
	}
    }

/*     Calculate the values of sigma and eta, and set IFLAG=0 if the current */
/*     simplex is not acceptable. */

    iflag = 1;
    parsig = alpha * rho;
    pareta = beta * rho;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wsig = 0.;
	weta = 0.;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing 2nd power */
	    d__1 = simi[j + i__ * simi_dim1];
	    wsig += d__1 * d__1;
/* L250: */
/* Computing 2nd power */
	    d__1 = sim[i__ + j * sim_dim1];
	    weta += d__1 * d__1;
	}
	vsig[j] = 1. / sqrt(wsig);
	veta[j] = sqrt(weta);
	if (vsig[j] < parsig || veta[j] > pareta) {
	    iflag = 0;
	}
/* L260: */
    }
    if (*iprint == 3) {
	s_wsle(&io___55);
	do_lio(&c__9, &c__1, "  SIMI = ", (ftnlen)9);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		do_lio(&c__5, &c__1, (char *)&simi[i__ + j * simi_dim1], (
			ftnlen)sizeof(doublereal));
	    }
	}
	e_wsle();
	s_wsle(&io___56);
	do_lio(&c__9, &c__1, "   SIM = ", (ftnlen)9);
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		do_lio(&c__5, &c__1, (char *)&sim[i__ + j * sim_dim1], (
			ftnlen)sizeof(doublereal));
	    }
	}
	e_wsle();
	s_wsle(&io___57);
	do_lio(&c__9, &c__1, "  VSIG = ", (ftnlen)9);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_lio(&c__5, &c__1, (char *)&vsig[j], (ftnlen)sizeof(doublereal))
		    ;
	}
	do_lio(&c__9, &c__1, " -- ", (ftnlen)4);
	do_lio(&c__5, &c__1, (char *)&parsig, (ftnlen)sizeof(doublereal));
	e_wsle();
	s_wsle(&io___58);
	do_lio(&c__9, &c__1, "  VETA = ", (ftnlen)9);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_lio(&c__5, &c__1, (char *)&veta[j], (ftnlen)sizeof(doublereal))
		    ;
	}
	do_lio(&c__9, &c__1, " -- ", (ftnlen)4);
	do_lio(&c__5, &c__1, (char *)&pareta, (ftnlen)sizeof(doublereal));
	e_wsle();
	s_wsle(&io___59);
	do_lio(&c__9, &c__1, "  IBRNCH, IFLAG = ", (ftnlen)18);
	do_lio(&c__3, &c__1, (char *)&ibrnch, (ftnlen)sizeof(integer));
	do_lio(&c__3, &c__1, (char *)&iflag, (ftnlen)sizeof(integer));
	e_wsle();
	s_wsle(&io___60);
	do_lio(&c__9, &c__1, "  A = ", (ftnlen)6);
	i__1 = mp;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *n;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		do_lio(&c__5, &c__1, (char *)&a[i__ + j * a_dim1], (ftnlen)
			sizeof(doublereal));
	    }
	}
	e_wsle();
    }

/*     If a new vertex is needed to improve acceptability, then decide which */
/*     vertex to drop from the simplex. */

    if (ibrnch == 1 || iflag == 1) {
	goto L370;
    }
    jdrop = 0;
    temp = pareta;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	if (veta[j] > temp) {
	    jdrop = j;
	    temp = veta[j];
	}
/* L270: */
    }
    if (jdrop == 0) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    if (vsig[j] < temp) {
		jdrop = j;
		temp = vsig[j];
	    }
/* L280: */
	}
    }

/*     Calculate the step to the new vertex and its sign. */

    temp = gamma * rho * vsig[jdrop];
    if (*iprint == 3) {
	s_wsle(&io___61);
	do_lio(&c__9, &c__1, "  SIMI =", (ftnlen)8);
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&simi[jdrop + i__ * simi_dim1], (
		    ftnlen)sizeof(doublereal));
	}
	e_wsle();
    }
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
/* L290: */
	dx[i__] = temp * simi[jdrop + i__ * simi_dim1];
    }
    if (*iprint == 3) {
	s_wsle(&io___62);
	do_lio(&c__9, &c__1, "  DX =", (ftnlen)6);
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_lio(&c__5, &c__1, (char *)&dx[i__], (ftnlen)sizeof(doublereal))
		    ;
	}
	e_wsle();
    }
    cvmaxp = 0.;
    cvmaxm = 0.;
    i__2 = mp;
    for (k = 1; k <= i__2; ++k) {
	sum = 0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L300: */
	    sum += a[i__ + k * a_dim1] * dx[i__];
	}
	if (k < mp) {
	    temp = datmat[k + np * datmat_dim1];
/* Computing MAX */
	    d__1 = cvmaxp, d__2 = -sum - temp;
	    cvmaxp = max(d__1,d__2);
/* Computing MAX */
	    d__1 = cvmaxm, d__2 = sum - temp;
	    cvmaxm = max(d__1,d__2);
	}
/* L310: */
    }
    dxsign = 1.;
    if (parmu * (cvmaxp - cvmaxm) > sum + sum) {
	dxsign = -1.;
    }

/*     Update the elements of SIM and SIMI, and set the next X. */

    temp = 0.;
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
	dx[i__] = dxsign * dx[i__];
	sim[i__ + jdrop * sim_dim1] = dx[i__];
/* L320: */
	temp += simi[jdrop + i__ * simi_dim1] * dx[i__];
    }
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
/* L330: */
	simi[jdrop + i__ * simi_dim1] /= temp;
    }
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	if (j != jdrop) {
	    temp = 0.;
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L340: */
		temp += simi[j + i__ * simi_dim1] * dx[i__];
	    }
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L350: */
		simi[j + i__ * simi_dim1] -= temp * simi[jdrop + i__ * 
			simi_dim1];
	    }
	}
/* L360: */
	x[j] = sim[j + np * sim_dim1] + dx[j];
    }
    goto L40;

/*     Calculate DX=x(*)-x(0). Branch if the length of DX is less than 0.5*RHO. */

L370:
    iz = 1;
    izdota = iz + *n * *n;
    ivmc = izdota + *n;
    isdirn = ivmc + mp;
    idxnew = isdirn + *n;
    ivmd = idxnew + *n;
    trstlp_(n, m, &a[a_offset], &con[1], &rho, &dx[1], &ifull, &iact[1], &w[
	    iz], &w[izdota], &w[ivmc], &w[isdirn], &w[idxnew], &w[ivmd], 
	    iprint);
    if (ifull == 0) {
	temp = 0.;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* L380: */
/* Computing 2nd power */
	    d__1 = dx[i__];
	    temp += d__1 * d__1;
	}
	if (temp < rho * .25 * rho) {
	    ibrnch = 1;
	    goto L550;
	}
    }

/*     Predict the change to F and the new maximum constraint violation if the */
/*     variables are altered from x(0) to x(0)+DX. */

    resnew = 0.;
    con[mp] = 0.;
    i__2 = mp;
    for (k = 1; k <= i__2; ++k) {
	sum = con[k];
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L390: */
	    sum -= a[i__ + k * a_dim1] * dx[i__];
	}
	if (k < mp) {
	    resnew = max(resnew,sum);
	}
/* L400: */
    }

/*     Increase PARMU if necessary and branch back if this change alters the */
/*     optimal vertex. Otherwise PREREM and PREREC will be set to the predicted */
/*     reductions in the merit function and the maximum constraint violation */
/*     respectively. */

    barmu = 0.;
    prerec = datmat[*mpp + np * datmat_dim1] - resnew;
    if (prerec > 0.) {
	barmu = sum / prerec;
    }
    if (parmu < barmu * 1.5) {
	parmu = barmu * 2.;
	if (*iprint >= 2) {
	    s_wsfe(&io___75);
	    do_fio(&c__1, (char *)&parmu, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
	phi = datmat[mp + np * datmat_dim1] + parmu * datmat[*mpp + np * 
		datmat_dim1];
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    temp = datmat[mp + j * datmat_dim1] + parmu * datmat[*mpp + j * 
		    datmat_dim1];
	    if (temp < phi) {
		goto L140;
	    }
	    if (temp == phi && parmu == 0.f) {
		if (datmat[*mpp + j * datmat_dim1] < datmat[*mpp + np * 
			datmat_dim1]) {
		    goto L140;
		}
	    }
/* L420: */
	}
    }
    prerem = parmu * prerec - sum;

/*     Calculate the constraint and objective functions at x(*). Then find the */
/*     actual reduction in the merit function. */

    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
/* L430: */
	x[i__] = sim[i__ + np * sim_dim1] + dx[i__];
    }
    ibrnch = 1;
    goto L40;
L440:
    vmold = datmat[mp + np * datmat_dim1] + parmu * datmat[*mpp + np * 
	    datmat_dim1];
    vmnew = f + parmu * resmax;
    trured = vmold - vmnew;
    if (parmu == 0. && f == datmat[mp + np * datmat_dim1]) {
	prerem = prerec;
	trured = datmat[*mpp + np * datmat_dim1] - resmax;
    }

/*     Begin the operations that decide whether x(*) should replace one of the */
/*     vertices of the current simplex, the change being mandatory if TRURED is */
/*     positive. Firstly, JDROP is set to the index of the vertex that is to be */
/*     replaced. */

    ratio = 0.;
    if (trured <= 0.f) {
	ratio = 1.f;
    }
    jdrop = 0;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	temp = 0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* L450: */
	    temp += simi[j + i__ * simi_dim1] * dx[i__];
	}
	temp = abs(temp);
	if (temp > ratio) {
	    jdrop = j;
	    ratio = temp;
	}
/* L460: */
	sigbar[j] = temp * vsig[j];
    }

/*     Calculate the value of ell. */

    edgmax = delta * rho;
    l = 0;
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	if (sigbar[j] >= parsig || sigbar[j] >= vsig[j]) {
	    temp = veta[j];
	    if (trured > 0.) {
		temp = 0.;
		i__1 = *n;
		for (i__ = 1; i__ <= i__1; ++i__) {
/* L470: */
/* Computing 2nd power */
		    d__1 = dx[i__] - sim[i__ + j * sim_dim1];
		    temp += d__1 * d__1;
		}
		temp = sqrt(temp);
	    }
	    if (temp > edgmax) {
		l = j;
		edgmax = temp;
	    }
	}
/* L480: */
    }
    if (l > 0) {
	jdrop = l;
    }
    if (jdrop == 0) {
	goto L550;
    }

/*     Revise the simplex by updating the elements of SIM, SIMI and DATMAT. */

    temp = 0.;
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
	sim[i__ + jdrop * sim_dim1] = dx[i__];
/* L490: */
	temp += simi[jdrop + i__ * simi_dim1] * dx[i__];
    }
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
/* L500: */
	simi[jdrop + i__ * simi_dim1] /= temp;
    }
    i__2 = *n;
    for (j = 1; j <= i__2; ++j) {
	if (j != jdrop) {
	    temp = 0.;
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L510: */
		temp += simi[j + i__ * simi_dim1] * dx[i__];
	    }
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
/* L520: */
		simi[j + i__ * simi_dim1] -= temp * simi[jdrop + i__ * 
			simi_dim1];
	    }
	}
/* L530: */
    }
    i__2 = *mpp;
    for (k = 1; k <= i__2; ++k) {
/* L540: */
	datmat[k + jdrop * datmat_dim1] = con[k];
    }

/*     Branch back for further iterations with the current RHO. */

    if (trured > 0. && trured >= prerem * .1) {
	goto L140;
    }
L550:
    if (iflag == 0) {
	ibrnch = 0;
	goto L140;
    }

/*     Otherwise reduce RHO if it is not at its least value and reset PARMU. */

    if (rho > *rhoend) {
	rho *= .5;
	if (rho <= *rhoend * 1.5) {
	    rho = *rhoend;
	}
	if (parmu > 0.) {
	    denom = 0.;
	    i__2 = mp;
	    for (k = 1; k <= i__2; ++k) {
		cmin = datmat[k + np * datmat_dim1];
		cmax = cmin;
		i__1 = *n;
		for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MIN */
		    d__1 = cmin, d__2 = datmat[k + i__ * datmat_dim1];
		    cmin = min(d__1,d__2);
/* L560: */
/* Computing MAX */
		    d__1 = cmax, d__2 = datmat[k + i__ * datmat_dim1];
		    cmax = max(d__1,d__2);
		}
		if (k <= *m && cmin < cmax * .5) {
		    temp = max(cmax,0.) - cmin;
		    if (denom <= 0.) {
			denom = temp;
		    } else {
			denom = min(denom,temp);
		    }
		}
/* L570: */
	    }
	    if (denom == 0.) {
		parmu = 0.;
	    } else if (cmax - cmin < parmu * denom) {
		parmu = (cmax - cmin) / denom;
	    }
	}
	if (*iprint >= 2) {
	    s_wsfe(&io___84);
	    do_fio(&c__1, (char *)&rho, (ftnlen)sizeof(doublereal));
	    do_fio(&c__1, (char *)&parmu, (ftnlen)sizeof(doublereal));
	    e_wsfe();
	}
	if (*iprint == 2) {
	    s_wsfe(&io___85);
	    do_fio(&c__1, (char *)&nfvals, (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&datmat[mp + np * datmat_dim1], (ftnlen)
		    sizeof(doublereal));
	    do_fio(&c__1, (char *)&datmat[*mpp + np * datmat_dim1], (ftnlen)
		    sizeof(doublereal));
	    i__2 = iptem;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		do_fio(&c__1, (char *)&sim[i__ + np * sim_dim1], (ftnlen)
			sizeof(doublereal));
	    }
	    e_wsfe();
	    if (iptem < *n) {
		s_wsfe(&io___86);
		i__2 = *n;
		for (i__ = iptemp; i__ <= i__2; ++i__) {
		    do_fio(&c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal))
			    ;
		}
		e_wsfe();
	    }
	}
	goto L140;
    }

/*     Return the best calculated values of the variables. */

    if (*iprint >= 1) {
	s_wsfe(&io___87);
	e_wsfe();
    }
    if (ifull == 1) {
	goto L620;
    }
L600:
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
/* L610: */
	x[i__] = sim[i__ + np * sim_dim1];
    }
    f = datmat[mp + np * datmat_dim1];
    resmax = datmat[*mpp + np * datmat_dim1];
L620:
    if (*iprint >= 1) {
	s_wsfe(&io___88);
	do_fio(&c__1, (char *)&nfvals, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&f, (ftnlen)sizeof(doublereal));
	do_fio(&c__1, (char *)&resmax, (ftnlen)sizeof(doublereal));
	i__2 = iptem;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
	if (iptem < *n) {
	    s_wsfe(&io___89);
	    i__2 = *n;
	    for (i__ = iptemp; i__ <= i__2; ++i__) {
		do_fio(&c__1, (char *)&x[i__], (ftnlen)sizeof(doublereal));
	    }
	    e_wsfe();
	}
    }
    *maxfun = nfvals;
    dinfo[2] = (doublereal) nfvals;
    dinfo[3] = f;
    dinfo[4] = resmax;
    return 0;
} /* cobylb_ */

