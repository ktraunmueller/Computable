/* det.f -- translated by f2c (version 20100827).
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

/*     Calculate determinant of square matrix */
/*     Author: Pearu Peterson, March 2002 */

/*     prefixes: d,z,s,c   (double,complex double,float,complex float) */
/*     suffixes: _c,_r     (column major order,row major order) */
/* Subroutine */ int ddet_c_(doublereal *det, doublereal *a, integer *n, 
	integer *piv, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int dgetrf_(integer *, integer *, doublereal *, 
	    integer *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument double*,double*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    dgetrf_(n, n, &a[a_offset], n, &piv[1], info);
    *det = 0.;
    if (*info != 0) {
	return 0;
    }
    *det = 1.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (piv[i__] != i__) {
	    *det = -(*det) * a[i__ + i__ * a_dim1];
	} else {
	    *det *= a[i__ + i__ * a_dim1];
	}
/* L10: */
    }
    return 0;
} /* ddet_c__ */

/* Subroutine */ int ddet_r_(doublereal *det, doublereal *a, integer *n, 
	integer *piv, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int ddet_c_(doublereal *, doublereal *, integer *
	    , integer *, integer *);

/* f2py intent(c,in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument double*,double*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    ddet_c_(det, &a[a_offset], n, &piv[1], info);
    return 0;
} /* ddet_r__ */

/* Subroutine */ int sdet_c_(real *det, real *a, integer *n, integer *piv, 
	integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int sgetrf_(integer *, integer *, real *, integer 
	    *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument float*,float*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    sgetrf_(n, n, &a[a_offset], n, &piv[1], info);
    *det = 0.f;
    if (*info != 0) {
	return 0;
    }
    *det = 1.f;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (piv[i__] != i__) {
	    *det = -(*det) * a[i__ + i__ * a_dim1];
	} else {
	    *det *= a[i__ + i__ * a_dim1];
	}
/* L10: */
    }
    return 0;
} /* sdet_c__ */

/* Subroutine */ int sdet_r_(real *det, real *a, integer *n, integer *piv, 
	integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int sdet_c_(real *, real *, integer *, integer *,
	     integer *);

/* f2py intent(c,in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument float*,float*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    sdet_c_(det, &a[a_offset], n, &piv[1], info);
    return 0;
} /* sdet_r__ */

/* Subroutine */ int zdet_c_(doublecomplex *det, doublecomplex *a, integer *
	n, integer *piv, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublecomplex z__1, z__2;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int zgetrf_(integer *, integer *, doublecomplex *,
	     integer *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument complex_double*,complex_double*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    zgetrf_(n, n, &a[a_offset], n, &piv[1], info);
    det->r = 0., det->i = 0.;
    if (*info != 0) {
	return 0;
    }
    det->r = 1., det->i = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (piv[i__] != i__) {
	    z__2.r = -det->r, z__2.i = -det->i;
	    i__2 = i__ + i__ * a_dim1;
	    z__1.r = z__2.r * a[i__2].r - z__2.i * a[i__2].i, z__1.i = z__2.r 
		    * a[i__2].i + z__2.i * a[i__2].r;
	    det->r = z__1.r, det->i = z__1.i;
	} else {
	    i__2 = i__ + i__ * a_dim1;
	    z__1.r = det->r * a[i__2].r - det->i * a[i__2].i, z__1.i = det->r 
		    * a[i__2].i + det->i * a[i__2].r;
	    det->r = z__1.r, det->i = z__1.i;
	}
/* L10: */
    }
    return 0;
} /* zdet_c__ */

/* Subroutine */ int zdet_r_(doublecomplex *det, doublecomplex *a, integer *
	n, integer *piv, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int zdet_c_(doublecomplex *, doublecomplex *, 
	    integer *, integer *, integer *);

/* f2py intent(c,in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument complex_double*,complex_double*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    zdet_c_(det, &a[a_offset], n, &piv[1], info);
    return 0;
} /* zdet_r__ */

/* Subroutine */ int cdet_c_(complex *det, complex *a, integer *n, integer *
	piv, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    complex q__1, q__2;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int cgetrf_(integer *, integer *, complex *, 
	    integer *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument complex_float*,complex_float*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    cgetrf_(n, n, &a[a_offset], n, &piv[1], info);
    det->r = 0.f, det->i = 0.f;
    if (*info != 0) {
	return 0;
    }
    det->r = 1.f, det->i = 0.f;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (piv[i__] != i__) {
	    q__2.r = -det->r, q__2.i = -det->i;
	    i__2 = i__ + i__ * a_dim1;
	    q__1.r = q__2.r * a[i__2].r - q__2.i * a[i__2].i, q__1.i = q__2.r 
		    * a[i__2].i + q__2.i * a[i__2].r;
	    det->r = q__1.r, det->i = q__1.i;
	} else {
	    i__2 = i__ + i__ * a_dim1;
	    q__1.r = det->r * a[i__2].r - det->i * a[i__2].i, q__1.i = det->r 
		    * a[i__2].i + det->i * a[i__2].r;
	    det->r = q__1.r, det->i = q__1.i;
	}
/* L10: */
    }
    return 0;
} /* cdet_c__ */

/* Subroutine */ int cdet_r_(complex *det, complex *a, integer *n, integer *
	piv, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int cdet_c_(complex *, complex *, integer *, 
	    integer *, integer *);

/* f2py intent(c,in,copy) :: a */
/* f2py intent(out) :: det,info */
/* f2py integer intent(hide,cache),depend(n),dimension(n) :: piv */
/* f2py integer intent(hide),depend(a) :: n = shape(a,0) */
/* f2py check(shape(a,0)==shape(a,1)) :: a */
/* f2py callprotoargument complex_float*,complex_float*,int*,int*,int* */
    /* Parameter adjustments */
    --piv;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    cdet_c_(det, &a[a_offset], n, &piv[1], info);
    return 0;
} /* cdet_r__ */

