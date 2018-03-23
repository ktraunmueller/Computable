/* idz_svd_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_realcomp__(integer *n, doublereal *a, doublecomplex *
	b)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer k;


/*       copies the real*8 array a into the complex*16 array b. */

/*       input: */
/*       n -- length of a and b */
/*       a -- real*8 array to be copied into b */

/*       output: */
/*       b -- complex*16 copy of a */



    /* Parameter adjustments */
    --b;
    --a;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	b[i__2].r = a[i__3], b[i__2].i = 0.;
    }


/* k */
    return 0;
} /* idz_realcomp__ */

