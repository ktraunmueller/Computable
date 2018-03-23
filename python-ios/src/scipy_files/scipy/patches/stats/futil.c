/* futil.f -- translated by f2c (version 20100827).
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

/*     Sorts an array arr(1:N) into ascending numerical order */
/*      using the QuickSort algorithm.  On output arr is replaced with its */
/*      sorted rearrangement. */
/* Subroutine */ int dqsort_(integer *n, doublereal *arr)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static doublereal a;
    static integer i__, j, k, l, ir;
    static doublereal temp;
    static integer istack[100], jstack;

/* F2PY INTENT(IN,OUT,COPY) ARR */
/* F2PY INTEGER, INTENT(HIDE), DEPEND(ARR) :: N=len(ARR) */
    /* Parameter adjustments */
    --arr;

    /* Function Body */
    jstack = 0;
    l = 1;
    ir = *n;
L1:
    if (ir - l < 7) {
	i__1 = ir;
	for (j = l + 1; j <= i__1; ++j) {
	    a = arr[j];
	    i__2 = l;
	    for (i__ = j - 1; i__ >= i__2; --i__) {
		if (arr[i__] <= a) {
		    goto L2;
		}
		arr[i__ + 1] = arr[i__];
	    }
	    i__ = l - 1;
L2:
	    arr[i__ + 1] = a;
	}
	if (jstack == 0) {
	    return 0;
	}
	ir = istack[jstack - 1];
	l = istack[jstack - 2];
	jstack += -2;
    } else {
	k = (l + ir) / 2;
	temp = arr[k];
	arr[k] = arr[l + 1];
	arr[l + 1] = temp;
	if (arr[l] > arr[ir]) {
	    temp = arr[l];
	    arr[l] = arr[ir];
	    arr[ir] = temp;
	}
	if (arr[l + 1] > arr[ir]) {
	    temp = arr[l + 1];
	    arr[l + 1] = arr[ir];
	    arr[ir] = temp;
	}
	if (arr[l] > arr[l + 1]) {
	    temp = arr[l];
	    arr[l] = arr[l + 1];
	    arr[l + 1] = temp;
	}
	i__ = l + 1;
	j = ir;
	a = arr[l + 1];
L3:
	++i__;
	if (arr[i__] < a) {
	    goto L3;
	}
L4:
	--j;
	if (arr[j] > a) {
	    goto L4;
	}
	if (j < i__) {
	    goto L5;
	}
	temp = arr[i__];
	arr[i__] = arr[j];
	arr[j] = temp;
	goto L3;
L5:
	arr[l + 1] = arr[j];
	arr[j] = a;
	jstack += 2;
	if (jstack > 100) {
	    return 0;
	}
	if (ir - i__ + 1 >= j - 1) {
	    istack[jstack - 1] = ir;
	    istack[jstack - 2] = i__;
	    ir = j - 1;
	} else {
	    istack[jstack - 1] = j - 1;
	    istack[jstack - 2] = l;
	    l = i__;
	}
    }
    goto L1;
} /* dqsort_ */

/*     Finds repeated elements of ARR and their occurrence incidence */
/*     reporting the result in REPLIST and REPNUM respectively. */
/*     NLIST is the number of repeated elements found. */
/*     Algorithm first sorts the list and then walks down it */
/*       counting repeats as they are found. */
/* Subroutine */ int dfreps_(doublereal *arr, integer *n, doublereal *replist,
	 integer *repnum, integer *nlist)
{
    static integer ind, nnum, repeat;
    extern /* Subroutine */ int dqsort_(integer *, doublereal *);
    static doublereal lastval;
    static integer howmany;

/* F2PY INTENT(IN) ARR */
/* F2PY INTENT(OUT) REPLIST */
/* F2PY INTENT(OUT) REPNUM */
/* F2PY INTENT(OUT) NLIST */
/* F2PY INTEGER, INTENT(HIDE), DEPEND(ARR) :: N=len(ARR) */
    /* Parameter adjustments */
    --repnum;
    --replist;
    --arr;

    /* Function Body */
    dqsort_(n, &arr[1]);
    lastval = arr[1];
    howmany = 0;
    ind = 2;
    nnum = 1;
    *nlist = 1;
    repeat = 0;
    while(ind <= *n) {
	if (arr[ind] != lastval) {
	    if (repeat == 1) {
		repnum[nnum] = howmany + 1;
		++nnum;
		repeat = 0;
		howmany = 0;
	    }
	} else {
	    ++howmany;
	    repeat = 1;
	    if (howmany == 1) {
		replist[*nlist] = arr[ind];
		++(*nlist);
	    }
	}
	lastval = arr[ind];
	++ind;
    }
    if (repeat == 1) {
	repnum[nnum] = howmany + 1;
    }
    --(*nlist);
    return 0;
} /* dfreps_ */

