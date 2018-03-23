/* idz_frm_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_frm_(integer *m, integer *n, doublecomplex *w, 
	doublecomplex *x, doublecomplex *y)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int idz_random_transf_(doublecomplex *, 
	    doublecomplex *, doublecomplex *);
    static integer iw;
    extern /* Subroutine */ int idz_permute_(integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *), zfftf_(integer *, 
	    doublecomplex *, doublecomplex *), idz_subselect_(integer *, 
	    doublecomplex *, integer *, doublecomplex *, doublecomplex *);


/*       transforms x into y via a composition */
/*       of Rokhlin's random transform, random subselection, and an FFT. */
/*       In contrast to routine idz_sfrm, the present routine works best */
/*       when the length of the transformed vector is the integer n */
/*       output by routine idz_frmi, or when the length */
/*       is not specified, but instead determined a posteriori */
/*       using the output of the present routine. The transformed vector */
/*       output by the present routine is randomly permuted. */

/*       input: */
/*       m -- length of x */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m, as obtained */
/*            from the routine idz_frmi; n is the length of y */
/*       w -- initialization array constructed by routine idz_frmi */
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
    i__1 = *m + 3 + *n;
    iw = (integer) w[i__1].r;
    idz_random_transf_(&x[1], &w[(*m << 4) + 71], &w[iw]);


/*       Subselect from  w(16*m+71 : 17*m+70)  to obtain y. */

    idz_subselect_(n, &w[3], m, &w[(*m << 4) + 71], &y[1]);


/*       Copy y into  w(16*m+71 : 16*m+n+70). */

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = (*m << 4) + 70 + k;
	i__3 = k;
	w[i__2].r = y[i__3].r, w[i__2].i = y[i__3].i;
    }


/*       Fourier transform  w(16*m+71 : 16*m+n+70). */

/* k */
    zfftf_(n, &w[(*m << 4) + 71], &w[*m + 4 + *n]);


/*       Permute  w(16*m+71 : 16*m+n+70)  to obtain y. */

    idz_permute_(n, &w[*m + 3], &w[(*m << 4) + 71], &y[1]);


    return 0;
} /* idz_frm__ */





/* Subroutine */ int idz_sfrm_(integer *l, integer *m, integer *n, 
	doublecomplex *w, doublecomplex *x, doublecomplex *y)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    extern /* Subroutine */ int idz_sfft_(integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *), idz_random_transf_(
	    doublecomplex *, doublecomplex *, doublecomplex *);
    static integer iw;
    extern /* Subroutine */ int idz_subselect_(integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *);


/*       transforms x into y via a composition */
/*       of Rokhlin's random transform, random subselection, and an FFT. */
/*       In contrast to routine idz_frm, the present routine works best */
/*       when the length l of the transformed vector is known a priori. */

/*       input: */
/*       l -- length of y; l must be less than or equal to n */
/*       m -- length of x */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m, as obtained */
/*            from the routine idz_frmi */
/*       w -- initialization array constructed by routine idz_sfrmi */
/*       x -- vector to be transformed */

/*       output: */
/*       y -- transform of x */

/*       _N.B._: l must be less than or equal to n. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Apply Rokhlin's random transformation to x, obtaining */
/*       w(19*m+71 : 20*m+70). */

    /* Parameter adjustments */
    --y;
    --x;
    --w;

    /* Function Body */
    i__1 = *m + 4 + *l;
    iw = (integer) w[i__1].r;
    idz_random_transf_(&x[1], &w[*m * 19 + 71], &w[iw]);


/*       Subselect from  w(19*m+71 : 20*m+70)  to obtain */
/*       w(20*m+71 : 20*m+n+70). */

    idz_subselect_(n, &w[4], m, &w[*m * 19 + 71], &w[*m * 20 + 71]);


/*       Fourier transform  w(20*m+71 : 20*m+n+70). */

    idz_sfft_(l, &w[*m + 4], n, &w[*m + 5 + *l], &w[*m * 20 + 71]);


/*       Copy the desired entries from  w(20*m+71 : 20*m+n+70) */
/*       to y. */

    idz_subselect_(l, &w[*m + 4], n, &w[*m * 20 + 71], &y[1]);


    return 0;
} /* idz_sfrm__ */

