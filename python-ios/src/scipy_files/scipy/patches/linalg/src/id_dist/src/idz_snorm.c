/* idz_snorm.f -- translated by f2c (version 20100827).
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


/*       routine idz_snorm estimates the spectral norm */
/*       of a matrix specified by routines for applying the matrix */
/*       and its adjoint to arbitrary vectors. This routine uses */
/*       the power method with a random starting vector. */

/*       routine idz_diffsnorm estimates the spectral norm */
/*       of the difference between two matrices specified by routines */
/*       for applying the matrices and their adjoints */
/*       to arbitrary vectors. This routine uses */
/*       the power method with a random starting vector. */

/*       routine idz_enorm calculates the Euclidean norm of a vector. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idz_snorm_(integer *m, integer *n, S_fp matveca, 
	doublecomplex *p1a, doublecomplex *p2a, doublecomplex *p3a, 
	doublecomplex *p4a, S_fp matvec, doublecomplex *p1, doublecomplex *p2,
	 doublecomplex *p3, doublecomplex *p4, integer *its, doublereal *
	snorm, doublecomplex *v, doublecomplex *u)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublereal d__1;
    doublecomplex z__1, z__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublecomplex *);
    static integer k, n2;
    extern /* Subroutine */ int idz_enorm_(integer *, doublecomplex *, 
	    doublereal *);
    static integer it;
    static doublereal enorm;


/*       estimates the spectral norm of a matrix a specified */
/*       by a routine matvec for applying a to an arbitrary vector, */
/*       and by a routine matveca for applying a^* */
/*       to an arbitrary vector. This routine uses the power method */
/*       with a random starting vector. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       matveca -- routine which applies the adjoint of a */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matveca(m,x,n,y,p1a,p2a,p3a,p4a), */

/*                  where m is the length of x, */
/*                  x is the vector to which the adjoint of a */
/*                  is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the adjoint of a and x, */
/*                  and p1a, p2a, p3a, and p4a are user-specified */
/*                  parameters */
/*       p1a -- parameter to be passed to routine matveca */
/*       p2a -- parameter to be passed to routine matveca */
/*       p3a -- parameter to be passed to routine matveca */
/*       p4a -- parameter to be passed to routine matveca */
/*       matvec -- routine which applies the matrix a */
/*                 to an arbitrary vector; this routine must have */
/*                 a calling sequence of the form */

/*                 matvec(n,x,m,y,p1,p2,p3,p4), */

/*                 where n is the length of x, */
/*                 x is the vector to which a is to be applied, */
/*                 m is the length of y, */
/*                 y is the product of a and x, */
/*                 and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matvec */
/*       p2 -- parameter to be passed to routine matvec */
/*       p3 -- parameter to be passed to routine matvec */
/*       p4 -- parameter to be passed to routine matvec */
/*       its -- number of iterations of the power method to conduct */

/*       output: */
/*       snorm -- estimate of the spectral norm of a */
/*       v -- estimate of a normalized right singular vector */
/*            corresponding to the greatest singular value of a */

/*       work: */
/*       u -- must be at least m complex*16 elements long */

/*       reference: */
/*       Kuczynski and Wozniakowski, "Estimating the largest eigenvalue */
/*            by the power and Lanczos algorithms with a random start," */
/*            SIAM Journal on Matrix Analysis and Applications, */
/*            13 (4): 1992, 1094-1122. */



/*       Fill the real and imaginary parts of each entry */
/*       of the initial vector v with i.i.d. random variables */
/*       drawn uniformly from [-1,1]. */

    /* Parameter adjustments */
    --u;
    --v;

    /* Function Body */
    n2 = *n << 1;
    id_srand_(&n2, &v[1]);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	d__1 = 2.;
	z__2.r = d__1 * v[i__3].r, z__2.i = d__1 * v[i__3].i;
	z__1.r = z__2.r - 1, z__1.i = z__2.i;
	v[i__2].r = z__1.r, v[i__2].i = z__1.i;
    }


/*       Normalize v. */

/* k */
    idz_enorm_(n, &v[1], &enorm);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	z__1.r = v[i__3].r / enorm, z__1.i = v[i__3].i / enorm;
	v[i__2].r = z__1.r, v[i__2].i = z__1.i;
    }


/* k */
    i__1 = *its;
    for (it = 1; it <= i__1; ++it) {

/*         Apply a to v, obtaining u. */

	(*matvec)(n, &v[1], m, &u[1], p1, p2, p3, p4);

/*         Apply a^* to u, obtaining v. */

	(*matveca)(m, &u[1], n, &v[1], p1a, p2a, p3a, p4a);

/*         Normalize v. */

	idz_enorm_(n, &v[1], snorm);

	if (*snorm != 0.) {

	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		i__3 = k;
		i__4 = k;
		z__1.r = v[i__4].r / *snorm, z__1.i = v[i__4].i / *snorm;
		v[i__3].r = z__1.r, v[i__3].i = z__1.i;
	    }

/* k */
	}

	*snorm = sqrt(*snorm);

    }


/* it */
    return 0;
} /* idz_snorm__ */





/* Subroutine */ int idz_enorm_(integer *n, doublecomplex *v, doublereal *
	enorm)
{
    /* System generated locals */
    integer i__1, i__2;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);
    double sqrt(doublereal);

    /* Local variables */
    static integer k;


/*       computes the Euclidean norm of v, the square root */
/*       of the sum of the squares of the absolute values */
/*       of the entries of v. */

/*       input: */
/*       n -- length of v */
/*       v -- vector whose Euclidean norm is to be calculated */

/*       output: */
/*       enorm -- Euclidean norm of v */



    /* Parameter adjustments */
    --v;

    /* Function Body */
    *enorm = 0.;

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	d_cnjg(&z__3, &v[k]);
	z__2.r = v[i__2].r * z__3.r - v[i__2].i * z__3.i, z__2.i = v[i__2].r *
		 z__3.i + v[i__2].i * z__3.r;
	z__1.r = *enorm + z__2.r, z__1.i = z__2.i;
	*enorm = z__1.r;
    }

/* k */
    *enorm = sqrt(*enorm);


    return 0;
} /* idz_enorm__ */





/* Subroutine */ int idz_diffsnorm_(integer *m, integer *n, U_fp matveca, 
	doublecomplex *p1a, doublecomplex *p2a, doublecomplex *p3a, 
	doublecomplex *p4a, U_fp matveca2, doublecomplex *p1a2, doublecomplex 
	*p2a2, doublecomplex *p3a2, doublecomplex *p4a2, U_fp matvec, 
	doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, 
	doublecomplex *p4, U_fp matvec2, doublecomplex *p12, doublecomplex *
	p22, doublecomplex *p32, doublecomplex *p42, integer *its, doublereal 
	*snorm, doublecomplex *w)
{
    static integer iu, iv, lu, lv, lw, iu1, iu2, iv1, lu1, lu2, lv1, iv2, lv2;
    extern /* Subroutine */ int idz_diffsnorm0_(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , U_fp, doublecomplex *, doublecomplex *, doublecomplex *, 
	    doublecomplex *, U_fp, doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, U_fp, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *, 
	    doublereal *, doublecomplex *, doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublecomplex *);


/*       estimates the spectral norm of the difference between matrices */
/*       a and a2, where a is specified by routines matvec and matveca */
/*       for applying a and a^* to arbitrary vectors, */
/*       and a2 is specified by routines matvec2 and matveca2 */
/*       for applying a2 and (a2)^* to arbitrary vectors. */
/*       This routine uses the power method */
/*       with a random starting vector. */

/*       input: */
/*       m -- number of rows in a, as well as the number of rows in a2 */
/*       n -- number of columns in a, as well as the number of columns */
/*            in a2 */
/*       matveca -- routine which applies the adjoint of a */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matveca(m,x,n,y,p1a,p2a,p3a,p4a), */

/*                  where m is the length of x, */
/*                  x is the vector to which the adjoint of a */
/*                  is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the adjoint of a and x, */
/*                  and p1a, p2a, p3a, and p4a are user-specified */
/*                  parameters */
/*       p1a -- parameter to be passed to routine matveca */
/*       p2a -- parameter to be passed to routine matveca */
/*       p3a -- parameter to be passed to routine matveca */
/*       p4a -- parameter to be passed to routine matveca */
/*       matveca2 -- routine which applies the adjoint of a2 */
/*                   to an arbitrary vector; this routine must have */
/*                   a calling sequence of the form */

/*                   matveca2(m,x,n,y,p1a2,p2a2,p3a2,p4a2), */

/*                   where m is the length of x, */
/*                   x is the vector to which the adjoint of a2 */
/*                   is to be applied, */
/*                   n is the length of y, */
/*                   y is the product of the adjoint of a2 and x, */
/*                   and p1a2, p2a2, p3a2, and p4a2 are user-specified */
/*                   parameters */
/*       p1a2 -- parameter to be passed to routine matveca2 */
/*       p2a2 -- parameter to be passed to routine matveca2 */
/*       p3a2 -- parameter to be passed to routine matveca2 */
/*       p4a2 -- parameter to be passed to routine matveca2 */
/*       matvec -- routine which applies the matrix a */
/*                 to an arbitrary vector; this routine must have */
/*                 a calling sequence of the form */

/*                 matvec(n,x,m,y,p1,p2,p3,p4), */

/*                 where n is the length of x, */
/*                 x is the vector to which a is to be applied, */
/*                 m is the length of y, */
/*                 y is the product of a and x, */
/*                 and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matvec */
/*       p2 -- parameter to be passed to routine matvec */
/*       p3 -- parameter to be passed to routine matvec */
/*       p4 -- parameter to be passed to routine matvec */
/*       matvec2 -- routine which applies the matrix a2 */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matvec2(n,x,m,y,p12,p22,p32,p42), */

/*                  where n is the length of x, */
/*                  x is the vector to which a2 is to be applied, */
/*                  m is the length of y, */
/*                  y is the product of a2 and x, and */
/*                  p12, p22, p32, and p42 are user-specified parameters */
/*       p12 -- parameter to be passed to routine matvec2 */
/*       p22 -- parameter to be passed to routine matvec2 */
/*       p32 -- parameter to be passed to routine matvec2 */
/*       p42 -- parameter to be passed to routine matvec2 */
/*       its -- number of iterations of the power method to conduct */

/*       output: */
/*       snorm -- estimate of the spectral norm of a-a2 */

/*       work: */
/*       w -- must be at least 3*m+3*n complex*16 elements long */

/*       reference: */
/*       Kuczynski and Wozniakowski, "Estimating the largest eigenvalue */
/*            by the power and Lanczos algorithms with a random start," */
/*            SIAM Journal on Matrix Analysis and Applications, */
/*            13 (4): 1992, 1094-1122. */



/*       Allocate memory in w. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    lw = 0;

    iu = lw + 1;
    lu = *m;
    lw += lu;

    iu1 = lw + 1;
    lu1 = *m;
    lw += lu1;

    iu2 = lw + 1;
    lu2 = *m;
    lw += lu2;

    iv = lw + 1;
    lv = *n;
    ++lw;

    iv1 = lw + 1;
    lv1 = *n;
    lw += lv1;

    iv2 = lw + 1;
    lv2 = *n;
    lw += lv2;


    idz_diffsnorm0_(m, n, (U_fp)matveca, p1a, p2a, p3a, p4a, (U_fp)matveca2, 
	    p1a2, p2a2, p3a2, p4a2, (U_fp)matvec, p1, p2, p3, p4, (U_fp)
	    matvec2, p12, p22, p32, p42, its, snorm, &w[iu], &w[iu1], &w[iu2],
	     &w[iv], &w[iv1], &w[iv2]);


    return 0;
} /* idz_diffsnorm__ */





/* Subroutine */ int idz_diffsnorm0_(integer *m, integer *n, S_fp matveca, 
	doublecomplex *p1a, doublecomplex *p2a, doublecomplex *p3a, 
	doublecomplex *p4a, S_fp matveca2, doublecomplex *p1a2, doublecomplex 
	*p2a2, doublecomplex *p3a2, doublecomplex *p4a2, S_fp matvec, 
	doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, 
	doublecomplex *p4, S_fp matvec2, doublecomplex *p12, doublecomplex *
	p22, doublecomplex *p32, doublecomplex *p42, integer *its, doublereal 
	*snorm, doublecomplex *u, doublecomplex *u1, doublecomplex *u2, 
	doublecomplex *v, doublecomplex *v1, doublecomplex *v2)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    doublereal d__1;
    doublecomplex z__1, z__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublecomplex *);
    static integer k, n2;
    extern /* Subroutine */ int idz_enorm_(integer *, doublecomplex *, 
	    doublereal *);
    static integer it;
    static doublereal enorm;


/*       routine idz_diffsnorm serves as a memory wrapper */
/*       for the present routine. (Please see routine idz_diffsnorm */
/*       for further documentation.) */



/*       Fill the real and imaginary parts of each entry */
/*       of the initial vector v with i.i.d. random variables */
/*       drawn uniformly from [-1,1]. */

    /* Parameter adjustments */
    --u2;
    --u1;
    --u;
    --v2;
    --v1;
    --v;

    /* Function Body */
    n2 = *n << 1;
    id_srand_(&n2, &v[1]);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	d__1 = 2.;
	z__2.r = d__1 * v[i__3].r, z__2.i = d__1 * v[i__3].i;
	z__1.r = z__2.r - 1, z__1.i = z__2.i;
	v[i__2].r = z__1.r, v[i__2].i = z__1.i;
    }


/*       Normalize v. */

/* k */
    idz_enorm_(n, &v[1], &enorm);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	z__1.r = v[i__3].r / enorm, z__1.i = v[i__3].i / enorm;
	v[i__2].r = z__1.r, v[i__2].i = z__1.i;
    }


/* k */
    i__1 = *its;
    for (it = 1; it <= i__1; ++it) {

/*         Apply a and a2 to v, obtaining u1 and u2. */

	(*matvec)(n, &v[1], m, &u1[1], p1, p2, p3, p4);
	(*matvec2)(n, &v[1], m, &u2[1], p12, p22, p32, p42);

/*         Form u = u1-u2. */

	i__2 = *m;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = k;
	    i__4 = k;
	    i__5 = k;
	    z__1.r = u1[i__4].r - u2[i__5].r, z__1.i = u1[i__4].i - u2[i__5]
		    .i;
	    u[i__3].r = z__1.r, u[i__3].i = z__1.i;
	}

/*         Apply a^* and (a2)^* to u, obtaining v1 and v2. */

/* k */
	(*matveca)(m, &u[1], n, &v1[1], p1a, p2a, p3a, p4a);
	(*matveca2)(m, &u[1], n, &v2[1], p1a2, p2a2, p3a2, p4a2);

/*         Form v = v1-v2. */

	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = k;
	    i__4 = k;
	    i__5 = k;
	    z__1.r = v1[i__4].r - v2[i__5].r, z__1.i = v1[i__4].i - v2[i__5]
		    .i;
	    v[i__3].r = z__1.r, v[i__3].i = z__1.i;
	}

/*         Normalize v. */

/* k */
	idz_enorm_(n, &v[1], snorm);

	if (*snorm > 0.) {

	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		i__3 = k;
		i__4 = k;
		z__1.r = v[i__4].r / *snorm, z__1.i = v[i__4].i / *snorm;
		v[i__3].r = z__1.r, v[i__3].i = z__1.i;
	    }

/* k */
	}

	*snorm = sqrt(*snorm);

    }


/* it */
    return 0;
} /* idz_diffsnorm0__ */

