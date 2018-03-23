/* id_rand_subr_0.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int id_frand__0_(int n__, integer *n, doublereal *r__, 
	doublereal *t)
{
    /* Initialized data */

    static doublereal s[55] = { .2793574644042651,.1882566493961346,
	    .5202478134503912,.7568505373052146,.5682465992936152,
	    .5153148754383294,.7806554095454596,.01982474428974643,
	    .2520464262278498,.6423784715775962,.5802024387972178,
	    .3784471040388249,.07839919528229308,.6334519212594525,
	    .03387627157788001,.170906628388467,.4801610983518325,
	    .8983424668099422,.05358948687598758,.1265377231771848,
	    .8979988627693677,.6470084038238917,.3031709395541237,
	    .6674702804438126,.6318240977112699,.223522963387305,
	    .2784629939177633,.2365462014457445,.7226213454977284,
	    .8986523045307989,.5488233229247885,.39246054121412,
	    .6288356378374988,.6370664115760445,.5925600062791174,
	    .4322113919396362,.9766098520360393,.5168619893947437,
	    .6799970440779681,.4196004604766881,.2324473089903044,
	    .1439046416143282,.4670307948601256,.7076498261128343,
	    .9458030397562582,.4557892460080424,.3905930854589403,
	    .3361770064397268,.8303274937900278,.3041110304032945,
	    .5752684022049654,.07985703137991175,.5522643936454465,
	    .01956754937251801,.9920272858340107 };
    static doublereal s0[55] = { .2793574644042651,.1882566493961346,
	    .5202478134503912,.7568505373052146,.5682465992936152,
	    .5153148754383294,.7806554095454596,.01982474428974643,
	    .2520464262278498,.6423784715775962,.5802024387972178,
	    .3784471040388249,.07839919528229308,.6334519212594525,
	    .03387627157788001,.170906628388467,.4801610983518325,
	    .8983424668099422,.05358948687598758,.1265377231771848,
	    .8979988627693677,.6470084038238917,.3031709395541237,
	    .6674702804438126,.6318240977112699,.223522963387305,
	    .2784629939177633,.2365462014457445,.7226213454977284,
	    .8986523045307989,.5488233229247885,.39246054121412,
	    .6288356378374988,.6370664115760445,.5925600062791174,
	    .4322113919396362,.9766098520360393,.5168619893947437,
	    .6799970440779681,.4196004604766881,.2324473089903044,
	    .1439046416143282,.4670307948601256,.7076498261128343,
	    .9458030397562582,.4557892460080424,.3905930854589403,
	    .3361770064397268,.8303274937900278,.3041110304032945,
	    .5752684022049654,.07985703137991175,.5522643936454465,
	    .01956754937251801,.9920272858340107 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;
    static doublereal x;


/*       generates n pseudorandom numbers drawn uniformly from [0,1], */
/*       via a very efficient lagged Fibonnaci method. */
/*       Unlike routine id_srand, the present routine requires that */
/*       n be at least 55. */

/*       input: */
/*       n -- number of pseudorandom numbers to generate */

/*       output: */
/*       r -- array of pseudorandom numbers */

/*       _N.B._: n must be at least 55. */

/*       reference: */
/*       Press, Teukolsky, Vetterling, Flannery, "Numerical Recipes," */
/*            3rd edition, Cambridge University Press, 2007, */
/*            Section 7.1.5. */


    /* Parameter adjustments */
    if (r__) {
	--r__;
	}
    if (t) {
	--t;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_id_frandi;
	case 2: goto L_id_frando;
	}




    for (k = 1; k <= 24; ++k) {

	x = s[k + 30] - s[k - 1];
	if (x < 0.) {
	    x += 1;
	}
	r__[k] = x;

    }


/* k */
    for (k = 25; k <= 55; ++k) {

	x = r__[k - 24] - s[k - 1];
	if (x < 0.) {
	    x += 1;
	}
	r__[k] = x;

    }


/* k */
    i__1 = *n;
    for (k = 56; k <= i__1; ++k) {

	x = r__[k - 24] - r__[k - 55];
	if (x < 0.) {
	    x += 1;
	}
	r__[k] = x;

    }


/* k */
    for (k = 1; k <= 55; ++k) {
	s[k - 1] = r__[*n - 55 + k];
    }


/* k */
    return 0;




L_id_frandi:

/*       initializes the seed values in s */
/*       (any appropriately random numbers will do). */

/*       input: */
/*       t -- values to copy into s */

    for (k = 1; k <= 55; ++k) {
	s[k - 1] = t[k];
    }

/* k */
    return 0;




L_id_frando:

/*       initializes the seed values in s to their original values. */

    for (k = 1; k <= 55; ++k) {
	s[k - 1] = s0[k - 1];
    }

/* k */
    return 0;
} /* id_frand__ */

/* Subroutine */ int id_frand_(integer *n, doublereal *r__)
{
    return id_frand__0_(0, n, r__, (doublereal *)0);
    }

/* Subroutine */ int id_frandi_(doublereal *t)
{
    return id_frand__0_(1, (integer *)0, (doublereal *)0, t);
    }

/* Subroutine */ int id_frando_(void)
{
    return id_frand__0_(2, (integer *)0, (doublereal *)0, (doublereal *)0);
    }

