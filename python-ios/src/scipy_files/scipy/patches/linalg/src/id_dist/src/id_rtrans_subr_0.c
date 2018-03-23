/* id_rtrans_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_random_transf_init_(integer *nsteps, integer *n, 
	doublereal *w, integer *keep)
{
    static integer ialbetas, lalbetas, iww, lww, iixs, lixs, ninire;
    extern /* Subroutine */ int idd_random_transf_init0_(integer *, integer *
	    , doublereal *, doublereal *);


/*       prepares and stores in array w the data used */
/*       by the routines idd_random_transf and idd_random_transf_inverse */
/*       to apply rapidly a random orthogonal matrix */
/*       to an arbitrary user-specified vector. */

/*       input: */
/*       nsteps -- the degree of randomness of the operator */
/*                 to be applied */
/*       n -- the size of the matrix to be applied */

/*       output: */
/*       w -- the first keep elements of w contain all the data */
/*            to be used by routines idd_random_tranf */
/*            and idd_random_transf_inverse. Please note that */
/*            the number of elements used by the present routine */
/*            is also equal to keep. This array should be at least */
/*            3*nsteps*n + 2*n + n/4 + 50 real*8 elements long. */
/*       keep - the number of elements in w actually used */
/*              by the present routine; keep is also the number */
/*              of elements that must not be changed between the call */
/*              to this routine and subsequent calls to routines */
/*              idd_random_transf and idd_random_transf_inverse. */


/*        . . . allocate memory */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    ninire = 2;

    ialbetas = 10;
    lalbetas = (*n << 1) * *nsteps + 10;

    iixs = ialbetas + lalbetas;
    lixs = *n * *nsteps / ninire + 10;

    iww = iixs + lixs;
    lww = (*n << 1) + *n / 4 + 20;

    *keep = iww + lww;

    w[1] = ialbetas + .1f;
    w[2] = iixs + .1f;
    w[3] = *nsteps + .1f;
    w[4] = iww + .1f;
    w[5] = *n + .1f;

    idd_random_transf_init0_(nsteps, n, &w[ialbetas], &w[iixs]);

    return 0;
} /* idd_random_transf_init__ */






/* Subroutine */ int idz_random_transf_init_(integer *nsteps, integer *n, 
	doublereal *w, integer *keep)
{
    static integer ialbetas, lalbetas, iww, lww, iixs, lixs, ninire, igammas, 
	    lgammas;
    extern /* Subroutine */ int idz_random_transf_init0_(integer *, integer *
	    , doublereal *, doublereal *, doublereal *);


/*       prepares and stores in array w the data used */
/*       by routines idz_random_transf and idz_random_transf_inverse */
/*       to apply rapidly a random unitary matrix */
/*       to an arbitrary user-specified vector. */

/*       input: */
/*       nsteps -- the degree of randomness of the operator */
/*                 to be applied */
/*       n -- the size of the matrix to be applied */

/*       output: */
/*       w -- the first keep elements of w contain all the data */
/*            to be used by routines idz_random_transf */
/*            and idz_random_transf_inverse. Please note that */
/*            the number of elements used by the present routine */
/*            is also equal to keep. This array should be at least */
/*            5*nsteps*n + 2*n + n/4 + 60 real*8 elements long. */
/*       keep - the number of elements in w actually used */
/*              by the present routine; keep is also the number */
/*              of elements that must not be changed between the call */
/*              to this routine and subsequent calls to routines */
/*              idz_random_transf and idz_random_transf_inverse. */


/*        . . . allocate memory */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    ninire = 2;

    ialbetas = 10;
    lalbetas = (*n << 1) * *nsteps + 10;

    igammas = ialbetas + lalbetas;
    lgammas = (*n << 1) * *nsteps + 10;

    iixs = igammas + lgammas;
    lixs = *n * *nsteps / ninire + 10;

    iww = iixs + lixs;
    lww = (*n << 1) + *n / 4 + 20;

    *keep = iww + lww;

    w[1] = ialbetas + .1f;
    w[2] = iixs + .1f;
    w[3] = *nsteps + .1f;
    w[4] = iww + .1f;
    w[5] = *n + .1f;
    w[6] = igammas + .1f;

    idz_random_transf_init0_(nsteps, n, &w[ialbetas], &w[igammas], &w[iixs]);

    return 0;
} /* idz_random_transf_init__ */






/* Subroutine */ int idd_random_transf_(doublereal *x, doublereal *y, 
	doublereal *w)
{
    static integer ialbetas, n;
    extern /* Subroutine */ int idd_random_transf0_(integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *)
	    ;
    static integer iww, iixs, nsteps;


/*       applies rapidly a random orthogonal matrix */
/*       to the user-specified real vector x, */
/*       using the data in array w stored there by a preceding */
/*       call to routine idd_random_transf_init. */

/*       input: */
/*       x -- the vector of length n to which the random matrix is */
/*            to be applied */
/*       w -- array containing all initialization data */

/*       output: */
/*       y -- the result of applying the random matrix to x */


/*        . . . allocate memory */

    /* Parameter adjustments */
    --w;
    --y;
    --x;

    /* Function Body */
    ialbetas = (integer) w[1];
    iixs = (integer) w[2];
    nsteps = (integer) w[3];
    iww = (integer) w[4];
    n = (integer) w[5];

    idd_random_transf0_(&nsteps, &x[1], &y[1], &n, &w[iww], &w[ialbetas], &w[
	    iixs]);

    return 0;
} /* idd_random_transf__ */






/* Subroutine */ int idd_random_transf_inverse_(doublereal *x, doublereal *y,
	 doublereal *w)
{
    static integer ialbetas, n, iww, iixs;
    extern /* Subroutine */ int idd_random_transf0_inv_(integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *);
    static integer nsteps;


/*       applies rapidly a random orthogonal matrix */
/*       to the user-specified real vector x, */
/*       using the data in array w stored there by a preceding */
/*       call to routine idd_random_transf_init. */
/*       The transformation applied by the present routine is */
/*       the inverse of the transformation applied */
/*       by routine idd_random_transf. */

/*       input: */
/*       x -- the vector of length n to which the random matrix is */
/*            to be applied */
/*       w -- array containing all initialization data */

/*       output: */
/*       y -- the result of applying the random matrix to x */


/*        . . . allocate memory */

    /* Parameter adjustments */
    --w;
    --y;
    --x;

    /* Function Body */
    ialbetas = (integer) w[1];
    iixs = (integer) w[2];
    nsteps = (integer) w[3];
    iww = (integer) w[4];
    n = (integer) w[5];

    idd_random_transf0_inv_(&nsteps, &x[1], &y[1], &n, &w[iww], &w[ialbetas],
	     &w[iixs]);

    return 0;
} /* idd_random_transf_inverse__ */






/* Subroutine */ int idz_random_transf_(doublecomplex *x, doublecomplex *y, 
	doublereal *w)
{
    static integer ialbetas, n;
    extern /* Subroutine */ int idz_random_transf0_(integer *, doublecomplex 
	    *, doublecomplex *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static integer iww, iixs, nsteps, igammas;


/*       applies rapidly a random unitary matrix */
/*       to the user-specified vector x, */
/*       using the data in array w stored there by a preceding */
/*       call to routine idz_random_transf_init. */

/*       input: */
/*       x -- the vector of length n to which the random matrix is */
/*            to be applied */
/*       w -- array containing all initialization data */

/*       output: */
/*       y -- the result of applying the random matrix to x */


/*        . . . allocate memory */

    /* Parameter adjustments */
    --w;
    --y;
    --x;

    /* Function Body */
    ialbetas = (integer) w[1];
    iixs = (integer) w[2];
    nsteps = (integer) w[3];
    iww = (integer) w[4];
    n = (integer) w[5];
    igammas = (integer) w[6];

    idz_random_transf0_(&nsteps, &x[1], &y[1], &n, &w[iww], &w[ialbetas], &w[
	    igammas], &w[iixs]);

    return 0;
} /* idz_random_transf__ */






/* Subroutine */ int idz_random_transf_inverse_(doublecomplex *x, 
	doublecomplex *y, doublereal *w)
{
    static integer ialbetas, n, iww, iixs, nsteps;
    extern /* Subroutine */ int idz_random_transf0_inv_(integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    static integer igammas;


/*       applies rapidly a random unitary matrix */
/*       to the user-specified vector x, */
/*       using the data in array w stored there by a preceding */
/*       call to routine idz_random_transf_init. */
/*       The transformation applied by the present routine is */
/*       the inverse of the transformation applied */
/*       by routine idz_random_transf. */

/*       input: */
/*       x -- the vector of length n to which the random matrix is */
/*            to be applied */
/*       w -- array containing all initialization data */

/*       output: */
/*       y -- the result of applying the random matrix to x */


/*        . . . allocate memory */

    /* Parameter adjustments */
    --w;
    --y;
    --x;

    /* Function Body */
    ialbetas = (integer) w[1];
    iixs = (integer) w[2];
    nsteps = (integer) w[3];
    iww = (integer) w[4];
    n = (integer) w[5];
    igammas = (integer) w[6];

    idz_random_transf0_inv_(&nsteps, &x[1], &y[1], &n, &w[iww], &w[ialbetas],
	     &w[igammas], &w[iixs]);

    return 0;
} /* idz_random_transf_inverse__ */

