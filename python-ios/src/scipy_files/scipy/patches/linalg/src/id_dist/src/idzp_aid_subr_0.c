/* idzp_aid_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzp_aid_(doublereal *eps, integer *m, integer *n, 
	doublecomplex *a, doublecomplex *work, integer *krank, integer *list, 
	doublecomplex *proj)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int idzp_aid0_(doublereal *, integer *, integer *
	    , doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *), idzp_aid1_(doublereal *, integer *, integer *, 
	    integer *, doublecomplex *, integer *, integer *, doublecomplex *)
	    ;
    static integer n2;
    extern /* Subroutine */ int idz_estrank_(doublereal *, integer *, 
	    integer *, doublecomplex *, doublecomplex *, integer *, 
	    doublecomplex *);
    static integer kranki;


/*       computes the ID of the matrix a, i.e., lists in list */
/*       the indices of krank columns of a such that */

/*       a(j,list(k))  =  a(j,list(k)) */

/*       for all j = 1, ..., m; k = 1, ..., krank, and */

/*                        krank */
/*       a(j,list(k))  =  Sigma  a(j,list(l)) * proj(l,k-krank)       (*) */
/*                         l=1 */

/*                     +  epsilon(j,k-krank) */

/*       for all j = 1, ..., m; k = krank+1, ..., n, */

/*       for some matrix epsilon dimensioned epsilon(m,n-krank) */
/*       such that the greatest singular value of epsilon */
/*       <= the greatest singular value of a * eps. */

/*       input: */
/*       eps -- precision to which the ID is to be computed */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       a -- matrix to be decomposed; the present routine does not */
/*            alter a */
/*       work -- initialization array that has been constructed */
/*               by routine idz_frmi */

/*       output: */
/*       krank -- numerical rank of a to precision eps */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd; */
/*               proj doubles as a work array in the present routine, so */
/*               proj must be at least n*(2*n2+1)+n2+1 complex*16 */
/*               elements long, where n2 is the greatest integer */
/*               less than or equal to m, such that n2 is */
/*               a positive integer power of two. */

/*       _N.B._: The algorithm used by this routine is randomized. */
/*               proj must be at least n*(2*n2+1)+n2+1 complex*16 */
/*               elements long, where n2 is the greatest integer */
/*               less than or equal to m, such that n2 is */
/*               a positive integer power of two. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Allocate memory in proj. */

    /* Parameter adjustments */
    --work;
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --proj;

    /* Function Body */
    n2 = (integer) work[2].r;


/*       Find the rank of a. */

    idz_estrank_(eps, m, n, &a[a_offset], &work[1], &kranki, &proj[1]);


    if (kranki == 0) {
	idzp_aid0_(eps, m, n, &a[a_offset], krank, &list[1], &proj[1], &proj[
		*m * *n + 1]);
    }

    if (kranki != 0) {
	idzp_aid1_(eps, &n2, n, &kranki, &proj[1], krank, &list[1], &proj[n2 
		* *n + 1]);
    }


    return 0;
} /* idzp_aid__ */

