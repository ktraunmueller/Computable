/* idd_frm_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_frm_(integer *m, integer *n, doublereal *w, 
	doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int idd_random_transf_(doublereal *, doublereal *
	    , doublereal *);
    static integer iw;
    extern /* Subroutine */ int idd_permute_(integer *, doublereal *, 
	    doublereal *, doublereal *), dfftf_(integer *, doublereal *, 
	    doublereal *), idd_subselect_(integer *, doublereal *, integer *,
	     doublereal *, doublereal *);


/*       transforms x into y via a composition */
/*       of Rokhlin's random transform, random subselection, and an FFT. */
/*       In contrast to routine idd_sfrm, the present routine works best */
/*       when the length of the transformed vector is the integer n */
/*       output by routine idd_frmi, or when the length */
/*       is not specified, but instead determined a posteriori */
/*       using the output of the present routine. The transformed vector */
/*       output by the present routine is randomly permuted. */

/*       input: */
/*       m -- length of x */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m, as obtained */
/*            from the routine idd_frmi; n is the length of y */
/*       w -- initialization array constructed by routine idd_frmi */
/*       x -- vector to be transformed */

/*       output: */
/*       y -- transform of x */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Apply Rokhlin's random transformation to x, obtaining */
/*       w(16*m+71 : 17*m+70). */

    /* Parameter adjustments */
    --x;
    --w;
    --y;

    /* Function Body */
    iw = (integer) w[*m + 3 + *n];
    idd_random_transf_(&x[1], &w[(*m << 4) + 71], &w[iw]);


/*       Subselect from  w(16*m+71 : 17*m+70)  to obtain y. */

    idd_subselect_(n, &w[3], m, &w[(*m << 4) + 71], &y[1]);


/*       Copy y into  w(16*m+71 : 16*m+n+70). */

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	w[(*m << 4) + 70 + k] = y[k];
    }


/*       Fourier transform  w(16*m+71 : 16*m+n+70). */

/* k */
    dfftf_(n, &w[(*m << 4) + 71], &w[*m + 4 + *n]);


/*       Permute  w(16*m+71 : 16*m+n+70)  to obtain y. */

    idd_permute_(n, &w[*m + 3], &w[(*m << 4) + 71], &y[1]);


    return 0;
} /* idd_frm__ */





/* Subroutine */ int idd_sfrm_(integer *l, integer *m, integer *n, 
	doublereal *w, doublereal *x, doublereal *y)
{
    extern /* Subroutine */ int idd_sfft_(integer *, doublereal *, integer *,
	     doublereal *, doublereal *);
    static integer l2;
    extern /* Subroutine */ int idd_random_transf_(doublereal *, doublereal *
	    , doublereal *);
    static integer iw;
    extern /* Subroutine */ int idd_subselect_(integer *, doublereal *, 
	    integer *, doublereal *, doublereal *);


/*       transforms x into y via a composition */
/*       of Rokhlin's random transform, random subselection, and an FFT. */
/*       In contrast to routine idd_frm, the present routine works best */
/*       when the length l of the transformed vector is known a priori. */

/*       input: */
/*       l -- length of y; l must be less than or equal to n */
/*       m -- length of x */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m, as obtained */
/*            from the routine idd_sfrmi */
/*       w -- initialization array constructed by routine idd_sfrmi */
/*       x -- vector to be transformed */

/*       output: */
/*       y -- transform of x */

/*       _N.B._: l must be less than or equal to n. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Retrieve the number of pairs of outputs to be calculated */
/*       via sfft. */

    /* Parameter adjustments */
    --y;
    --x;
    --w;

    /* Function Body */
    l2 = (integer) w[3];


/*       Apply Rokhlin's random transformation to x, obtaining */
/*       w(25*m+91 : 26*m+90). */

    iw = (integer) w[*m + 4 + *l + l2];
    idd_random_transf_(&x[1], &w[*m * 25 + 91], &w[iw]);


/*       Subselect from  w(25*m+91 : 26*m+90)  to obtain */
/*       w(26*m+91 : 26*m+n+90). */

    idd_subselect_(n, &w[4], m, &w[*m * 25 + 91], &w[*m * 26 + 91]);


/*       Fourier transform  w(26*m+91 : 26*m+n+90). */

    idd_sfft_(&l2, &w[*m + 4 + *l], n, &w[*m + 5 + *l + l2], &w[*m * 26 + 91]
	    );


/*       Copy the desired entries from  w(26*m+91 : 26*m+n+90) */
/*       to y. */

    idd_subselect_(l, &w[*m + 4], n, &w[*m * 26 + 91], &y[1]);


    return 0;
} /* idd_sfrm__ */

