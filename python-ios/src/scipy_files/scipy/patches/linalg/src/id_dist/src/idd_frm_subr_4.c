/* idd_frm_subr_4.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_frmi_(integer *m, integer *n, doublereal *w)
{
    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static integer l, ia, lw;
    extern /* Subroutine */ int id_randperm_(integer *, doublereal *);
    static integer keep;
    extern /* Subroutine */ int dffti_(integer *, doublereal *);
    static integer nsteps;
    extern /* Subroutine */ int idd_poweroftwo_(integer *, integer *, 
	    integer *), idd_random_transf_init_(integer *, integer *, 
	    doublereal *, integer *);


/*       initializes data for the routine idd_frm. */

/*       input: */
/*       m -- length of the vector to be transformed */

/*       output: */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m */
/*       w -- initialization array to be used by routine idd_frm */


/*       glossary for the fully initialized w: */

/*       w(1) = m */
/*       w(2) = n */
/*       w(3:2+m) stores a permutation of m objects */
/*       w(3+m:2+m+n) stores a permutation of n objects */
/*       w(3+m+n) = address in w of the initialization array */
/*                  for idd_random_transf */
/*       w(4+m+n:int(w(3+m+n))-1) stores the initialization array */
/*                                for dfft */
/*       w(int(w(3+m+n)):16*m+70) stores the initialization array */
/*                                for idd_random_transf */


/*       _N.B._: n is an output of the present routine; */
/*               this routine changes n. */




/*       Find the greatest integer less than or equal to m */
/*       which is a power of two. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    idd_poweroftwo_(m, &l, n);


/*       Store m and n in w. */

    w[1] = (doublereal) (*m);
    w[2] = (doublereal) (*n);


/*       Store random permutations of m and n objects in w. */

    id_randperm_(m, &w[3]);
    id_randperm_(n, &w[*m + 3]);


/*       Store the address within w of the idd_random_transf_init */
/*       initialization data. */

    ia = *m + 4 + *n + (*n << 1) + 15;
    w[*m + 3 + *n] = (doublereal) ia;


/*       Store the initialization data for dfft in w. */

    dffti_(n, &w[*m + 4 + *n]);


/*       Store the initialization data for idd_random_transf_init in w. */

    nsteps = 3;
    idd_random_transf_init_(&nsteps, m, &w[ia], &keep);


/*       Calculate the total number of elements used in w. */

    lw = *m + 3 + *n + (*n << 1) + 15 + nsteps * 3 * *m + (*m << 1) + *m / 4 
	    + 50;

    if ((*m << 4) + 70 < lw) {
/* call prinf('lw = *',lw,1) */
/* call prinf('16m+70 = *',16*m+70,1) */
	s_stop("", (ftnlen)0);
    }


    return 0;
} /* idd_frmi__ */





/* Subroutine */ int idd_sfrmi_(integer *l, integer *m, integer *n, 
	doublereal *w)
{
    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    extern /* Subroutine */ int idd_sffti_(integer *, doublereal *, integer *
	    , doublereal *);
    static integer l2, ia, lw;
    extern /* Subroutine */ int id_randperm_(integer *, doublereal *);
    static integer keep;
    extern /* Subroutine */ int idd_copyints_(integer *, doublereal *, 
	    doublereal *), idd_pairsamps_(integer *, integer *, doublereal *,
	     integer *, doublereal *, doublereal *);
    static integer idummy, nsteps;
    extern /* Subroutine */ int idd_poweroftwo_(integer *, integer *, 
	    integer *), idd_random_transf_init_(integer *, integer *, 
	    doublereal *, integer *);


/*       initializes data for the routine idd_sfrm. */

/*       input: */
/*       l -- length of the transformed (output) vector */
/*       m -- length of the vector to be transformed */

/*       output: */
/*       n -- greatest integer expressible as a positive integer power */
/*            of 2 that is less than or equal to m */
/*       w -- initialization array to be used by routine idd_sfrm */


/*       glossary for the fully initialized w: */

/*       w(1) = m */
/*       w(2) = n */
/*       w(3) = l2 */
/*       w(4:3+m) stores a permutation of m objects */
/*       w(4+m:3+m+l) stores the indices of the l outputs which idd_sfft */
/*                    calculates */
/*       w(4+m+l:3+m+l+l2) stores the indices of the l2 pairs of outputs */
/*                         which idd_sfft calculates */
/*       w(4+m+l+l2) = address in w of the initialization array */
/*                     for idd_random_transf */
/*       w(5+m+l+l2:int(w(4+m+l+l2))-1) stores the initialization array */
/*                                      for idd_sfft */
/*       w(int(w(4+m+l+l2)):25*m+90) stores the initialization array */
/*                                   for idd_random_transf */


/*       _N.B._: n is an output of the present routine; */
/*               this routine changes n. */




/*       Find the greatest integer less than or equal to m */
/*       which is a power of two. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    idd_poweroftwo_(m, &idummy, n);


/*       Store m and n in w. */

    w[1] = (doublereal) (*m);
    w[2] = (doublereal) (*n);


/*       Store random permutations of m and n objects in w. */

    id_randperm_(m, &w[4]);
    id_randperm_(n, &w[*m + 4]);


/*       Find the pairs of integers covering the integers in */
/*       w(4+m : 3+m+(l+1)/2). */

    idd_pairsamps_(n, l, &w[*m + 4], &l2, &w[*m + 4 + (*l << 1)], &w[*m + 4 
	    + *l * 3]);
    w[3] = (doublereal) l2;
    idd_copyints_(&l2, &w[*m + 4 + (*l << 1)], &w[*m + 4 + *l]);


/*       Store the address within w of the idd_random_transf_init */
/*       initialization data. */

    ia = *m + 5 + *l + l2 + (l2 << 2) + 30 + (*n << 3);
    w[*m + 4 + *l + l2] = (doublereal) ia;


/*       Store the initialization data for idd_sfft in w. */

    idd_sffti_(&l2, &w[*m + 4 + *l], n, &w[*m + 5 + *l + l2]);


/*       Store the initialization data for idd_random_transf_init in w. */

    nsteps = 3;
    idd_random_transf_init_(&nsteps, m, &w[ia], &keep);


/*       Calculate the total number of elements used in w. */

    lw = *m + 4 + *l + l2 + (l2 << 2) + 30 + (*n << 3) + nsteps * 3 * *m + (*
	    m << 1) + *m / 4 + 50;

    if (*m * 25 + 90 < lw) {
/* call prinf('lw = *',lw,1) */
/* call prinf('25m+90 = *',25*m+90,1) */
	s_stop("", (ftnlen)0);
    }


    return 0;
} /* idd_sfrmi__ */

