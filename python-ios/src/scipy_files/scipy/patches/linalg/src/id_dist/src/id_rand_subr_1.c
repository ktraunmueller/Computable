/* id_rand_subr_1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int id_srand_(integer *n, doublereal *r__)
{
    return id_srand__0_(0, n, r__, (doublereal *)0);
    }

/* Subroutine */ int id_srandi_(doublereal *t)
{
    return id_srand__0_(1, (integer *)0, (doublereal *)0, t);
    }

/* Subroutine */ int id_srando_(void)
{
    return id_srand__0_(2, (integer *)0, (doublereal *)0, (doublereal *)0);
    }

