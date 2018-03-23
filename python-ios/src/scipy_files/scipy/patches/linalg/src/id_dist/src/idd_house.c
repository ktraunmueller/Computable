/* idd_house.f -- translated by f2c (version 20100827).
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


/*       routine idd_house calculates the vector and scalar */
/*       needed to apply the Householder tranformation reflecting */
/*       a given vector into its first component. */

/*       routine idd_houseapp applies a Householder matrix to a vector. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idd_houseapp_(integer *n, doublereal *vn, doublereal *u,
	 integer *ifrescal, doublereal *scal, doublereal *v)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Local variables */
    static integer k;
    static doublereal sum, fact;


/*       applies the Householder matrix */
/*       identity_matrix - scal * vn * transpose(vn) */
/*       to the vector u, yielding the vector v; */

/*       scal = 2/(1 + vn(2)^2 + ... + vn(n)^2) */
/*       when vn(2), ..., vn(n) don't all vanish; */

/*       scal = 0 */
/*       when vn(2), ..., vn(n) do all vanish */
/*       (including when n = 1). */

/*       input: */
/*       n -- size of vn, u, and v, though the indexing on vn goes */
/*            from 2 to n */
/*       vn -- components 2 to n of the Householder vector vn; */
/*             vn(1) is assumed to be 1 */
/*       u -- vector to be transformed */
/*       ifrescal -- set to 1 to recompute scal from vn(2), ..., vn(n); */
/*                   set to 0 to use scal as input */
/*       scal -- see the entry for ifrescal in the decription */
/*               of the input */

/*       output: */
/*       scal -- see the entry for ifrescal in the decription */
/*               of the input */
/*       v -- result of applying the Householder matrix to u; */
/*            it's O.K. to have v be the same as u */
/*            in order to apply the matrix to the vector in place */

/*       reference: */
/*       Golub and Van Loan, "Matrix Computations," 3rd edition, */
/*            Johns Hopkins University Press, 1996, Chapter 5. */



/*       Get out of this routine if n = 1. */

    /* Parameter adjustments */
    --v;
    --u;
    vn -= 2;

    /* Function Body */
    if (*n == 1) {
	v[1] = u[1];
	return 0;
    }


    if (*ifrescal == 1) {


/*         Calculate (vn(2))^2 + ... + (vn(n))^2. */

	sum = 0.;
	i__1 = *n;
	for (k = 2; k <= i__1; ++k) {
/* Computing 2nd power */
	    d__1 = vn[k];
	    sum += d__1 * d__1;
	}


/*         Calculate scal. */

/* k */
	if (sum == 0.) {
	    *scal = 0.;
	}
	if (sum != 0.) {
	    *scal = 2 / (sum + 1);
	}


    }


/*       Calculate fact = scal * transpose(vn) * u. */

    fact = u[1];

    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	fact += vn[k] * u[k];
    }

/* k */
    fact *= *scal;


/*       Subtract fact*vn from u, yielding v. */

    v[1] = u[1] - fact;

    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	v[k] = u[k] - fact * vn[k];
    }


/* k */
    return 0;
} /* idd_houseapp__ */





/* Subroutine */ int idd_house_(integer *n, doublereal *x, doublereal *rss, 
	doublereal *vn, doublereal *scal)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer k;
    static doublereal v1, x1, sum;


/*       constructs the vector vn with vn(1) = 1 */
/*       and the scalar scal such that */
/*       H := identity_matrix - scal * vn * transpose(vn) is orthogonal */
/*       and Hx = +/- e_1 * the root-sum-square of the entries of x */
/*       (H is the Householder matrix corresponding to x). */

/*       input: */
/*       n -- size of x and vn, though the indexing on vn goes */
/*            from 2 to n */
/*       x -- vector to reflect into its first component */

/*       output: */
/*       rss -- first entry of the vector resulting from the application */
/*              of the Householder matrix to x; */
/*              its absolute value is the root-sum-square */
/*              of the entries of x */
/*       vn -- entries 2 to n of the Householder vector vn; */
/*             vn(1) is assumed to be 1 */
/*       scal -- scalar multiplying vn * transpose(vn); */

/*               scal = 2/(1 + vn(2)^2 + ... + vn(n)^2) */
/*               when vn(2), ..., vn(n) don't all vanish; */

/*               scal = 0 */
/*               when vn(2), ..., vn(n) do all vanish */
/*               (including when n = 1) */

/*       reference: */
/*       Golub and Van Loan, "Matrix Computations," 3rd edition, */
/*            Johns Hopkins University Press, 1996, Chapter 5. */



    /* Parameter adjustments */
    --x;
    vn -= 2;

    /* Function Body */
    x1 = x[1];


/*       Get out of this routine if n = 1. */

    if (*n == 1) {
	*rss = x1;
	*scal = 0.;
	return 0;
    }


/*       Calculate (x(2))^2 + ... (x(n))^2 */
/*       and the root-sum-square value of the entries in x. */


    sum = 0.;
    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
/* Computing 2nd power */
	d__1 = x[k];
	sum += d__1 * d__1;
    }


/*       Get out of this routine if sum = 0; */
/*       flag this case as such by setting v(2), ..., v(n) all to 0. */

/* k */
    if (sum == 0.) {

	*rss = x1;
	i__1 = *n;
	for (k = 2; k <= i__1; ++k) {
	    vn[k] = 0.;
	}
/* k */
	*scal = 0.;

	return 0;

    }


/* Computing 2nd power */
    d__1 = x1;
    *rss = d__1 * d__1 + sum;
    *rss = sqrt(*rss);


/*       Determine the first component v1 */
/*       of the unnormalized Householder vector */
/*       v = x - rss * (1 0 0 ... 0 0)^T. */

/*       If x1 <= 0, then form x1-rss directly, */
/*       since that expression cannot involve any cancellation. */

    if (x1 <= 0.) {
	v1 = x1 - *rss;
    }

/*       If x1 > 0, then use the fact that */
/*       x1-rss = -sum / (x1+rss), */
/*       in order to avoid potential cancellation. */

    if (x1 > 0.) {
	v1 = -sum / (x1 + *rss);
    }


/*       Compute the vector vn and the scalar scal such that vn(1) = 1 */
/*       in the Householder transformation */
/*       identity_matrix - scal * vn * transpose(vn). */

    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	vn[k] = x[k] / v1;
    }

/*       scal = 2 */
/*            / ( vn(1)^2 + vn(2)^2 + ... + vn(n)^2 ) */

/*            = 2 */
/*            / ( 1 + vn(2)^2 + ... + vn(n)^2 ) */

/*            = 2*v(1)^2 */
/*            / ( v(1)^2 + (v(1)*vn(2))^2 + ... + (v(1)*vn(n))^2 ) */

/*            = 2*v(1)^2 */
/*            / ( v(1)^2 + (v(2)^2 + ... + v(n)^2) ) */

/* k */
/* Computing 2nd power */
    d__1 = v1;
/* Computing 2nd power */
    d__2 = v1;
    *scal = d__1 * d__1 * 2 / (d__2 * d__2 + sum);


    return 0;
} /* idd_house__ */





/* Subroutine */ int idd_housemat_(integer *n, doublereal *vn, doublereal *
	scal, doublereal *h__)
{
    /* System generated locals */
    integer h_dim1, h_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;
    static doublereal factor1, factor2;


/*       fills h with the Householder matrix */
/*       identity_matrix - scal * vn * transpose(vn). */

/*       input: */
/*       n -- size of vn and h, though the indexing of vn goes */
/*            from 2 to n */
/*       vn -- entries 2 to n of the vector vn; */
/*             vn(1) is assumed to be 1 */
/*       scal -- scalar multiplying vn * transpose(vn) */

/*       output: */
/*       h -- identity_matrix - scal * vn * transpose(vn) */



/*       Fill h with the identity matrix. */

    /* Parameter adjustments */
    h_dim1 = *n;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;
    vn -= 2;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {

	    if (j == k) {
		h__[k + j * h_dim1] = 1.;
	    }
	    if (j != k) {
		h__[k + j * h_dim1] = 0.;
	    }

	}
/* k */
    }


/*       Subtract from h the matrix scal*vn*transpose(vn). */

/* j */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {

	    if (j == 1) {
		factor1 = 1.;
	    }
	    if (j != 1) {
		factor1 = vn[j];
	    }

	    if (k == 1) {
		factor2 = 1.;
	    }
	    if (k != 1) {
		factor2 = vn[k];
	    }

	    h__[k + j * h_dim1] -= *scal * factor1 * factor2;

	}
/* k */
    }


/* j */
    return 0;
} /* idd_housemat__ */

