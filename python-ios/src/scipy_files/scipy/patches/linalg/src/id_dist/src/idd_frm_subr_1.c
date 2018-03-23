/* idd_frm_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_pairsamps_(integer *n, integer *l, integer *ind, 
	integer *l2, integer *ind2, integer *marker)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;


/*       calculates the indices of the l2 pairs of integers */
/*       to which the l individual integers from ind belong. */
/*       The integers in ind may range from 1 to n. */

/*       input: */
/*       n -- upper bound on the integers in ind */
/*            (the number 1 must be a lower bound); */
/*            n must be even */
/*       l -- length of ind */
/*       ind -- integers selected from 1 to n */

/*       output: */
/*       l2 -- length of ind2 */
/*       ind2 -- indices in the range from 1 to n/2 of the pairs */
/*               of integers to which the entries of ind belong */

/*       work: */
/*       marker -- must be at least n/2 integer elements long */

/*       _N.B._: n must be even. */



/*       Unmark all pairs. */

    /* Parameter adjustments */
    --marker;
    --ind2;
    --ind;

    /* Function Body */
    i__1 = *n / 2;
    for (k = 1; k <= i__1; ++k) {
	marker[k] = 0;
    }


/*       Mark the required pairs. */

/* k */
    i__1 = *l;
    for (k = 1; k <= i__1; ++k) {
	++marker[(ind[k] + 1) / 2];
    }


/*       Record the required pairs in indpair. */

/* k */
    *l2 = 0;

    i__1 = *n / 2;
    for (k = 1; k <= i__1; ++k) {

	if (marker[k] != 0) {
	    ++(*l2);
	    ind2[*l2] = k;
	}

    }


/* k */
    return 0;
} /* idd_pairsamps__ */

