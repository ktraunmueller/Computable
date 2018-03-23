/* getbreak.f -- translated by f2c (version 20100827).
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

/* -*- fortran -*- */
/*     GetBreak */
doublereal sgetbreak_(void)
{
    /* System generated locals */
    real ret_val, r__1;

    /* Local variables */
    static real eps;
    extern doublereal slamch_(char *, ftnlen);


/*     Get breakdown parameter tolerance; for the test routine, */
/*     set to machine precision. */


    eps = slamch_("EPS", (ftnlen)3);
/* Computing 2nd power */
    r__1 = eps;
    ret_val = r__1 * r__1;

    return ret_val;

} /* sgetbreak_ */

/*     END FUNCTION sGETBREAK */
doublereal dgetbreak_(void)
{
    /* System generated locals */
    doublereal ret_val, d__1;

    /* Local variables */
    static doublereal eps;
    extern doublereal dlamch_(char *, ftnlen);


/*     Get breakdown parameter tolerance; for the test routine, */
/*     set to machine precision. */


    eps = dlamch_("EPS", (ftnlen)3);
/* Computing 2nd power */
    d__1 = eps;
    ret_val = d__1 * d__1;

    return ret_val;

} /* dgetbreak_ */

/*     END FUNCTION dGETBREAK */
doublereal cgetbreak_(void)
{
    /* System generated locals */
    real ret_val, r__1;

    /* Local variables */
    static real eps;
    extern doublereal slamch_(char *, ftnlen);


/*     Get breakdown parameter tolerance; for the test routine, */
/*     set to machine precision. */


    eps = slamch_("EPS", (ftnlen)3);
/* Computing 2nd power */
    r__1 = eps;
    ret_val = r__1 * r__1;

    return ret_val;

} /* cgetbreak_ */

/*     END FUNCTION cGETBREAK */
doublereal zgetbreak_(void)
{
    /* System generated locals */
    doublereal ret_val, d__1;

    /* Local variables */
    static doublereal eps;
    extern doublereal dlamch_(char *, ftnlen);


/*     Get breakdown parameter tolerance; for the test routine, */
/*     set to machine precision. */


    eps = dlamch_("EPS", (ftnlen)3);
/* Computing 2nd power */
    d__1 = eps;
    ret_val = d__1 * d__1;

    return ret_val;

} /* zgetbreak_ */

