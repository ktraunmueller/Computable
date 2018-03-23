/* swilk.f -- translated by f2c (version 20100827).
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

static integer c__6 = 6;
static integer c__1 = 1;
static integer c__2 = 2;
static integer c__4 = 4;
static integer c__3 = 3;

/* Subroutine */ int swilk_(logical *init, real *x, integer *n, integer *n1, 
	integer *n2, real *a, real *w, real *pw, integer *ifault)
{
    /* Initialized data */

    static real c1[6] = { 0.f,.221157f,-.147981f,-2.07119f,4.434685f,
	    -2.706056f };
    static real g[2] = { -2.273f,.459f };
    static real z90 = 1.2816f;
    static real z95 = 1.6449f;
    static real z99 = 2.3263f;
    static real zm = 1.7509f;
    static real zss = .56268f;
    static real bf1 = .8378f;
    static real xx90 = .556f;
    static real xx95 = .622f;
    static real zero = 0.f;
    static real c2[6] = { 0.f,.042981f,-.293762f,-1.752461f,5.682633f,
	    -3.582633f };
    static real one = 1.f;
    static real two = 2.f;
    static real three = 3.f;
    static real sqrth = .70711f;
    static real qtr = .25f;
    static real th = .375f;
    static real small = 1e-19f;
    static real pi6 = 1.909859f;
    static real stqr = 1.047198f;
    static logical upper = TRUE_;
    static real c3[4] = { .544f,-.39978f,.025054f,-6.714e-4f };
    static real c4[4] = { 1.3822f,-.77857f,.062767f,-.0020322f };
    static real c5[4] = { -1.5861f,-.31082f,-.083751f,.0038915f };
    static real c6[3] = { -.4803f,-.082676f,.0030302f };
    static real c7[2] = { .164f,.533f };
    static real c8[2] = { .1736f,.315f };
    static real c9[2] = { .256f,-.00635f };

    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2, r__3, r__4;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double sqrt(doublereal);
    integer i_sign(integer *, integer *);
    double asin(doublereal), log(doublereal), exp(doublereal), pow_dd(
	    doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j;
    static real m, s, y, a1, a2;
    static integer i1;
    static real w1, bf, an, ld, sa, xi, sx, xx;
    static integer nn2;
    static real fac, asa, an25, ssa, z90f, sax, zfm, z95f, zsd, z99f, rsn, 
	    ssx, xsx, zbar;
    extern doublereal ppnd_(real *, integer *), poly_(real *, integer *, real 
	    *);
    static real summ2, gamma, delta, range;
    static integer ncens;
    static real ssumm2;
    extern doublereal alnorm_(doublereal *, logical *);
    static real ssassx;


/*        ALGORITHM AS R94 APPL. STATIST. (1995) VOL.44, NO.4 */

/*        Calculates the Shapiro-Wilk W test and its significance level */


/*        Auxiliary routines */



    /* Parameter adjustments */
    --a;
    --x;

    /* Function Body */

    *pw = one;
    if (*w >= zero) {
	*w = one;
    }
    an = (real) (*n);
    *ifault = 3;
    nn2 = *n / 2;
    if (*n2 < nn2) {
	return 0;
    }
    *ifault = 1;
    if (*n < 3) {
	return 0;
    }

/*        If INIT is false, calculates coefficients for the test */

    if (! (*init)) {
	if (*n == 3) {
	    a[1] = sqrth;
	} else {
	    an25 = an + qtr;
	    summ2 = zero;
	    i__1 = *n2;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		r__1 = ((real) i__ - th) / an25;
		a[i__] = ppnd_(&r__1, ifault);
/* Computing 2nd power */
		r__1 = a[i__];
		summ2 += r__1 * r__1;
/* L30: */
	    }
	    summ2 *= two;
	    ssumm2 = sqrt(summ2);
	    rsn = one / sqrt(an);
	    a1 = poly_(c1, &c__6, &rsn) - a[1] / ssumm2;

/*        Normalize coefficients */

	    if (*n > 5) {
		i1 = 3;
		a2 = -a[2] / ssumm2 + poly_(c2, &c__6, &rsn);
/* Computing 2nd power */
		r__1 = a[1];
/* Computing 2nd power */
		r__2 = a[2];
/* Computing 2nd power */
		r__3 = a1;
/* Computing 2nd power */
		r__4 = a2;
		fac = sqrt((summ2 - two * (r__1 * r__1) - two * (r__2 * r__2))
			 / (one - two * (r__3 * r__3) - two * (r__4 * r__4)));
		a[1] = a1;
		a[2] = a2;
	    } else {
		i1 = 2;
/* Computing 2nd power */
		r__1 = a[1];
/* Computing 2nd power */
		r__2 = a1;
		fac = sqrt((summ2 - two * (r__1 * r__1)) / (one - two * (r__2 
			* r__2)));
		a[1] = a1;
	    }
	    i__1 = nn2;
	    for (i__ = i1; i__ <= i__1; ++i__) {
		a[i__] = -a[i__] / fac;
/* L40: */
	    }
	}
	*init = TRUE_;
    }
    if (*n1 < 3) {
	return 0;
    }
    ncens = *n - *n1;
    *ifault = 4;
    if (ncens < 0 || ncens > 0 && *n < 20) {
	return 0;
    }
    *ifault = 5;
    delta = (real) ncens / an;
    if (delta > .8f) {
	return 0;
    }

/*        If W input as negative, calculate significance level of -W */

    if (*w < zero) {
	w1 = one + *w;
	*ifault = 0;
	goto L70;
    }

/*        Check for zero range */

    *ifault = 6;
    range = x[*n1] - x[1];
    if (range < small) {
	return 0;
    }

/*        Check for correct sort order on range - scaled X */

    *ifault = 7;
    xx = x[1] / range;
    sx = xx;
    sa = -a[1];
    j = *n - 1;
    i__1 = *n1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	xi = x[i__] / range;
/* CCCC   IF (XX-XI .GT. SMALL) PRINT *,' ANYTHING' */
	sx += xi;
	if (i__ != j) {
	    i__2 = i__ - j;
	    sa += i_sign(&c__1, &i__2) * a[min(i__,j)];
	}
	xx = xi;
	--j;
/* L50: */
    }
    *ifault = 0;
    if (*n > 5000) {
	*ifault = 2;
    }

/*        Calculate W statistic as squared correlation */
/*        between data and coefficients */

    sa /= *n1;
    sx /= *n1;
    ssa = zero;
    ssx = zero;
    sax = zero;
    j = *n;
    i__1 = *n1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (i__ != j) {
	    i__2 = i__ - j;
	    asa = i_sign(&c__1, &i__2) * a[min(i__,j)] - sa;
	} else {
	    asa = -sa;
	}
	xsx = x[i__] / range - sx;
	ssa += asa * asa;
	ssx += xsx * xsx;
	sax += asa * xsx;
	--j;
/* L60: */
    }

/*        W1 equals (1-W) claculated to avoid excessive rounding error */
/*        for W very near 1 (a potential problem in very large samples) */

    ssassx = sqrt(ssa * ssx);
    w1 = (ssassx - sax) * (ssassx + sax) / (ssa * ssx);
L70:
    *w = one - w1;

/*        Calculate significance level for W (exact for N=3) */

    if (*n == 3) {
	*pw = pi6 * (asin(sqrt(*w)) - stqr);
	return 0;
    }
    y = log(w1);
    xx = log(an);
    m = zero;
    s = one;
    if (*n <= 11) {
	gamma = poly_(g, &c__2, &an);
	if (y >= gamma) {
	    *pw = small;
	    return 0;
	}
	y = -log(gamma - y);
	m = poly_(c3, &c__4, &an);
	s = exp(poly_(c4, &c__4, &an));
    } else {
	m = poly_(c5, &c__4, &xx);
	s = exp(poly_(c6, &c__3, &xx));
    }
    if (ncens > 0) {

/*        Censoring by proportion NCENS/N.  Calculate mean and sd */
/*        of normal equivalent deviate of W. */

	ld = -log(delta);
	bf = one + xx * bf1;
	d__2 = (doublereal) xx90;
	d__3 = (doublereal) xx;
	r__1 = pow_dd(&d__2, &d__3);
	d__1 = (doublereal) poly_(c7, &c__2, &r__1);
	d__4 = (doublereal) ld;
	z90f = z90 + bf * pow_dd(&d__1, &d__4);
	d__2 = (doublereal) xx95;
	d__3 = (doublereal) xx;
	r__1 = pow_dd(&d__2, &d__3);
	d__1 = (doublereal) poly_(c8, &c__2, &r__1);
	d__4 = (doublereal) ld;
	z95f = z95 + bf * pow_dd(&d__1, &d__4);
	d__1 = (doublereal) poly_(c9, &c__2, &xx);
	d__2 = (doublereal) ld;
	z99f = z99 + bf * pow_dd(&d__1, &d__2);

/*        Regress Z90F,...,Z99F on normal deviates Z90,...,Z99 to get */
/*        pseudo-mean and pseudo-sd of z as the slope and intercept */

	zfm = (z90f + z95f + z99f) / three;
	zsd = (z90 * (z90f - zfm) + z95 * (z95f - zfm) + z99 * (z99f - zfm)) /
		 zss;
	zbar = zfm - zsd * zm;
	m += zbar * s;
	s *= zsd;
    }
    d__1 = (doublereal) ((y - m) / s);
    *pw = (real) alnorm_(&d__1, &upper);

    return 0;
} /* swilk_ */

doublereal alnorm_(doublereal *x, logical *upper)
{
    /* Initialized data */

    static doublereal ltone = 7.;
    static doublereal utzero = 38.;
    static doublereal zero = 0.;
    static doublereal half = .5;
    static doublereal one = 1.;
    static doublereal con = 1.28;
    static doublereal a1 = .398942280444;
    static doublereal a2 = .399903438504;
    static doublereal a3 = 5.75885480458;
    static doublereal a4 = 29.8213557808;
    static doublereal a5 = 2.62433121679;
    static doublereal a6 = 48.6959930692;
    static doublereal a7 = 5.92885724438;
    static doublereal b1 = .398942280385;
    static doublereal b2 = 3.8052e-8;
    static doublereal b3 = 1.00000615302;
    static doublereal b4 = 3.98064794e-4;
    static doublereal b5 = 1.98615381364;
    static doublereal b6 = .151679116635;
    static doublereal b7 = 5.29330324926;
    static doublereal b8 = 4.8385912808;
    static doublereal b9 = 15.1508972451;
    static doublereal b10 = .742380924027;
    static doublereal b11 = 30.789933034;
    static doublereal b12 = 3.99019417011;

    /* System generated locals */
    doublereal ret_val, d__1;

    /* Builtin functions */
    double exp(doublereal);

    /* Local variables */
    static doublereal y, z__;
    static logical up;


/*       EVALUATES THE TAIL AREA OF THE STANDARDIZED NORMAL CURVE FROM */
/*       X TO INFINITY IF UPPER IS .TRUE. OR FROM MINUS INFINITY TO X */
/*       IF UPPER IS .FALSE. */

/*  NOTE NOVEMBER 2001: MODIFY UTZERO.  ALTHOUGH NOT NECESSARY */
/*  WHEN USING ALNORM FOR SIMPLY COMPUTING PERCENT POINTS, */
/*  EXTENDING RANGE IS HELPFUL FOR USE WITH FUNCTIONS THAT */
/*  USE ALNORM IN INTERMEDIATE COMPUTATIONS. */


/*       LTONE AND UTZERO MUST BE SET TO SUIT THE PARTICULAR COMPUTER */

/* CCCC DATA LTONE, UTZERO /7.0D0, 18.66D0/ */


    up = *upper;
    z__ = *x;
    if (z__ >= zero) {
	goto L10;
    }
    up = ! up;
    z__ = -z__;
L10:
    if (z__ <= ltone || up && z__ <= utzero) {
	goto L20;
    }
    ret_val = zero;
    goto L40;
L20:
    y = half * z__ * z__;
    if (z__ > con) {
	goto L30;
    }

    ret_val = half - z__ * (a1 - a2 * y / (y + a3 - a4 / (y + a5 + a6 / (y + 
	    a7))));
    goto L40;

L30:
    d__1 = -y;
    ret_val = b1 * exp(d__1) / (z__ - b2 + b3 / (z__ + b4 + b5 / (z__ - b6 + 
	    b7 / (z__ + b8 - b9 / (z__ + b10 + b11 / (z__ + b12))))));

L40:
    if (! up) {
	ret_val = one - ret_val;
    }
    return ret_val;
} /* alnorm_ */

doublereal ppnd_(real *p, integer *ifault)
{
    /* Initialized data */

    static real a0 = 2.50662823884f;
    static real a1 = -18.61500062529f;
    static real a2 = 41.39119773534f;
    static real a3 = -25.44106049637f;
    static real b1 = -8.4735109309f;
    static real b2 = 23.08336743743f;
    static real b3 = -21.06224101826f;
    static real b4 = 3.13082909833f;
    static real c0 = -2.78718931138f;
    static real c1 = -2.29796479134f;
    static real c2 = 4.85014127135f;
    static real c3 = 2.32121276858f;
    static real d1 = 3.54388924762f;
    static real d2 = 1.63706781897f;
    static real zero = 0.f;
    static real half = .5f;
    static real one = 1.f;
    static real split = .42f;

    /* System generated locals */
    real ret_val;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);

    /* Local variables */
    static real q, r__;


/*  ALGORITHM AS 111  APPL. STATIST. (1977), VOL.26, NO.1 */

/*  PRODUCES NORMAL DEVIATE CORRESPONDING TO LOWER TAIL AREA OF P */
/*  REAL VERSION FOR EPS = 2 **(-31) */
/*  THE HASH SUMS ARE THE SUMS OF THE MODULI OF THE COEFFICIENTS */
/*  THEY HAVE NO INHERENT MEANINGS BUT ARE INCLUDED FOR USE IN */
/*  CHECKING TRANSCRIPTIONS */
/*  STANDARD FUNCTIONS ABS, ALOG AND SQRT ARE USED */

/*  NOTE: WE COULD USE DATAPLOT NORPPF, BUT VARIOUS APPLIED */
/*        STATISTICS ALGORITHMS USE THIS.  SO WE PROVIDE IT TO */
/*        MAKE USE OF APPLIED STATISTICS ALGORITHMS EASIER. */


    *ifault = 0;
    q = *p - half;
    if (dabs(q) > split) {
	goto L1;
    }
    r__ = q * q;
    ret_val = q * (((a3 * r__ + a2) * r__ + a1) * r__ + a0) / ((((b4 * r__ + 
	    b3) * r__ + b2) * r__ + b1) * r__ + one);
    return ret_val;
L1:
    r__ = *p;
    if (q > zero) {
	r__ = one - *p;
    }
    if (r__ <= zero) {
	goto L2;
    }
    r__ = sqrt(-log(r__));
    ret_val = (((c3 * r__ + c2) * r__ + c1) * r__ + c0) / ((d2 * r__ + d1) * 
	    r__ + one);
    if (q < zero) {
	ret_val = -ret_val;
    }
    return ret_val;
L2:
    *ifault = 1;
    ret_val = zero;
    return ret_val;
} /* ppnd_ */

doublereal poly_(real *c__, integer *nord, real *x)
{
    /* System generated locals */
    integer i__1;
    real ret_val;

    /* Local variables */
    static integer i__, j;
    static real p;
    static integer n2;



/*        ALGORITHM AS 181.2   APPL. STATIST.  (1982) VOL. 31, NO. 2 */

/*        CALCULATES THE ALGEBRAIC POLYNOMIAL OF ORDER NORED-1 WITH */
/*        ARRAY OF COEFFICIENTS C.  ZERO ORDER COEFFICIENT IS C(1) */

    /* Parameter adjustments */
    --c__;

    /* Function Body */
    ret_val = c__[1];
    if (*nord == 1) {
	return ret_val;
    }
    p = *x * c__[*nord];
    if (*nord == 2) {
	goto L20;
    }
    n2 = *nord - 2;
    j = n2 + 1;
    i__1 = n2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p = (p + c__[j]) * *x;
	--j;
/* L10: */
    }
L20:
    ret_val += p;
    return ret_val;
} /* poly_ */

