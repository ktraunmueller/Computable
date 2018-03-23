/* idzp_rid.f -- translated by f2c (version 20100827).
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


/*       routine idzp_rid computes the ID, to a specified precision, */
/*       of a matrix specified by a routine for applying its adjoint */
/*       to arbitrary vectors. This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idzp_rid_(integer *lproj, doublereal *eps, integer *m, 
	integer *n, U_fp matveca, doublecomplex *p1, doublecomplex *p2, 
	doublecomplex *p3, doublecomplex *p4, integer *krank, integer *list, 
	doublecomplex *proj, integer *ier)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer k, lw, ira, lra;
    extern /* Subroutine */ int idz_findrank_(integer *, doublereal *, 
	    integer *, integer *, U_fp, doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *, doublecomplex *);
    static integer iwork, lwork;
    extern /* Subroutine */ int idz_adjointer_(integer *, integer *, 
	    doublecomplex *, doublecomplex *);
    static integer kranki;
    extern /* Subroutine */ int idzp_id_(doublereal *, integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublecomplex *);


/*       computes the ID of a, i.e., lists in list the indices */
/*       of krank columns of a such that */

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
/*       lproj -- maximum usable length (in complex*16 elements) */
/*                of the array proj */
/*       eps -- precision to which the ID is to be computed */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       matveca -- routine which applies the adjoint */
/*                  of the matrix to be ID'd to an arbitrary vector; */
/*                  this routine must have a calling sequence */
/*                  of the form */

/*                  matveca(m,x,n,y,p1,p2,p3,p4), */

/*                  where m is the length of x, */
/*                  x is the vector to which the adjoint */
/*                  of the matrix is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the adjoint of the matrix and x, */
/*                  and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matveca */
/*       p2 -- parameter to be passed to routine matveca */
/*       p3 -- parameter to be passed to routine matveca */
/*       p4 -- parameter to be passed to routine matveca */

/*       output: */
/*       krank -- numerical rank */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd; */
/*               the present routine uses proj as a work array, too, so */
/*               proj must be at least m+1 + 2*n*(krank+1) complex*16 */
/*               elements long, where krank is the rank output */
/*               by the present routine */
/*       ier -- 0 when the routine terminates successfully; */
/*              -1000 when lproj is too small */

/*       _N.B._: The algorithm used by this routine is randomized. */
/*               proj must be at least m+1 + 2*n*(krank+1) complex*16 */
/*               elements long, where krank is the rank output */
/*               by the present routine. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



    /* Parameter adjustments */
    --list;
    --proj;

    /* Function Body */
    *ier = 0;


/*       Allocate memory in proj. */

    lw = 0;

    iwork = lw + 1;
    lwork = *m + (*n << 1) + 1;
    lw += lwork;

    ira = lw + 1;


/*       Find the rank of a. */

    lra = *lproj - lwork;
    idz_findrank_(&lra, eps, m, n, (U_fp)matveca, p1, p2, p3, p4, &kranki, &
	    proj[ira], ier, &proj[iwork]);
    if (*ier != 0) {
	return 0;
    }


    if (*lproj < lwork + (kranki << 1) * *n) {
	*ier = -1000;
	return 0;
    }


/*       Take the adjoint of ra. */

    idz_adjointer_(n, &kranki, &proj[ira], &proj[ira + kranki * *n]);


/*       Move the adjoint thus obtained to the beginning of proj. */

    i__1 = kranki * *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = ira + kranki * *n + k - 1;
	proj[i__2].r = proj[i__3].r, proj[i__2].i = proj[i__3].i;
    }


/*       ID the adjoint. */

/* k */
    idzp_id_(eps, &kranki, n, &proj[1], krank, &list[1], &proj[kranki * *n + 
	    1]);


    return 0;
} /* idzp_rid__ */





/* Subroutine */ int idz_findrank_(integer *lra, doublereal *eps, integer *m,
	 integer *n, U_fp matveca, doublecomplex *p1, doublecomplex *p2, 
	doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
	ra, integer *ier, doublecomplex *w)
{
    /* System generated locals */
    integer ra_dim1, ra_offset;

    /* Local variables */
    static integer ix, iy, lw, lx, ly;
    extern /* Subroutine */ int idz_findrank0_(integer *, doublereal *, 
	    integer *, integer *, U_fp, doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *, doublecomplex *);
    static integer iscal, lscal;


/*       estimates the numerical rank krank of a matrix a to precision */
/*       eps, where the routine matveca applies the adjoint of a */
/*       to an arbitrary vector. This routine applies the adjoint of a */
/*       to krank random vectors, and returns the resulting vectors */
/*       as the columns of ra. */

/*       input: */
/*       lra -- maximum usable length (in complex*16 elements) */
/*              of array ra */
/*       eps -- precision defining the numerical rank */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       matveca -- routine which applies the adjoint */
/*                  of the matrix whose rank is to be estimated */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matveca(m,x,n,y,p1,p2,p3,p4), */

/*                  where m is the length of x, */
/*                  x is the vector to which the adjoint */
/*                  of the matrix is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the adjoint of the matrix and x, */
/*                  and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matveca */
/*       p2 -- parameter to be passed to routine matveca */
/*       p3 -- parameter to be passed to routine matveca */
/*       p4 -- parameter to be passed to routine matveca */

/*       output: */
/*       krank -- estimate of the numerical rank of a */
/*       ra -- product of the adjoint of a and a matrix whose entries */
/*             are pseudorandom realizations of i.i.d. random numbers, */
/*             uniformly distributed on [0,1]; */
/*             ra must be at least 2*n*krank complex*16 elements long */
/*       ier -- 0 when the routine terminates successfully; */
/*              -1000 when lra is too small */

/*       work: */
/*       w -- must be at least m+2*n+1 complex*16 elements long */

/*       _N.B._: ra must be at least 2*n*krank complex*16 elements long. */
/*               Also, the algorithm used by this routine is randomized. */



    /* Parameter adjustments */
    --w;
    ra_dim1 = *n;
    ra_offset = 1 + ra_dim1;
    ra -= ra_offset;

    /* Function Body */
    lw = 0;

    ix = lw + 1;
    lx = *m;
    lw += lx;

    iy = lw + 1;
    ly = *n;
    lw += ly;

    iscal = lw + 1;
    lscal = *n + 1;
    lw += lscal;


    idz_findrank0_(lra, eps, m, n, (U_fp)matveca, p1, p2, p3, p4, krank, &ra[
	    ra_offset], ier, &w[ix], &w[iy], &w[iscal]);


    return 0;
} /* idz_findrank__ */





/* Subroutine */ int idz_findrank0_(integer *lra, doublereal *eps, integer *
	m, integer *n, S_fp matveca, doublecomplex *p1, doublecomplex *p2, 
	doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
	ra, integer *ier, doublecomplex *x, doublecomplex *y, doublecomplex *
	scal)
{
    /* System generated locals */
    integer ra_dim1, ra_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double z_abs(doublecomplex *);

    /* Local variables */
    extern /* Subroutine */ int id_srand_(integer *, doublecomplex *);
    static integer ifrescal;
    static doublecomplex residual;
    static integer k, m2;
    extern /* Subroutine */ int idz_house_(integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublecomplex *), idz_crunch_(
	    integer *, integer *, doublecomplex *), idz_houseapp_(integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    doublecomplex *);


/*       routine idz_findrank serves as a memory wrapper */
/*       for the present routine. (Please see routine idz_findrank */
/*       for further documentation.) */



    /* Parameter adjustments */
    --x;
    --scal;
    --y;
    ra_dim1 = *n;
    ra_offset = 1 + ra_dim1 * 3;
    ra -= ra_offset;

    /* Function Body */
    *ier = 0;


    *krank = 0;


/*       Loop until the residual is greater than eps, */
/*       or krank = m or krank = n. */

L1000:


    if (*lra < (*n << 1) * (*krank + 1)) {
	*ier = -1000;
	return 0;
    }


/*         Apply the adjoint of a to a random vector. */

    m2 = *m << 1;
    id_srand_(&m2, &x[1]);
    (*matveca)(m, &x[1], n, &ra[((*krank + 1 << 1) + 1) * ra_dim1 + 1], p1, 
	    p2, p3, p4);

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k + ((*krank + 1 << 1) + 1) * ra_dim1;
	y[i__2].r = ra[i__3].r, y[i__2].i = ra[i__3].i;
    }


/* k */
    if (*krank > 0) {

/*           Apply the previous Householder transformations to y. */

	ifrescal = 0;

	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *n - k + 1;
	    idz_houseapp_(&i__2, &ra[((k << 1) + 2) * ra_dim1 + 1], &y[k], &
		    ifrescal, &scal[k], &y[k]);
	}

/* k */
    }


/*         Compute the Householder vector associated with y. */

/* krank .gt. 0 */
    i__1 = *n - *krank;
    idz_house_(&i__1, &y[*krank + 1], &residual, &ra[((*krank + 1 << 1) + 2) 
	    * ra_dim1 + 1], &scal[*krank + 1]);


    ++(*krank);


    if (z_abs(&residual) > *eps && *krank < *m && *krank < *n) {
	goto L1000;
    }


/*       Delete the Householder vectors from the array ra. */

    idz_crunch_(n, krank, &ra[ra_offset]);


    return 0;
} /* idz_findrank0__ */





/* Subroutine */ int idz_crunch_(integer *n, integer *l, doublecomplex *a)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       removes every other block of n entries from a vector. */

/*       input: */
/*       n -- length of each block to remove */
/*       l -- half of the total number of blocks */
/*       a -- original array */

/*       output: */
/*       a -- array with every other block of n entries removed */



    /* Parameter adjustments */
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *l;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {

	    i__3 = k + j * a_dim1;
	    i__4 = k + ((j << 1) - 1) * a_dim1;
	    a[i__3].r = a[i__4].r, a[i__3].i = a[i__4].i;

	}
/* k */
    }


/* j */
    return 0;
} /* idz_crunch__ */





/* Subroutine */ int idz_adjointer_(integer *m, integer *n, doublecomplex *a,
	 doublecomplex *aa)
{
    /* System generated locals */
    integer a_dim1, a_offset, aa_dim1, aa_offset, i__1, i__2, i__3;
    doublecomplex z__1;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer j, k;


/*       forms the adjoint aa of a. */

/*       input: */
/*       m -- first dimension of a, and second dimension of aa */
/*       n -- second dimension of a, and first dimension of aa */
/*       a -- matrix whose adjoint is to be taken */

/*       output: */
/*       aa -- adjoint of a */



    /* Parameter adjustments */
    aa_dim1 = *n;
    aa_offset = 1 + aa_dim1;
    aa -= aa_offset;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {

	    i__3 = k + j * aa_dim1;
	    d_cnjg(&z__1, &a[j + k * a_dim1]);
	    aa[i__3].r = z__1.r, aa[i__3].i = z__1.i;

	}
/* j */
    }


/* k */
    return 0;
} /* idz_adjointer__ */

