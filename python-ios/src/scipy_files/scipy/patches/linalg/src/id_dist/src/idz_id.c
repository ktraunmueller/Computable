/* idz_id.f -- translated by f2c (version 20100827).
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


/*       routine idzp_id computes the ID of a matrix, */
/*       to a specified precision. */

/*       routine idzr_id computes the ID of a matrix, */
/*       to a specified rank. */

/*       routine idz_reconid reconstructs a matrix from its ID. */

/*       routine idz_copycols collects together selected columns */
/*       of a matrix. */

/*       routine idz_getcols collects together selected columns */
/*       of a matrix specified by a routine for applying the matrix */
/*       to arbitrary vectors. */

/*       routine idz_reconint constructs p in the ID a = b p, */
/*       where the columns of b are a subset of the columns of a, */
/*       and p is the projection coefficient matrix, */
/*       given list, krank, and proj output by routines idzr_id */
/*       or idzp_id. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idzp_id_(doublereal *eps, integer *m, integer *n, 
	doublecomplex *a, integer *krank, integer *list, doublereal *rnorms)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int idzp_qrpiv_(doublereal *, integer *, integer 
	    *, doublecomplex *, integer *, integer *, doublereal *), 
	    idz_lssolve_(integer *, integer *, doublecomplex *, integer *);
    static integer iswap;


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
/*       The present routine stores the krank x (n-krank) matrix proj */
/*       in the memory initially occupied by a. */

/*       input: */
/*       eps -- relative precision of the resulting ID */
/*       m -- first dimension of a */
/*       n -- second dimension of a, as well as the dimension required */
/*            of list */
/*       a -- matrix to be ID'd */

/*       output: */
/*       a -- the first krank*(n-krank) elements of a constitute */
/*            the krank x (n-krank) interpolation matrix proj */
/*       krank -- numerical rank */
/*       list -- list of the indices of the krank columns of a */
/*               through which the other columns of a are expressed; */
/*               also, list describes the permutation of proj */
/*               required to reconstruct a as indicated in (*) above */
/*       rnorms -- absolute values of the entries on the diagonal */
/*                 of the triangular matrix used to compute the ID */
/*                 (these may be used to check the stability of the ID) */

/*       _N.B._: This routine changes a. */

/*       reference: */
/*       Cheng, Gimbutas, Martinsson, Rokhlin, "On the compression of */
/*            low-rank matrices," SIAM Journal on Scientific Computing, */
/*            26 (4): 1389-1404, 2005. */



/*       QR decompose a. */

    /* Parameter adjustments */
    --rnorms;
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    idzp_qrpiv_(eps, m, n, &a[a_offset], krank, &list[1], &rnorms[1]);


/*       Build the list of columns chosen in a */
/*       by multiplying together the permutations in list, */
/*       with the permutation swapping 1 and list(1) taken rightmost */
/*       in the product, that swapping 2 and list(2) taken next */
/*       rightmost, ..., that swapping krank and list(krank) taken */
/*       leftmost. */

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	rnorms[k] = (doublereal) k;
    }

/* k */
    if (*krank > 0) {
	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {

/*           Swap rnorms(k) and rnorms(list(k)). */

	    iswap = (integer) rnorms[k];
	    rnorms[k] = rnorms[list[k]];
	    rnorms[list[k]] = (doublereal) iswap;

	}
/* k */
    }

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	list[k] = (integer) rnorms[k];
    }


/*       Fill rnorms for the output. */

/* k */
    if (*krank > 0) {

	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = k;
	    i__3 = k + k * a_dim1;
	    rnorms[i__2] = a[i__3].r;
	}

/* k */
    }


/*       Backsolve for proj, storing it at the beginning of a. */

    if (*krank > 0) {
	idz_lssolve_(m, n, &a[a_offset], krank);
    }


    return 0;
} /* idzp_id__ */





/* Subroutine */ int idzr_id_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, integer *list, doublereal *rnorms)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Local variables */
    static integer j, k;
    static doublereal ss;
    extern /* Subroutine */ int idzr_qrpiv_(integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublereal *), 
	    idz_lssolve_(integer *, integer *, doublecomplex *, integer *);
    static integer iswap;


/*       computes the ID of a, i.e., lists in list the indices */
/*       of krank columns of a such that */

/*       a(j,list(k))  =  a(j,list(k)) */

/*       for all j = 1, ..., m; k = 1, ..., krank, and */

/*                        krank */
/*       a(j,list(k))  =  Sigma  a(j,list(l)) * proj(l,k-krank)       (*) */
/*                         l=1 */

/*                     +  epsilon(j,k-krank) */

/*       for all j = 1, ..., m; k = krank+1, ..., n, */

/*       for some matrix epsilon, dimensioned epsilon(m,n-krank), */
/*       whose norm is (hopefully) minimized by the pivoting procedure. */
/*       The present routine stores the krank x (n-krank) matrix proj */
/*       in the memory initially occupied by a. */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a, as well as the dimension required */
/*            of list */
/*       a -- matrix to be ID'd */
/*       krank -- desired rank of the output matrix */
/*                (please note that if krank > m or krank > n, */
/*                then the rank of the output matrix will be */
/*                less than krank) */

/*       output: */
/*       a -- the first krank*(n-krank) elements of a constitute */
/*            the krank x (n-krank) interpolation matrix proj */
/*       list -- list of the indices of the krank columns of a */
/*               through which the other columns of a are expressed; */
/*               also, list describes the permutation of proj */
/*               required to reconstruct a as indicated in (*) above */
/*       rnorms -- absolute values of the entries on the diagonal */
/*                 of the triangular matrix used to compute the ID */
/*                 (these may be used to check the stability of the ID) */

/*       _N.B._: This routine changes a. */

/*       reference: */
/*       Cheng, Gimbutas, Martinsson, Rokhlin, "On the compression of */
/*            low-rank matrices," SIAM Journal on Scientific Computing, */
/*            26 (4): 1389-1404, 2005. */



/*       QR decompose a. */

    /* Parameter adjustments */
    --rnorms;
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    idzr_qrpiv_(m, n, &a[a_offset], krank, &list[1], &rnorms[1]);


/*       Build the list of columns chosen in a */
/*       by multiplying together the permutations in list, */
/*       with the permutation swapping 1 and list(1) taken rightmost */
/*       in the product, that swapping 2 and list(2) taken next */
/*       rightmost, ..., that swapping krank and list(krank) taken */
/*       leftmost. */

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	rnorms[k] = (doublereal) k;
    }

/* k */
    if (*krank > 0) {
	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {

/*           Swap rnorms(k) and rnorms(list(k)). */

	    iswap = (integer) rnorms[k];
	    rnorms[k] = rnorms[list[k]];
	    rnorms[list[k]] = (doublereal) iswap;

	}
/* k */
    }

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	list[k] = (integer) rnorms[k];
    }


/*       Fill rnorms for the output. */

/* k */
    ss = 0.;

    i__1 = *krank;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k + k * a_dim1;
	rnorms[i__2] = a[i__3].r;
/* Computing 2nd power */
	d__1 = rnorms[k];
	ss += d__1 * d__1;
    }


/*       Backsolve for proj, storing it at the beginning of a. */

/* k */
    if (*krank > 0 && ss > 0.) {
	idz_lssolve_(m, n, &a[a_offset], krank);
    }

    if (ss == 0.) {

	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *m;
	    for (j = 1; j <= i__2; ++j) {

		i__3 = j + k * a_dim1;
		a[i__3].r = 0., a[i__3].i = 0.;

	    }
/* j */
	}

/* k */
    }


    return 0;
} /* idzr_id__ */





/* Subroutine */ int idz_reconid_(integer *m, integer *krank, doublecomplex *
	col, integer *n, integer *list, doublecomplex *proj, doublecomplex *
	approx)
{
    /* System generated locals */
    integer col_dim1, col_offset, proj_dim1, proj_offset, approx_dim1, 
	    approx_offset, i__1, i__2, i__3, i__4, i__5, i__6, i__7;
    doublecomplex z__1, z__2;

    /* Local variables */
    static integer j, k, l;


/*       reconstructs the matrix that the routine idzp_id */
/*       or idzr_id has decomposed, using the columns col */
/*       of the reconstructed matrix whose indices are listed in list, */
/*       in addition to the interpolation matrix proj. */

/*       input: */
/*       m -- first dimension of cols and approx */
/*       krank -- first dimension of cols and proj; also, */
/*                n-krank is the second dimension of proj */
/*       col -- columns of the matrix to be reconstructed */
/*       n -- second dimension of approx; also, */
/*            n-krank is the second dimension of proj */
/*       list(k) -- index of col(1:m,k) in the reconstructed matrix */
/*                  when k <= krank; in general, list describes */
/*                  the permutation required for reconstruction */
/*                  via cols and proj */
/*       proj -- interpolation matrix */

/*       output: */
/*       approx -- reconstructed matrix */



    /* Parameter adjustments */
    col_dim1 = *m;
    col_offset = 1 + col_dim1;
    col -= col_offset;
    approx_dim1 = *m;
    approx_offset = 1 + approx_dim1;
    approx -= approx_offset;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --list;

    /* Function Body */
    i__1 = *m;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {

	    i__3 = j + list[k] * approx_dim1;
	    approx[i__3].r = 0., approx[i__3].i = 0.;

/*           Add in the contributions due to the identity matrix. */

	    if (k <= *krank) {
		i__3 = j + list[k] * approx_dim1;
		i__4 = j + list[k] * approx_dim1;
		i__5 = j + k * col_dim1;
		z__1.r = approx[i__4].r + col[i__5].r, z__1.i = approx[i__4]
			.i + col[i__5].i;
		approx[i__3].r = z__1.r, approx[i__3].i = z__1.i;
	    }

/*           Add in the contributions due to proj. */

	    if (k > *krank) {
		if (*krank > 0) {

		    i__3 = *krank;
		    for (l = 1; l <= i__3; ++l) {
			i__4 = j + list[k] * approx_dim1;
			i__5 = j + list[k] * approx_dim1;
			i__6 = j + l * col_dim1;
			i__7 = l + (k - *krank) * proj_dim1;
			z__2.r = col[i__6].r * proj[i__7].r - col[i__6].i * 
				proj[i__7].i, z__2.i = col[i__6].r * proj[
				i__7].i + col[i__6].i * proj[i__7].r;
			z__1.r = approx[i__5].r + z__2.r, z__1.i = approx[
				i__5].i + z__2.i;
			approx[i__4].r = z__1.r, approx[i__4].i = z__1.i;
		    }

/* l */
		}
	    }

	}
/* k */
    }


/* j */
    return 0;
} /* idz_reconid__ */





/* Subroutine */ int idz_lssolve_(integer *m, integer *n, doublecomplex *a, 
	integer *krank)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;
    doublecomplex z__1, z__2;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *), z_div(doublecomplex *, 
	    doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer j, k, l;
    static doublecomplex sum;
    extern /* Subroutine */ int idz_moverup_(integer *, integer *, integer *,
	     doublecomplex *);
    static doublereal rdenom, rnumer;


/*       backsolves for proj satisfying R_11 proj ~ R_12, */
/*       where R_11 = a(1:krank,1:krank) */
/*       and R_12 = a(1:krank,krank+1:n). */
/*       This routine overwrites the beginning of a with proj. */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a; also, */
/*            n-krank is the second dimension of proj */
/*       a -- trapezoidal input matrix */
/*       krank -- first dimension of proj; also, */
/*                n-krank is the second dimension of proj */

/*       output: */
/*       a -- the first krank*(n-krank) elements of a constitute */
/*            the krank x (n-krank) matrix proj */



/*       Overwrite a(1:krank,krank+1:n) with proj. */

    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    i__1 = *n - *krank;
    for (k = 1; k <= i__1; ++k) {
	for (j = *krank; j >= 1; --j) {

	    sum.r = 0., sum.i = 0.;

	    i__2 = *krank;
	    for (l = j + 1; l <= i__2; ++l) {
		i__3 = j + l * a_dim1;
		i__4 = l + (*krank + k) * a_dim1;
		z__2.r = a[i__3].r * a[i__4].r - a[i__3].i * a[i__4].i, 
			z__2.i = a[i__3].r * a[i__4].i + a[i__3].i * a[i__4]
			.r;
		z__1.r = sum.r + z__2.r, z__1.i = sum.i + z__2.i;
		sum.r = z__1.r, sum.i = z__1.i;
	    }

/* l */
	    i__2 = j + (*krank + k) * a_dim1;
	    i__3 = j + (*krank + k) * a_dim1;
	    z__1.r = a[i__3].r - sum.r, z__1.i = a[i__3].i - sum.i;
	    a[i__2].r = z__1.r, a[i__2].i = z__1.i;

/*           Make sure that the entry in proj won't be too big; */
/*           set the entry to 0 when roundoff would make it too big */
/*           (in which case a(j,j) is so small that the contribution */
/*           from this entry in proj to the overall matrix approximation */
/*           is supposed to be negligible). */

	    i__2 = j + (*krank + k) * a_dim1;
	    d_cnjg(&z__2, &a[j + (*krank + k) * a_dim1]);
	    z__1.r = a[i__2].r * z__2.r - a[i__2].i * z__2.i, z__1.i = a[i__2]
		    .r * z__2.i + a[i__2].i * z__2.r;
	    rnumer = z__1.r;
	    i__2 = j + j * a_dim1;
	    d_cnjg(&z__2, &a[j + j * a_dim1]);
	    z__1.r = a[i__2].r * z__2.r - a[i__2].i * z__2.i, z__1.i = a[i__2]
		    .r * z__2.i + a[i__2].i * z__2.r;
	    rdenom = z__1.r;

	    if (rnumer < rdenom * 1073741824) {
		i__2 = j + (*krank + k) * a_dim1;
		z_div(&z__1, &a[j + (*krank + k) * a_dim1], &a[j + j * a_dim1]
			);
		a[i__2].r = z__1.r, a[i__2].i = z__1.i;
	    } else {
		i__2 = j + (*krank + k) * a_dim1;
		a[i__2].r = 0., a[i__2].i = 0.;
	    }

	}
/* j */
    }


/*       Move proj from a(1:krank,krank+1:n) to the beginning of a. */

/* k */
    idz_moverup_(m, n, krank, &a[a_offset]);


    return 0;
} /* idz_lssolve__ */





/* Subroutine */ int idz_moverup_(integer *m, integer *n, integer *krank, 
	doublecomplex *a)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       moves the krank x (n-krank) matrix in a(1:krank,krank+1:n), */
/*       where a is initially dimensioned m x n, to the beginning of a. */
/*       (This is not the most natural way to code the move, */
/*       but one of my usually well-behaved compilers chokes */
/*       on more natural ways.) */

/*       input: */
/*       m -- initial first dimension of a */
/*       n -- initial second dimension of a */
/*       krank -- number of rows to move */
/*       a -- m x n matrix whose krank x (n-krank) block */
/*            a(1:krank,krank+1:n) is to be moved */

/*       output: */
/*       a -- array starting with the moved krank x (n-krank) block */



    /* Parameter adjustments */
    --a;

    /* Function Body */
    i__1 = *n - *krank;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *krank;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = j + *krank * (k - 1);
	    i__4 = j + *m * (*krank + k - 1);
	    a[i__3].r = a[i__4].r, a[i__3].i = a[i__4].i;
	}
/* j */
    }


/* k */
    return 0;
} /* idz_moverup__ */





/* Subroutine */ int idz_getcols_(integer *m, integer *n, S_fp matvec, 
	doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, 
	doublecomplex *p4, integer *krank, integer *list, doublecomplex *col, 
	doublecomplex *x)
{
    /* System generated locals */
    integer col_dim1, col_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer j, k;


/*       collects together the columns of the matrix a indexed by list */
/*       into the matrix col, where routine matvec applies a */
/*       to an arbitrary vector. */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       matvec -- routine which applies a to an arbitrary vector; */
/*                 this routine must have a calling sequence of the form */

/*                 matvec(m,x,n,y,p1,p2,p3,p4) */

/*                 where m is the length of x, */
/*                 x is the vector to which the matrix is to be applied, */
/*                 n is the length of y, */
/*                 y is the product of the matrix and x, */
/*                 and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matvec */
/*       p2 -- parameter to be passed to routine matvec */
/*       p3 -- parameter to be passed to routine matvec */
/*       p4 -- parameter to be passed to routine matvec */
/*       krank -- number of columns to be extracted */
/*       list -- indices of the columns to be extracted */

/*       output: */
/*       col -- columns of a indexed by list */

/*       work: */
/*       x -- must be at least n complex*16 elements long */



    /* Parameter adjustments */
    --x;
    col_dim1 = *m;
    col_offset = 1 + col_dim1;
    col -= col_offset;
    --list;

    /* Function Body */
    i__1 = *krank;
    for (j = 1; j <= i__1; ++j) {

	i__2 = *n;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = k;
	    x[i__3].r = 0., x[i__3].i = 0.;
	}

/* k */
	i__2 = list[j];
	x[i__2].r = 1., x[i__2].i = 0.;

	(*matvec)(n, &x[1], m, &col[j * col_dim1 + 1], p1, p2, p3, p4);

    }


/* j */
    return 0;
} /* idz_getcols__ */





/* Subroutine */ int idz_reconint_(integer *n, integer *list, integer *krank,
	 doublecomplex *proj, doublecomplex *p)
{
    /* System generated locals */
    integer proj_dim1, proj_offset, p_dim1, p_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       constructs p in the ID a = b p, */
/*       where the columns of b are a subset of the columns of a, */
/*       and p is the projection coefficient matrix, */
/*       given list, krank, and proj output */
/*       by routines idzp_id or idzr_id. */

/*       input: */
/*       n -- part of the second dimension of proj and p */
/*       list -- list of columns retained from the original matrix */
/*               in the ID */
/*       krank -- rank of the ID */
/*       proj -- matrix of projection coefficients in the ID */

/*       output: */
/*       p -- projection matrix in the ID */



    /* Parameter adjustments */
    --list;
    p_dim1 = *krank;
    p_offset = 1 + p_dim1;
    p -= p_offset;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;

    /* Function Body */
    i__1 = *krank;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {

	    if (j <= *krank) {
		if (j == k) {
		    i__3 = k + list[j] * p_dim1;
		    p[i__3].r = 1., p[i__3].i = 0.;
		}
		if (j != k) {
		    i__3 = k + list[j] * p_dim1;
		    p[i__3].r = 0., p[i__3].i = 0.;
		}
	    }

	    if (j > *krank) {
		i__3 = k + list[j] * p_dim1;
		i__4 = k + (j - *krank) * proj_dim1;
		p[i__3].r = proj[i__4].r, p[i__3].i = proj[i__4].i;
	    }

	}
/* j */
    }


/* k */
    return 0;
} /* idz_reconint__ */





/* Subroutine */ int idz_copycols_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, integer *list, doublecomplex *col)
{
    /* System generated locals */
    integer a_dim1, a_offset, col_dim1, col_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;


/*       collects together the columns of the matrix a indexed by list */
/*       into the matrix col. */

/*       input: */
/*       m -- first dimension of a */
/*       n -- second dimension of a */
/*       a -- matrix whose columns are to be extracted */
/*       krank -- number of columns to be extracted */
/*       list -- indices of the columns to be extracted */

/*       output: */
/*       col -- columns of a indexed by list */



    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    col_dim1 = *m;
    col_offset = 1 + col_dim1;
    col -= col_offset;
    --list;

    /* Function Body */
    i__1 = *krank;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {

	    i__3 = j + k * col_dim1;
	    i__4 = j + list[k] * a_dim1;
	    col[i__3].r = a[i__4].r, col[i__3].i = a[i__4].i;

	}
/* j */
    }


/* k */
    return 0;
} /* idz_copycols__ */

