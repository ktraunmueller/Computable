/* idz_house.f -- translated by f2c (version 20100827).
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


/*       routine idz_house calculates the vector and scalar */
/*       needed to apply the Householder tranformation reflecting */
/*       a given vector into its first component. */

/*       routine idz_houseapp applies a Householder matrix to a vector. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idz_houseapp_(integer *n, doublecomplex *vn, 
	doublecomplex *u, integer *ifrescal, doublereal *scal, doublecomplex *
	v)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer k;
    static doublereal sum;
    static doublecomplex fact;


/*       applies the Householder matrix */
/*       identity_matrix - scal * vn * adjoint(vn) */
/*       to the vector u, yielding the vector v; */

/*       scal = 2/(1 + |vn(2)|^2 + ... + |vn(n)|^2) */
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
	v[1].r = u[1].r, v[1].i = u[1].i;
	return 0;
    }


    if (*ifrescal == 1) {


/*         Calculate |vn(2)|^2 + ... + |vn(n)|^2. */

	sum = 0.;
	i__1 = *n;
	for (k = 2; k <= i__1; ++k) {
	    i__2 = k;
	    d_cnjg(&z__3, &vn[k]);
	    z__2.r = vn[i__2].r * z__3.r - vn[i__2].i * z__3.i, z__2.i = vn[
		    i__2].r * z__3.i + vn[i__2].i * z__3.r;
	    z__1.r = sum + z__2.r, z__1.i = z__2.i;
	    sum = z__1.r;
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


/*       Calculate fact = scal * adjoint(vn) * u. */

    fact.r = u[1].r, fact.i = u[1].i;

    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	d_cnjg(&z__3, &vn[k]);
	i__2 = k;
	z__2.r = z__3.r * u[i__2].r - z__3.i * u[i__2].i, z__2.i = z__3.r * u[
		i__2].i + z__3.i * u[i__2].r;
	z__1.r = fact.r + z__2.r, z__1.i = fact.i + z__2.i;
	fact.r = z__1.r, fact.i = z__1.i;
    }

/* k */
    z__1.r = *scal * fact.r, z__1.i = *scal * fact.i;
    fact.r = z__1.r, fact.i = z__1.i;


/*       Subtract fact*vn from u, yielding v. */

    z__1.r = u[1].r - fact.r, z__1.i = u[1].i - fact.i;
    v[1].r = z__1.r, v[1].i = z__1.i;

    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	i__4 = k;
	z__2.r = fact.r * vn[i__4].r - fact.i * vn[i__4].i, z__2.i = fact.r * 
		vn[i__4].i + fact.i * vn[i__4].r;
	z__1.r = u[i__3].r - z__2.r, z__1.i = u[i__3].i - z__2.i;
	v[i__2].r = z__1.r, v[i__2].i = z__1.i;
    }


/* k */
    return 0;
} /* idz_houseapp__ */





/* Subroutine */ int idz_house_(integer *n, doublecomplex *x, doublecomplex *
	css, doublecomplex *vn, doublereal *scal)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;
    doublecomplex z__1, z__2, z__3, z__4, z__5, z__6, z__7;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);
    double sqrt(doublereal), z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer k;
    static doublecomplex v1, x1;
    static doublereal sum, rss, test;
    static doublecomplex phase;


/*       constructs the vector vn with vn(1) = 1, */
/*       and the scalar scal, such that the obviously self-adjoint */
/*       H := identity_matrix - scal * vn * adjoint(vn) is unitary, */
/*       the absolute value of the first entry of Hx */
/*       is the root-sum-square of the entries of x, */
/*       and all other entries of Hx are zero */
/*       (H is the Householder matrix corresponding to x). */

/*       input: */
/*       n -- size of x and vn, though the indexing on vn goes */
/*            from 2 to n */
/*       x -- vector to reflect into its first component */

/*       output: */
/*       css -- root-sum-square of the entries of x * the phase of x(1) */
/*       vn -- entries 2 to n of the Householder vector vn; */
/*             vn(1) is assumed to be 1 */
/*       scal -- scalar multiplying vn * adjoint(vn); */

/*               scal = 2/(1 + |vn(2)|^2 + ... + |vn(n)|^2) */
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
    x1.r = x[1].r, x1.i = x[1].i;


/*       Get out of this routine if n = 1. */

    if (*n == 1) {
	css->r = x1.r, css->i = x1.i;
	*scal = 0.;
	return 0;
    }


/*       Calculate |x(2)|^2 + ... |x(n)|^2 */
/*       and the root-sum-square value of the entries in x. */


    sum = 0.;
    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	i__2 = k;
	d_cnjg(&z__3, &x[k]);
	z__2.r = x[i__2].r * z__3.r - x[i__2].i * z__3.i, z__2.i = x[i__2].r *
		 z__3.i + x[i__2].i * z__3.r;
	z__1.r = sum + z__2.r, z__1.i = z__2.i;
	sum = z__1.r;
    }


/*       Get out of this routine if sum = 0; */
/*       flag this case as such by setting v(2), ..., v(n) all to 0. */

/* k */
    if (sum == 0.) {

	css->r = x1.r, css->i = x1.i;
	i__1 = *n;
	for (k = 2; k <= i__1; ++k) {
	    i__2 = k;
	    vn[i__2].r = 0., vn[i__2].i = 0.;
	}
/* k */
	*scal = 0.;

	return 0;

    }


    d_cnjg(&z__3, &x1);
    z__2.r = x1.r * z__3.r - x1.i * z__3.i, z__2.i = x1.r * z__3.i + x1.i * 
	    z__3.r;
    z__1.r = z__2.r + sum, z__1.i = z__2.i;
    rss = z__1.r;
    rss = sqrt(rss);


/*       Determine the first component v1 */
/*       of the unnormalized Householder vector */
/*       v = x - phase(x1) * rss * (1 0 0 ... 0 0)^T. */

    if (x1.r == 0. && x1.i == 0.) {
	phase.r = 1., phase.i = 0.;
    }
    if (x1.r != 0. || x1.i != 0.) {
	d__1 = z_abs(&x1);
	z__1.r = x1.r / d__1, z__1.i = x1.i / d__1;
	phase.r = z__1.r, phase.i = z__1.i;
    }
    d_cnjg(&z__2, &phase);
    z__1.r = z__2.r * x1.r - z__2.i * x1.i, z__1.i = z__2.r * x1.i + z__2.i * 
	    x1.r;
    test = z__1.r;
    z__1.r = rss * phase.r, z__1.i = rss * phase.i;
    css->r = z__1.r, css->i = z__1.i;

/*       If test <= 0, then form x1-phase*rss directly, */
/*       since that expression cannot involve any cancellation. */

    if (test <= 0.) {
	z__2.r = rss * phase.r, z__2.i = rss * phase.i;
	z__1.r = x1.r - z__2.r, z__1.i = x1.i - z__2.i;
	v1.r = z__1.r, v1.i = z__1.i;
    }

/*       If test > 0, then use the fact that */
/*       x1-phase*rss = -phase*sum / ((phase)^* * x1 + rss), */
/*       in order to avoid potential cancellation. */

    if (test > 0.) {
	z__3.r = -phase.r, z__3.i = -phase.i;
	z__2.r = sum * z__3.r, z__2.i = sum * z__3.i;
	d_cnjg(&z__6, &phase);
	z__5.r = z__6.r * x1.r - z__6.i * x1.i, z__5.i = z__6.r * x1.i + 
		z__6.i * x1.r;
	z__4.r = z__5.r + rss, z__4.i = z__5.i;
	z_div(&z__1, &z__2, &z__4);
	v1.r = z__1.r, v1.i = z__1.i;
    }


/*       Compute the vector vn and the scalar scal such that vn(1) = 1 */
/*       in the Householder transformation */
/*       identity_matrix - scal * vn * adjoint(vn). */

    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	i__2 = k;
	z_div(&z__1, &x[k], &v1);
	vn[i__2].r = z__1.r, vn[i__2].i = z__1.i;
    }

/*       scal = 2 */
/*            / ( |vn(1)|^2 + |vn(2)|^2 + ... + |vn(n)|^2 ) */

/*            = 2 */
/*            / ( 1 + |vn(2)|^2 + ... + |vn(n)|^2 ) */

/*            = 2*|v(1)|^2 */
/*            / ( |v(1)|^2 + |v(1)*vn(2)|^2 + ... + |v(1)*vn(n)|^2 ) */

/*            = 2*|v(1)|^2 */
/*            / ( |v(1)|^2 + (|v(2)|^2 + ... + |v(n)|^2) ) */

/* k */
    d__1 = 2.;
    z__3.r = d__1 * v1.r, z__3.i = d__1 * v1.i;
    d_cnjg(&z__4, &v1);
    z__2.r = z__3.r * z__4.r - z__3.i * z__4.i, z__2.i = z__3.r * z__4.i + 
	    z__3.i * z__4.r;
    d_cnjg(&z__7, &v1);
    z__6.r = v1.r * z__7.r - v1.i * z__7.i, z__6.i = v1.r * z__7.i + v1.i * 
	    z__7.r;
    z__5.r = z__6.r + sum, z__5.i = z__6.i;
    z_div(&z__1, &z__2, &z__5);
    *scal = z__1.r;


    z__1.r = rss * phase.r, z__1.i = rss * phase.i;
    rss = z__1.r;


    return 0;
} /* idz_house__ */





/* Subroutine */ int idz_housemat_(integer *n, doublecomplex *vn, doublereal 
	*scal, doublecomplex *h__)
{
    /* System generated locals */
    integer h_dim1, h_offset, i__1, i__2, i__3, i__4;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer j, k;
    static doublecomplex factor1, factor2;


/*       fills h with the Householder matrix */
/*       identity_matrix - scal * vn * adjoint(vn). */

/*       input: */
/*       n -- size of vn and h, though the indexing of vn goes */
/*            from 2 to n */
/*       vn -- entries 2 to n of the vector vn; */
/*             vn(1) is assumed to be 1 */
/*       scal -- scalar multiplying vn * adjoint(vn) */

/*       output: */
/*       h -- identity_matrix - scal * vn * adjoint(vn) */



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
		i__3 = k + j * h_dim1;
		h__[i__3].r = 1., h__[i__3].i = 0.;
	    }
	    if (j != k) {
		i__3 = k + j * h_dim1;
		h__[i__3].r = 0., h__[i__3].i = 0.;
	    }

	}
/* k */
    }


/*       Subtract from h the matrix scal*vn*adjoint(vn). */

/* j */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {

	    if (j == 1) {
		factor1.r = 1., factor1.i = 0.;
	    }
	    if (j != 1) {
		i__3 = j;
		factor1.r = vn[i__3].r, factor1.i = vn[i__3].i;
	    }

	    if (k == 1) {
		factor2.r = 1., factor2.i = 0.;
	    }
	    if (k != 1) {
		d_cnjg(&z__1, &vn[k]);
		factor2.r = z__1.r, factor2.i = z__1.i;
	    }

	    i__3 = k + j * h_dim1;
	    i__4 = k + j * h_dim1;
	    z__3.r = *scal * factor1.r, z__3.i = *scal * factor1.i;
	    z__2.r = z__3.r * factor2.r - z__3.i * factor2.i, z__2.i = z__3.r 
		    * factor2.i + z__3.i * factor2.r;
	    z__1.r = h__[i__4].r - z__2.r, z__1.i = h__[i__4].i - z__2.i;
	    h__[i__3].r = z__1.r, h__[i__3].i = z__1.i;

	}
/* k */
    }


/* j */
    return 0;
} /* idz_housemat__ */

