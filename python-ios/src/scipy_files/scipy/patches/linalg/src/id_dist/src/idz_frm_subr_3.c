/* idz_frm_subr_3.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idz_frmi_(integer *m, integer *n, doublecomplex *w)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static integer l, ia, lw;
    extern /* Subroutine */ int id_randperm_(integer *, doublecomplex *);
    static integer keep;
    extern /* Subroutine */ int zffti_(integer *, doublecomplex *);
    static integer nsteps;
    extern /* Subroutine */ int idz_poweroftwo_(integer *, integer *, 
	    integer *), idz_random_transf_init_(integer *, integer *, 
	    doublecomplex *, integer *);


/*       initializes data for the routine idz_frm. */

/*       input: */
/*       m -- length of the vector to be transformed */

/*       output: */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m */
/*       w -- initialization array to be used by routine idz_frm */


/*       glossary for the fully initialized w: */

/*       w(1) = m */
/*       w(2) = n */
/*       w(3:2+m) stores a permutation of m objects */
/*       w(3+m:2+m+n) stores a permutation of n objects */
/*       w(3+m+n) = address in w of the initialization array */
/*                  for idz_random_transf */
/*       w(4+m+n:int(w(3+m+n))-1) stores the initialization array */
/*                                for zfft */
/*       w(int(w(3+m+n)):16*m+70) stores the initialization array */
/*                                for idz_random_transf */


/*       _N.B._: n is an output of the present routine; */
/*               this routine changes n. */




/*       Find the greatest integer less than or equal to m */
/*       which is a power of two. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    idz_poweroftwo_(m, &l, n);


/*       Store m and n in w. */

    w[1].r = (doublereal) (*m), w[1].i = 0.;
    w[2].r = (doublereal) (*n), w[2].i = 0.;


/*       Store random permutations of m and n objects in w. */

    id_randperm_(m, &w[3]);
    id_randperm_(n, &w[*m + 3]);


/*       Store the address within w of the idz_random_transf_init */
/*       initialization data. */

    ia = *m + 4 + *n + (*n << 1) + 15;
    i__1 = *m + 3 + *n;
    w[i__1].r = (doublereal) ia, w[i__1].i = 0.;


/*       Store the initialization data for zfft in w. */

    zffti_(n, &w[*m + 4 + *n]);


/*       Store the initialization data for idz_random_transf_init in w. */

    nsteps = 3;
    idz_random_transf_init_(&nsteps, m, &w[ia], &keep);


/*       Calculate the total number of elements used in w. */

    lw = *m + 3 + *n + (*n << 1) + 15 + nsteps * 3 * *m + (*m << 1) + *m / 4 
	    + 50;

    if ((*m << 4) + 70 < lw) {
/* call prinf('lw = *',lw,1) */
/* call prinf('16m+70 = *',16*m+70,1) */
	s_stop("", (ftnlen)0);
    }


    return 0;
} /* idz_frmi__ */





/* Subroutine */ int idz_sfrmi_(integer *l, integer *m, integer *n, 
	doublecomplex *w)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int idz_sffti_(integer *, doublecomplex *, 
	    integer *, doublecomplex *);
    static integer ia, lw;
    extern /* Subroutine */ int id_randperm_(integer *, doublecomplex *);
    static integer keep, idummy, nsteps;
    extern /* Subroutine */ int idz_poweroftwo_(integer *, integer *, 
	    integer *), idz_random_transf_init_(integer *, integer *, 
	    doublecomplex *, integer *);


/*       initializes data for the routine idz_sfrm. */

/*       input: */
/*       l -- length of the transformed (output) vector */
/*       m -- length of the vector to be transformed */

/*       output: */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m */
/*       w -- initialization array to be used by routine idz_sfrm */


/*       glossary for the fully initialized w: */

/*       w(1) = m */
/*       w(2) = n */
/*       w(3) is unused */
/*       w(4:3+m) stores a permutation of m objects */
/*       w(4+m:3+m+l) stores the indices of the l outputs which idz_sfft */
/*                    calculates */
/*       w(4+m+l) = address in w of the initialization array */
/*                  for idz_random_transf */
/*       w(5+m+l:int(w(4+m+l))-1) stores the initialization array */
/*                                for idz_sfft */
/*       w(int(w(4+m+l)):19*m+70) stores the initialization array */
/*                                for idz_random_transf */


/*       _N.B._: n is an output of the present routine; */
/*               this routine changes n. */




/*       Find the greatest integer less than or equal to m */
/*       which is a power of two. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    idz_poweroftwo_(m, &idummy, n);


/*       Store m and n in w. */

    w[1].r = (doublereal) (*m), w[1].i = 0.;
    w[2].r = (doublereal) (*n), w[2].i = 0.;
    w[3].r = 0., w[3].i = 0.;


/*       Store random permutations of m and n objects in w. */

    id_randperm_(m, &w[4]);
    id_randperm_(n, &w[*m + 4]);


/*       Store the address within w of the idz_random_transf_init */
/*       initialization data. */

    ia = *m + 5 + *l + (*l << 1) + 15 + *n * 3;
    i__1 = *m + 4 + *l;
    w[i__1].r = (doublereal) ia, w[i__1].i = 0.;


/*       Store the initialization data for idz_sfft in w. */

    idz_sffti_(l, &w[*m + 4], n, &w[*m + 5 + *l]);


/*       Store the initialization data for idz_random_transf_init in w. */

    nsteps = 3;
    idz_random_transf_init_(&nsteps, m, &w[ia], &keep);


/*       Calculate the total number of elements used in w. */

    lw = *m + 4 + *l + (*l << 1) + 15 + *n * 3 + nsteps * 3 * *m + (*m << 1) 
	    + *m / 4 + 50;

    if (*m * 19 + 70 < lw) {
/* call prinf('lw = *',lw,1) */
/* call prinf('19m+70 = *',19*m+70,1) */
	s_stop("", (ftnlen)0);
    }


    return 0;
} /* idz_sfrmi__ */





/* Subroutine */ int idz_poweroftwo_(integer *m, integer *l, integer *n)
{

/*       computes l = floor(log_2(m)) and n = 2**l. */

/*       input: */
/*       m -- integer whose log_2 is to be taken */

/*       output: */
/*       l -- floor(log_2(m)) */
/*       n -- 2**l */



    *l = 0;
    *n = 1;

L1000:
    ++(*l);
    *n <<= 1;
    if (*n <= *m) {
	goto L1000;
    }

    --(*l);
    *n /= 2;


    return 0;
} /* idz_poweroftwo__ */

