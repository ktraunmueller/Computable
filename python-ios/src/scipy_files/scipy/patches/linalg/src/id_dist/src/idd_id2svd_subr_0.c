/* idd_id2svd_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_id2svd_(integer *m, integer *krank, doublereal *b, 
	integer *n, integer *list, doublereal *proj, doublereal *u, 
	doublereal *v, doublereal *s, integer *ier, doublereal *w)
{
    /* System generated locals */
    integer b_dim1, b_offset, proj_dim1, proj_offset, u_dim1, u_offset, 
	    v_dim1, v_offset, i__1;

    /* Local variables */
    static integer ip, ir, lp, it, lr, lt, lw;
    extern /* Subroutine */ int idd_id2svd0_(integer *, integer *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static integer ir2, ir3, lr2, lr3, iind, lind, iindt, lindt, iwork, lwork;


/*       converts an approximation to a matrix in the form of an ID */
/*       to an approximation in the form of an SVD. */

/*       input: */
/*       m -- first dimension of b */
/*       krank -- rank of the ID */
/*       b -- columns of the original matrix in the ID */
/*       list -- list of columns chosen from the original matrix */
/*               in the ID */
/*       n -- length of list and part of the second dimension of proj */
/*       proj -- projection coefficients in the ID */

/*       output: */
/*       u -- left singular vectors */
/*       v -- right singular vectors */
/*       s -- singular values */
/*       ier -- 0 when the routine terminates successfully; */
/*              nonzero otherwise */

/*       work: */
/*       w -- must be at least (krank+1)*(m+3*n)+26*krank**2 real*8 */
/*            elements long */

/*       _N.B._: This routine destroys b. */



/* Computing 2nd power */
    i__1 = *krank;
    /* Parameter adjustments */
    --s;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    b_dim1 = *m;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --w;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --list;

    /* Function Body */
    lw = 0;

    iwork = lw + 1;
/* Computing 2nd power */
    i__1 = *krank;
    lwork = i__1 * i__1 * 25;
    lw += lwork;

    ip = lw + 1;
    lp = *krank * *n;
    lw += lp;

    it = lw + 1;
    lt = *n * *krank;
    lw += lt;

    ir = lw + 1;
    lr = *krank * *n;
    lw += lr;

    ir2 = lw + 1;
    lr2 = *krank * *m;
    lw += lr2;

    ir3 = lw + 1;
    lr3 = *krank * *krank;
    lw += lr3;

    iind = lw + 1;
    lind = *n / 2 + 1;
    ++lw;

    iindt = lw + 1;
    lindt = *m / 2 + 1;
    ++lw;


    idd_id2svd0_(m, krank, &b[b_offset], n, &list[1], &proj[proj_offset], &u[
	    u_offset], &v[v_offset], &s[1], ier, &w[iwork], &w[ip], &w[it], &
	    w[ir], &w[ir2], &w[ir3], &w[iind], &w[iindt]);


    return 0;
} /* idd_id2svd__ */

