/* idd_id.f -- translated by f2c (version 20100827).
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


/*       routine iddp_id computes the ID of a matrix, */
/*       to a specified precision. */

/*       routine iddr_id computes the ID of a matrix, */
/*       to a specified rank. */

/*       routine idd_reconid reconstructs a matrix from its ID. */

/*       routine idd_copycols collects together selected columns */
/*       of a matrix. */

/*       routine idd_getcols collects together selected columns */
/*       of a matrix specified by a routine for applying the matrix */
/*       to arbitrary vectors. */

/*       routine idd_reconint constructs p in the ID a = b p, */
/*       where the columns of b are a subset of the columns of a, */
/*       and p is the projection coefficient matrix, */
/*       given list, krank, and proj output by routines iddr_id */
/*       or iddp_id. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int iddp_id_(doublereal *eps, integer *m, integer *n, 
	doublereal *a, integer *krank, integer *list, doublereal *rnorms)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static integer k;
    extern /* Subroutine */ int iddp_qrpiv_(doublereal *, integer *, integer 
	    *, doublereal *, integer *, integer *, doublereal *), 
	    idd_lssolve_(integer *, integer *, doublereal *, integer *);
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
    iddp_qrpiv_(eps, m, n, &a[a_offset], krank, &list[1], &rnorms[1]);


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
	    rnorms[k] = a[k + k * a_dim1];
	}

/* k */
    }


/*       Backsolve for proj, storing it at the beginning of a. */

    if (*krank > 0) {
	idd_lssolve_(m, n, &a[a_offset], krank);
    }


    return 0;
} /* iddp_id__ */





/* Subroutine */ int iddr_id_(integer *m, integer *n, doublereal *a, integer 
	*krank, integer *list, doublereal *rnorms)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1;

    /* Local variables */
    static integer j, k;
    static doublereal ss;
    extern /* Subroutine */ int iddr_qrpiv_(integer *, integer *, doublereal 
	    *, integer *, integer *, doublereal *), idd_lssolve_(integer *, 
	    integer *, doublereal *, integer *);
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
    iddr_qrpiv_(m, n, &a[a_offset], krank, &list[1], &rnorms[1]);


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
	rnorms[k] = a[k + k * a_dim1];
/* Computing 2nd power */
	d__1 = rnorms[k];
	ss += d__1 * d__1;
    }


/*       Backsolve for proj, storing it at the beginning of a. */

/* k */
    if (*krank > 0 && ss > 0.) {
	idd_lssolve_(m, n, &a[a_offset], krank);
    }

    if (ss == 0.) {

	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *m;
	    for (j = 1; j <= i__2; ++j) {

		a[j + k * a_dim1] = 0.;

	    }
/* j */
	}

/* k */
    }


    return 0;
} /* iddr_id__ */





/* Subroutine */ int idd_reconid_(integer *m, integer *krank, doublereal *
	col, integer *n, integer *list, doublereal *proj, doublereal *approx)
{
    /* System generated locals */
    integer col_dim1, col_offset, proj_dim1, proj_offset, approx_dim1, 
	    approx_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer j, k, l;


/*       reconstructs the matrix that the routine iddp_id */
/*       or iddr_id has decomposed, using the columns col */
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

	    approx[j + list[k] * approx_dim1] = 0.;

/*           Add in the contributions due to the identity matrix. */

	    if (k <= *krank) {
		approx[j + list[k] * approx_dim1] += col[j + k * col_dim1];
	    }

/*           Add in the contributions due to proj. */

	    if (k > *krank) {
		if (*krank > 0) {

		    i__3 = *krank;
		    for (l = 1; l <= i__3; ++l) {
			approx[j + list[k] * approx_dim1] += col[j + l * 
				col_dim1] * proj[l + (k - *krank) * proj_dim1]
				;
		    }

/* l */
		}
	    }

	}
/* k */
    }


/* j */
    return 0;
} /* idd_reconid__ */





/* Subroutine */ int idd_lssolve_(integer *m, integer *n, doublereal *a, 
	integer *krank)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1, d__2;

    /* Local variables */
    static integer j, k, l;
    static doublereal sum;
    extern /* Subroutine */ int idd_moverup_(integer *, integer *, integer *,
	     doublereal *);


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

	    sum = 0.;

	    i__2 = *krank;
	    for (l = j + 1; l <= i__2; ++l) {
		sum += a[j + l * a_dim1] * a[l + (*krank + k) * a_dim1];
	    }

/* l */
	    a[j + (*krank + k) * a_dim1] -= sum;

/*           Make sure that the entry in proj won't be too big; */
/*           set the entry to 0 when roundoff would make it too big */
/*           (in which case a(j,j) is so small that the contribution */
/*           from this entry in proj to the overall matrix approximation */
/*           is supposed to be negligible). */

	    if ((d__2 = a[j + (*krank + k) * a_dim1], abs(d__2)) < (d__1 = a[
		    j + j * a_dim1], abs(d__1)) * 1048576) {
		a[j + (*krank + k) * a_dim1] /= a[j + j * a_dim1];
	    } else {
		a[j + (*krank + k) * a_dim1] = 0.;
	    }

	}
/* j */
    }


/*       Move proj from a(1:krank,krank+1:n) to the beginning of a. */

/* k */
    idd_moverup_(m, n, krank, &a[a_offset]);


    return 0;
} /* idd_lssolve__ */





/* Subroutine */ int idd_moverup_(integer *m, integer *n, integer *krank, 
	doublereal *a)
{
    /* System generated locals */
    integer i__1, i__2;

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
	    a[j + *krank * (k - 1)] = a[j + *m * (*krank + k - 1)];
	}
/* j */
    }


/* k */
    return 0;
} /* idd_moverup__ */





/* Subroutine */ int idd_getcols_(integer *m, integer *n, S_fp matvec, 
	doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, 
	integer *krank, integer *list, doublereal *col, doublereal *x)
{
    /* System generated locals */
    integer col_dim1, col_offset, i__1, i__2;

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
/*       x -- must be at least n real*8 elements long */



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
	    x[k] = 0.;
	}

/* k */
	x[list[j]] = 1.;

	(*matvec)(n, &x[1], m, &col[j * col_dim1 + 1], p1, p2, p3, p4);

    }


/* j */
    return 0;
} /* idd_getcols__ */





/* Subroutine */ int idd_reconint_(integer *n, integer *list, integer *krank,
	 doublereal *proj, doublereal *p)
{
    /* System generated locals */
    integer proj_dim1, proj_offset, p_dim1, p_offset, i__1, i__2;

    /* Local variables */
    static integer j, k;


/*       constructs p in the ID a = b p, */
/*       where the columns of b are a subset of the columns of a, */
/*       and p is the projection coefficient matrix, */
/*       given list, krank, and proj output */
/*       by routines iddp_id or iddr_id. */

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
		    p[k + list[j] * p_dim1] = 1.;
		}
		if (j != k) {
		    p[k + list[j] * p_dim1] = 0.;
		}
	    }

	    if (j > *krank) {
		p[k + list[j] * p_dim1] = proj[k + (j - *krank) * proj_dim1];
	    }

	}
/* j */
    }


/* k */
    return 0;
} /* idd_reconint__ */





/* Subroutine */ int idd_copycols_(integer *m, integer *n, doublereal *a, 
	integer *krank, integer *list, doublereal *col)
{
    /* System generated locals */
    integer a_dim1, a_offset, col_dim1, col_offset, i__1, i__2;

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

	    col[j + k * col_dim1] = a[j + list[k] * a_dim1];

	}
/* j */
    }


/* k */
    return 0;
} /* idd_copycols__ */

