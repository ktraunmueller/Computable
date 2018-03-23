/* idzp_aid_subr_3.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_estrank_(doublereal *eps, integer *m, integer *n, 
	doublecomplex *a, doublecomplex *w, integer *krank, doublecomplex *ra)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    static integer n2, ira, lra, lra2;
    extern /* Subroutine */ int idz_estrank0_(doublereal *, integer *, 
	    integer *, doublecomplex *, doublecomplex *, integer *, integer *,
	     doublecomplex *, doublecomplex *, doublecomplex *);
    static integer irat, lrat, iscal, lscal;


/*       estimates the numerical rank krank of an m x n matrix a */
/*       to precision eps. This routine applies n2 random vectors */
/*       to a, obtaining ra, where n2 is the greatest integer */
/*       less than or equal to m such that n2 is a positive integer */
/*       power of two. krank is typically about 8 higher than */
/*       the actual numerical rank. */

/*       input: */
/*       eps -- precision defining the numerical rank */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       a -- matrix whose rank is to be estimated */
/*       w -- initialization array that has been constructed */
/*            by routine idz_frmi */

/*       output: */
/*       krank -- estimate of the numerical rank of a; */
/*                this routine returns krank = 0 when the actual */
/*                numerical rank is nearly full (that is, */
/*                greater than n - 8 or n2 - 8) */
/*       ra -- product of an n2 x m random matrix and the m x n matrix */
/*             a, where n2 is the greatest integer less than or equal */
/*             to m such that n2 is a positive integer power of two; */
/*             ra doubles as a work array in the present routine, and so */
/*             must be at least n*n2+(n+1)*(n2+1) complex*16 elements */
/*             long */

/*       _N.B._: ra must be at least n*n2+(n2+1)*(n+1) complex*16 */
/*               elements long for use in the present routine */
/*               (here, n2 is the greatest integer less than or equal */
/*               to m, such that n2 is a positive integer power of two). */
/*               This routine returns krank = 0 when the actual */
/*               numerical rank is nearly full. */



/*       Extract from the array w initialized by routine idz_frmi */
/*       the greatest integer less than or equal to m that is */
/*       a positive integer power of two. */

    /* Parameter adjustments */
    --w;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ra;

    /* Function Body */
    n2 = (integer) w[2].r;


/*       Allocate memory in ra. */

    lra = 0;

    ira = lra + 1;
    lra2 = n2 * *n;
    lra += lra2;

    irat = lra + 1;
    lrat = *n * (n2 + 1);
    lra += lrat;

    iscal = lra + 1;
    lscal = n2 + 1;
    lra += lscal;

    idz_estrank0_(eps, m, n, &a[a_offset], &w[1], &n2, krank, &ra[ira], &ra[
	    irat], &ra[iscal]);


    return 0;
} /* idz_estrank__ */

