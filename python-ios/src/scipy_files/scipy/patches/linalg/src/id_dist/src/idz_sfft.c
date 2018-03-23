/* idz_sfft.f -- translated by f2c (version 20100827).
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


/*       routine idz_sffti initializes routine idz_sfft. */

/*       routine idz_sfft rapidly computes a subset of the entries */
/*       of the DFT of a vector, composed with permutation matrices */
/*       both on input and on output. */

/*       routine idz_ldiv finds the greatest integer less than or equal */
/*       to a specified integer, that is divisible by another (larger) */
/*       specified integer. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idz_ldiv_(integer *l, integer *n, integer *m)
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
} /* idz_ldiv__ */





/* Subroutine */ int idz_sffti_(integer *l, integer *ind, integer *n, 
	doublecomplex *wsave)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    doublereal d__1, d__2, d__3, d__4, d__5, d__6;
    doublecomplex z__1, z__2, z__3, z__4, z__5, z__6, z__7, z__8, z__9, z__10,
	     z__11, z__12;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal);
    void z_exp(doublecomplex *, doublecomplex *);

    /* Local variables */
    extern /* Subroutine */ int idz_ldiv_(integer *, integer *, integer *);
    static integer i__, j, k, m;
    static doublereal r1;
    static doublecomplex ci;
    static integer ii;
    static doublereal fact;
    static integer imodm, idivm;
    extern /* Subroutine */ int zffti_(integer *, doublecomplex *);
    static doublereal twopi;
    static integer nblock;
    static doublecomplex twopii;


/*       initializes wsave for use with routine idz_sfft. */

/*       input: */
/*       l -- number of entries in the output of idz_sfft to compute */
/*       ind -- indices of the entries in the output of idz_sfft */
/*              to compute */
/*       n -- length of the vector to be transformed */

/*       output: */
/*       wsave -- array needed by routine idz_sfft for processing */


    /* Parameter adjustments */
    --ind;
    --wsave;

    /* Function Body */
    ci.r = 0.f, ci.i = 1.f;
    r1 = 1.;
    twopi = atan(r1) * 8;
    z__1.r = twopi * ci.r, z__1.i = twopi * ci.i;
    twopii.r = z__1.r, twopii.i = z__1.i;


/*       Determine the block lengths for the FFTs. */

    idz_ldiv_(l, n, &nblock);
    m = *n / nblock;


/*       Initialize wsave for use with routine zfftf. */

    zffti_(&nblock, &wsave[1]);


/*       Calculate the coefficients in the linear combinations */
/*       needed for the direct portion of the calculation. */

    fact = 1 / sqrt(r1 * *n);

    ii = (*l << 1) + 15;

    i__1 = *l;
    for (j = 1; j <= i__1; ++j) {

	i__ = ind[j];

	idivm = (i__ - 1) / m;
	imodm = i__ - 1 - m * idivm;

	i__2 = m;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = ii + m * (j - 1) + k;
	    z__7.r = -twopii.r, z__7.i = -twopii.i;
	    d__1 = (doublereal) imodm;
	    z__6.r = d__1 * z__7.r, z__6.i = d__1 * z__7.i;
	    i__4 = k - 1;
	    d__2 = (doublereal) i__4;
	    z__5.r = d__2 * z__6.r, z__5.i = d__2 * z__6.i;
	    d__3 = r1 * m;
	    z__4.r = z__5.r / d__3, z__4.i = z__5.i / d__3;
	    z_exp(&z__3, &z__4);
	    z__12.r = -twopii.r, z__12.i = -twopii.i;
	    i__5 = k - 1;
	    d__4 = (doublereal) i__5;
	    z__11.r = d__4 * z__12.r, z__11.i = d__4 * z__12.i;
	    d__5 = (doublereal) idivm;
	    z__10.r = d__5 * z__11.r, z__10.i = d__5 * z__11.i;
	    d__6 = r1 * *n;
	    z__9.r = z__10.r / d__6, z__9.i = z__10.i / d__6;
	    z_exp(&z__8, &z__9);
	    z__2.r = z__3.r * z__8.r - z__3.i * z__8.i, z__2.i = z__3.r * 
		    z__8.i + z__3.i * z__8.r;
	    z__1.r = fact * z__2.r, z__1.i = fact * z__2.i;
	    wsave[i__3].r = z__1.r, wsave[i__3].i = z__1.i;
	}

/* k */
    }


/* j */
    return 0;
} /* idz_sffti__ */





/* Subroutine */ int idz_sfft_(integer *l, integer *ind, integer *n, 
	doublecomplex *wsave, doublecomplex *v)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublecomplex z__1, z__2;

    /* Builtin functions */
    double atan(doublereal);

    /* Local variables */
    extern /* Subroutine */ int idz_ldiv_(integer *, integer *, integer *);
    static integer i__, j, k, m;
    static doublereal r1;
    static doublecomplex ci;
    static integer ii, iii;
    static doublecomplex sum;
    static integer idivm;
    extern /* Subroutine */ int zfftf_(integer *, doublecomplex *, 
	    doublecomplex *);
    static doublereal twopi;
    static integer nblock;


/*       computes a subset of the entries of the DFT of v, */
/*       composed with permutation matrices both on input and on output, */
/*       via a two-stage procedure (routine zfftf2 is supposed */
/*       to calculate the full vector from which idz_sfft returns */
/*       a subset of the entries, when zfftf2 has the same parameter */
/*       nblock as in the present routine). */

/*       input: */
/*       l -- number of entries in the output to compute */
/*       ind -- indices of the entries of the output to compute */
/*       n -- length of v */
/*       v -- vector to be transformed */
/*       wsave -- processing array initialized by routine idz_sffti */

/*       output: */
/*       v -- entries indexed by ind are given their appropriate */
/*            transformed values */

/*       _N.B._: The user has to boost the memory allocations */
/*               for wsave (and change iii accordingly) if s/he wishes */
/*               to use strange sizes of n; it's best to stick to powers */
/*               of 2. */

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
    ci.r = 0.f, ci.i = 1.f;
    r1 = 1.;
    twopi = atan(r1) * 8;


/*       Determine the block lengths for the FFTs. */

    idz_ldiv_(l, n, &nblock);


    m = *n / nblock;


/*       FFT each block of length nblock of v. */

    i__1 = m;
    for (k = 1; k <= i__1; ++k) {
	zfftf_(&nblock, &v[nblock * (k - 1) + 1], &wsave[1]);
    }


/*       Transpose v to obtain wsave(2*l+15+2*n+1 : 2*l+15+3*n). */

/* k */
    iii = (*l << 1) + 15 + (*n << 1);

    i__1 = m;
    for (k = 1; k <= i__1; ++k) {
	i__2 = nblock;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = iii + m * (j - 1) + k;
	    i__4 = nblock * (k - 1) + j;
	    wsave[i__3].r = v[i__4].r, wsave[i__3].i = v[i__4].i;
	}
/* j */
    }


/*       Directly calculate the desired entries of v. */

/* k */
    ii = (*l << 1) + 15;
    iii = (*l << 1) + 15 + (*n << 1);

    i__1 = *l;
    for (j = 1; j <= i__1; ++j) {

	i__ = ind[j];

	idivm = (i__ - 1) / m;

	sum.r = 0., sum.i = 0.;

	i__2 = m;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = ii + m * (j - 1) + k;
	    i__4 = iii + m * idivm + k;
	    z__2.r = wsave[i__3].r * wsave[i__4].r - wsave[i__3].i * wsave[
		    i__4].i, z__2.i = wsave[i__3].r * wsave[i__4].i + wsave[
		    i__3].i * wsave[i__4].r;
	    z__1.r = sum.r + z__2.r, z__1.i = sum.i + z__2.i;
	    sum.r = z__1.r, sum.i = z__1.i;
	}

/* k */
	i__2 = i__;
	v[i__2].r = sum.r, v[i__2].i = sum.i;

    }


/* j */
    return 0;
} /* idz_sfft__ */

