/* calc_lwork.f -- translated by f2c (version 20100827).
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

/* Table of constant values */

static integer c__1 = 1;
static integer c_n1 = -1;
static integer c__2 = 2;
static integer c__9 = 9;
static integer c__0 = 0;
static integer c__6 = 6;
static integer c__8 = 8;
static integer c__4 = 4;

/* Subroutine */ int gehrd_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *n, integer *lo, integer *hi, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1, i__2, i__3[2];
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nb;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);


/*     Returned maxwrk is acctually optimal lwork. */

/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: n,lo,hi */
/* Computing MIN */
/* Writing concatenation */
    i__3[0] = 1, a__1[0] = prefix;
    i__3[1] = 5, a__1[1] = "GEHRD";
    s_cat(ch__1, a__1, i__3, &c__2, (ftnlen)6);
    i__1 = 64, i__2 = ilaenv_(&c__1, ch__1, " ", n, lo, hi, &c_n1, (ftnlen)6, 
	    (ftnlen)1);
    nb = min(i__1,i__2);
    *max_lwork__ = *n * nb;
/* Computing MIN */
    i__1 = *max_lwork__, i__2 = max(1,*n);
    *min_lwork__ = min(i__1,i__2);
    return 0;
} /* gehrd_ */

/* Subroutine */ int gesdd_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *m, integer *n, integer *compute_uv__, ftnlen 
	prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer minmn, wrkbl, mnthr, bdspac, bdspan;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer minwrk, maxwrk, smlsiz;

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&m,&n,&compute_uv) */
/* f2py callprotoargument int*,int*,char*,int*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: m,n,compute_uv */
    minmn = min(*m,*n);
    mnthr = (integer) (minmn * 11. / 6.);
    minwrk = 1;
    maxwrk = 1;
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "GESDD";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    smlsiz = ilaenv_(&c__9, ch__1, " ", &c__0, &c__0, &c__0, &c__0, (ftnlen)6,
	     (ftnlen)1);
    if (*m >= *n) {

/*           Compute space needed for DBDSDC */

	bdspac = *n * 3 * *n + *n * 7;
/* Computing MAX */
	i__2 = *n * 12 + 4, i__3 = (*n << 3) + 2 + smlsiz * (smlsiz + 8);
	bdspan = max(i__2,i__3);
	if (*m >= mnthr) {
	    if (*compute_uv__ == 0) {

/*     Path 1 (M much larger than N, JOBZ='N') */

/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GEQRF";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		maxwrk = *n + *n * ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &
			c_n1, (ftnlen)6, (ftnlen)1);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GEBRD";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = maxwrk, i__3 = *n * 3 + (*n << 1) * ilaenv_(&c__1, 
			ch__1, " ", n, n, &c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
		maxwrk = max(i__2,i__3);
		maxwrk = max(maxwrk,bdspac);
		minwrk = bdspac;
	    } else {

/*     Path 4 (M much larger than N, JOBZ='A') */

/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GEQRF";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		wrkbl = *n + *n * ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &
			c_n1, (ftnlen)6, (ftnlen)1);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORGQR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *n + *m * ilaenv_(&c__1, ch__1, " ", m, 
			m, n, &c_n1, (ftnlen)6, (ftnlen)1);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GEBRD";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *n * 3 + (*n << 1) * ilaenv_(&c__1, 
			ch__1, " ", n, n, &c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *n * 3 + *n * ilaenv_(&c__1, ch__1, 
			"QLN", n, n, n, &c_n1, (ftnlen)6, (ftnlen)3);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *n * 3 + *n * ilaenv_(&c__1, ch__1, 
			"PRT", n, n, n, &c_n1, (ftnlen)6, (ftnlen)3);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
		i__2 = wrkbl, i__3 = bdspac + (*n << 1);
		wrkbl = max(i__2,i__3);
		maxwrk = *n * *n + wrkbl;
		minwrk = bdspac + *n * *n + *m + *n;
	    }
	} else {

/*     Path 5 (M at least N, but not much larger) */

/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "GEBRD";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    wrkbl = *n * 3 + (*m + *n) * ilaenv_(&c__1, ch__1, " ", m, n, &
		    c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
	    if (*compute_uv__ == 0) {
/* Computing MAX */
		i__2 = wrkbl, i__3 = bdspac + *n * 3;
		maxwrk = max(i__2,i__3);
		minwrk = *n * 3 + max(*m,bdspac);
	    } else {
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = maxwrk, i__3 = *n * 3 + *m * ilaenv_(&c__1, ch__1, 
			"QLN", m, m, n, &c_n1, (ftnlen)6, (ftnlen)3);
		maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = maxwrk, i__3 = *n * 3 + *n * ilaenv_(&c__1, ch__1, 
			"PRT", n, n, n, &c_n1, (ftnlen)6, (ftnlen)3);
		maxwrk = max(i__2,i__3);
/* Computing MAX */
		i__2 = maxwrk, i__3 = bdspac + (*n << 1) + *m;
		maxwrk = max(i__2,i__3);
		minwrk = bdspac + (*n << 1) + *m;
	    }
	}
    } else {

/*     Compute space needed for DBDSDC */

	bdspac = *m * 3 * *m + *m * 7;
/* Computing MAX */
	i__2 = *m * 12 + 4, i__3 = (*m << 3) + 2 + smlsiz * (smlsiz + 8);
	bdspan = max(i__2,i__3);
	if (*n >= mnthr) {
	    if (*compute_uv__ == 0) {

/*     Path 1t (N much larger than M, JOBZ='N') */

/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GELQF";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		maxwrk = *m + *m * ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &
			c_n1, (ftnlen)6, (ftnlen)1);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GEBRD";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = maxwrk, i__3 = *m * 3 + (*m << 1) * ilaenv_(&c__1, 
			ch__1, " ", m, m, &c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
		maxwrk = max(i__2,i__3);
		maxwrk = max(maxwrk,bdspac);
		minwrk = bdspac;
	    } else {

/*     Path 4t (N much larger than M, JOBZ='A') */

/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GELQF";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		wrkbl = *m + *m * ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &
			c_n1, (ftnlen)6, (ftnlen)1);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORGLQ";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *m + *n * ilaenv_(&c__1, ch__1, " ", n, 
			n, m, &c_n1, (ftnlen)6, (ftnlen)1);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "GEBRD";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *m * 3 + (*m << 1) * ilaenv_(&c__1, 
			ch__1, " ", m, m, &c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *m * 3 + *m * ilaenv_(&c__1, ch__1, 
			"QLN", m, m, m, &c_n1, (ftnlen)6, (ftnlen)3);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = wrkbl, i__3 = *m * 3 + *m * ilaenv_(&c__1, ch__1, 
			"PRT", m, m, m, &c_n1, (ftnlen)6, (ftnlen)3);
		wrkbl = max(i__2,i__3);
/* Computing MAX */
		i__2 = wrkbl, i__3 = bdspac + (*m << 1);
		wrkbl = max(i__2,i__3);
		maxwrk = wrkbl + *m * *m;
		minwrk = bdspac + *m * *m + *m + *n;
	    }
	} else {
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "GEBRD";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    wrkbl = *m * 3 + (*m + *n) * ilaenv_(&c__1, ch__1, " ", m, n, &
		    c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
	    if (*compute_uv__ == 0) {
/* Computing MAX */
		i__2 = wrkbl, i__3 = bdspac + *m * 3;
		maxwrk = max(i__2,i__3);
		minwrk = *m * 3 + max(*n,bdspac);
	    } else {
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = maxwrk, i__3 = *m * 3 + *m * ilaenv_(&c__1, ch__1, 
			"QLN", m, m, n, &c_n1, (ftnlen)6, (ftnlen)3);
		maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
		i__1[0] = 1, a__1[0] = prefix;
		i__1[1] = 5, a__1[1] = "ORMBR";
		s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
		i__2 = maxwrk, i__3 = *m * 3 + *n * ilaenv_(&c__1, ch__1, 
			"PRT", n, n, m, &c_n1, (ftnlen)6, (ftnlen)3);
		maxwrk = max(i__2,i__3);
/* Computing MAX */
		i__2 = maxwrk, i__3 = bdspac + (*m << 1);
		maxwrk = max(i__2,i__3);
		minwrk = bdspac + (*m << 1) + *n;
	    }
	}
    }
    *min_lwork__ = minwrk;
    *max_lwork__ = max(minwrk,maxwrk);
    return 0;
} /* gesdd_ */

/* Subroutine */ int gelss_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *m, integer *n, integer *nrhs, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer mm, minmn, maxmn, mnthr, bdspac;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer minwrk, maxwrk;

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&m,&n,&nrhs) */
/* f2py callprotoargument int*,int*,char*,int*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: m,n,nrhs */
    minmn = min(*m,*n);
    maxmn = max(*m,*n);
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "GELSS";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    mnthr = ilaenv_(&c__6, ch__1, " ", m, n, nrhs, &c_n1, (ftnlen)6, (ftnlen)
	    1);
    minwrk = 1;
    maxwrk = 0;
    mm = *m;
    if (*m >= *n && *m >= mnthr) {

/*     Path 1a - overdetermined, with many more rows than columns */

	mm = *n;
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "GEQRF";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = *n + *n * ilaenv_(&c__1, ch__1, " ", m, n, &
		c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
	maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "ORMQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = *n + *nrhs * ilaenv_(&c__1, ch__1, "LT", m, 
		nrhs, n, &c_n1, (ftnlen)6, (ftnlen)2);
	maxwrk = max(i__2,i__3);
    }
    if (*m >= *n) {

/*     Path 1 - overdetermined or exactly determined */

/*     Compute workspace neede for BDSQR */

/* Computing MAX */
	i__2 = 1, i__3 = *n * 5;
	bdspac = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "GEBRD";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = *n * 3 + (mm + *n) * ilaenv_(&c__1, ch__1, 
		" ", &mm, n, &c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
	maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "ORMBR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = *n * 3 + *nrhs * ilaenv_(&c__1, ch__1, "QLT", &
		mm, nrhs, n, &c_n1, (ftnlen)6, (ftnlen)3);
	maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "ORGBR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = *n * 3 + (*n - 1) * ilaenv_(&c__1, ch__1, "P", 
		n, n, n, &c_n1, (ftnlen)6, (ftnlen)1);
	maxwrk = max(i__2,i__3);
	maxwrk = max(maxwrk,bdspac);
/* Computing MAX */
	i__2 = maxwrk, i__3 = *n * *nrhs;
	maxwrk = max(i__2,i__3);
/* Computing MAX */
	i__2 = *n * 3 + mm, i__3 = *n * 3 + *nrhs, i__2 = max(i__2,i__3);
	minwrk = max(i__2,bdspac);
	maxwrk = max(minwrk,maxwrk);
    }
    if (*n > *m) {

/*     Compute workspace neede for DBDSQR */

/* Computing MAX */
	i__2 = 1, i__3 = *m * 5;
	bdspac = max(i__2,i__3);
/* Computing MAX */
	i__2 = *m * 3 + *nrhs, i__3 = *m * 3 + *n, i__2 = max(i__2,i__3);
	minwrk = max(i__2,bdspac);
	if (*n >= mnthr) {

/*     Path 2a - underdetermined, with many more columns */
/*     than rows */

/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "GELQF";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    maxwrk = *m + *m * ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &c_n1, 
		    (ftnlen)6, (ftnlen)1);
/* Computing MAX */
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "GEBRD";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    i__2 = maxwrk, i__3 = *m * *m + (*m << 2) + (*m << 1) * ilaenv_(&
		    c__1, ch__1, " ", m, m, &c_n1, &c_n1, (ftnlen)6, (ftnlen)
		    1);
	    maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "ORMBR";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    i__2 = maxwrk, i__3 = *m * *m + (*m << 2) + *nrhs * ilaenv_(&c__1,
		     ch__1, "QLT", m, nrhs, m, &c_n1, (ftnlen)6, (ftnlen)3);
	    maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "ORGBR";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    i__2 = maxwrk, i__3 = *m * *m + (*m << 2) + (*m - 1) * ilaenv_(&
		    c__1, ch__1, "P", m, m, m, &c_n1, (ftnlen)6, (ftnlen)1);
	    maxwrk = max(i__2,i__3);
/* Computing MAX */
	    i__2 = maxwrk, i__3 = *m * *m + *m + bdspac;
	    maxwrk = max(i__2,i__3);
	    if (*nrhs > 1) {
/* Computing MAX */
		i__2 = maxwrk, i__3 = *m * *m + *m + *m * *nrhs;
		maxwrk = max(i__2,i__3);
	    } else {
/* Computing MAX */
		i__2 = maxwrk, i__3 = *m * *m + (*m << 1);
		maxwrk = max(i__2,i__3);
	    }
/* Computing MAX */
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "ORMLQ";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    i__2 = maxwrk, i__3 = *m + *nrhs * ilaenv_(&c__1, ch__1, "LT", n, 
		    nrhs, m, &c_n1, (ftnlen)6, (ftnlen)2);
	    maxwrk = max(i__2,i__3);
	} else {

/*     Path 2 - underdetermined */

/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "GEBRD";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    maxwrk = *m * 3 + (*n + *m) * ilaenv_(&c__1, ch__1, " ", m, n, &
		    c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
/* Computing MAX */
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "ORMBR";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    i__2 = maxwrk, i__3 = *m * 3 + *nrhs * ilaenv_(&c__1, ch__1, 
		    "QLT", m, nrhs, m, &c_n1, (ftnlen)6, (ftnlen)3);
	    maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	    i__1[0] = 1, a__1[0] = prefix;
	    i__1[1] = 5, a__1[1] = "ORGBR";
	    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	    i__2 = maxwrk, i__3 = *m * 3 + *m * ilaenv_(&c__1, ch__1, "P", m, 
		    n, m, &c_n1, (ftnlen)6, (ftnlen)1);
	    maxwrk = max(i__2,i__3);
	    maxwrk = max(maxwrk,bdspac);
/* Computing MAX */
	    i__2 = maxwrk, i__3 = *n * *nrhs;
	    maxwrk = max(i__2,i__3);
	}
    }
    maxwrk = max(minwrk,maxwrk);
    minwrk = max(minwrk,1);
    *min_lwork__ = minwrk;
    *max_lwork__ = maxwrk;
    return 0;
} /* gelss_ */

/* Subroutine */ int getri_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *n, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2];
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nb;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&n) */
/* f2py callprotoargument int*,int*,char*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: n */
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "GETRI";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    nb = ilaenv_(&c__1, ch__1, " ", n, &c_n1, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
    *min_lwork__ = *n;
    *max_lwork__ = *n * nb;
    return 0;
} /* getri_ */

/* Subroutine */ int geev_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *n, integer *compute_vl__, integer *compute_vr__, 
	ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3, i__4, i__5;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer k, maxb;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer minwrk, maxwrk;
    static logical wantvl;
    static integer hswork;
    static logical wantvr;

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&n,&compute_vl,&compute_vr) */
/* f2py callprotoargument int*,int*,char*,int*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py integer optional,intent(in) :: compute_vl = 1,compute_vr = 1 */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: n */
    wantvl = *compute_vl__ == 1;
    wantvr = *compute_vr__ == 1;
    minwrk = 1;
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "GEHRD";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    maxwrk = (*n << 1) + *n * ilaenv_(&c__1, ch__1, " ", n, &c__1, n, &c__0, (
	    ftnlen)6, (ftnlen)1);
    if (! wantvl && ! wantvr) {
/* Computing MAX */
	i__2 = 1, i__3 = *n * 3;
	minwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = ilaenv_(&c__8, ch__1, "EN", n, &c__1, n, &c_n1, (ftnlen)6, (
		ftnlen)2);
	maxb = max(i__2,2);
/* Computing MIN */
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__4 = 2, i__5 = ilaenv_(&c__4, ch__1, "EN", n, &c__1, n, &c_n1, (
		ftnlen)6, (ftnlen)2);
	i__2 = min(maxb,*n), i__3 = max(i__4,i__5);
	k = min(i__2,i__3);
/* Computing MAX */
	i__2 = k * (k + 2), i__3 = *n << 1;
	hswork = max(i__2,i__3);
/* Computing MAX */
	i__2 = maxwrk, i__3 = *n + 1, i__2 = max(i__2,i__3), i__3 = *n + 
		hswork;
	maxwrk = max(i__2,i__3);
    } else {
/* Computing MAX */
	i__2 = 1, i__3 = *n << 2;
	minwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "ORGHR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = (*n << 1) + (*n - 1) * ilaenv_(&c__1, ch__1, 
		" ", n, &c__1, n, &c_n1, (ftnlen)6, (ftnlen)1);
	maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = ilaenv_(&c__8, ch__1, "SV", n, &c__1, n, &c_n1, (ftnlen)6, (
		ftnlen)2);
	maxb = max(i__2,2);
/* Computing MIN */
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__4 = 2, i__5 = ilaenv_(&c__4, ch__1, "SV", n, &c__1, n, &c_n1, (
		ftnlen)6, (ftnlen)2);
	i__2 = min(maxb,*n), i__3 = max(i__4,i__5);
	k = min(i__2,i__3);
/* Computing MAX */
	i__2 = k * (k + 2), i__3 = *n << 1;
	hswork = max(i__2,i__3);
/* Computing MAX */
	i__2 = maxwrk, i__3 = *n + 1, i__2 = max(i__2,i__3), i__3 = *n + 
		hswork;
	maxwrk = max(i__2,i__3);
/* Computing MAX */
	i__2 = maxwrk, i__3 = *n << 2;
	maxwrk = max(i__2,i__3);
    }
    *min_lwork__ = minwrk;
    *max_lwork__ = maxwrk;
    return 0;
} /* geev_ */

/* Subroutine */ int heev_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *n, integer *lower, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nb;
    static char uplo[1];
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&n,&lower) */
/* f2py callprotoargument int*,int*,char*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py integer optional,intent(in) :: lower = 0 */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: n */
    *(unsigned char *)uplo = 'L';
    if (*lower == 0) {
	*(unsigned char *)uplo = 'U';
    }
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "HETRD";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    nb = ilaenv_(&c__1, ch__1, uplo, n, &c_n1, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
/* Computing MAX */
    i__2 = 1, i__3 = (*n << 1) - 1;
    *min_lwork__ = max(i__2,i__3);
/* Computing MAX */
    i__2 = 1, i__3 = (nb + 1) * *n;
    *max_lwork__ = max(i__2,i__3);
    return 0;
} /* heev_ */

/* Subroutine */ int syev_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *n, integer *lower, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nb;
    static char uplo[1];
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&n,&lower) */
/* f2py callprotoargument int*,int*,char*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py integer optional,intent(in) :: lower = 0 */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: n */
    *(unsigned char *)uplo = 'L';
    if (*lower == 0) {
	*(unsigned char *)uplo = 'U';
    }
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "SYTRD";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    nb = ilaenv_(&c__1, ch__1, uplo, n, &c_n1, &c_n1, &c_n1, (ftnlen)6, (
	    ftnlen)1);
/* Computing MAX */
    i__2 = 1, i__3 = *n * 3 - 1;
    *min_lwork__ = max(i__2,i__3);
/* Computing MAX */
    i__2 = 1, i__3 = (nb + 2) * *n;
    *max_lwork__ = max(i__2,i__3);
    return 0;
} /* syev_ */

/* Subroutine */ int gees_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *n, integer *compute_v__, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3, i__4, i__5;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer k, maxb;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer minwrk, maxwrk, hswork;

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&n,&compute_v) */
/* f2py callprotoargument int*,int*,char*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py integer optional,intent(in) :: compute_v = 1 */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: n */
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "GEHRD";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    maxwrk = *n + *n * ilaenv_(&c__1, ch__1, " ", n, &c__1, n, &c__0, (ftnlen)
	    6, (ftnlen)1);
/* Computing MAX */
    i__2 = 1, i__3 = *n << 1;
    minwrk = max(i__2,i__3);
    if (*compute_v__ == 0) {
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = ilaenv_(&c__8, ch__1, "SN", n, &c__1, n, &c_n1, (ftnlen)6, (
		ftnlen)2);
	maxb = max(i__2,2);
/* Computing MIN */
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__4 = 2, i__5 = ilaenv_(&c__4, ch__1, "SN", n, &c__1, n, &c_n1, (
		ftnlen)6, (ftnlen)2);
	i__2 = min(maxb,*n), i__3 = max(i__4,i__5);
	k = min(i__2,i__3);
/* Computing MAX */
	i__2 = k * (k + 2), i__3 = *n << 1;
	hswork = max(i__2,i__3);
/* Computing MAX */
	i__2 = max(maxwrk,hswork);
	maxwrk = max(i__2,1);
    } else {
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "UNGHR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = maxwrk, i__3 = *n + (*n - 1) * ilaenv_(&c__1, ch__1, " ", n, &
		c__1, n, &c_n1, (ftnlen)6, (ftnlen)1);
	maxwrk = max(i__2,i__3);
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__2 = ilaenv_(&c__8, ch__1, "EN", n, &c__1, n, &c_n1, (ftnlen)6, (
		ftnlen)2);
	maxb = max(i__2,2);
/* Computing MIN */
/* Computing MAX */
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "HSEQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	i__4 = 2, i__5 = ilaenv_(&c__4, ch__1, "EN", n, &c__1, n, &c_n1, (
		ftnlen)6, (ftnlen)2);
	i__2 = min(maxb,*n), i__3 = max(i__4,i__5);
	k = min(i__2,i__3);
/* Computing MAX */
	i__2 = k * (k + 2), i__3 = *n << 1;
	hswork = max(i__2,i__3);
/* Computing MAX */
	i__2 = max(maxwrk,hswork);
	maxwrk = max(i__2,1);
    }
    *min_lwork__ = minwrk;
    *max_lwork__ = maxwrk;
    return 0;
} /* gees_ */

/* Subroutine */ int geqrf_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *m, integer *n, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nb;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&m,&n) */
/* f2py callprotoargument int*,int*,char*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: m,n */
/* Writing concatenation */
    i__1[0] = 1, a__1[0] = prefix;
    i__1[1] = 5, a__1[1] = "GEQRF";
    s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
    nb = ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &c_n1, (ftnlen)6, (ftnlen)1);
    *min_lwork__ = max(1,*n);
/* Computing MAX */
    i__2 = 1, i__3 = *n * nb;
    *max_lwork__ = max(i__2,i__3);
    return 0;
} /* geqrf_ */

/* Subroutine */ int gqr_(integer *min_lwork__, integer *max_lwork__, char *
	prefix, integer *m, integer *n, ftnlen prefix_len)
{
    /* System generated locals */
    address a__1[2];
    integer i__1[2], i__2, i__3;
    char ch__1[6];

    /* Builtin functions */
    /* Subroutine */ int s_cat(char *, char **, integer *, integer *, ftnlen);

    /* Local variables */
    static integer nb;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);

/* f2py callstatement (*f2py_func)(&min_lwork,&max_lwork,prefix,&m,&n) */
/* f2py callprotoargument int*,int*,char*,int*,int* */
/* f2py intent(out,out=minwrk) :: min_lwork */
/* f2py intent(out,out=maxwrk) :: max_lwork */
/* f2py intent(in) :: prefix */
/* f2py intent(in) :: m,n */
    if (*(unsigned char *)prefix == 'd' || *(unsigned char *)prefix == 's' || 
	    *(unsigned char *)prefix == 'D' || *(unsigned char *)prefix == 
	    'S') {
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "ORGQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	nb = ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &c_n1, (ftnlen)6, (
		ftnlen)1);
    } else {
/* Writing concatenation */
	i__1[0] = 1, a__1[0] = prefix;
	i__1[1] = 5, a__1[1] = "UNGQR";
	s_cat(ch__1, a__1, i__1, &c__2, (ftnlen)6);
	nb = ilaenv_(&c__1, ch__1, " ", m, n, &c_n1, &c_n1, (ftnlen)6, (
		ftnlen)1);
    }
    *min_lwork__ = max(1,*n);
/* Computing MAX */
    i__2 = 1, i__3 = *n * nb;
    *max_lwork__ = max(i__2,i__3);
    return 0;
} /* gqr_ */

