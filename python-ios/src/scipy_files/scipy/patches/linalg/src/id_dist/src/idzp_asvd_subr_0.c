/* idzp_asvd_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzp_asvd_(integer *lw, doublereal *eps, integer *m, 
	integer *n, doublecomplex *a, doublecomplex *winit, integer *krank, 
	integer *iu, integer *iv, integer *is, doublecomplex *w, integer *ier)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;

    /* Local variables */
    extern /* Subroutine */ int idzp_aid_(doublereal *, integer *, integer *,
	     doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    doublecomplex *);
    static integer k;
    extern /* Subroutine */ int idzp_asvd0_(integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *, 
	    doublecomplex *, doublecomplex *);
    static integer ls, lu, lv, lw2, isi, iui, ivi, icol, lcol, iproj, ilist, 
	    lproj, llist, iwork, lwork;
    extern /* Subroutine */ int idz_realcomplex_(integer *, doublecomplex *, 
	    doublecomplex *);


/*       constructs a rank-krank SVD  U Sigma V^*  approximating a */
/*       to precision eps, where U is an m x krank matrix whose */
/*       columns are orthonormal, V is an n x krank matrix whose */
/*       columns are orthonormal, and Sigma is a diagonal krank x krank */
/*       matrix whose entries are all nonnegative. */
/*       The entries of U are stored in w, starting at w(iu); */
/*       the entries of V are stored in w, starting at w(iv). */
/*       The diagonal entries of Sigma are stored in w, */
/*       starting at w(is). This routine uses a randomized algorithm. */

/*       input: */
/*       lw -- maximum usable length (in complex*16 elements) */
/*             of the array w */
/*       eps -- precision of the desired approximation */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       a -- matrix to be approximated; the present routine does not */
/*            alter a */
/*       winit -- initialization array that has been constructed */
/*                by routine idz_frmi */

/*       output: */
/*       krank -- rank of the SVD constructed */
/*       iu -- index in w of the first entry of the matrix */
/*             of orthonormal left singular vectors of a */
/*       iv -- index in w of the first entry of the matrix */
/*             of orthonormal right singular vectors of a */
/*       is -- index in w of the first entry of the array */
/*             of singular values of a */
/*       w -- array containing the singular values and singular vectors */
/*            of a; w doubles as a work array, and so must be at least */
/*            max( (krank+1)*(3*m+5*n+11)+8*krank**2, (2*n+1)*(n2+1) ) */
/*            complex*16 elements long, where n2 is the greatest integer */
/*            less than or equal to m, such that n2 is */
/*            a positive integer power of two; krank is the rank output */
/*            by this routine */
/*       ier -- 0 when the routine terminates successfully; */
/*              -1000 when lw is too small; */
/*              other nonzero values when idz_id2svd bombs */

/*       _N.B._: w must be at least */
/*               max( (krank+1)*(3*m+5*n+11)+8*krank^2, (2*n+1)*(n2+1) ) */
/*               complex*16 elements long, where n2 is */
/*               the greatest integer less than or equal to m, */
/*               such that n2 is a positive integer power of two; */
/*               krank is the rank output by this routine. */
/*               Also, the algorithm used by this routine is randomized. */



/*       Allocate memory in w. */

    /* Parameter adjustments */
    --winit;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --w;

    /* Function Body */
    lw2 = 0;

    ilist = lw2 + 1;
    llist = *n;
    lw2 += llist;

    iproj = lw2 + 1;


/*       ID a. */

    idzp_aid_(eps, m, n, &a[a_offset], &winit[1], krank, &w[ilist], &w[iproj]
	    );


    if (*krank > 0) {


/*         Allocate more memory in w. */

	lproj = *krank * (*n - *krank);
	lw2 += lproj;

	icol = lw2 + 1;
	lcol = *m * *krank;
	lw2 += lcol;

	iui = lw2 + 1;
	lu = *m * *krank;
	lw2 += lu;

	ivi = lw2 + 1;
	lv = *n * *krank;
	lw2 += lv;

	isi = lw2 + 1;
	ls = *krank;
	lw2 += ls;

	iwork = lw2 + 1;
/* Computing 2nd power */
	i__1 = *krank;
	lwork = (*krank + 1) * (*m + *n * 3 + 10) + i__1 * i__1 * 9;
	lw2 += lwork;


	if (*lw < lw2) {
	    *ier = -1000;
	    return 0;
	}


	idzp_asvd0_(m, n, &a[a_offset], krank, &w[ilist], &w[iproj], &w[iui],
		 &w[ivi], &w[isi], ier, &w[icol], &w[iwork]);
	if (*ier != 0) {
	    return 0;
	}


	*iu = 1;
	*iv = *iu + lu;
	*is = *iv + lv;


/*         Copy the singular values and singular vectors */
/*         into their proper locations. */

	i__1 = lu;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *iu + k - 1;
	    i__3 = iui + k - 1;
	    w[i__2].r = w[i__3].r, w[i__2].i = w[i__3].i;
	}

/* k */
	i__1 = lv;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *iv + k - 1;
	    i__3 = ivi + k - 1;
	    w[i__2].r = w[i__3].r, w[i__2].i = w[i__3].i;
	}

/* k */
	idz_realcomplex_(&ls, &w[isi], &w[*is]);


    }


/* krank .gt. 0 */
    return 0;
} /* idzp_asvd__ */

