/* iddp_rsvd.f -- translated by f2c (version 20100827).
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


/*       routine iddp_rsvd computes the SVD, to a specified precision, */
/*       of a matrix specified by routines for applying the matrix */
/*       and its transpose to arbitrary vectors. */
/*       This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int iddp_rsvd_(integer *lw, doublereal *eps, integer *m, 
	integer *n, U_fp matvect, doublereal *p1t, doublereal *p2t, 
	doublereal *p3t, doublereal *p4t, U_fp matvec, doublereal *p1, 
	doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, 
	integer *iu, integer *iv, integer *is, doublereal *w, integer *ier)
{
    /* System generated locals */
    integer i_1;

    /* Local variables */
    extern /* Subroutine */ int iddp_rid_(integer *, doublereal *, integer *,
	     integer *, U_fp, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, doublereal *, doublereal *, integer *);
    static integer k;
    extern int iddp_rsvd0_(integer *m, integer *n, U_fp matvect,
                     doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t,
                     U_fp matvec, doublereal *p1, doublereal *p2, doublereal *p3,
                     doublereal *p4, integer *krank, doublereal *u, doublereal *v,
                     doublereal *s, integer *ier, integer *list, doublereal *proj,
                            doublereal *col, doublereal *work);
    
    static integer lp, ls, lu, lv, lw2, isi, iui, ivi, icol, lcol, iproj, 
	    ilist, lproj, llist, iwork, lwork;


/*       constructs a rank-krank SVD  U Sigma V^T  approximating a */
/*       to precision eps, where matvect is a routine which applies a^T */
/*       to an arbitrary vector, and matvec is a routine */
/*       which applies a to an arbitrary vector; U is an m x krank */
/*       matrix whose columns are orthonormal, V is an n x krank */
/*       matrix whose columns are orthonormal, and Sigma is a diagonal */
/*       krank x krank matrix whose entries are all nonnegative. */
/*       The entries of U are stored in w, starting at w(iu); */
/*       the entries of V are stored in w, starting at w(iv). */
/*       The diagonal entries of Sigma are stored in w, */
/*       starting at w(is). This routine uses a randomized algorithm. */

/*       input: */
/*       lw -- maximum usable length (in real*8 elements) */
/*             of the array w */
/*       eps -- precision of the desired approximation */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       matvect -- routine which applies the transpose */
/*                  of the matrix to be SVD'd */
/*                  to an arbitrary vector; this routine must have */
/*                  a calling sequence of the form */

/*                  matvect(m,x,n,y,p1t,p2t,p3t,p4t), */

/*                  where m is the length of x, */
/*                  x is the vector to which the transpose */
/*                  of the matrix is to be applied, */
/*                  n is the length of y, */
/*                  y is the product of the transposed matrix and x, */
/*                  and p1t, p2t, p3t, and p4t are user-specified */
/*                  parameters */
/*       p1t -- parameter to be passed to routine matvect */
/*       p2t -- parameter to be passed to routine matvect */
/*       p3t -- parameter to be passed to routine matvect */
/*       p4t -- parameter to be passed to routine matvect */
/*       matvec -- routine which applies the matrix to be SVD'd */
/*                 to an arbitrary vector; this routine must have */
/*                 a calling sequence of the form */

/*                 matvec(n,x,m,y,p1,p2,p3,p4), */

/*                 where n is the length of x, */
/*                 x is the vector to which the matrix is to be applied, */
/*                 m is the length of y, */
/*                 y is the product of the matrix and x, */
/*                 and p1, p2, p3, and p4 are user-specified parameters */
/*       p1 -- parameter to be passed to routine matvec */
/*       p2 -- parameter to be passed to routine matvec */
/*       p3 -- parameter to be passed to routine matvec */
/*       p4 -- parameter to be passed to routine matvec */

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
/*            (krank+1)*(3*m+5*n+1)+25*krank**2 real*8 elements long, */
/*            where krank is the rank returned by the present routine */
/*       ier -- 0 when the routine terminates successfully; */
/*              -1000 when lw is too small; */
/*              other nonzero values when idd_id2svd bombs */

/*       _N.B._: w must be at least (krank+1)*(3*m+5*n+1)+25*krank**2 */
/*               real*8 elements long, where krank is the rank */
/*               returned by the present routine. Also, the algorithm */
/*               used by the present routine is randomized. */



/*       Allocate some memory. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    lw2 = 0;

    ilist = lw2 + 1;
    llist = *n;
    lw2 += llist;

    iproj = lw2 + 1;


/*       ID a. */

    lp = *lw - lw2;
    iddp_rid_(&lp, eps, m, n, (U_fp)matvect, p1t, p2t, p3t, p4t, krank, &w[
	    ilist], &w[iproj], ier);
    if (*ier != 0) {
	return 0;
    }


    if (*krank > 0) {


/*         Allocate more memory. */

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
	i_1 = *krank;
	lwork = (*krank + 1) * (*m + *n * 3) + i_1 * i_1 * 26;
	lw2 += lwork;


	if (*lw < lw2) {
	    *ier = -1000;
	    return 0;
	}


	iddp_rsvd0_(m, n, (U_fp)matvect, p1t, p2t, p3t, p4t, (U_fp)matvec, 
		p1, p2, p3, p4, krank, &w[iui], &w[ivi], &w[isi], ier, &w[
		ilist], &w[iproj], &w[icol], &w[iwork]);
	if (*ier != 0) {
	    return 0;
	}


	*iu = 1;
	*iv = *iu + lu;
	*is = *iv + lv;


/*         Copy the singular values and singular vectors */
/*         into their proper locations. */

	i_1 = lu;
	for (k = 1; k <= i_1; ++k) {
	    w[*iu + k - 1] = w[iui + k - 1];
	}

/* k */
	i_1 = lv;
	for (k = 1; k <= i_1; ++k) {
	    w[*iv + k - 1] = w[ivi + k - 1];
	}

/* k */
	i_1 = ls;
	for (k = 1; k <= i_1; ++k) {
	    w[*is + k - 1] = w[isi + k - 1];
	}


/* k */
    }


/* krank .gt. 0 */
    return 0;
} /* iddp_rsvd_ */





/* Subroutine */ int iddp_rsvd0_(integer *m, integer *n, U_fp matvect, 
	doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, 
	U_fp matvec, doublereal *p1, doublereal *p2, doublereal *p3, 
	doublereal *p4, integer *krank, doublereal *u, doublereal *v, 
	doublereal *s, integer *ier, integer *list, doublereal *proj, 
	doublereal *col, doublereal *work)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, proj_dim1, proj_offset, i_1;

    /* Local variables */
    extern /* Subroutine */ int idd_id2svd_(integer *, integer *, doublereal 
	    *, integer *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *, doublereal *), idd_getcols_(integer *, 
	    integer *, U_fp, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *);


/*       routine iddp_rsvd serves as a memory wrapper */
/*       for the present routine (please see routine iddp_rsvd */
/*       for further documentation). */



/*       Collect together the columns of a indexed by list into col. */

/* Computing 2nd power */
    i_1 = *krank;
    /* Parameter adjustments */
    --list;
    --work;
    --col;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;

    /* Function Body */
    idd_getcols_(m, n, (U_fp)matvec, p1, p2, p3, p4, krank, &list[1], &col[1]
	    , &work[1]);


/*       Convert the ID to an SVD. */

    idd_id2svd_(m, krank, &col[1], n, &list[1], &proj[proj_offset], &u[
	    u_offset], &v[v_offset], &s[1], ier, &work[1]);


    return 0;
} /* iddp_rsvd0_ */

