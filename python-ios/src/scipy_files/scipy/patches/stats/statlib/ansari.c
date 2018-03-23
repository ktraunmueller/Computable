/* ansari.f -- translated by f2c (version 20100827).
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

/* Routine AS 93 returns frequencies.   The following short routine calculates */
/* the distribution function from these frequencies (overwriting them). */
/* The calling arguments are as for AS 93.   The distribution function is */
/* returned in array A1.   The first element in A1 is F(ASTART).   N.B. ASTART */
/* is a real variable. */

/* Subroutine */ int wprob_(integer *test, integer *other, real *astart, real 
	*a1, integer *l1, real *a2, real *a3, integer *ifault)
{
    /* Initialized data */

    static real zero = 0.f;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    static real sum;
    static integer nrows;
    extern /* Subroutine */ int gscale_(integer *, integer *, real *, real *, 
	    integer *, real *, real *, integer *);


/*     Local variables */

    /* Parameter adjustments */
    --a3;
    --a2;
    --a1;

    /* Function Body */

    gscale_(test, other, astart, &a1[1], l1, &a2[1], &a3[1], ifault);
    if (*ifault != 0) {
	return 0;
    }

/*     Scale column of F */

    nrows = *test * *other / 2 + 1;
    sum = zero;
    i__1 = nrows;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sum += a1[i__];
	a1[i__] = sum;
/* L10: */
    }
    i__1 = nrows;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L20: */
	a1[i__] /= sum;
    }

    return 0;
} /* wprob_ */

/* ---------------------------------------------------------------------- */
/* Subroutine */ int gscale_(integer *test, integer *other, real *astart, 
	real *a1, integer *l1, real *a2, real *a3, integer *ifault)
{
    /* Initialized data */

    static real one = 1.f;

    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, m, n;
    static real ai;
    static integer nc, ks, mm1, ln1, nm1, nm2, ln3, ln2, n2b1, n2b2, ndo, 
	    lres, mnow;
    static logical symm;
    static integer l1out, l2out;
    extern /* Subroutine */ int imply_(real *, integer *, integer *, real *, 
	    integer *, integer *, integer *), start1_(integer *, real *, 
	    integer *, integer *), start2_(integer *, real *, integer *, 
	    integer *), frqadd_(real *, integer *, integer *, integer *, real 
	    *, integer *, integer *);


/*       ALGORITHM AS 93 APPL. STATIST. (1976) VOL.25, NO.1 */

/*       FROM THE SIZES OF TWO SAMPLES THE DISTRIBUTION OF THE */
/*       ANSARI-BRADLEY TEST FOR SCALE IS GENERATED IN ARRAY A1. */

    /* Parameter adjustments */
    --a3;
    --a2;
    --a1;

    /* Function Body */

/*       TYPE CONVERSION (EFFECT DEPENDS ON TYPE STATEMENT ABOVE). */


/*       CHECK PROBLEM SIZE AND DEFINE BASE VALUE OF THE DISTRIBUTION. */

    m = min(*test,*other);
    *ifault = 2;
    if (m < 0) {
	return 0;
    }
    i__1 = (*test + 1) / 2;
    i__2 = *test / 2 + 1;
    *astart = (real) i__1 * (real) i__2;
    n = max(*test,*other);

/*       CHECK SIZE OF RESULT ARRAY. */

    *ifault = 1;
    lres = m * n / 2 + 1;
    if (*l1 < lres) {
	return 0;
    }
    symm = (m + n) % 2 == 0;

/*       TREAT SMALL SAMPLES SEPARATELY. */

    mm1 = m - 1;
    if (m > 2) {
	goto L5;
    }

/*       START-UP PROCEDURES ONLY NEEDED. */

    if (mm1 < 0) {
	goto L1;
    } else if (mm1 == 0) {
	goto L2;
    } else {
	goto L3;
    }

/*       ONE SAMPLE ONLY. */

L1:
    a1[1] = one;
    goto L15;

/*       SMALLER SAMPLE SIZE = 1. */

L2:
    start1_(&n, &a1[1], l1, &ln1);
    goto L4;

/*       SMALLER SAMPLE SIZE = 2. */

L3:
    start2_(&n, &a1[1], l1, &ln1);

/*     RETURN IF A1 IS NOT IN REVERSE ORDER. */

L4:
    if (symm || *other > *test) {
	goto L15;
    }
    goto L13;

/*       FULL GENERATOR NEEDED */
/*       SET UP INITIAL CONDITIONS (DEPENDS ON MOD(N, 2)). */

L5:
    nm1 = n - 1;
    nm2 = n - 2;
    mnow = 3;
    nc = 3;
    if (n % 2 == 1) {
	goto L6;
    }
/*       SET UP FOR EVEN N. */

    n2b1 = 3;
    n2b2 = 2;
    start2_(&n, &a1[1], l1, &ln1);
    start2_(&nm2, &a3[1], l1, &ln3);
    start1_(&nm1, &a2[1], l1, &ln2);
    goto L8;

/*       SET UP FOR ODD N. */

L6:
    n2b1 = 2;
    n2b2 = 3;
    start1_(&n, &a1[1], l1, &ln1);
    start2_(&nm1, &a2[1], l1, &ln2);

/*       INCREASE ORDER OF DISTRIBUTION IN A1 BY 2 */
/*       (USING A2 AND IMPLYING A3). */

L7:
    frqadd_(&a1[1], &ln1, &l1out, l1, &a2[1], &ln2, &n2b1);
    ln1 += n;
    imply_(&a1[1], &l1out, &ln1, &a3[1], &ln3, l1, &nc);
    ++nc;
    if (mnow == m) {
	goto L9;
    }
    ++mnow;

/*       INCREASE ORDER OF DISTRIBUTION IN A2 BY 2 (USING A3). */

L8:
    frqadd_(&a2[1], &ln2, &l2out, l1, &a3[1], &ln3, &n2b2);
    ln2 += nm1;
    imply_(&a2[1], &l2out, &ln2, &a3[1], &j, l1, &nc);
    ++nc;
    if (mnow == m) {
	goto L9;
    }
    ++mnow;
    goto L7;

/*       IF SYMMETRICAL, RESULTS IN A1 ARE COMPLETE. */

L9:
    if (symm) {
	goto L15;
    }

/*       FOR A SKEW RESULT ADD A2 (OFFSET) INTO A1. */

    ks = (m + 3) / 2;
    j = 1;
    i__1 = lres;
    for (i__ = ks; i__ <= i__1; ++i__) {
	if (i__ > ln1) {
	    goto L10;
	}
	a1[i__] += a2[j];
	goto L11;
L10:
	a1[i__] = a2[j];
L11:
	++j;
/* L12: */
    }

/*       DISTRIBUTION IN A1 POSSIBLY IN REVERSE ORDER. */

    if (*other < *test) {
	goto L15;
    }

/*       REVERSE THE RESULTS IN A1. */

L13:
    j = lres;
    ndo = lres / 2;
    i__1 = ndo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ai = a1[i__];
	a1[i__] = a1[j];
	a1[j] = ai;
	--j;
/* L14: */
    }

/*       FINAL RESULTS NOW IN A1. */

L15:
    *ifault = 0;
    return 0;
} /* gscale_ */

/* Subroutine */ int start1_(integer *n, real *f, integer *l, integer *lout)
{
    /* Initialized data */

    static real one = 1.f;
    static real two = 2.f;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;


/*       ALGORITHM AS 93.1 APPL. STATIST. (1976) VOL.25, NO.1 */

/*       GENERATES A 1,N ANSARI-BRADLEY DISTRIBUTION IN F. */

    /* Parameter adjustments */
    --f;

    /* Function Body */
    *lout = 1 + *n / 2;
    i__1 = *lout;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L1: */
	f[i__] = two;
    }
    if (*n % 2 == 0) {
	f[*lout] = one;
    }
    return 0;
} /* start1_ */


/* Subroutine */ int start2_(integer *n, real *f, integer *l, integer *lout)
{
    /* Initialized data */

    static real one = 1.f;
    static real two = 2.f;
    static real three = 3.f;
    static real four = 4.f;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static real a, b;
    static integer i__, j, nu, lt1, ndo;


/*       ALGORITHM AS 93.2 APPL. STATIST. (1976) VOL.25, NO.1 */

/*       GENERATES A 2,N ANSARI-BRADLEY DISTRIBUTION IN F. */

    /* Parameter adjustments */
    --f;

    /* Function Body */

/*       DERIVE F FOR 2, NU, WHERE NU IS HIGHEST EVEN INTEGER */
/*       LESS THAN OR EQUAL TO N. */
/*       DEFINE NU AND ARRAY LIMITS. */

    nu = *n - *n % 2;
    j = nu + 1;
    *lout = j;
    lt1 = *lout + 1;
    ndo = lt1 / 2;
    a = one;
    b = three;

/*       GENERATE THE SYMMETRICAL 2,NU DISTRIBUTION. */

    i__1 = ndo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	f[i__] = a;
	f[j] = a;
	--j;
	a += b;
	b = four - b;
/* L1: */
    }
    if (nu == *n) {
	return 0;
    }

/*       ADD AN OFFSET 1,N DISTRIBUTION INTO F TO GIVE 2,N RESULT. */

    nu = ndo + 1;
    i__1 = *lout;
    for (i__ = nu; i__ <= i__1; ++i__) {
/* L2: */
	f[i__] += two;
    }
    f[lt1] = two;
    *lout = lt1;
    return 0;
} /* start2_ */


/* Subroutine */ int frqadd_(real *f1, integer *l1in, integer *l1out, integer 
	*l1, real *f2, integer *l2, integer *nstart)
{
    /* Initialized data */

    static real mul2 = 2.f;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i1, i2, nxt;


/*       ALGORITHM AS 93.3 APPL. STATIST. (1976) VOL.25, NO.1 */

/*       ARRAY F1 HAS TWICE THE CONTENTS OF ARRAY F2 ADDED INTO IT */
/*       STARTING WITH ELEMENTS NSTART AND 1 IN F1 AND F2 RESPECTIVELY. */

    /* Parameter adjustments */
    --f1;
    --f2;

    /* Function Body */
    i2 = 1;
    i__1 = *l1in;
    for (i1 = *nstart; i1 <= i__1; ++i1) {
	f1[i1] += mul2 * f2[i2];
	++i2;
/* L1: */
    }
    nxt = *l1in + 1;
    *l1out = *l2 + *nstart - 1;
    i__1 = *l1out;
    for (i1 = nxt; i1 <= i__1; ++i1) {
	f1[i1] = mul2 * f2[i2];
	++i2;
/* L2: */
    }
    ++(*nstart);
    return 0;
} /* frqadd_ */


/* Subroutine */ int imply_(real *f1, integer *l1in, integer *l1out, real *f2,
	 integer *l2, integer *l2max, integer *noff)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i1, i2, j1, j2, ndo;
    static real sum, diff;
    static integer j2min;


/*       ALGORITHM AS 93.4 APPL. STATIST. (1976) VOL.25, NO.1 */

/*       GIVEN L1IN ELEMENTS OF AN ARRAY F1, A SYMMETRICAL */
/*       ARRAY F2 IS DERIVED AND ADDED ONTO F1, LEAVING THE */
/*       FIRST NOFF ELEMENTS OF F1 UNCHANGED AND GIVING A */
/*       SYMMETRICAL RESULT OF L1OUT ELEMENTS IN F1. */


/*       SET-UP SUBSCRIPTS AND LOOP COUNTER. */

    /* Parameter adjustments */
    --f1;
    --f2;

    /* Function Body */
    i2 = 1 - *noff;
    j1 = *l1out;
    j2 = *l1out - *noff;
    *l2 = j2;
    j2min = (j2 + 1) / 2;
    ndo = (*l1out + 1) / 2;

/*       DERIVE AND IMPLY NEW VALUES FROM OUTSIDE INWARDS. */

    i__1 = ndo;
    for (i1 = 1; i1 <= i__1; ++i1) {

/*       GET NEW F1 VALUE FROM SUM OF L/H ELEMENTS OF */
/*       F1 + F2 (IF F2 IS IN RANGE). */

	if (i2 > 0) {
	    goto L1;
	}
	sum = f1[i1];
	goto L2;
L1:
	sum = f1[i1] + f2[i2];

/*       REVISE LEFT ELEMENT OF F1. */

	f1[i1] = sum;

/*       IF F2 NOT COMPLETE IMPLY AND ASSIGN F2 VALUES */
/*       AND REVISE SUBSCRIPTS. */

L2:
	++i2;
	if (j2 < j2min) {
	    goto L5;
	}
	if (j1 <= *l1in) {
	    goto L3;
	}
	diff = sum;
	goto L4;
L3:
	diff = sum - f1[j1];
L4:
	f2[i1] = diff;
	f2[j2] = diff;
	--j2;

/*       ASSIGN R/H ELEMENT OF F1 AND REVISE SUBSCRIPT. */

L5:
	f1[j1] = sum;
	--j1;
/* L6: */
    }
    return 0;
} /* imply_ */

