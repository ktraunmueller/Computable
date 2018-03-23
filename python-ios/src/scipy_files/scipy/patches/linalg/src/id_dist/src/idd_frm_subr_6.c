/* idd_frm_subr_6.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int idd_poweroftwo_(integer *m, integer *l, integer *n)
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
} /* idd_poweroftwo__ */

