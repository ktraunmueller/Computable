/* id_rtrans.f -- translated by f2c (version 20100827).
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

/*       this file contains the following user-callable routines: */


/*       routine idd_random_transf applies rapidly */
/*       a random orthogonal matrix to a user-supplied vector. */

/*       routine idd_random_transf_inverse applies rapidly */
/*       the inverse of the operator applied */
/*       by routine idd_random_transf. */

/*       routine idz_random_transf applies rapidly */
/*       a random unitary matrix to a user-supplied vector. */

/*       routine idz_random_transf_inverse applies rapidly */
/*       the inverse of the operator applied */
/*       by routine idz_random_transf. */

/*       routine idd_random_transf_init initializes data */
/*       for routines idd_random_transf and idd_random_transf_inverse. */

/*       routine idz_random_transf_init initializes data */
/*       for routines idz_random_transf and idz_random_transf_inverse. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */





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
} /* idd_random_transf_init_ */






/* Subroutine */ int idz_random_transf_init_(integer *nsteps, integer *n,
	doublereal *w, integer *keep)
{
    static integer ialbetas, lalbetas, iww, lww, iixs, lixs, ninire, igammas, 
	    lgammas;
    extern /* Subroutine */ int idz_random_transf_init0_(integer *, integer *
	    , doublereal *, doublecomplex *, doublereal *);


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
} /* idz_random_transf_init_ */






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
} /* idd_random_transf_ */






/* Subroutine */ int idd_random_transf_inverse_(doublereal *x, doublereal *y,
	 doublereal *w)
{
    static integer ialbetas, n, iww, iixs;
    extern /* Subroutine */ int idd_random_transf0_inv_(integer *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     integer *);
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
} /* idd_random_transf_inverse_ */






/* Subroutine */ int idz_random_transf_(doublecomplex *x, doublecomplex *y, 
	doublereal *w)
{
    static integer ialbetas, n;
    extern /* Subroutine */ int idz_random_transf0_(integer *, doublecomplex 
	    *, doublecomplex *, integer *, doublecomplex *, doublereal *,
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
} /* idz_random_transf_ */






/* Subroutine */ int idz_random_transf_inverse_(doublecomplex *x, 
	doublecomplex *y, doublereal *w)
{
    static integer ialbetas, n, iww, iixs, nsteps;
    extern /* Subroutine */ int idz_random_transf0_inv_(integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *,
	    doublereal *, doublecomplex *, integer *);
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
} /* idz_random_transf_inverse_ */






/* Subroutine */ int idd_random_transf0_inv_(integer *nsteps, doublereal *x, 
	doublereal *y, integer *n, doublereal *w2, doublereal *albetas, 
	integer *iixs)
{
    /* System generated locals */
    integer albetas_dim2, albetas_offset, iixs_dim1, iixs_offset, i_1;

    /* Local variables */
    static integer i_, j, ijk;
    extern /* Subroutine */ int idd_random_transf00_inv_(doublereal *, 
	    doublereal *, integer *, doublereal *, integer *);


/*       routine idd_random_transf_inverse serves as a memory wrapper */
/*       for the present routine; see routine idd_random_transf_inverse */
/*       for documentation. */

    /* Parameter adjustments */
    --x;
    --y;
    iixs_dim1 = *n;
    iixs_offset = 1 + iixs_dim1;
    iixs -= iixs_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;
    --w2;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	w2[i_] = x[i_];
/* L1200: */
    }

    for (ijk = *nsteps; ijk >= 1; --ijk) {

	idd_random_transf00_inv_(&w2[1], &y[1], n, &albetas[(ijk * 
		albetas_dim2 + 1 << 1) + 1], &iixs[ijk * iixs_dim1 + 1]);

	i_1 = *n;
	for (j = 1; j <= i_1; ++j) {

	    w2[j] = y[j];
/* L1400: */
	}
/* L2000: */
    }

    return 0;
} /* idd_random_transf0_inv_ */






/* Subroutine */ int idd_random_transf00_inv_(doublereal *x, doublereal *y, 
	integer *n, doublereal *albetas, integer *ixs)
{
    /* System generated locals */
    integer i_1;

    /* Local variables */
    static doublereal a, b;
    static integer i_, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform required */
/*       by routine idd_random_transf0_inv (please see the latter). */


/*        implement 2 \times 2 matrices */

    /* Parameter adjustments */
    --ixs;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {
	y[i_] = x[i_];
/* L1600: */
    }

    for (i_ = *n - 1; i_ >= 1; --i_) {

	alpha = albetas[(i_ << 1) + 1];
	beta = albetas[(i_ << 1) + 2];

	a = y[i_];
	b = y[i_ + 1];

	y[i_] = alpha * a - beta * b;
	y[i_ + 1] = beta * a + alpha * b;
/* L1800: */
    }

/*        implement the permutation */

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	j = ixs[i_];
	x[j] = y[i_];
/* L2600: */
    }

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	y[i_] = x[i_];
/* L2800: */
    }

    return 0;
} /* idd_random_transf00_inv_ */






/* Subroutine */ int idz_random_transf0_inv_(integer *nsteps, doublecomplex *
	x, doublecomplex *y, integer *n, doublecomplex *w2, doublereal *
	albetas, doublecomplex *gammas, integer *iixs)
{
    /* System generated locals */
    integer gammas_dim1, gammas_offset, albetas_dim2, albetas_offset, 
	    iixs_dim1, iixs_offset, i_1, i_2, i_3;

    /* Local variables */
    static integer i_, j, ijk;
    extern /* Subroutine */ int idz_random_transf00_inv_(doublecomplex *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *, 
	    integer *);


/*       routine idz_random_transf_inverse serves as a memory wrapper */
/*       for the present routine; please see routine */
/*       idz_random_transf_inverse for documentation. */

    /* Parameter adjustments */
    --x;
    --y;
    iixs_dim1 = *n;
    iixs_offset = 1 + iixs_dim1;
    iixs -= iixs_offset;
    gammas_dim1 = *n;
    gammas_offset = 1 + gammas_dim1;
    gammas -= gammas_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;
    --w2;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	i_2 = i_;
	i_3 = i_;
	w2[i_2].r = x[i_3].r, w2[i_2].i = x[i_3].i;
/* L1200: */
    }

    for (ijk = *nsteps; ijk >= 1; --ijk) {

	idz_random_transf00_inv_(&w2[1], &y[1], n, &albetas[(ijk * 
		albetas_dim2 + 1 << 1) + 1], &gammas[ijk * gammas_dim1 + 1], &
		iixs[ijk * iixs_dim1 + 1]);

	i_1 = *n;
	for (j = 1; j <= i_1; ++j) {

	    i_2 = j;
	    i_3 = j;
	    w2[i_2].r = y[i_3].r, w2[i_2].i = y[i_3].i;
/* L1400: */
	}
/* L2000: */
    }

    return 0;
} /* idz_random_transf0_inv_ */






/* Subroutine */ int idz_random_transf00_inv_(doublecomplex *x, 
	doublecomplex *y, integer *n, doublereal *albetas, doublecomplex *
	gammas, integer *ixs)
{
    /* System generated locals */
    integer i_1, i_2, i_3;
    doublecomplex z_1, z_2, z_3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static doublecomplex a, b;
    static integer i_, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform */
/*       required by routine idz_random_transf0_inv */
/*       (please see the latter). */

/*        implement 2 \times 2 matrices */

    /* Parameter adjustments */
    --ixs;
    --gammas;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    for (i_ = *n - 1; i_ >= 1; --i_) {

	alpha = albetas[(i_ << 1) + 1];
	beta = albetas[(i_ << 1) + 2];

	i_1 = i_;
	a.r = x[i_1].r, a.i = x[i_1].i;
	i_1 = i_ + 1;
	b.r = x[i_1].r, b.i = x[i_1].i;

	i_1 = i_;
	z_2.r = alpha * a.r, z_2.i = alpha * a.i;
	z_3.r = beta * b.r, z_3.i = beta * b.i;
	z_1.r = z_2.r - z_3.r, z_1.i = z_2.i - z_3.i;
	x[i_1].r = z_1.r, x[i_1].i = z_1.i;
	i_1 = i_ + 1;
	z_2.r = beta * a.r, z_2.i = beta * a.i;
	z_3.r = alpha * b.r, z_3.i = alpha * b.i;
	z_1.r = z_2.r + z_3.r, z_1.i = z_2.i + z_3.i;
	x[i_1].r = z_1.r, x[i_1].i = z_1.i;
/* L1600: */
    }

/*        implement the permutation */
/*        and divide by the random numbers on the unit circle */
/*        (or, equivalently, multiply by their conjugates) */

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	j = ixs[i_];
	i_2 = j;
	i_3 = i_;
	d_cnjg(&z_2, &gammas[i_]);
	z_1.r = x[i_3].r * z_2.r - x[i_3].i * z_2.i, z_1.i = x[i_3].r *
		 z_2.i + x[i_3].i * z_2.r;
	y[i_2].r = z_1.r, y[i_2].i = z_1.i;
/* L1800: */
    }

    return 0;
} /* idz_random_transf00_inv_ */






/* Subroutine */ int idd_random_transf0_(integer *nsteps, doublereal *x, 
	doublereal *y, integer *n, doublereal *w2, doublereal *albetas, 
	doublereal *iixs)
{
    /* System generated locals */
    integer albetas_dim2, albetas_offset, iixs_dim1, iixs_offset, i_1, i_2;

    /* Local variables */
    static integer i_, j;
    extern /* Subroutine */ int idd_random_transf00_(doublereal *, 
	    doublereal *, integer *, doublereal *, integer *);
    static integer ijk;


/*       routine idd_random_transf serves as a memory wrapper */
/*       for the present routine; please see routine idd_random_transf */
/*       for documentation. */

    /* Parameter adjustments */
    --x;
    --y;
    iixs_dim1 = *n;
    iixs_offset = 1 + iixs_dim1;
    iixs -= iixs_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;
    --w2;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	w2[i_] = x[i_];
/* L1200: */
    }

    i_1 = *nsteps;
    for (ijk = 1; ijk <= i_1; ++ijk) {

	idd_random_transf00_(&w2[1], &y[1], n, &albetas[(ijk * albetas_dim2 
		+ 1 << 1) + 1], &iixs[ijk * iixs_dim1 + 1]);

	i_2 = *n;
	for (j = 1; j <= i_2; ++j) {

	    w2[j] = y[j];
/* L1400: */
	}
/* L2000: */
    }

    return 0;
} /* idd_random_transf0_ */






/* Subroutine */ int idd_random_transf00_(doublereal *x, doublereal *y, 
	integer *n, doublereal *albetas, integer *ixs)
{
    /* System generated locals */
    integer i_1;

    /* Local variables */
    static doublereal a, b;
    static integer i_, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform */
/*       required by routine idd_random_transf0 (please see the latter). */

/*        implement the permutation */

    /* Parameter adjustments */
    --ixs;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	j = ixs[i_];
	y[i_] = x[j];
/* L1600: */
    }

/*        implement 2 \times 2 matrices */

    i_1 = *n - 1;
    for (i_ = 1; i_ <= i_1; ++i_) {

	alpha = albetas[(i_ << 1) + 1];
	beta = albetas[(i_ << 1) + 2];

	a = y[i_];
	b = y[i_ + 1];

	y[i_] = alpha * a + beta * b;
	y[i_ + 1] = -beta * a + alpha * b;
/* L1800: */
    }

    return 0;
} /* idd_random_transf00_ */






/* Subroutine */ int idz_random_transf_init0_(integer *nsteps, integer *n, 
	doublereal *albetas, doublecomplex *gammas, doublereal *ixs)
{
    /* System generated locals */
    integer albetas_dim2, albetas_offset, ixs_dim1, ixs_offset, gammas_dim1, 
	    gammas_offset, i_1;

    /* Local variables */
    extern /* Subroutine */ int idz_random_transf_init00_(integer *, 
	    doublereal *, doublereal *, integer *);
    static integer ijk;


/*       routine idz_random_transf_init serves as a memory wrapper */
/*       for the present routine; please see routine */
/*       idz_random_transf_init for documentation. */

    /* Parameter adjustments */
    ixs_dim1 = *n;
    ixs_offset = 1 + ixs_dim1;
    ixs -= ixs_offset;
    gammas_dim1 = *n;
    gammas_offset = 1 + gammas_dim1;
    gammas -= gammas_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;

    /* Function Body */
    i_1 = *nsteps;
    for (ijk = 1; ijk <= i_1; ++ijk) {

	idz_random_transf_init00_(n, &albetas[(ijk * albetas_dim2 + 1 << 1) 
		+ 1], &gammas[ijk * gammas_dim1 + 1], &ixs[ijk * ixs_dim1 + 1]
		);
/* L2000: */
    }
    return 0;
} /* idz_random_transf_init0_ */






/* Subroutine */ int idz_random_transf_init00_(integer *n, doublereal *
	albetas, doublereal *gammas, integer *ixs)
{
    /* System generated locals */
    integer i_1;
    doublereal d_1, d_2;

    /* Builtin functions */
    double atan(doublereal), sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer ifrepeat;
    static doublereal d_;
    static integer i_;
    extern /* Subroutine */ int id_randperm_(integer *, integer *);
    static doublereal done, twopi;


/*       constructs one stage of the random transform */
/*       initialized by routine idz_random_transf_init0 */
/*       (please see the latter). */

    /* Parameter adjustments */
    --ixs;
    --gammas;
    albetas -= 3;

    /* Function Body */
    done = 1.;
    twopi = atan(done) * 8;

/*        construct the random permutation */

    ifrepeat = 0;
    id_randperm_(n, &ixs[1]);

/*        construct the random variables */

    i_1 = *n << 1;
    id_srand_(&i_1, &albetas[3]);
    i_1 = *n << 1;
    id_srand_(&i_1, &gammas[1]);

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	albetas[(i_ << 1) + 1] = albetas[(i_ << 1) + 1] * 2 - 1;
	albetas[(i_ << 1) + 2] = albetas[(i_ << 1) + 2] * 2 - 1;
	gammas[(i_ << 1) - 1] = gammas[(i_ << 1) - 1] * 2 - 1;
	gammas[i_ * 2] = gammas[i_ * 2] * 2 - 1;
/* L1300: */
    }

/*        construct the random 2 \times 2 transformations */

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

/* Computing 2nd power */
	d_1 = albetas[(i_ << 1) + 1];
/* Computing 2nd power */
	d_2 = albetas[(i_ << 1) + 2];
	d_ = d_1 * d_1 + d_2 * d_2;
	d_ = 1 / sqrt(d_);
	albetas[(i_ << 1) + 1] *= d_;
	albetas[(i_ << 1) + 2] *= d_;
/* L1400: */
    }

/*        construct the random multipliers on the unit circle */

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

/* Computing 2nd power */
	d_1 = gammas[(i_ << 1) - 1];
/* Computing 2nd power */
	d_2 = gammas[i_ * 2];
	d_ = d_1 * d_1 + d_2 * d_2;
	d_ = 1 / sqrt(d_);

/*        fill the real part */

	gammas[(i_ << 1) - 1] *= d_;

/*        fill the imaginary part */

	gammas[i_ * 2] *= d_;
/* L1500: */
    }

    return 0;
} /* idz_random_transf_init00_ */






/* Subroutine */ int idz_random_transf0_(integer *nsteps, doublecomplex *x, 
	doublecomplex *y, integer *n, doublecomplex *w2, doublereal *albetas, 
	doublereal *gammas, doublereal *iixs)
{
    /* System generated locals */
    integer gammas_dim1, gammas_offset, albetas_dim2, albetas_offset, 
	    iixs_dim1, iixs_offset, i_1, i_2, i_3, i_4;

    /* Local variables */
    static integer i_, j;
    extern /* Subroutine */ int idz_random_transf00_(doublecomplex *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *, 
	    integer *);
    static integer ijk;


/*       routine idz_random_transf serves as a memory wrapper */
/*       for the present routine; please see routine idz_random_transf */
/*       for documentation. */

    /* Parameter adjustments */
    --x;
    --y;
    iixs_dim1 = *n;
    iixs_offset = 1 + iixs_dim1;
    iixs -= iixs_offset;
    gammas_dim1 = *n;
    gammas_offset = 1 + gammas_dim1;
    gammas -= gammas_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;
    --w2;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	i_2 = i_;
	i_3 = i_;
	w2[i_2].r = x[i_3].r, w2[i_2].i = x[i_3].i;
/* L1200: */
    }

    i_1 = *nsteps;
    for (ijk = 1; ijk <= i_1; ++ijk) {

	idz_random_transf00_(&w2[1], &y[1], n, &albetas[(ijk * albetas_dim2 
		+ 1 << 1) + 1], &gammas[ijk * gammas_dim1 + 1], &iixs[ijk * 
		iixs_dim1 + 1]);
	i_2 = *n;
	for (j = 1; j <= i_2; ++j) {

	    i_3 = j;
	    i_4 = j;
	    w2[i_3].r = y[i_4].r, w2[i_3].i = y[i_4].i;
/* L1400: */
	}
/* L2000: */
    }

    return 0;
} /* idz_random_transf0_ */






/* Subroutine */ int idz_random_transf00_(doublecomplex *x, doublecomplex *y,
	 integer *n, doublereal *albetas, doublecomplex *gammas, integer *ixs)
{
    /* System generated locals */
    integer i_1, i_2, i_3, i_4;
    doublereal d_1;
    doublecomplex z_1, z_2, z_3;

    /* Local variables */
    static doublecomplex a, b;
    static integer i_, j;
    static doublereal beta, alpha;


/*       implements one step of the random transform */
/*       required by routine idz_random_transf0 (please see the latter). */

/*        implement the permutation */
/*        and multiply by the random numbers */
/*        on the unit circle */

    /* Parameter adjustments */
    --ixs;
    --gammas;
    albetas -= 3;
    --y;
    --x;

    /* Function Body */
    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	j = ixs[i_];
	i_2 = i_;
	i_3 = j;
	i_4 = i_;
	z_1.r = x[i_3].r * gammas[i_4].r - x[i_3].i * gammas[i_4].i, 
		z_1.i = x[i_3].r * gammas[i_4].i + x[i_3].i * gammas[i_4]
		.r;
	y[i_2].r = z_1.r, y[i_2].i = z_1.i;
/* L1600: */
    }

/*        implement 2 \times 2 matrices */

    i_1 = *n - 1;
    for (i_ = 1; i_ <= i_1; ++i_) {

	alpha = albetas[(i_ << 1) + 1];
	beta = albetas[(i_ << 1) + 2];

	i_2 = i_;
	a.r = y[i_2].r, a.i = y[i_2].i;
	i_2 = i_ + 1;
	b.r = y[i_2].r, b.i = y[i_2].i;

	i_2 = i_;
	z_2.r = alpha * a.r, z_2.i = alpha * a.i;
	z_3.r = beta * b.r, z_3.i = beta * b.i;
	z_1.r = z_2.r + z_3.r, z_1.i = z_2.i + z_3.i;
	y[i_2].r = z_1.r, y[i_2].i = z_1.i;
	i_2 = i_ + 1;
	d_1 = -beta;
	z_2.r = d_1 * a.r, z_2.i = d_1 * a.i;
	z_3.r = alpha * b.r, z_3.i = alpha * b.i;
	z_1.r = z_2.r + z_3.r, z_1.i = z_2.i + z_3.i;
	y[i_2].r = z_1.r, y[i_2].i = z_1.i;
/* L2600: */
    }

    return 0;
} /* idz_random_transf00_ */






/* Subroutine */ int idd_random_transf_init0_(integer *nsteps, integer *n, 
	doublereal *albetas, doublereal *ixs)
{
    /* System generated locals */
    integer albetas_dim2, albetas_offset, ixs_dim1, ixs_offset, i_1;

    /* Local variables */
    extern /* Subroutine */ int idd_random_transf_init00_(integer *, 
	    doublereal *, integer *);
    static integer ijk;


/*       routine idd_random_transf_init serves as a memory wrapper */
/*       for the present routine; please see routine */
/*       idd_random_transf_init for documentation. */

    /* Parameter adjustments */
    ixs_dim1 = *n;
    ixs_offset = 1 + ixs_dim1;
    ixs -= ixs_offset;
    albetas_dim2 = *n;
    albetas_offset = 1 + 2 * (1 + albetas_dim2);
    albetas -= albetas_offset;

    /* Function Body */
    i_1 = *nsteps;
    for (ijk = 1; ijk <= i_1; ++ijk) {

	idd_random_transf_init00_(n, &albetas[(ijk * albetas_dim2 + 1 << 1) 
		+ 1], &ixs[ijk * ixs_dim1 + 1]);
/* L2000: */
    }
    return 0;
} /* idd_random_transf_init0_ */






/* Subroutine */ int idd_random_transf_init00_(integer *n, doublereal *
	albetas, integer *ixs)
{
    /* System generated locals */
    integer i_1;
    doublereal d_1, d_2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer ifrepeat;
    static doublereal d_;
    static integer i_;
    extern /* Subroutine */ int id_randperm_(integer *, integer *);


/*       constructs one stage of the random transform */
/*       initialized by routine idd_random_transf_init0 */
/*       (please see the latter). */

/*        construct the random permutation */

    /* Parameter adjustments */
    --ixs;
    albetas -= 3;

    /* Function Body */
    ifrepeat = 0;
    id_randperm_(n, &ixs[1]);

/*        construct the random variables */

    i_1 = *n << 1;
    id_srand_(&i_1, &albetas[3]);

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

	albetas[(i_ << 1) + 1] = albetas[(i_ << 1) + 1] * 2 - 1;
	albetas[(i_ << 1) + 2] = albetas[(i_ << 1) + 2] * 2 - 1;
/* L1300: */
    }

/*        construct the random 2 \times 2 transformations */

    i_1 = *n;
    for (i_ = 1; i_ <= i_1; ++i_) {

/* Computing 2nd power */
	d_1 = albetas[(i_ << 1) + 1];
/* Computing 2nd power */
	d_2 = albetas[(i_ << 1) + 2];
	d_ = d_1 * d_1 + d_2 * d_2;
	d_ = 1 / sqrt(d_);
	albetas[(i_ << 1) + 1] *= d_;
	albetas[(i_ << 1) + 2] *= d_;
/* L1400: */
    }
    return 0;
} /* idd_random_transf_init00_ */

