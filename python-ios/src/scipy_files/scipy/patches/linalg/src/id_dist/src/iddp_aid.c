/* iddp_aid.f -- translated by f2c (version 20100827).
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


/*       routine iddp_aid computes the ID, to a specified precision, */
/*       of an arbitrary matrix. This routine is randomized. */

/*       routine idd_estrank estimates the numerical rank, */
/*       to a specified precision, of an arbitrary matrix. */
/*       This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int iddp_aid_(doublereal *eps, integer *m, integer *n, 
	doublereal *a, doublereal *work, integer *krank, integer *list, 
	doublereal *proj)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int iddp_aid0_(doublereal *, integer *, integer *
	    , doublereal *, integer *, integer *, doublereal *, doublereal *),
	     iddp_aid1_(doublereal *, integer *, integer *, integer *, 
	    doublereal *, integer *, integer *, doublereal *);
    static integer n2;
    extern /* Subroutine */ int idd_estrank_(doublereal *, integer *, 
	    integer *, doublereal *, doublereal *, integer *, doublereal *);
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
/*               by routine idd_frmi */

/*       output: */
/*       krank -- numerical rank of a to precision eps */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd; */
/*               proj doubles as a work array in the present routine, so */
/*               proj must be at least n*(2*n2+1)+n2+1 real*8 elements */
/*               long, where n2 is the greatest integer less than */
/*               or equal to m, such that n2 is a positive integer */
/*               power of two. */

/*       _N.B._: The algorithm used by this routine is randomized. */
/*               proj must be at least n*(2*n2+1)+n2+1 real*8 elements */
/*               long, where n2 is the greatest integer less than */
/*               or equal to m, such that n2 is a positive integer */
/*               power of two. */

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
    n2 = (integer) work[2];


/*       Find the rank of a. */

    idd_estrank_(eps, m, n, &a[a_offset], &work[1], &kranki, &proj[1]);


    if (kranki == 0) {
	iddp_aid0_(eps, m, n, &a[a_offset], krank, &list[1], &proj[1], &proj[
		*m * *n + 1]);
    }

    if (kranki != 0) {
	iddp_aid1_(eps, &n2, n, &kranki, &proj[1], krank, &list[1], &proj[n2 
		* *n + 1]);
    }


    return 0;
} /* iddp_aid__ */





/* Subroutine */ int iddp_aid0_(doublereal *eps, integer *m, integer *n, 
	doublereal *a, integer *krank, integer *list, doublereal *proj, 
	doublereal *rnorms)
{
    /* System generated locals */
    integer a_dim1, a_offset, proj_dim1, proj_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int iddp_id_(doublereal *, integer *, integer *, 
	    doublereal *, integer *, integer *, doublereal *);


/*       uses routine iddp_id to ID a without modifying its entries */
/*       (in contrast to the usual behavior of iddp_id). */

/*       input: */
/*       eps -- precision of the decomposition to be constructed */
/*       m -- first dimension of a */
/*       n -- second dimension of a */

/*       output: */
/*       krank -- numerical rank of the ID */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns in a; */
/*               proj doubles as a work array in the present routine, so */
/*               must be at least m*n real*8 elements long */

/*       work: */
/*       rnorms -- must be at least n real*8 elements long */

/*       _N.B._: proj must be at least m*n real*8 elements long */



/*       Copy a into proj. */

    /* Parameter adjustments */
    --rnorms;
    proj_dim1 = *m;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    proj[j + k * proj_dim1] = a[j + k * a_dim1];
	}
/* j */
    }


/*       ID proj. */

/* k */
    iddp_id_(eps, m, n, &proj[proj_offset], krank, &list[1], &rnorms[1]);


    return 0;
} /* iddp_aid0__ */





/* Subroutine */ int iddp_aid1_(doublereal *eps, integer *n2, integer *n, 
	integer *kranki, doublereal *proj, integer *krank, integer *list, 
	doublereal *rnorms)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int iddp_id_(doublereal *, integer *, integer *, 
	    doublereal *, integer *, integer *, doublereal *);


/*       IDs the uppermost kranki x n block of the n2 x n matrix */
/*       input as proj. */

/*       input: */
/*       eps -- precision of the decomposition to be constructed */
/*       n2 -- first dimension of proj as input */
/*       n -- second dimension of proj as input */
/*       kranki -- number of rows to extract from proj */
/*       proj -- matrix containing the kranki x n block to be ID'd */

/*       output: */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd */
/*       krank -- numerical rank of the ID */
/*       list -- indices of the columns in the ID */

/*       work: */
/*       rnorms -- must be at least n real*8 elements long */



/*       Move the uppermost kranki x n block of the n2 x n matrix proj */
/*       to the beginning of proj. */

    /* Parameter adjustments */
    --rnorms;
    --list;
    --proj;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *kranki;
	for (j = 1; j <= i__2; ++j) {
	    proj[j + *kranki * (k - 1)] = proj[j + *n2 * (k - 1)];
	}
/* j */
    }


/*       ID proj. */

/* k */
    iddp_id_(eps, kranki, n, &proj[1], krank, &list[1], &rnorms[1]);


    return 0;
} /* iddp_aid1__ */





/* Subroutine */ int idd_estrank_(doublereal *eps, integer *m, integer *n, 
	doublereal *a, doublereal *w, integer *krank, doublereal *ra)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    static integer n2, ira, lra, lra2;
    extern /* Subroutine */ int idd_estrank0_(doublereal *, integer *, 
	    integer *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *);
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
/*            by routine idd_frmi */

/*       output: */
/*       krank -- estimate of the numerical rank of a; */
/*                this routine returns krank = 0 when the actual */
/*                numerical rank is nearly full (that is, */
/*                greater than n - 8 or n2 - 8) */
/*       ra -- product of an n2 x m random matrix and the m x n matrix */
/*             a, where n2 is the greatest integer less than or equal */
/*             to m such that n2 is a positive integer power of two; */
/*             ra doubles as a work array in the present routine, and so */
/*             must be at least n*n2+(n+1)*(n2+1) real*8 elements long */

/*       _N.B._: ra must be at least n*n2+(n2+1)*(n+1) real*8 */
/*               elements long for use in the present routine */
/*               (here, n2 is the greatest integer less than or equal */
/*               to m, such that n2 is a positive integer power of two). */
/*               This routine returns krank = 0 when the actual */
/*               numerical rank is nearly full. */



/*       Extract from the array w initialized by routine idd_frmi */
/*       the greatest integer less than or equal to m that is */
/*       a positive integer power of two. */

    /* Parameter adjustments */
    --w;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ra;

    /* Function Body */
    n2 = (integer) w[2];


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

    idd_estrank0_(eps, m, n, &a[a_offset], &w[1], &n2, krank, &ra[ira], &ra[
	    irat], &ra[iscal]);


    return 0;
} /* idd_estrank__ */





/* Subroutine */ int idd_estrank0_(doublereal *eps, integer *m, integer *n, 
	doublereal *a, doublereal *w, integer *n2, integer *krank, doublereal 
	*ra, doublereal *rat, doublereal *scal)
{
    /* System generated locals */
    integer a_dim1, a_offset, ra_dim1, ra_offset, rat_dim1, rat_offset, i__1, 
	    i__2;

    /* Local variables */
    static integer ifrescal;
    static doublereal residual;
    static integer k;
    extern /* Subroutine */ int idd_house_(integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), idd_houseapp_(integer 
	    *, doublereal *, doublereal *, integer *, doublereal *, 
	    doublereal *);
    static integer nulls;
    extern /* Subroutine */ int idd_frm_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *), idd_atransposer_(integer *, integer 
	    *, doublereal *, doublereal *);


/*       routine idd_estrank serves as a memory wrapper */
/*       for the present routine. (Please see routine idd_estrank */
/*       for further documentation.) */



/*       Apply the random matrix to every column of a, obtaining ra. */

    /* Parameter adjustments */
    --w;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --scal;
    rat_dim1 = *n;
    rat_offset = 1 + rat_dim1;
    rat -= rat_offset;
    ra_dim1 = *n2;
    ra_offset = 1 + ra_dim1;
    ra -= ra_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	idd_frm_(m, n2, &w[1], &a[k * a_dim1 + 1], &ra[k * ra_dim1 + 1]);
    }


/*       Transpose ra to obtain rat. */

/* k */
    idd_atransposer_(n2, n, &ra[ra_offset], &rat[rat_offset]);


    *krank = 0;
    nulls = 0;


/*       Loop until nulls = 7, krank+nulls = n2, or krank+nulls = n. */

L1000:


    if (*krank > 0) {

/*           Apply the previous Householder transformations */
/*           to rat(:,krank+1). */

	ifrescal = 0;

	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *n - k + 1;
	    idd_houseapp_(&i__2, &rat[k * rat_dim1 + 1], &rat[k + (*krank + 
		    1) * rat_dim1], &ifrescal, &scal[k], &rat[k + (*krank + 1)
		     * rat_dim1]);
	}

/* k */
    }


/*         Compute the Householder vector associated */
/*         with rat(krank+1:*,krank+1). */

/* krank .gt. 0 */
    i__1 = *n - *krank;
    idd_house_(&i__1, &rat[*krank + 1 + (*krank + 1) * rat_dim1], &residual, 
	    &rat[(*krank + 1) * rat_dim1 + 1], &scal[*krank + 1]);
    residual = abs(residual);


    ++(*krank);
    if (residual <= *eps) {
	++nulls;
    }


    if (nulls < 7 && *krank + nulls < *n2 && *krank + nulls < *n) {
	goto L1000;
    }


    if (nulls < 7) {
	*krank = 0;
    }


    return 0;
} /* idd_estrank0__ */





/* Subroutine */ int idd_atransposer_(integer *m, integer *n, doublereal *a, 
	doublereal *at)
{
    /* System generated locals */
    integer a_dim1, a_offset, at_dim1, at_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;


/*       transposes a to obtain at. */

/*       input: */
/*       m -- first dimension of a, and second dimension of at */
/*       n -- second dimension of a, and first dimension of at */
/*       a -- matrix to be transposed */

/*       output: */
/*       at -- transpose of a */



    /* Parameter adjustments */
    at_dim1 = *n;
    at_offset = 1 + at_dim1;
    at -= at_offset;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {

	    at[k + j * at_dim1] = a[j + k * a_dim1];

	}
/* j */
    }


/* k */
    return 0;
} /* idd_atransposer__ */

