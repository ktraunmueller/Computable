/* dfft_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int zfftb_(integer *n, doublereal *c__, doublereal *wsave)
{
    static integer iw1, iw2;
    extern /* Subroutine */ int zfftb1_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *);

    /* Parameter adjustments */
    --wsave;
    --c__;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    iw1 = *n + *n + 1;
    iw2 = iw1 + *n + *n;
    zfftb1_(n, &c__[1], &wsave[1], &wsave[iw1], &wsave[iw2]);
    return 0;
} /* zfftb_ */

