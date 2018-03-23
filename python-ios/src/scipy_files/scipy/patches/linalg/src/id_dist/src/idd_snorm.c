/* idd_snorm.f -- translated by f2c (version 20100827).
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


/*       routine idd_snorm estimates the spectral norm */
/*       of a matrix specified by routines for applying the matrix */
/*       and its transpose to arbitrary vectors. This routine uses */
/*       the power method with a random starting vector. */

/*       routine idd_diffsnorm estimates the spectral norm */
/*       of the difference between two matrices specified by routines */
/*       for applying the matrices and their transposes */
/*       to arbitrary vectors. This routine uses */
/*       the power method with a random starting vector. */

/*       routine idd_enorm calculates the Euclidean norm of a vector. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idd_snorm_(integer *m, integer *n, S_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	S_fp matvec, doublereal *p1, doublereal *p2, doublereal *p3, 
	doublereal *p4, integer *its, doublereal *snorm, doublereal *v, 
	doublereal *u)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer k;
    extern /* Subroutine */ int idd_enorm_(integer *, doublereal *, 
	    doublereal *);
    static integer it;
    static doublereal enorm;


/*       estimates the spectral norm of a matrix a specified */
/*       by a routine matvec for applying a to an arbitrary vector, */
/*       and by a routine matvect for applying a^T */
/*       to an arbitrary vector. This routine uses the power method */
/*       with a random starting vector. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       matvect -- routine which applies the transpose of a */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matvect(m,x,n,y,p1t,p2t,p3t,p4t), */

/*                  where m is the length of x, */
/*                  x is the vector to which the transpose of a */
/*                  is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the transpose of a and x, */
/*                  and p1t, p2t, p3t, and p4t are user-specified */
/*                  parameters */
/*       p1t -- parameter to be passed to routine matvect */
/*       p2t -- parameter to be passed to routine matvect */
/*       p3t -- parameter to be passed to routine matvect */
/*       p4t -- parameter to be passed to routine matvect */
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
/*       u -- must be at least m real*8 elements long */

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
    id_srand_(n, &v[1]);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	v[k] = v[k] * 2 - 1;
    }


/*       Normalize v. */

/* k */
    idd_enorm_(n, &v[1], &enorm);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	v[k] /= enorm;
    }


/* k */
    i__1 = *its;
    for (it = 1; it <= i__1; ++it) {

/*         Apply a to v, obtaining u. */

	(*matvec)(n, &v[1], m, &u[1], p1, p2, p3, p4);

/*         Apply a^T to u, obtaining v. */

	(*matvect)(m, &u[1], n, &v[1], p1t, p2t, p3t, p4t);

/*         Normalize v. */

	idd_enorm_(n, &v[1], snorm);

	if (*snorm > 0.) {

	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		v[k] /= *snorm;
	    }

/* k */
	}

	*snorm = sqrt(*snorm);

    }


/* it */
    return 0;
} /* idd_snorm__ */





/* Subroutine */ int idd_enorm_(integer *n, doublereal *v, doublereal *enorm)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer k;


/*       computes the Euclidean norm of v, the square root */
/*       of the sum of the squares of the entries of v. */

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
/* Computing 2nd power */
	d__1 = v[k];
	*enorm += d__1 * d__1;
    }

/* k */
    *enorm = sqrt(*enorm);


    return 0;
} /* idd_enorm__ */





/* Subroutine */ int idd_diffsnorm_(integer *m, integer *n, U_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	U_fp matvect2, doublereal *p1t2, doublereal *p2t2, doublereal *p3t2, 
	doublereal *p4t2, U_fp matvec, doublereal *p1, doublereal *p2, 
	doublereal *p3, doublereal *p4, U_fp matvec2, doublereal *p12, 
	doublereal *p22, doublereal *p32, doublereal *p42, integer *its, 
	doublereal *snorm, doublereal *w)
{
    static integer iu, iv, lu, lv, lw, iu1, iu2, iv1, lu1, lu2, lv1, iv2, lv2;
    extern /* Subroutine */ int idd_diffsnorm0_(integer *, integer *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, U_fp, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);


/*       estimates the spectral norm of the difference between matrices */
/*       a and a2, where a is specified by routines matvec and matvect */
/*       for applying a and a^T to arbitrary vectors, */
/*       and a2 is specified by routines matvec2 and matvect2 */
/*       for applying a2 and (a2)^T to arbitrary vectors. */
/*       This routine uses the power method */
/*       with a random starting vector. */

/*       input: */
/*       m -- number of rows in a, as well as the number of rows in a2 */
/*       n -- number of columns in a, as well as the number of columns */
/*            in a2 */
/*       matvect -- routine which applies the transpose of a */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matvect(m,x,n,y,p1t,p2t,p3t,p4t), */

/*                  where m is the length of x, */
/*                  x is the vector to which the transpose of a */
/*                  is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the transpose of a and x, */
/*                  and p1t, p2t, p3t, and p4t are user-specified */
/*                  parameters */
/*       p1t -- parameter to be passed to routine matvect */
/*       p2t -- parameter to be passed to routine matvect */
/*       p3t -- parameter to be passed to routine matvect */
/*       p4t -- parameter to be passed to routine matvect */
/*       matvect2 -- routine which applies the transpose of a2 */
/*                   to an arbitrary vector; this routine must have */
/*                   a calling sequence of the form */

/*                   matvect2(m,x,n,y,p1t2,p2t2,p3t2,p4t2), */

/*                   where m is the length of x, */
/*                   x is the vector to which the transpose of a2 */
/*                   is to be applied, */
/*                   n is the length of y, */
/*                   y is the product of the transpose of a2 and x, */
/*                   and p1t2, p2t2, p3t2, and p4t2 are user-specified */
/*                   parameters */
/*       p1t2 -- parameter to be passed to routine matvect2 */
/*       p2t2 -- parameter to be passed to routine matvect2 */
/*       p3t2 -- parameter to be passed to routine matvect2 */
/*       p4t2 -- parameter to be passed to routine matvect2 */
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
/*       w -- must be at least 3*m+3*n real*8 elements long */

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


    idd_diffsnorm0_(m, n, (U_fp)matvect, p1t, p2t, p3t, p4t, (U_fp)matvect2, 
	    p1t2, p2t2, p3t2, p4t2, (U_fp)matvec, p1, p2, p3, p4, (U_fp)
	    matvec2, p12, p22, p32, p42, its, snorm, &w[iu], &w[iu1], &w[iu2],
	     &w[iv], &w[iv1], &w[iv2]);


    return 0;
} /* idd_diffsnorm__ */





/* Subroutine */ int idd_diffsnorm0_(integer *m, integer *n, S_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	S_fp matvect2, doublereal *p1t2, doublereal *p2t2, doublereal *p3t2, 
	doublereal *p4t2, S_fp matvec, doublereal *p1, doublereal *p2, 
	doublereal *p3, doublereal *p4, S_fp matvec2, doublereal *p12, 
	doublereal *p22, doublereal *p32, doublereal *p42, integer *its, 
	doublereal *snorm, doublereal *u, doublereal *u1, doublereal *u2, 
	doublereal *v, doublereal *v1, doublereal *v2)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublereal *);
    static integer k;
    extern /* Subroutine */ int idd_enorm_(integer *, doublereal *, 
	    doublereal *);
    static integer it;
    static doublereal enorm;


/*       routine idd_diffsnorm serves as a memory wrapper */
/*       for the present routine. (Please see routine idd_diffsnorm */
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
    id_srand_(n, &v[1]);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	v[k] = v[k] * 2 - 1;
    }


/*       Normalize v. */

/* k */
    idd_enorm_(n, &v[1], &enorm);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	v[k] /= enorm;
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
	    u[k] = u1[k] - u2[k];
	}

/*         Apply a^T and (a2)^T to u, obtaining v1 and v2. */

/* k */
	(*matvect)(m, &u[1], n, &v1[1], p1t, p2t, p3t, p4t);
	(*matvect2)(m, &u[1], n, &v2[1], p1t2, p2t2, p3t2, p4t2);

/*         Form v = v1-v2. */

	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {
	    v[k] = v1[k] - v2[k];
	}

/*         Normalize v. */

/* k */
	idd_enorm_(n, &v[1], snorm);

	if (*snorm > 0.) {

	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		v[k] /= *snorm;
	    }

/* k */
	}

	*snorm = sqrt(*snorm);

    }


/* it */
    return 0;
} /* idd_diffsnorm0__ */

