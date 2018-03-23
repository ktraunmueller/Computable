/* spearman.f -- translated by f2c (version 20100827).
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

static logical c_true = TRUE_;

doublereal prho_(integer *n, integer *is, integer *ifault)
{
    /* Initialized data */

    static doublereal zero = 0.;
    static doublereal c6 = .3932;
    static doublereal c7 = .0879;
    static doublereal c8 = .0151;
    static doublereal c9 = .0072;
    static doublereal c10 = .0831;
    static doublereal c11 = .0131;
    static doublereal c12 = 4.6e-4;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal six = 6.;
    static doublereal c1 = .2274;
    static doublereal c2 = .2531;
    static doublereal c3 = .1745;
    static doublereal c4 = .0758;
    static doublereal c5 = .1033;

    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal ret_val;

    /* Builtin functions */
    double sqrt(doublereal), exp(doublereal);

    /* Local variables */
    static doublereal b;
    static integer i__, l[6], m;
    static doublereal u, x, y;
    static integer n1, js, nn, mt, ifr, ise, nfac;
    extern doublereal alnorm_(doublereal *, logical *);


/*        Algorithm AS 89   Appl. Statist. (1975) Vol.24, No. 3, P377. */

/*        To evaluate the probability of obtaining a value greater than or */
/*        equal to is, where is=(n**3-n)*(1-r)/6, r=Spearman's rho and n */
/*        must be greater than 1 */

/*     Auxiliary function required: ALNORM = algorithm AS66 */


/*        Test admissibility of arguments and initialize */

    ret_val = one;
    *ifault = 1;
    if (*n <= 1) {
	return ret_val;
    }
    *ifault = 0;
    if (*is <= 0) {
	return ret_val;
    }
    ret_val = zero;
    if (*is > *n * (*n * *n - 1) / 3) {
	return ret_val;
    }
    js = *is;
    if (js != js / 2 << 1) {
	++js;
    }
    if (*n > 6) {
	goto L6;
    }

/*        Exact evaluation of probability */

    nfac = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	nfac *= i__;
	l[i__ - 1] = i__;
/* L1: */
    }
    ret_val = one / (doublereal) nfac;
    if (js == *n * (*n * *n - 1) / 3) {
	return ret_val;
    }
    ifr = 0;
    i__1 = nfac;
    for (m = 1; m <= i__1; ++m) {
	ise = 0;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing 2nd power */
	    i__3 = i__ - l[i__ - 1];
	    ise += i__3 * i__3;
/* L2: */
	}
	if (js <= ise) {
	    ++ifr;
	}
	n1 = *n;
L3:
	mt = l[0];
	nn = n1 - 1;
	i__2 = nn;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    l[i__ - 1] = l[i__];
/* L4: */
	}
	l[n1 - 1] = mt;
	if (l[n1 - 1] != n1 || n1 == 2) {
	    goto L5;
	}
	--n1;
	if (m != nfac) {
	    goto L3;
	}
L5:
	;
    }
    ret_val = (doublereal) ifr / (doublereal) nfac;
    return ret_val;

/*        Evaluation by Edgeworth series expansion */

L6:
    b = one / (doublereal) (*n);
    x = (six * ((doublereal) js - one) * b / (one / (b * b) - one) - one) * 
	    sqrt(one / b - one);
    y = x * x;
    u = x * b * (c1 + b * (c2 + c3 * b) + y * (-c4 + b * (c5 + c6 * b) - y * 
	    b * (c7 + c8 * b - y * (c9 - c10 * b + y * b * (c11 - c12 * y)))))
	    ;

/*      Call to algorithm AS 66 */

    ret_val = u / exp(y / two) + alnorm_(&x, &c_true);
    if (ret_val < zero) {
	ret_val = zero;
    }
    if (ret_val > one) {
	ret_val = one;
    }
    return ret_val;
} /* prho_ */

