/* idz_svd_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idzr_svd_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *u, doublecomplex *v, doublereal *s, 
	integer *ier, doublecomplex *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, u_dim1, u_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int idz_adjer_(integer *, integer *, 
	    doublecomplex *, doublecomplex *);
    static integer ifadjoint, io;
    extern /* Subroutine */ int idzr_qrpiv_(integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, doublecomplex *);
    static integer ldr, ldu;
    extern /* Subroutine */ int idz_qmatmat_(integer *, integer *, integer *,
	     doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *);
    static integer info;
    static char jobz[1];
    extern /* Subroutine */ int idz_permuter_(integer *, doublecomplex *, 
	    integer *, integer *, doublecomplex *);
    static integer lwork;
    extern /* Subroutine */ int idz_retriever_(integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *);
    static integer ldvadj;
    extern /* Subroutine */ int zgesdd_(char *, integer *, integer *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *, 
	    integer *, doublecomplex *, integer *, doublecomplex *, integer *,
	     doublecomplex *, doublecomplex *, integer *, ftnlen);


/*       constructs a rank-krank SVD  u diag(s) v^*  approximating a, */
/*       where u is an m x krank matrix whose columns are orthonormal, */
/*       v is an n x krank matrix whose columns are orthonormal, */
/*       and diag(s) is a diagonal krank x krank matrix whose entries */
/*       are all nonnegative. This routine combines a QR code */
/*       (which is based on plane/Householder reflections) */
/*       with the LAPACK routine zgesdd. */

/*       input: */
/*       m -- first dimension of a and u */
/*       n -- second dimension of a, and first dimension of v */
/*       a -- matrix to be SVD'd */
/*       krank -- desired rank of the approximation to a */

/*       output: */
/*       u -- left singular vectors of a corresponding */
/*            to the k greatest singular values of a */
/*       v -- right singular vectors of a corresponding */
/*            to the k greatest singular values of a */
/*       s -- k greatest singular values of a */
/*       ier -- 0 when the routine terminates successfully; */
/*              nonzero when the routine encounters an error */

/*       work: */
/*       r -- must be at least */
/*            (krank+2)*n+8*min(m,n)+6*krank**2+8*krank */
/*            complex*16 elements long */

/*       _N.B._: This routine destroys a. Also, please beware that */
/*               the source code for this routine could be clearer. */



    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --s;
    --v;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    --r__;

    /* Function Body */
    io = min(*m,*n) << 3;


    *ier = 0;


/*       Compute a pivoted QR decomposition of a. */

    idzr_qrpiv_(m, n, &a[a_offset], krank, &r__[1], &r__[io + 1]);


/*       Extract R from the QR decomposition. */

    idz_retriever_(m, n, &a[a_offset], krank, &r__[io + 1]);


/*       Rearrange R according to ind. */

    idz_permuter_(krank, &r__[1], krank, n, &r__[io + 1]);


/*       Use LAPACK to SVD r, */
/*       storing the krank (krank x 1) left singular vectors */
/*       in r(io+krank*n+1 : io+krank*n+krank*krank). */

    *(unsigned char *)jobz = 'S';
    ldr = *krank;
/* Computing 2nd power */
    i__1 = *krank;
    lwork = i__1 * i__1 + (*krank << 1) + *n << 1;
    ldu = *krank;
    ldvadj = *krank;

    zgesdd_(jobz, krank, n, &r__[io + 1], &ldr, &s[1], &r__[io + *krank * *n 
	    + 1], &ldu, &v[1], &ldvadj, &r__[io + *krank * *n + *krank * *
	    krank + 1], &lwork, &r__[io + *krank * *n + *krank * *krank + 
	    lwork + 1], &r__[1], &info, (ftnlen)1);

    if (info != 0) {
	*ier = info;
	return 0;
    }


/*       Multiply the U from R from the left by Q to obtain the U */
/*       for A. */

    i__1 = *krank;
    for (k = 1; k <= i__1; ++k) {

	i__2 = *krank;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = j + k * u_dim1;
	    i__4 = io + *krank * *n + j + *krank * (k - 1);
	    u[i__3].r = r__[i__4].r, u[i__3].i = r__[i__4].i;
	}

/* j */
	i__2 = *m;
	for (j = *krank + 1; j <= i__2; ++j) {
	    i__3 = j + k * u_dim1;
	    u[i__3].r = 0., u[i__3].i = 0.;
	}

/* j */
    }

/* k */
    ifadjoint = 0;
    idz_qmatmat_(&ifadjoint, m, n, &a[a_offset], krank, krank, &u[u_offset], 
	    &r__[1]);


/*       Take the adjoint of v to obtain r. */

    idz_adjer_(krank, n, &v[1], &r__[1]);


/*       Copy r into v. */

    i__1 = *n * *krank;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	v[i__2].r = r__[i__3].r, v[i__2].i = r__[i__3].i;
    }


/* k */
    return 0;
} /* idzr_svd__ */





/* Subroutine */ int idzp_svd_(integer *lw, doublereal *eps, integer *m, 
	integer *n, doublecomplex *a, integer *krank, integer *iu, integer *
	iv, integer *is, doublecomplex *w, integer *ier)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer j, k;
    extern /* Subroutine */ int idz_adjer_(integer *, integer *, 
	    doublecomplex *, doublecomplex *);
    static integer ifadjoint, io, ls, lu, lv;
    extern /* Subroutine */ int idzp_qrpiv_(doublereal *, integer *, integer 
	    *, doublecomplex *, integer *, doublecomplex *, doublecomplex *);
    static integer ldr, ldu, isi, ivi;
    extern /* Subroutine */ int idz_qmatmat_(integer *, integer *, integer *,
	     doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *);
    static integer info;
    static char jobz[1];
    extern /* Subroutine */ int idz_realcomp_(integer *, doublecomplex *, 
	    doublecomplex *), idz_permuter_(integer *, doublecomplex *, 
	    integer *, integer *, doublecomplex *);
    static integer lwork;
    extern /* Subroutine */ int idz_retriever_(integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *);
    static integer ldvadj;
    extern /* Subroutine */ int zgesdd_();


/*       constructs a rank-krank SVD  U Sigma V^*  approximating a */
/*       to precision eps, where U is an m x krank matrix whose */
/*       columns are orthonormal, V is an n x krank matrix whose */
/*       columns are orthonormal, and Sigma is a diagonal krank x krank */
/*       matrix whose entries are all nonnegative. */
/*       The entries of U are stored in w, starting at w(iu); */
/*       the entries of V are stored in w, starting at w(iv). */
/*       The diagonal entries of Sigma are stored in w, */
/*       starting at w(is). This routine combines a QR code */
/*       (which is based on plane/Householder reflections) */
/*       with the LAPACK routine zgesdd. */

/*       input: */
/*       lw -- maximum usable length of w (in complex*16 elements) */
/*       eps -- precision to which the SVD approximates a */
/*       m -- first dimension of a and u */
/*       n -- second dimension of a, and first dimension of v */
/*       a -- matrix to be SVD'd */

/*       output: */
/*       krank -- rank of the approximation to a */
/*       iu -- index in w of the first entry of the matrix */
/*             of orthonormal left singular vectors of a */
/*       iv -- index in w of the first entry of the matrix */
/*             of orthonormal right singular vectors of a */
/*       is -- index in w of the first entry of the array */
/*             of singular values of a; the singular values are stored */
/*             as complex*16 numbers whose imaginary parts are zeros */
/*       w -- array containing the singular values and singular vectors */
/*            of a; w doubles as a work array, and so must be at least */
/*            (krank+1)*(m+2*n+9)+8*min(m,n)+6*krank**2 */
/*            complex*16 elements long, where krank is the rank */
/*            output by the present routine */
/*       ier -- 0 when the routine terminates successfully; */
/*              -1000 when lw is too small; */
/*              other nonzero values when zgesdd bombs */

/*       _N.B._: This routine destroys a. Also, please beware that */
/*               the source code for this routine could be clearer. */
/*               w must be at least */
/*               (krank+1)*(m+2*n+9)+8*min(m,n)+6*krank**2 */
/*               complex*16 elements long, where krank is the rank */
/*               output by the present routine. */



    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --w;

    /* Function Body */
    io = min(*m,*n) << 3;


    *ier = 0;


/*       Compute a pivoted QR decomposition of a. */

    idzp_qrpiv_(eps, m, n, &a[a_offset], krank, &w[1], &w[io + 1]);


    if (*krank > 0) {


/*         Extract R from the QR decomposition. */

	idz_retriever_(m, n, &a[a_offset], krank, &w[io + 1]);


/*         Rearrange R according to ind. */

	idz_permuter_(krank, &w[1], krank, n, &w[io + 1]);


/*         Use LAPACK to SVD R, */
/*         storing the krank (krank x 1) left singular vectors */
/*         in w(io+krank*n+1 : io+krank*n+krank*krank). */

	*(unsigned char *)jobz = 'S';
	ldr = *krank;
/* Computing 2nd power */
	i__1 = *krank;
	lwork = i__1 * i__1 + (*krank << 1) + *n << 1;
	ldu = *krank;
	ldvadj = *krank;

/* Computing 2nd power */
	i__1 = *krank;
	ivi = io + *krank * *n + *krank * *krank + lwork + i__1 * i__1 * 3 + (
		*krank << 2) + 1;
	lv = *n * *krank;

	isi = ivi + lv;
	ls = *krank;

	if (*lw < isi + ls + *m * *krank - 1) {
	    *ier = -1000;
	    return 0;
	}

	zgesdd_(jobz, krank, n, &w[io + 1], &ldr, &w[isi], &w[io + *krank * *
		n + 1], &ldu, &w[ivi], &ldvadj, &w[io + *krank * *n + *krank *
		 *krank + 1], &lwork, &w[io + *krank * *n + *krank * *krank + 
		lwork + 1], &w[1], &info, (ftnlen)1);

	if (info != 0) {
	    *ier = info;
	    return 0;
	}


/*         Take the adjoint of w(ivi:ivi+lv-1) to obtain V. */

	*iv = 1;
	idz_adjer_(krank, n, &w[ivi], &w[*iv]);


/*         Copy w(isi:isi+ls/2) into w(is:is+ls-1). */

	*is = *iv + lv;

	idz_realcomp_(&ls, &w[isi], &w[*is]);


/*         Multiply the U from R from the left by Q to obtain the U */
/*         for A. */

	*iu = *is + ls;
	lu = *m * *krank;

	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {

	    i__2 = *krank;
	    for (j = 1; j <= i__2; ++j) {
		i__3 = *iu - 1 + j + *krank * (k - 1);
		i__4 = io + *krank * *n + j + *krank * (k - 1);
		w[i__3].r = w[i__4].r, w[i__3].i = w[i__4].i;
	    }

/* j */
	}

/* k */
	for (k = *krank; k >= 1; --k) {

	    i__1 = *krank + 1;
	    for (j = *m; j >= i__1; --j) {
		i__2 = *iu - 1 + j + *m * (k - 1);
		w[i__2].r = 0., w[i__2].i = 0.;
	    }

/* j */
	    for (j = *krank; j >= 1; --j) {
		i__1 = *iu - 1 + j + *m * (k - 1);
		i__2 = *iu - 1 + j + *krank * (k - 1);
		w[i__1].r = w[i__2].r, w[i__1].i = w[i__2].i;
	    }

/* j */
	}

/* k */
	ifadjoint = 0;
	idz_qmatmat_(&ifadjoint, m, n, &a[a_offset], krank, krank, &w[*iu], &
		w[*iu + lu + 1]);


    }


/* krank .gt. 0 */
    return 0;
} /* idzp_svd__ */

