/* id_rand.f -- translated by f2c (version 20100827).
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

/*       this file contains the following user-callable routines: */


/*       routine id_frand generates pseudorandom numbers */
/*       drawn uniformly from [0,1]. id_frand is more */
/*       efficient that id_srand, but cannot generate */
/*       fewer than 55 pseudorandom numbers per call. */

/*       routine id_srand generates pseudorandom numbers */
/*       drawn uniformly from [0,1]. id_srand is less */
/*       efficient that id_frand, but can generate */
/*       fewer than 55 pseudorandom numbers per call. */

/*       entry id_frandi initializes the seed values */
/*       for routine id_frand. */

/*       entry id_srandi initializes the seed values */
/*       for routine id_srand. */

/*       entry id_frando initializes the seed values */
/*       for routine id_frand to their original values. */

/*       entry id_srando initializes the seed values */
/*       for routine id_srand to their original values. */

/*       routine id_randperm generates a uniformly random permutation. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




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

/* Subroutine */ int id_frand__(integer *n, doublereal *r__)
{
    return id_frand__0_(0, n, r__, (doublereal *)0);
    }

/* Subroutine */ int id_frandi__(doublereal *t)
{
    return id_frand__0_(1, (integer *)0, (doublereal *)0, t);
    }

/* Subroutine */ int id_frando__(void)
{
    return id_frand__0_(2, (integer *)0, (doublereal *)0, (doublereal *)0);
    }





/* Subroutine */ int id_srand__0_(int n__, integer *n, doublereal *r__, 
	doublereal *t)
{
    /* Initialized data */

    static integer l = 55;
    static integer m = 24;
    static doublereal s[55] = { .8966049453474352,.7789471911260157,
	    .6071529762908476,.8287077988663865,.8249336255502409,
	    .5735259423199479,.2436346323812991,.2656149927259701,
	    .6594784809929011,.3432392503145575,.5051287353012308,
	    .1444493249757482,.7643753221285416,.4843422506977382,
	    .4427513254774826,.2965991475108561,.2650513544474467,
	    .02768759325778929,.6106305243078063,.4246918885003141,
	    .2863757386932874,.6211983878375777,.7534336463880467,
	    .7471458603576737,.2017455446928328,.9334235874832779,
	    .6343440435422822,.8819824804812527,.0199476140122246,
	    .7023693520374801,.6010088924817263,.06498095955562046,
	    .3090915456102685,.3014924769096677,.5820726822705102,
	    .3630527222866207,.3787166916242271,.3932772088505305,
	    .5570720335382,.9712062146993835,.1338293907964648,
	    .1857441593107195,.9102503893692572,.2623337538798778,
	    .3542828591321135,.02246286032456513,.7935703170405717,
	    .06051464729640567,.7271929955172147,.001968513010678739,
	    .4914223624495486,.873002317678945,.9639777091743168,
	    .1084256187532446,.8539399636754 };
    static doublereal s0[55] = { .8966049453474352,.7789471911260157,
	    .6071529762908476,.8287077988663865,.8249336255502409,
	    .5735259423199479,.2436346323812991,.2656149927259701,
	    .6594784809929011,.3432392503145575,.5051287353012308,
	    .1444493249757482,.7643753221285416,.4843422506977382,
	    .4427513254774826,.2965991475108561,.2650513544474467,
	    .02768759325778929,.6106305243078063,.4246918885003141,
	    .2863757386932874,.6211983878375777,.7534336463880467,
	    .7471458603576737,.2017455446928328,.9334235874832779,
	    .6343440435422822,.8819824804812527,.0199476140122246,
	    .7023693520374801,.6010088924817263,.06498095955562046,
	    .3090915456102685,.3014924769096677,.5820726822705102,
	    .3630527222866207,.3787166916242271,.3932772088505305,
	    .5570720335382,.9712062146993835,.1338293907964648,
	    .1857441593107195,.9102503893692572,.2623337538798778,
	    .3542828591321135,.02246286032456513,.7935703170405717,
	    .06051464729640567,.7271929955172147,.001968513010678739,
	    .4914223624495486,.873002317678945,.9639777091743168,
	    .1084256187532446,.8539399636754 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k;
    static doublereal x;


/*       generates n pseudorandom numbers drawn uniformly from [0,1], */
/*       via a very efficient lagged Fibonnaci method. */
/*       Unlike routine id_frand, the present routine does not requires */
/*       that n be at least 55. */

/*       input: */
/*       n -- number of pseudorandom numbers to generate */

/*       output: */
/*       r -- array of pseudorandom numbers */

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
	case 1: goto L_id_srandi;
	case 2: goto L_id_srando;
	}





    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {

/*         Run one step of the recurrence. */

	x = s[m - 1] - s[l - 1];
	if (x < 0.) {
	    x += 1;
	}
	s[l - 1] = x;
	r__[k] = x;

/*         Decrement l and m. */

	--l;
	--m;

/*         Circle back to the end if required. */

	if (l == 0) {
	    l = 55;
	}
	if (m == 0) {
	    m = 55;
	}

    }


/* k */
    return 0;




L_id_srandi:

/*       initializes the seed values in s */
/*       (any appropriately random numbers will do). */

/*       input: */
/*       t -- values to copy into s */

    for (k = 1; k <= 55; ++k) {
	s[k - 1] = t[k];
    }

/* k */
    l = 55;
    m = 24;

    return 0;




L_id_srando:

/*       initializes the seed values in s to their original values. */

    for (k = 1; k <= 55; ++k) {
	s[k - 1] = s0[k - 1];
    }

/* k */
    l = 55;
    m = 24;

    return 0;
} /* id_srand__ */

/* Subroutine */ int id_srand__(integer *n, doublereal *r__)
{
    return id_srand__0_(0, n, r__, (doublereal *)0);
    }

/* Subroutine */ int id_srandi__(doublereal *t)
{
    return id_srand__0_(1, (integer *)0, (doublereal *)0, t);
    }

/* Subroutine */ int id_srando__(void)
{
    return id_srand__0_(2, (integer *)0, (doublereal *)0, (doublereal *)0);
    }





/* Subroutine */ int id_randperm__(integer *n, integer *ind)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    extern /* Subroutine */ int id_srand__(integer *, doublereal *);
    static integer j, m;
    static doublereal r__;
    static integer iswap;


/*       draws a permutation ind uniformly at random from the group */
/*       of all permutations of n objects. */

/*       input: */
/*       n -- length of ind */

/*       output: */
/*       ind -- random permutation of length n */



/*       Initialize ind. */

    /* Parameter adjustments */
    --ind;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	ind[j] = j;
    }


/*       Shuffle ind via the Fisher-Yates (Knuth/Durstenfeld) algorithm. */

/* j */
    for (m = *n; m >= 2; --m) {

/*         Draw an integer uniformly at random from 1, 2, ..., m. */

	id_srand__(&c__1, &r__);
	j = (integer) (m * r__ + 1);

/*         Uncomment the following line if r could equal 1: */
/*         if(j .eq. m+1) j = m */

/*         Swap ind(j) and ind(m). */

	iswap = ind[j];
	ind[j] = ind[m];
	ind[m] = iswap;

    }


/* m */
    return 0;
} /* id_randperm__ */

