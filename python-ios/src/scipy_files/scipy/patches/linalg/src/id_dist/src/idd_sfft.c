/* idd_sfft.f -- translated by f2c (version 20100827).
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

/*       this file contains the following user-callable routines: */


/*       routine idd_sffti initializes routine idd_sfft. */

/*       routine idd_sfft rapidly computes a subset of the entries */
/*       of the DFT of a vector, composed with permutation matrices */
/*       both on input and on output. */

/*       routine idd_ldiv finds the greatest integer less than or equal */
/*       to a specified integer, that is divisible by another (larger) */
/*       specified integer. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idd_ldiv_(integer *l, integer *n, integer *m)
{

/*       finds the greatest integer less than or equal to l */
/*       that divides n. */

/*       input: */
/*       l -- integer at least as great as m */
/*       n -- integer divisible by m */

/*       output: */
/*       m -- greatest integer less than or equal to l that divides n */



    *m = *l;

L1000:
    if (*m * (*n / *m) == *n) {
	goto L2000;
    }

    --(*m);
    goto L1000;

L2000:


    return 0;
} /* idd_ldiv_ */





/* Subroutine */ int idd_sffti_(integer *l, integer *ind, integer *n, 
	doublereal *wsave)
{
    extern /* Subroutine */ int idd_sffti1_(integer *ind, integer *n, doublereal *wsave);
    extern int idd_sffti2_(integer *l, integer *ind, integer *n,
                           doublecomplex *wsave);


/*       initializes wsave for using routine idd_sfft. */

/*       input: */
/*       l -- number of pairs of entries in the output of idd_sfft */
/*            to compute */
/*       ind -- indices of the pairs of entries in the output */
/*              of idd_sfft to compute; the indices must be chosen */
/*              in the range from 1 to n/2 */
/*       n -- length of the vector to be transformed */

/*       output: */
/*       wsave -- array needed by routine idd_sfft for processing */
/*                (the present routine does not use the last n elements */
/*                 of wsave, but routine idd_sfft does) */



    /* Parameter adjustments */
    --ind;
    --wsave;

    /* Function Body */
    if (*l == 1) {
	idd_sffti1_(&ind[1], n, &wsave[1]);
    }
    if (*l > 1) {
	idd_sffti2_(l, &ind[1], n, &wsave[1]);
    }


    return 0;
} /* idd_sffti_ */





/* Subroutine */ int idd_sffti1_(integer *ind, integer *n, doublereal *wsave)
{
    /* System generated locals */
    integer i_1;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal), cos(doublereal), sin(
	    doublereal);

    /* Local variables */
    static integer k;
    static doublereal r1, fact, twopi;


/*       routine idd_sffti serves as a wrapper around */
/*       the present routine; please see routine idd_sffti */
/*       for documentation. */


    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    r1 = 1.;
    twopi = atan(r1) * 8;


    fact = 1 / sqrt(r1 * *n);


    i_1 = *n;
    for (k = 1; k <= i_1; ++k) {
	wsave[k] = cos(twopi * (k - 1) * *ind / (r1 * *n)) * fact;
    }

/* k */
    i_1 = *n;
    for (k = 1; k <= i_1; ++k) {
	wsave[*n + k] = -sin(twopi * (k - 1) * *ind / (r1 * *n)) * fact;
    }


/* k */
    return 0;
} /* idd_sffti1_ */





/* Subroutine */ int idd_sffti2_(integer *l, integer *ind, integer *n, 
	doublecomplex *wsave)
{
    /* System generated locals */
    integer i_1, i_2, i_3, i_4, i_5, i_6;
    doublereal d_1, d_2, d_3, d_4, d_5, d_6;
    doublecomplex z_1, z_2, z_3, z_4, z_5, z_6, z_7, z_8, z_9, z_10,
	     z_11, z_12;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal);
    void z_exp(doublecomplex *, doublecomplex *);

    /* Local variables */
    extern /* Subroutine */ int idd_ldiv_(integer *, integer *, integer *);
    static integer i_, j, k, m;
    static doublereal r1;
    static doublecomplex ci;
    static integer ii;
    static doublereal fact;
    extern /* Subroutine */ int dffti_(integer *, doublecomplex *);
    static integer imodm, idivm;
    static doublereal twopi;
    static integer nblock;
    static doublecomplex twopii;


/*       routine idd_sffti serves as a wrapper around */
/*       the present routine; please see routine idd_sffti */
/*       for documentation. */


    /* Parameter adjustments */
    --ind;
    --wsave;

    /* Function Body */
    ci.r = 0.f, ci.i = 1.f;
    r1 = 1.;
    twopi = atan(r1) * 8;
    z_1.r = twopi * ci.r, z_1.i = twopi * ci.i;
    twopii.r = z_1.r, twopii.i = z_1.i;


/*       Determine the block lengths for the FFTs. */

    idd_ldiv_(l, n, &nblock);
    m = *n / nblock;


/*       Initialize wsave for using routine dfftf. */

    dffti_(&nblock, &wsave[1]);


/*       Calculate the coefficients in the linear combinations */
/*       needed for the direct portion of the calculation. */

    fact = 1 / sqrt(r1 * *n);

    ii = (*l << 1) + 15;

    i_1 = *l;
    for (j = 1; j <= i_1; ++j) {


	i_ = ind[j];


	if (i_ <= *n / 2 - m / 2) {

	    idivm = (i_ - 1) / m;
	    imodm = i_ - 1 - m * idivm;

	    i_2 = m;
	    for (k = 1; k <= i_2; ++k) {
		i_3 = ii + m * (j - 1) + k;
		z_7.r = -twopii.r, z_7.i = -twopii.i;
		i_4 = k - 1;
		d_1 = (doublereal) i_4;
		z_6.r = d_1 * z_7.r, z_6.i = d_1 * z_7.i;
		d_2 = (doublereal) imodm;
		z_5.r = d_2 * z_6.r, z_5.i = d_2 * z_6.i;
		d_3 = r1 * m;
		z_4.r = z_5.r / d_3, z_4.i = z_5.i / d_3;
		z_exp(&z_3, &z_4);
		z_12.r = -twopii.r, z_12.i = -twopii.i;
		i_5 = k - 1;
		d_4 = (doublereal) i_5;
		z_11.r = d_4 * z_12.r, z_11.i = d_4 * z_12.i;
		i_6 = idivm + 1;
		d_5 = (doublereal) i_6;
		z_10.r = d_5 * z_11.r, z_10.i = d_5 * z_11.i;
		d_6 = r1 * *n;
		z_9.r = z_10.r / d_6, z_9.i = z_10.i / d_6;
		z_exp(&z_8, &z_9);
		z_2.r = z_3.r * z_8.r - z_3.i * z_8.i, z_2.i = z_3.r * 
			z_8.i + z_3.i * z_8.r;
		z_1.r = fact * z_2.r, z_1.i = fact * z_2.i;
		wsave[i_3].r = z_1.r, wsave[i_3].i = z_1.i;
	    }

/* k */
	}


/* i .le. n/2-m/2 */
	if (i_ > *n / 2 - m / 2) {

	    idivm = i_ / (m / 2);
	    imodm = i_ - m / 2 * idivm;

	    i_2 = m;
	    for (k = 1; k <= i_2; ++k) {
		i_3 = ii + m * (j - 1) + k;
		z_6.r = -twopii.r, z_6.i = -twopii.i;
		i_4 = k - 1;
		d_1 = (doublereal) i_4;
		z_5.r = d_1 * z_6.r, z_5.i = d_1 * z_6.i;
		d_2 = (doublereal) imodm;
		z_4.r = d_2 * z_5.r, z_4.i = d_2 * z_5.i;
		d_3 = r1 * m;
		z_3.r = z_4.r / d_3, z_3.i = z_4.i / d_3;
		z_exp(&z_2, &z_3);
		z_1.r = fact * z_2.r, z_1.i = fact * z_2.i;
		wsave[i_3].r = z_1.r, wsave[i_3].i = z_1.i;
	    }

/* k */
	}


/* i .gt. n/2-m/2 */
    }


/* j */
    return 0;
} /* idd_sffti2_ */





/* Subroutine */ int idd_sfft_(integer *l, integer *ind, integer *n, 
	doublecomplex *wsave, doublereal *v)
{
    extern /* Subroutine */ int idd_sfft1_(integer *, integer *, doublereal *
                                           , doublereal *);
    extern int idd_sfft2_(integer *l, integer *ind, integer *n,
                          doublereal *v, doublecomplex *wsave);


/*       computes a subset of the entries of the DFT of v, */
/*       composed with permutation matrices both on input and on output, */
/*       via a two-stage procedure (debugging code routine dfftf2 above */
/*       is supposed to calculate the full vector from which idd_sfft */
/*       returns a subset of the entries, when dfftf2 has */
/*       the same parameter nblock as in the present routine). */

/*       input: */
/*       l -- number of pairs of entries in the output to compute */
/*       ind -- indices of the pairs of entries in the output */
/*              to compute; the indices must be chosen */
/*              in the range from 1 to n/2 */
/*       n -- length of v; n must be a positive integer power of 2 */
/*       v -- vector to be transformed */
/*       wsave -- processing array initialized by routine idd_sffti */

/*       output: */
/*       v -- pairs of entries indexed by ind are given */
/*            their appropriately transformed values */

/*       _N.B._: n must be a positive integer power of 2. */

/*       references: */
/*       Sorensen and Burrus, "Efficient computation of the DFT with */
/*            only a subset of input or output points," */
/*            IEEE Transactions on Signal Processing, 41 (3): 1184-1200, */
/*            1993. */
/*       Woolfe, Liberty, Rokhlin, Tygert, "A fast randomized algorithm */
/*            for the approximation of matrices," Applied and */
/*            Computational Harmonic Analysis, 25 (3): 335-366, 2008; */
/*            Section 3.3. */



    /* Parameter adjustments */
    --ind;
    --v;
    --wsave;

    /* Function Body */
    if (*l == 1) {
	idd_sfft1_(&ind[1], n, &v[1], &wsave[1]);
    }
    if (*l > 1) {
	idd_sfft2_(l, &ind[1], n, &v[1], &wsave[1]);
    }


    return 0;
} /* idd_sfft_ */





/* Subroutine */ int idd_sfft1_(integer *ind, integer *n, doublereal *v, 
	doublereal *wsave)
{
    /* System generated locals */
    integer i_1;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal);

    /* Local variables */
    static integer k;
    static doublereal r1, fact, sumi, sumr, twopi;


/*       routine idd_sfft serves as a wrapper around */
/*       the present routine; please see routine idd_sfft */
/*       for documentation. */


    /* Parameter adjustments */
    --wsave;
    --v;

    /* Function Body */
    r1 = 1.;
    twopi = atan(r1) * 8;


    if (*ind < *n / 2) {


	sumr = 0.;

	i_1 = *n;
	for (k = 1; k <= i_1; ++k) {
	    sumr += wsave[k] * v[k];
	}


/* k */
	sumi = 0.;

	i_1 = *n;
	for (k = 1; k <= i_1; ++k) {
	    sumi += wsave[*n + k] * v[k];
	}


/* k */
    }


/* ind .lt. n/2 */
    if (*ind == *n / 2) {


	fact = 1 / sqrt(r1 * *n);


	sumr = 0.;

	i_1 = *n;
	for (k = 1; k <= i_1; ++k) {
	    sumr += v[k];
	}

/* k */
	sumr *= fact;


	sumi = 0.;

	i_1 = *n / 2;
	for (k = 1; k <= i_1; ++k) {
	    sumi += v[(k << 1) - 1];
	    sumi -= v[k * 2];
	}

/* k */
	sumi *= fact;


    }


/* ind .eq. n/2 */
    v[(*ind << 1) - 1] = sumr;
    v[*ind * 2] = sumi;


    return 0;
} /* idd_sfft1_ */





/* Subroutine */ int idd_sfft2_(integer *l, integer *ind, integer *n, 
	doublereal *v, doublecomplex *wsave)
{
    /* System generated locals */
    integer i_1, i_2, i_3, i_4, i_5;
    doublecomplex z_1, z_2;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int idd_ldiv_(integer *, integer *, integer *);
    static integer i_, j, k, m;
    static doublereal r1;
    static doublecomplex ci;
    static integer ii, iii;
    static doublecomplex sum;
    static doublereal fact, rsum;
    extern /* Subroutine */ int dfftf_(integer *, doublereal *, doublecomplex 
	    *);
    static integer imodm, idivm;
    static doublereal twopi;
    static integer nblock;


/*       routine idd_sfft serves as a wrapper around */
/*       the present routine; please see routine idd_sfft */
/*       for documentation. */


    /* Parameter adjustments */
    --ind;
    --wsave;
    --v;

    /* Function Body */
    ci.r = 0.f, ci.i = 1.f;
    r1 = 1.;
    twopi = atan(r1) * 8;


/*       Determine the block lengths for the FFTs. */

    idd_ldiv_(l, n, &nblock);


    m = *n / nblock;


/*       FFT each block of length nblock of v. */

    i_1 = m;
    for (k = 1; k <= i_1; ++k) {
	dfftf_(&nblock, &v[nblock * (k - 1) + 1], &wsave[1]);
    }


/*       Transpose v to obtain wsave(2*l+15+2*n+1 : 2*l+15+3*n). */

/* k */
    iii = (*l << 1) + 15 + (*n << 1);

    i_1 = m;
    for (k = 1; k <= i_1; ++k) {
	i_2 = nblock / 2 - 1;
	for (j = 1; j <= i_2; ++j) {
	    i_3 = iii + m * (j - 1) + k;
	    i_4 = nblock * (k - 1) + (j << 1);
	    i_5 = nblock * (k - 1) + (j << 1) + 1;
	    z_2.r = v[i_5] * ci.r, z_2.i = v[i_5] * ci.i;
	    z_1.r = v[i_4] + z_2.r, z_1.i = z_2.i;
	    wsave[i_3].r = z_1.r, wsave[i_3].i = z_1.i;
	}
/* j */
    }

/*       Handle the purely real frequency components separately. */

/* k */
    i_1 = m;
    for (k = 1; k <= i_1; ++k) {
	i_2 = iii + m * (nblock / 2 - 1) + k;
	i_3 = nblock * (k - 1) + nblock;
	wsave[i_2].r = v[i_3], wsave[i_2].i = 0.;
	i_2 = iii + m * (nblock / 2) + k;
	i_3 = nblock * (k - 1) + 1;
	wsave[i_2].r = v[i_3], wsave[i_2].i = 0.;
    }


/*       Directly calculate the desired entries of v. */

/* k */
    ii = (*l << 1) + 15;

    i_1 = *l;
    for (j = 1; j <= i_1; ++j) {


	i_ = ind[j];


	if (i_ <= *n / 2 - m / 2) {

	    idivm = (i_ - 1) / m;
	    imodm = i_ - 1 - m * idivm;

	    sum.r = 0., sum.i = 0.;

	    i_2 = m;
	    for (k = 1; k <= i_2; ++k) {
		i_3 = iii + m * idivm + k;
		i_4 = ii + m * (j - 1) + k;
		z_2.r = wsave[i_3].r * wsave[i_4].r - wsave[i_3].i * 
			wsave[i_4].i, z_2.i = wsave[i_3].r * wsave[i_4].i 
			+ wsave[i_3].i * wsave[i_4].r;
		z_1.r = sum.r + z_2.r, z_1.i = sum.i + z_2.i;
		sum.r = z_1.r, sum.i = z_1.i;
	    }

/* k */
	    i_2 = (i_ << 1) - 1;
	    v[i_2] = sum.r;
	    i_2 = i_ << 1;
	    z_2.r = -ci.r, z_2.i = -ci.i;
	    z_1.r = z_2.r * sum.r - z_2.i * sum.i, z_1.i = z_2.r * sum.i 
		    + z_2.i * sum.r;
	    v[i_2] = z_1.r;

	}


/* i .le. n/2-m/2 */
	if (i_ > *n / 2 - m / 2) {

	    if (i_ < *n / 2) {

		idivm = i_ / (m / 2);
		imodm = i_ - m / 2 * idivm;

		sum.r = 0., sum.i = 0.;

		i_2 = m;
		for (k = 1; k <= i_2; ++k) {
		    i_3 = iii + m * (nblock / 2) + k;
		    i_4 = ii + m * (j - 1) + k;
		    z_2.r = wsave[i_3].r * wsave[i_4].r - wsave[i_3].i * 
			    wsave[i_4].i, z_2.i = wsave[i_3].r * wsave[
			    i_4].i + wsave[i_3].i * wsave[i_4].r;
		    z_1.r = sum.r + z_2.r, z_1.i = sum.i + z_2.i;
		    sum.r = z_1.r, sum.i = z_1.i;
		}

/* k */
		i_2 = (i_ << 1) - 1;
		v[i_2] = sum.r;
		i_2 = i_ << 1;
		z_2.r = -ci.r, z_2.i = -ci.i;
		z_1.r = z_2.r * sum.r - z_2.i * sum.i, z_1.i = z_2.r * 
			sum.i + z_2.i * sum.r;
		v[i_2] = z_1.r;

	    }

	    if (i_ == *n / 2) {

		fact = 1 / sqrt(r1 * *n);


		rsum = 0.;

		i_2 = m;
		for (k = 1; k <= i_2; ++k) {
		    i_3 = iii + m * (nblock / 2) + k;
		    z_1.r = rsum + wsave[i_3].r, z_1.i = wsave[i_3].i;
		    rsum = z_1.r;
		}

/* k */
		v[*n - 1] = rsum * fact;


		rsum = 0.;

		i_2 = m / 2;
		for (k = 1; k <= i_2; ++k) {
		    i_3 = iii + m * (nblock / 2) + (k << 1) - 1;
		    z_1.r = rsum + wsave[i_3].r, z_1.i = wsave[i_3].i;
		    rsum = z_1.r;
		    i_3 = iii + m * (nblock / 2) + (k << 1);
		    z_1.r = rsum - wsave[i_3].r, z_1.i = -wsave[i_3].i;
		    rsum = z_1.r;
		}

/* k */
		v[*n] = rsum * fact;

	    }

	}


/* i .gt. n/2-m/2 */
    }


/* j */
    return 0;
} /* idd_sfft2_ */

