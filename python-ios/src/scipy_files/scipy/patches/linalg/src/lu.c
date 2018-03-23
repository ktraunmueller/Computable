/* lu.f -- translated by f2c (version 20100827).
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

/* Table of constant values */

static integer c__1 = 1;
static integer c_n1 = -1;

/*     Calculate LU decomposition of a matrix */
/*     Author: Pearu Peterson, March 2002 */

/*     prefixes: d,z,s,c   (double,complex double,float,complex float) */
/*     suffixes: _c,_r     (column major order,row major order) */
/* Subroutine */ int dlu_c_(doublereal *p, doublereal *l, doublereal *u, 
	doublereal *a, integer *m, integer *n, integer *k, integer *piv, 
	integer *info, integer *permute_l__, integer *m1)
{
    /* System generated locals */
    integer l_dim1, l_offset, u_dim1, u_offset, a_dim1, a_offset, p_dim1, 
	    p_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int dgetrf_(integer *, integer *, doublereal *, 
	    integer *, integer *, integer *), dlaswp_(integer *, doublereal *,
	     integer *, integer *, integer *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: info */
/* f2py integer intent(hide,cache),depend(k),dimension(k) :: piv */
/* f2py integer intent(hide),depend(a) :: m = shape(a,0) */
/* f2py integer intent(hide),depend(a) :: n = shape(a,1) */
/* f2py integer intent(hide),depend(m,n) :: k = (m<n?m:n) */
/* f2py intent(out) :: p,l,u */
/* f2py integer optional,intent(in):: permute_l = 0 */
/* f2py integer intent(hide),depend(permute_l,m) :: m1 = (permute_l?1:m) */
/* f2py depend(m1) :: p */
/* f2py callprotoargument double*,double*,double*,double*,int*,int*,int*,int*,int*,int*,int* */
    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --piv;
    u_dim1 = *k;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    l_dim1 = *m;
    l_offset = 1 + l_dim1;
    l -= l_offset;
    p_dim1 = *m1;
    p_offset = 1 + p_dim1;
    p -= p_offset;

    /* Function Body */
    dgetrf_(m, n, &a[a_offset], m, &piv[1], info);
    if (*info < 0) {
	return 0;
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    if (j <= *k) {
		if (i__ == j) {
		    l[i__ + j * l_dim1] = 1.;
		} else if (i__ > j) {
		    l[i__ + j * l_dim1] = a[i__ + j * a_dim1];
		}
	    }
	    if (i__ <= *k && i__ <= j) {
		u[i__ + j * u_dim1] = a[i__ + j * a_dim1];
	    }
/* L10: */
	}
/* L20: */
    }
    if (*permute_l__ != 0) {
	dlaswp_(k, &l[l_offset], m, &c__1, k, &piv[1], &c_n1);
    } else {
	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    p[i__ + i__ * p_dim1] = 1.;
/* L25: */
	}
	dlaswp_(m, &p[p_offset], m, &c__1, k, &piv[1], &c_n1);
    }
    return 0;
} /* dlu_c__ */

/* Subroutine */ int zlu_c_(doublereal *p, doublecomplex *l, doublecomplex *
	u, doublecomplex *a, integer *m, integer *n, integer *k, integer *piv,
	 integer *info, integer *permute_l__, integer *m1)
{
    /* System generated locals */
    integer l_dim1, l_offset, u_dim1, u_offset, a_dim1, a_offset, p_dim1, 
	    p_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int dlaswp_(integer *, doublereal *, integer *, 
	    integer *, integer *, integer *, integer *), zgetrf_(integer *, 
	    integer *, doublecomplex *, integer *, integer *, integer *), 
	    zlaswp_(integer *, doublecomplex *, integer *, integer *, integer 
	    *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: info */
/* f2py integer intent(hide,cache),depend(k),dimension(k) :: piv */
/* f2py integer intent(hide),depend(a) :: m = shape(a,0) */
/* f2py integer intent(hide),depend(a) :: n = shape(a,1) */
/* f2py integer intent(hide),depend(m,n) :: k = (m<n?m:n) */
/* f2py intent(out) :: p,l,u */
/* f2py integer optional,intent(in):: permute_l = 0 */
/* f2py integer intent(hide),depend(permute_l,m) :: m1 = (permute_l?1:m) */
/* f2py depend(m1) :: p */
/* f2py callprotoargument double*,complex_double*,complex_double*,complex_double*,int*,int*,int*,int*,int*,int
*,int* */
    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --piv;
    u_dim1 = *k;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    l_dim1 = *m;
    l_offset = 1 + l_dim1;
    l -= l_offset;
    p_dim1 = *m1;
    p_offset = 1 + p_dim1;
    p -= p_offset;

    /* Function Body */
    zgetrf_(m, n, &a[a_offset], m, &piv[1], info);
    if (*info < 0) {
	return 0;
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    if (j <= *k) {
		if (i__ == j) {
		    i__3 = i__ + j * l_dim1;
		    l[i__3].r = 1., l[i__3].i = 0.;
		} else if (i__ > j) {
		    i__3 = i__ + j * l_dim1;
		    i__4 = i__ + j * a_dim1;
		    l[i__3].r = a[i__4].r, l[i__3].i = a[i__4].i;
		}
	    }
	    if (i__ <= *k && i__ <= j) {
		i__3 = i__ + j * u_dim1;
		i__4 = i__ + j * a_dim1;
		u[i__3].r = a[i__4].r, u[i__3].i = a[i__4].i;
	    }
/* L10: */
	}
/* L20: */
    }
    if (*permute_l__ != 0) {
	zlaswp_(k, &l[l_offset], m, &c__1, k, &piv[1], &c_n1);
    } else {
	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    p[i__ + i__ * p_dim1] = 1.;
/* L25: */
	}
	dlaswp_(m, &p[p_offset], m, &c__1, k, &piv[1], &c_n1);
    }
    return 0;
} /* zlu_c__ */

/* Subroutine */ int slu_c_(real *p, real *l, real *u, real *a, integer *m, 
	integer *n, integer *k, integer *piv, integer *info, integer *
	permute_l__, integer *m1)
{
    /* System generated locals */
    integer l_dim1, l_offset, u_dim1, u_offset, a_dim1, a_offset, p_dim1, 
	    p_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int sgetrf_(integer *, integer *, real *, integer 
	    *, integer *, integer *), slaswp_(integer *, real *, integer *, 
	    integer *, integer *, integer *, integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: info */
/* f2py integer intent(hide,cache),depend(k),dimension(k) :: piv */
/* f2py integer intent(hide),depend(a) :: m = shape(a,0) */
/* f2py integer intent(hide),depend(a) :: n = shape(a,1) */
/* f2py integer intent(hide),depend(m,n) :: k = (m<n?m:n) */
/* f2py intent(out) :: p,l,u */
/* f2py integer optional,intent(in):: permute_l = 0 */
/* f2py integer intent(hide),depend(permute_l,m) :: m1 = (permute_l?1:m) */
/* f2py depend(m1) :: p */
/* f2py callprotoargument float*,float*,float*,float*,int*,int*,int*,int*,int*,int*,int* */
    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --piv;
    u_dim1 = *k;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    l_dim1 = *m;
    l_offset = 1 + l_dim1;
    l -= l_offset;
    p_dim1 = *m1;
    p_offset = 1 + p_dim1;
    p -= p_offset;

    /* Function Body */
    sgetrf_(m, n, &a[a_offset], m, &piv[1], info);
    if (*info < 0) {
	return 0;
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    if (j <= *k) {
		if (i__ == j) {
		    l[i__ + j * l_dim1] = 1.f;
		} else if (i__ > j) {
		    l[i__ + j * l_dim1] = a[i__ + j * a_dim1];
		}
	    }
	    if (i__ <= *k && i__ <= j) {
		u[i__ + j * u_dim1] = a[i__ + j * a_dim1];
	    }
/* L10: */
	}
/* L20: */
    }
    if (*permute_l__ != 0) {
	slaswp_(k, &l[l_offset], m, &c__1, k, &piv[1], &c_n1);
    } else {
	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    p[i__ + i__ * p_dim1] = 1.f;
/* L25: */
	}
	slaswp_(m, &p[p_offset], m, &c__1, k, &piv[1], &c_n1);
    }
    return 0;
} /* slu_c__ */

/* Subroutine */ int clu_c_(real *p, complex *l, complex *u, complex *a, 
	integer *m, integer *n, integer *k, integer *piv, integer *info, 
	integer *permute_l__, integer *m1)
{
    /* System generated locals */
    integer l_dim1, l_offset, u_dim1, u_offset, a_dim1, a_offset, p_dim1, 
	    p_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int cgetrf_(integer *, integer *, complex *, 
	    integer *, integer *, integer *), claswp_(integer *, complex *, 
	    integer *, integer *, integer *, integer *, integer *), slaswp_(
	    integer *, real *, integer *, integer *, integer *, integer *, 
	    integer *);

/* f2py intent(in,copy) :: a */
/* f2py intent(out) :: info */
/* f2py integer intent(hide,cache),depend(k),dimension(k) :: piv */
/* f2py integer intent(hide),depend(a) :: m = shape(a,0) */
/* f2py integer intent(hide),depend(a) :: n = shape(a,1) */
/* f2py integer intent(hide),depend(m,n) :: k = (m<n?m:n) */
/* f2py intent(out) :: p,l,u */
/* f2py integer optional,intent(in):: permute_l = 0 */
/* f2py integer intent(hide),depend(permute_l,m) :: m1 = (permute_l?1:m) */
/* f2py depend(m1) :: p */
/* f2py callprotoargument float*,complex_float*,complex_float*,complex_float*,int*,int*,int*,int*,int*,int*,in
t* */
    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --piv;
    u_dim1 = *k;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    l_dim1 = *m;
    l_offset = 1 + l_dim1;
    l -= l_offset;
    p_dim1 = *m1;
    p_offset = 1 + p_dim1;
    p -= p_offset;

    /* Function Body */
    cgetrf_(m, n, &a[a_offset], m, &piv[1], info);
    if (*info < 0) {
	return 0;
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    if (j <= *k) {
		if (i__ == j) {
		    i__3 = i__ + j * l_dim1;
		    l[i__3].r = 1.f, l[i__3].i = 0.f;
		} else if (i__ > j) {
		    i__3 = i__ + j * l_dim1;
		    i__4 = i__ + j * a_dim1;
		    l[i__3].r = a[i__4].r, l[i__3].i = a[i__4].i;
		}
	    }
	    if (i__ <= *k && i__ <= j) {
		i__3 = i__ + j * u_dim1;
		i__4 = i__ + j * a_dim1;
		u[i__3].r = a[i__4].r, u[i__3].i = a[i__4].i;
	    }
/* L10: */
	}
/* L20: */
    }
    if (*permute_l__ != 0) {
	claswp_(k, &l[l_offset], m, &c__1, k, &piv[1], &c_n1);
    } else {
	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    p[i__ + i__ * p_dim1] = 1.f;
/* L25: */
	}
	slaswp_(m, &p[p_offset], m, &c__1, k, &piv[1], &c_n1);
    }
    return 0;
} /* clu_c__ */

