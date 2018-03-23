/* prini.f -- translated by f2c (version 20100827).
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





/* Subroutine */ int prini_0_(int n__, integer *ip1, integer *iq1, char *mes, 
	real *a, integer *n, doublereal *a2, doublereal *a4, integer *ia, 
	shortint *ia2, char *aa, ftnlen mes_len, ftnlen aa_len)
{
    /* Format strings */
    static char fmt_1200[] = "(6(2x,e11.5))";
    static char fmt_1400[] = "(6(2x,e11.5))";
    static char fmt_1450[] = "(2(2x,e22.16))";
    static char fmt_1500[] = "(6(2x,e11.5))";
    static char fmt_1600[] = "(10(1x,i7))";
    static char fmt_2000[] = "(1x,80a1)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer j, ip, iq;
    extern /* Subroutine */ int messpr_(char *, integer *, integer *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___3 = { 0, 0, 0, fmt_1200, 0 };
    static cilist io___5 = { 0, 0, 0, fmt_1200, 0 };
    static cilist io___6 = { 0, 0, 0, fmt_1400, 0 };
    static cilist io___7 = { 0, 0, 0, fmt_1400, 0 };
    static cilist io___8 = { 0, 0, 0, fmt_1450, 0 };
    static cilist io___9 = { 0, 0, 0, fmt_1450, 0 };
    static cilist io___10 = { 0, 0, 0, fmt_1500, 0 };
    static cilist io___11 = { 0, 0, 0, fmt_1500, 0 };
    static cilist io___12 = { 0, 0, 0, fmt_1600, 0 };
    static cilist io___13 = { 0, 0, 0, fmt_1600, 0 };
    static cilist io___14 = { 0, 0, 0, fmt_1600, 0 };
    static cilist io___15 = { 0, 0, 0, fmt_1600, 0 };
    static cilist io___16 = { 0, 0, 0, fmt_2000, 0 };
    static cilist io___17 = { 0, 0, 0, fmt_2000, 0 };


    /* Parameter adjustments */
    if (mes) {
	--mes;
	}
    if (a) {
	--a;
	}
    if (a2) {
	--a2;
	}
    if (a4) {
	--a4;
	}
    if (ia) {
	--ia;
	}
    if (ia2) {
	--ia2;
	}
    if (aa) {
	--aa;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_prin;
	case 2: goto L_prin2;
	case 3: goto L_prin2_long;
	case 4: goto L_prinq;
	case 5: goto L_prinf;
	case 6: goto L_prinf2;
	case 7: goto L_prina;
	}

    ip = *ip1;
    iq = *iq1;
    return 0;






L_prin:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
    if (ip != 0 && *n != 0) {
	io___3.ciunit = ip;
	s_wsfe(&io___3);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a[j], (ftnlen)sizeof(real));
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___5.ciunit = iq;
	s_wsfe(&io___5);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a[j], (ftnlen)sizeof(real));
	}
	e_wsfe();
    }
    return 0;





L_prin2:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
    if (ip != 0 && *n != 0) {
	io___6.ciunit = ip;
	s_wsfe(&io___6);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a2[j], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___7.ciunit = iq;
	s_wsfe(&io___7);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a2[j], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    return 0;





L_prin2_long:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
    if (ip != 0 && *n != 0) {
	io___8.ciunit = ip;
	s_wsfe(&io___8);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a2[j], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___9.ciunit = iq;
	s_wsfe(&io___9);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a2[j], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    return 0;





L_prinq:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
    if (ip != 0 && *n != 0) {
	io___10.ciunit = ip;
	s_wsfe(&io___10);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a4[j], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___11.ciunit = iq;
	s_wsfe(&io___11);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&a4[j], (ftnlen)sizeof(doublereal));
	}
	e_wsfe();
    }
    return 0;





L_prinf:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
    if (ip != 0 && *n != 0) {
	io___12.ciunit = ip;
	s_wsfe(&io___12);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&ia[j], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___13.ciunit = iq;
	s_wsfe(&io___13);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&ia[j], (ftnlen)sizeof(integer));
	}
	e_wsfe();
    }
    return 0;





L_prinf2:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
    if (ip != 0 && *n != 0) {
	io___14.ciunit = ip;
	s_wsfe(&io___14);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&ia2[j], (ftnlen)sizeof(shortint));
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___15.ciunit = iq;
	s_wsfe(&io___15);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, (char *)&ia2[j], (ftnlen)sizeof(shortint));
	}
	e_wsfe();
    }
    return 0;





L_prina:
    messpr_(mes + 1, &ip, &iq, (ftnlen)1);
/* L2000: */
    if (ip != 0 && *n != 0) {
	io___16.ciunit = ip;
	s_wsfe(&io___16);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, aa + j, (ftnlen)1);
	}
	e_wsfe();
    }
    if (iq != 0 && *n != 0) {
	io___17.ciunit = iq;
	s_wsfe(&io___17);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    do_fio(&c__1, aa + j, (ftnlen)1);
	}
	e_wsfe();
    }
    return 0;
} /* prini_ */

/* Subroutine */ int prini_(integer *ip1, integer *iq1)
{
    return prini_0_(0, ip1, iq1, (char *)0, (real *)0, (integer *)0, (
	    doublereal *)0, (doublereal *)0, (integer *)0, (shortint *)0, (
	    char *)0, (ftnint)0, (ftnint)0);
    }

/* Subroutine */ int prin_(char *mes, real *a, integer *n, ftnlen mes_len)
{
    return prini_0_(1, (integer *)0, (integer *)0, mes, a, n, (doublereal *)0,
	     (doublereal *)0, (integer *)0, (shortint *)0, (char *)0, mes_len,
	     (ftnint)0);
    }

/* Subroutine */ int prin2_(char *mes, doublereal *a2, integer *n, ftnlen 
	mes_len)
{
    return prini_0_(2, (integer *)0, (integer *)0, mes, (real *)0, n, a2, (
	    doublereal *)0, (integer *)0, (shortint *)0, (char *)0, mes_len, (
	    ftnint)0);
    }

/* Subroutine */ int prin2_long_(char *mes, doublereal *a2, integer *n, 
	ftnlen mes_len)
{
    return prini_0_(3, (integer *)0, (integer *)0, mes, (real *)0, n, a2, (
	    doublereal *)0, (integer *)0, (shortint *)0, (char *)0, mes_len, (
	    ftnint)0);
    }

/* Subroutine */ int prinq_(char *mes, doublereal *a4, integer *n, ftnlen 
	mes_len)
{
    return prini_0_(4, (integer *)0, (integer *)0, mes, (real *)0, n, (
	    doublereal *)0, a4, (integer *)0, (shortint *)0, (char *)0, 
	    mes_len, (ftnint)0);
    }

/* Subroutine */ int prinf_(char *mes, integer *ia, integer *n, ftnlen 
	mes_len)
{
    return prini_0_(5, (integer *)0, (integer *)0, mes, (real *)0, n, (
	    doublereal *)0, (doublereal *)0, ia, (shortint *)0, (char *)0, 
	    mes_len, (ftnint)0);
    }

/* Subroutine */ int prinf2_(char *mes, shortint *ia2, integer *n, ftnlen 
	mes_len)
{
    return prini_0_(6, (integer *)0, (integer *)0, mes, (real *)0, n, (
	    doublereal *)0, (doublereal *)0, (integer *)0, ia2, (char *)0, 
	    mes_len, (ftnint)0);
    }

/* Subroutine */ int prina_(char *mes, char *aa, integer *n, ftnlen mes_len, 
	ftnlen aa_len)
{
    return prini_0_(7, (integer *)0, (integer *)0, mes, (real *)0, n, (
	    doublereal *)0, (doublereal *)0, (integer *)0, (shortint *)0, aa, 
	    mes_len, aa_len);
    }






/* Subroutine */ int messpr_(char *mes, integer *ip, integer *iq, ftnlen 
	mes_len)
{
    /* Initialized data */

    static char ast[1] = "*";

    /* Format strings */
    static char fmt_1800[] = "(1x,80a1)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer i__, i1;

    /* Fortran I/O blocks */
    static cilist io___21 = { 0, 0, 0, fmt_1800, 0 };
    static cilist io___22 = { 0, 0, 0, fmt_1800, 0 };


    /* Parameter adjustments */
    --mes;

    /* Function Body */

/*         DETERMINE THE LENGTH OF THE MESSAGE */

    i1 = 0;
    for (i__ = 1; i__ <= 10000; ++i__) {
	if (*(unsigned char *)&mes[i__] == *(unsigned char *)&ast[0]) {
	    goto L1600;
	}
	i1 = i__;
/* L1400: */
    }
L1600:
    if (i1 != 0 && *ip != 0) {
	io___21.ciunit = *ip;
	s_wsfe(&io___21);
	i__1 = i1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, mes + i__, (ftnlen)1);
	}
	e_wsfe();
    }
    if (i1 != 0 && *iq != 0) {
	io___22.ciunit = *iq;
	s_wsfe(&io___22);
	i__1 = i1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    do_fio(&c__1, mes + i__, (ftnlen)1);
	}
	e_wsfe();
    }
    return 0;
} /* messpr_ */






/* Subroutine */ int ztime_(integer *i__)
{
    static integer j;

    j = 1;
    j = 7 - *i__ + j;
/* CCC    I=MRUN(J) */
    return 0;
} /* ztime_ */






/* Subroutine */ int msgmerge_(char *a, char *b, char *c__, ftnlen a_len, 
	ftnlen b_len, ftnlen c_len)
{
    /* Initialized data */

    static char ast[1] = "*";

    static integer i__, iadd;

    /* Parameter adjustments */
    --c__;
    --b;
    --a;

    /* Function Body */

    for (i__ = 1; i__ <= 1000; ++i__) {

	if (*(unsigned char *)&a[i__] == *(unsigned char *)&ast[0]) {
	    goto L1400;
	}
	*(unsigned char *)&c__[i__] = *(unsigned char *)&a[i__];
	iadd = i__;
/* L1200: */
    }

L1400:

    for (i__ = 1; i__ <= 1000; ++i__) {

	*(unsigned char *)&c__[iadd + i__] = *(unsigned char *)&b[i__];
	if (*(unsigned char *)&b[i__] == *(unsigned char *)&ast[0]) {
	    return 0;
	}
/* L1800: */
    }
    return 0;
} /* msgmerge_ */






/* Subroutine */ int fileflush_(integer *iw)
{
    /* Format strings */
    static char fmt_1200[] = "(1a1)";

    /* System generated locals */
    integer i__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    integer f_clos(cllist *), f_open(olist *), s_rsfe(cilist *), e_rsfe(void);

    /* Local variables */
    static integer i__;

    /* Fortran I/O blocks */
    static cilist io___28 = { 0, 0, 1, fmt_1200, 0 };



    cl__1.cerr = 0;
    cl__1.cunit = *iw;
    cl__1.csta = 0;
    f_clos(&cl__1);
    o__1.oerr = 0;
    o__1.ounit = *iw;
    o__1.ofnm = 0;
    o__1.orl = 0;
    o__1.osta = "old";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    for (i__ = 1; i__ <= 1000000; ++i__) {

	io___28.ciunit = *iw;
	i__1 = s_rsfe(&io___28);
	if (i__1 != 0) {
	    goto L1600;
	}
	i__1 = e_rsfe();
	if (i__1 != 0) {
	    goto L1600;
	}
/* L1400: */
    }
L1600:

    return 0;
} /* fileflush_ */






/* Subroutine */ int mach_zero_(doublereal *zero_mach__)
{
    static doublereal d__;
    static integer i__;
    static doublereal d1, d3, d2, d4;
    extern /* Subroutine */ int mach_zero0_(doublereal *, doublereal *, 
	    doublereal *);


    *zero_mach__ = 100.;

    d1 = 1.1f;
    d3 = 1.1f;
    d__ = 1.11f;
    for (i__ = 1; i__ <= 1000; ++i__) {

	d__ /= 2;
	d2 = d1 + d__;
	mach_zero0_(&d2, &d3, &d4);

	if (d4 == 0.) {
	    goto L1400;
	}

/* L1200: */
    }
L1400:

    *zero_mach__ = d__;
    return 0;
} /* mach_zero__ */






/* Subroutine */ int mach_zero0_(doublereal *a, doublereal *b, doublereal *
	c__)
{

    *c__ = *b - *a;
    return 0;
} /* mach_zero0__ */

