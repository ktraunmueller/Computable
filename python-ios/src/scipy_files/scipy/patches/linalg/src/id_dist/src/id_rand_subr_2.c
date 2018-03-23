/* id_rand_subr_2.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int id_randperm_(integer *n, integer *ind)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer j, m;
    static doublereal r__;
    static integer iswap;


/*       draws a permutation ind uniformly at random from the group */
/*       of all permutations of n objects. */

/*       input: */
/*       n -- length of ind */

/*       output: */
/*       ind -- random permutation of length n */



/*       Initialize ind. */

    /* Parameter adjustments */
    --ind;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	ind[j] = j;
    }


/*       Shuffle ind via the Fisher-Yates (Knuth/Durstenfeld) algorithm. */

/* j */
    for (m = *n; m >= 2; --m) {

/*         Draw an integer uniformly at random from 1, 2, ..., m. */

	id_srand_(&c__1, &r__);
	j = (integer) (m * r__ + 1);

/*         Uncomment the following line if r could equal 1: */
/*         if(j .eq. m+1) j = m */

/*         Swap ind(j) and ind(m). */

	iswap = ind[j];
	ind[j] = ind[m];
	ind[m] = iswap;

    }


/* m */
    return 0;
} /* id_randperm__ */

