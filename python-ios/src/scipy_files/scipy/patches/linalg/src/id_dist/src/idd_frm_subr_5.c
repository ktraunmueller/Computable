/* idd_frm_subr_5.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_copyints_(integer *n, integer *ia, integer *ib)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;


/*       copies ia into ib. */

/*       input: */
/*       n -- length of ia and ib */
/*       ia -- array to be copied */

/*       output: */
/*       ib -- copy of ia */



    /* Parameter adjustments */
    --ib;
    --ia;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	ib[k] = ia[k];
    }


/* k */
    return 0;
} /* idd_copyints__ */

