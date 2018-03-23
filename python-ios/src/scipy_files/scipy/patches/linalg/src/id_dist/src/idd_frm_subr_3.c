/* idd_frm_subr_3.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_subselect_(integer *n, integer *ind, integer *m, 
	doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;


/*       copies into y the entries of x indicated by ind. */

/*       input: */
/*       n -- number of entries of x to copy into y */
/*       ind -- indices of the entries in x to copy into y */
/*       m -- length of x */
/*       x -- vector whose entries are to be copied */

/*       output: */
/*       y -- collection of entries of x specified by ind */



    /* Parameter adjustments */
    --y;
    --ind;
    --x;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	y[k] = x[ind[k]];
    }


/* k */
    return 0;
} /* idd_subselect__ */

