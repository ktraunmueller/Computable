/* dfft.f -- translated by f2c (version 20100827).
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


/*                       FFTPACK */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*                   VERSION 4  APRIL 1985 */

/*      A PACKAGE OF FORTRAN SUBPROGRAMS FOR THE FAST FOURIER */
/*       TRANSFORM OF PERIODIC AND OTHER SYMMETRIC SEQUENCES */

/*                          BY */

/*                   PAUL N SWARZTRAUBER */

/*   NATIONAL CENTER FOR ATMOSPHERIC RESEARCH  BOULDER,COLORADO 80307 */

/*    WHICH IS SPONSORED BY THE NATIONAL SCIENCE FOUNDATION */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* THIS PACKAGE CONSISTS OF PROGRAMS WHICH PERFORM FAST FOURIER */
/* TRANSFORMS FOR BOTH COMPLEX AND REAL PERIODIC SEQUENCES AND */
/* CERTAIN OTHER SYMMETRIC SEQUENCES THAT ARE LISTED BELOW. */

/* 1.   DFFTI     INITIALIZE  DFFTF AND DFFTB */
/* 2.   DFFTF     FORWARD TRANSFORM OF A REAL PERIODIC SEQUENCE */
/* 3.   DFFTB     BACKWARD TRANSFORM OF A REAL COEFFICIENT ARRAY */

/* 4.   DZFFTI    INITIALIZE DZFFTF AND DZFFTB */
/* 5.   DZFFTF    A SIMPLIFIED REAL PERIODIC FORWARD TRANSFORM */
/* 6.   DZFFTB    A SIMPLIFIED REAL PERIODIC BACKWARD TRANSFORM */

/* 7.   DSINTI     INITIALIZE DSINT */
/* 8.   DSINT      SINE TRANSFORM OF A REAL ODD SEQUENCE */

/* 9.   DCOSTI     INITIALIZE DCOST */
/* 10.  DCOST      COSINE TRANSFORM OF A REAL EVEN SEQUENCE */

/* 11.  DSINQI     INITIALIZE DSINQF AND DSINQB */
/* 12.  DSINQF     FORWARD SINE TRANSFORM WITH ODD WAVE NUMBERS */
/* 13.  DSINQB     UNNORMALIZED INVERSE OF DSINQF */

/* 14.  DCOSQI     INITIALIZE DCOSQF AND DCOSQB */
/* 15.  DCOSQF     FORWARD COSINE TRANSFORM WITH ODD WAVE NUMBERS */
/* 16.  DCOSQB     UNNORMALIZED INVERSE OF DCOSQF */

/* 17.  ZFFTI     INITIALIZE ZFFTF AND ZFFTB */
/* 18.  ZFFTF     FORWARD TRANSFORM OF A COMPLEX PERIODIC SEQUENCE */
/* 19.  ZFFTB     UNNORMALIZED INVERSE OF ZFFTF */


/* ****************************************************************** */

/* SUBROUTINE DFFTI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DFFTI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* BOTH DFFTF AND DFFTB. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED. */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 2*N+15. */
/*         THE SAME WORK ARRAY CAN BE USED FOR BOTH DFFTF AND DFFTB */
/*         AS LONG AS N REMAINS UNCHANGED. DIFFERENT WSAVE ARRAYS */
/*         ARE REQUIRED FOR DIFFERENT VALUES OF N. THE CONTENTS OF */
/*         WSAVE MUST NOT BE CHANGED BETWEEN CALLS OF DFFTF OR DFFTB. */

/* ****************************************************************** */

/* SUBROUTINE DFFTF(N,R,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DFFTF COMPUTES THE FOURIER COEFFICIENTS OF A REAL */
/* PERODIC SEQUENCE (FOURIER ANALYSIS). THE TRANSFORM IS DEFINED */
/* BELOW AT OUTPUT PARAMETER R. */

/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY R TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */
/*         N MAY CHANGE SO LONG AS DIFFERENT WORK ARRAYS ARE PROVIDED */

/* R       A REAL ARRAY OF LENGTH N WHICH CONTAINS THE SEQUENCE */
/*         TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 2*N+15. */
/*         IN THE PROGRAM THAT CALLS DFFTF. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DFFTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */
/*         THE SAME WSAVE ARRAY CAN BE USED BY DFFTF AND DFFTB. */


/* OUTPUT PARAMETERS */

/* R       R(1) = THE SUM FROM I=1 TO I=N OF R(I) */

/*         IF N IS EVEN SET L =N/2   , IF N IS ODD SET L = (N+1)/2 */

/*           THEN FOR K = 2,...,L */

/*              R(2*K-2) = THE SUM FROM I = 1 TO I = N OF */

/*                   R(I)*COS((K-1)*(I-1)*2*PI/N) */

/*              R(2*K-1) = THE SUM FROM I = 1 TO I = N OF */

/*                  -R(I)*SIN((K-1)*(I-1)*2*PI/N) */

/*         IF N IS EVEN */

/*              R(N) = THE SUM FROM I = 1 TO I = N OF */

/*                   (-1)**(I-1)*R(I) */

/*  *****  NOTE */
/*              THIS TRANSFORM IS UNNORMALIZED SINCE A CALL OF DFFTF */
/*              FOLLOWED BY A CALL OF DFFTB WILL MULTIPLY THE INPUT */
/*              SEQUENCE BY N. */

/* WSAVE   CONTAINS RESULTS WHICH MUST NOT BE DESTROYED BETWEEN */
/*         CALLS OF DFFTF OR DFFTB. */


/* ****************************************************************** */

/* SUBROUTINE DFFTB(N,R,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DFFTB COMPUTES THE REAL PERODIC SEQUENCE FROM ITS */
/* FOURIER COEFFICIENTS (FOURIER SYNTHESIS). THE TRANSFORM IS DEFINED */
/* BELOW AT OUTPUT PARAMETER R. */

/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY R TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */
/*         N MAY CHANGE SO LONG AS DIFFERENT WORK ARRAYS ARE PROVIDED */

/* R       A REAL ARRAY OF LENGTH N WHICH CONTAINS THE SEQUENCE */
/*         TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 2*N+15. */
/*         IN THE PROGRAM THAT CALLS DFFTB. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DFFTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */
/*         THE SAME WSAVE ARRAY CAN BE USED BY DFFTF AND DFFTB. */


/* OUTPUT PARAMETERS */

/* R       FOR N EVEN AND FOR I = 1,...,N */

/*              R(I) = R(1)+(-1)**(I-1)*R(N) */

/*                   PLUS THE SUM FROM K=2 TO K=N/2 OF */

/*                    2.*R(2*K-2)*COS((K-1)*(I-1)*2*PI/N) */

/*                   -2.*R(2*K-1)*SIN((K-1)*(I-1)*2*PI/N) */

/*         FOR N ODD AND FOR I = 1,...,N */

/*              R(I) = R(1) PLUS THE SUM FROM K=2 TO K=(N+1)/2 OF */

/*                   2.*R(2*K-2)*COS((K-1)*(I-1)*2*PI/N) */

/*                  -2.*R(2*K-1)*SIN((K-1)*(I-1)*2*PI/N) */

/*  *****  NOTE */
/*              THIS TRANSFORM IS UNNORMALIZED SINCE A CALL OF DFFTF */
/*              FOLLOWED BY A CALL OF DFFTB WILL MULTIPLY THE INPUT */
/*              SEQUENCE BY N. */

/* WSAVE   CONTAINS RESULTS WHICH MUST NOT BE DESTROYED BETWEEN */
/*         CALLS OF DFFTB OR DFFTF. */


/* ****************************************************************** */

/* SUBROUTINE DZFFTI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DZFFTI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* BOTH DZFFTF AND DZFFTB. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED. */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         THE SAME WORK ARRAY CAN BE USED FOR BOTH DZFFTF AND DZFFTB */
/*         AS LONG AS N REMAINS UNCHANGED. DIFFERENT WSAVE ARRAYS */
/*         ARE REQUIRED FOR DIFFERENT VALUES OF N. */


/* ****************************************************************** */

/* SUBROUTINE DZFFTF(N,R,AZERO,A,B,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DZFFTF COMPUTES THE FOURIER COEFFICIENTS OF A REAL */
/* PERODIC SEQUENCE (FOURIER ANALYSIS). THE TRANSFORM IS DEFINED */
/* BELOW AT OUTPUT PARAMETERS AZERO,A AND B. DZFFTF IS A SIMPLIFIED */
/* BUT SLOWER VERSION OF DFFTF. */

/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY R TO BE TRANSFORMED.  THE METHOD */
/*         IS MUST EFFICIENT WHEN N IS THE PRODUCT OF SMALL PRIMES. */

/* R       A REAL ARRAY OF LENGTH N WHICH CONTAINS THE SEQUENCE */
/*         TO BE TRANSFORMED. R IS NOT DESTROYED. */


/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         IN THE PROGRAM THAT CALLS DZFFTF. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DZFFTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */
/*         THE SAME WSAVE ARRAY CAN BE USED BY DZFFTF AND DZFFTB. */

/* OUTPUT PARAMETERS */

/* AZERO   THE SUM FROM I=1 TO I=N OF R(I)/N */

/* A,B     FOR N EVEN B(N/2)=0. AND A(N/2) IS THE SUM FROM I=1 TO */
/*         I=N OF (-1)**(I-1)*R(I)/N */

/*         FOR N EVEN DEFINE KMAX=N/2-1 */
/*         FOR N ODD  DEFINE KMAX=(N-1)/2 */

/*         THEN FOR  K=1,...,KMAX */

/*              A(K) EQUALS THE SUM FROM I=1 TO I=N OF */

/*                   2./N*R(I)*COS(K*(I-1)*2*PI/N) */

/*              B(K) EQUALS THE SUM FROM I=1 TO I=N OF */

/*                   2./N*R(I)*SIN(K*(I-1)*2*PI/N) */


/* ****************************************************************** */

/* SUBROUTINE DZFFTB(N,R,AZERO,A,B,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DZFFTB COMPUTES A REAL PERODIC SEQUENCE FROM ITS */
/* FOURIER COEFFICIENTS (FOURIER SYNTHESIS). THE TRANSFORM IS */
/* DEFINED BELOW AT OUTPUT PARAMETER R. DZFFTB IS A SIMPLIFIED */
/* BUT SLOWER VERSION OF DFFTB. */

/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE OUTPUT ARRAY R.  THE METHOD IS MOST */
/*         EFFICIENT WHEN N IS THE PRODUCT OF SMALL PRIMES. */

/* AZERO   THE CONSTANT FOURIER COEFFICIENT */

/* A,B     ARRAYS WHICH CONTAIN THE REMAINING FOURIER COEFFICIENTS */
/*         THESE ARRAYS ARE NOT DESTROYED. */

/*         THE LENGTH OF THESE ARRAYS DEPENDS ON WHETHER N IS EVEN OR */
/*         ODD. */

/*         IF N IS EVEN N/2    LOCATIONS ARE REQUIRED */
/*         IF N IS ODD (N-1)/2 LOCATIONS ARE REQUIRED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         IN THE PROGRAM THAT CALLS DZFFTB. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DZFFTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */
/*         THE SAME WSAVE ARRAY CAN BE USED BY DZFFTF AND DZFFTB. */


/* OUTPUT PARAMETERS */

/* R       IF N IS EVEN DEFINE KMAX=N/2 */
/*         IF N IS ODD  DEFINE KMAX=(N-1)/2 */

/*         THEN FOR I=1,...,N */

/*              R(I)=AZERO PLUS THE SUM FROM K=1 TO K=KMAX OF */

/*              A(K)*COS(K*(I-1)*2*PI/N)+B(K)*SIN(K*(I-1)*2*PI/N) */

/* ********************* COMPLEX NOTATION ************************** */

/*         FOR J=1,...,N */

/*         R(J) EQUALS THE SUM FROM K=-KMAX TO K=KMAX OF */

/*              C(K)*EXP(I*K*(J-1)*2*PI/N) */

/*         WHERE */

/*              C(K) = .5*CMPLX(A(K),-B(K))   FOR K=1,...,KMAX */

/*              C(-K) = CONJG(C(K)) */

/*              C(0) = AZERO */

/*                   AND I=SQRT(-1) */

/* *************** AMPLITUDE - PHASE NOTATION *********************** */

/*         FOR I=1,...,N */

/*         R(I) EQUALS AZERO PLUS THE SUM FROM K=1 TO K=KMAX OF */

/*              ALPHA(K)*COS(K*(I-1)*2*PI/N+BETA(K)) */

/*         WHERE */

/*              ALPHA(K) = SQRT(A(K)*A(K)+B(K)*B(K)) */

/*              COS(BETA(K))=A(K)/ALPHA(K) */

/*              SIN(BETA(K))=-B(K)/ALPHA(K) */

/* ****************************************************************** */

/* SUBROUTINE DSINTI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DSINTI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* SUBROUTINE DSINT. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N+1 IS A PRODUCT OF SMALL PRIMES. */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WITH AT LEAST INT(2.5*N+15) LOCATIONS. */
/*         DIFFERENT WSAVE ARRAYS ARE REQUIRED FOR DIFFERENT VALUES */
/*         OF N. THE CONTENTS OF WSAVE MUST NOT BE CHANGED BETWEEN */
/*         CALLS OF DSINT. */

/* ****************************************************************** */

/* SUBROUTINE DSINT(N,X,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DSINT COMPUTES THE DISCRETE FOURIER SINE TRANSFORM */
/* OF AN ODD SEQUENCE X(I). THE TRANSFORM IS DEFINED BELOW AT */
/* OUTPUT PARAMETER X. */

/* DSINT IS THE UNNORMALIZED INVERSE OF ITSELF SINCE A CALL OF DSINT */
/* FOLLOWED BY ANOTHER CALL OF DSINT WILL MULTIPLY THE INPUT SEQUENCE */
/* X BY 2*(N+1). */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE DSINT MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE DSINTI(N,WSAVE). */

/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N+1 IS THE PRODUCT OF SMALL PRIMES. */

/* X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED */


/* WSAVE   A WORK ARRAY WITH DIMENSION AT LEAST INT(2.5*N+15) */
/*         IN THE PROGRAM THAT CALLS DSINT. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DSINTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */

/* OUTPUT PARAMETERS */

/* X       FOR I=1,...,N */

/*              X(I)= THE SUM FROM K=1 TO K=N */

/*                   2*X(K)*SIN(K*I*PI/(N+1)) */

/*              A CALL OF DSINT FOLLOWED BY ANOTHER CALL OF */
/*              DSINT WILL MULTIPLY THE SEQUENCE X BY 2*(N+1). */
/*              HENCE DSINT IS THE UNNORMALIZED INVERSE */
/*              OF ITSELF. */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT BE */
/*         DESTROYED BETWEEN CALLS OF DSINT. */

/* ****************************************************************** */

/* SUBROUTINE DCOSTI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DCOSTI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* SUBROUTINE DCOST. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N-1 IS A PRODUCT OF SMALL PRIMES. */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         DIFFERENT WSAVE ARRAYS ARE REQUIRED FOR DIFFERENT VALUES */
/*         OF N. THE CONTENTS OF WSAVE MUST NOT BE CHANGED BETWEEN */
/*         CALLS OF DCOST. */

/* ****************************************************************** */

/* SUBROUTINE DCOST(N,X,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DCOST COMPUTES THE DISCRETE FOURIER COSINE TRANSFORM */
/* OF AN EVEN SEQUENCE X(I). THE TRANSFORM IS DEFINED BELOW AT OUTPUT */
/* PARAMETER X. */

/* DCOST IS THE UNNORMALIZED INVERSE OF ITSELF SINCE A CALL OF DCOST */
/* FOLLOWED BY ANOTHER CALL OF DCOST WILL MULTIPLY THE INPUT SEQUENCE */
/* X BY 2*(N-1). THE TRANSFORM IS DEFINED BELOW AT OUTPUT PARAMETER X */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE DCOST MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE DCOSTI(N,WSAVE). */

/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE SEQUENCE X. N MUST BE GREATER THAN 1. */
/*         THE METHOD IS MOST EFFICIENT WHEN N-1 IS A PRODUCT OF */
/*         SMALL PRIMES. */

/* X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15 */
/*         IN THE PROGRAM THAT CALLS DCOST. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DCOSTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */

/* OUTPUT PARAMETERS */

/* X       FOR I=1,...,N */

/*             X(I) = X(1)+(-1)**(I-1)*X(N) */

/*              + THE SUM FROM K=2 TO K=N-1 */

/*                  2*X(K)*COS((K-1)*(I-1)*PI/(N-1)) */

/*              A CALL OF DCOST FOLLOWED BY ANOTHER CALL OF */
/*              DCOST WILL MULTIPLY THE SEQUENCE X BY 2*(N-1) */
/*              HENCE DCOST IS THE UNNORMALIZED INVERSE */
/*              OF ITSELF. */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT BE */
/*         DESTROYED BETWEEN CALLS OF DCOST. */

/* ****************************************************************** */

/* SUBROUTINE DSINQI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DSINQI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* BOTH DSINQF AND DSINQB. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED. THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         THE SAME WORK ARRAY CAN BE USED FOR BOTH DSINQF AND DSINQB */
/*         AS LONG AS N REMAINS UNCHANGED. DIFFERENT WSAVE ARRAYS */
/*         ARE REQUIRED FOR DIFFERENT VALUES OF N. THE CONTENTS OF */
/*         WSAVE MUST NOT BE CHANGED BETWEEN CALLS OF DSINQF OR DSINQB. */

/* ****************************************************************** */

/* SUBROUTINE DSINQF(N,X,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DSINQF COMPUTES THE FAST FOURIER TRANSFORM OF QUARTER */
/* WAVE DATA. THAT IS , DSINQF COMPUTES THE COEFFICIENTS IN A SINE */
/* SERIES REPRESENTATION WITH ONLY ODD WAVE NUMBERS. THE TRANSFORM */
/* IS DEFINED BELOW AT OUTPUT PARAMETER X. */

/* DSINQB IS THE UNNORMALIZED INVERSE OF DSINQF SINCE A CALL OF DSINQF */
/* FOLLOWED BY A CALL OF DSINQB WILL MULTIPLY THE INPUT SEQUENCE X */
/* BY 4*N. */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE DSINQF MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE DSINQI(N,WSAVE). */


/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY X TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */

/* X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         IN THE PROGRAM THAT CALLS DSINQF. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DSINQI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */

/* OUTPUT PARAMETERS */

/* X       FOR I=1,...,N */

/*              X(I) = (-1)**(I-1)*X(N) */

/*                 + THE SUM FROM K=1 TO K=N-1 OF */

/*                 2*X(K)*SIN((2*I-1)*K*PI/(2*N)) */

/*              A CALL OF DSINQF FOLLOWED BY A CALL OF */
/*              DSINQB WILL MULTIPLY THE SEQUENCE X BY 4*N. */
/*              THEREFORE DSINQB IS THE UNNORMALIZED INVERSE */
/*              OF DSINQF. */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT */
/*         BE DESTROYED BETWEEN CALLS OF DSINQF OR DSINQB. */

/* ****************************************************************** */

/* SUBROUTINE DSINQB(N,X,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DSINQB COMPUTES THE FAST FOURIER TRANSFORM OF QUARTER */
/* WAVE DATA. THAT IS , DSINQB COMPUTES A SEQUENCE FROM ITS */
/* REPRESENTATION IN TERMS OF A SINE SERIES WITH ODD WAVE NUMBERS. */
/* THE TRANSFORM IS DEFINED BELOW AT OUTPUT PARAMETER X. */

/* DSINQF IS THE UNNORMALIZED INVERSE OF DSINQB SINCE A CALL OF DSINQB */
/* FOLLOWED BY A CALL OF DSINQF WILL MULTIPLY THE INPUT SEQUENCE X */
/* BY 4*N. */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE DSINQB MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE DSINQI(N,WSAVE). */


/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY X TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */

/* X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         IN THE PROGRAM THAT CALLS DSINQB. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DSINQI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */

/* OUTPUT PARAMETERS */

/* X       FOR I=1,...,N */

/*              X(I)= THE SUM FROM K=1 TO K=N OF */

/*                4*X(K)*SIN((2K-1)*I*PI/(2*N)) */

/*              A CALL OF DSINQB FOLLOWED BY A CALL OF */
/*              DSINQF WILL MULTIPLY THE SEQUENCE X BY 4*N. */
/*              THEREFORE DSINQF IS THE UNNORMALIZED INVERSE */
/*              OF DSINQB. */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT */
/*         BE DESTROYED BETWEEN CALLS OF DSINQB OR DSINQF. */

/* ****************************************************************** */

/* SUBROUTINE DCOSQI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DCOSQI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* BOTH DCOSQF AND DCOSQB. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE ARRAY TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15. */
/*         THE SAME WORK ARRAY CAN BE USED FOR BOTH DCOSQF AND DCOSQB */
/*         AS LONG AS N REMAINS UNCHANGED. DIFFERENT WSAVE ARRAYS */
/*         ARE REQUIRED FOR DIFFERENT VALUES OF N. THE CONTENTS OF */
/*         WSAVE MUST NOT BE CHANGED BETWEEN CALLS OF DCOSQF OR DCOSQB. */

/* ****************************************************************** */

/* SUBROUTINE DCOSQF(N,X,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DCOSQF COMPUTES THE FAST FOURIER TRANSFORM OF QUARTER */
/* WAVE DATA. THAT IS , DCOSQF COMPUTES THE COEFFICIENTS IN A COSINE */
/* SERIES REPRESENTATION WITH ONLY ODD WAVE NUMBERS. THE TRANSFORM */
/* IS DEFINED BELOW AT OUTPUT PARAMETER X */

/* DCOSQF IS THE UNNORMALIZED INVERSE OF DCOSQB SINCE A CALL OF DCOSQF */
/* FOLLOWED BY A CALL OF DCOSQB WILL MULTIPLY THE INPUT SEQUENCE X */
/* BY 4*N. */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE DCOSQF MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE DCOSQI(N,WSAVE). */


/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY X TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */

/* X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 3*N+15 */
/*         IN THE PROGRAM THAT CALLS DCOSQF. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DCOSQI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */

/* OUTPUT PARAMETERS */

/* X       FOR I=1,...,N */

/*              X(I) = X(1) PLUS THE SUM FROM K=2 TO K=N OF */

/*                 2*X(K)*COS((2*I-1)*(K-1)*PI/(2*N)) */

/*              A CALL OF DCOSQF FOLLOWED BY A CALL OF */
/*              DCOSQB WILL MULTIPLY THE SEQUENCE X BY 4*N. */
/*              THEREFORE DCOSQB IS THE UNNORMALIZED INVERSE */
/*              OF DCOSQF. */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT */
/*         BE DESTROYED BETWEEN CALLS OF DCOSQF OR DCOSQB. */

/* ****************************************************************** */

/* SUBROUTINE DCOSQB(N,X,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE DCOSQB COMPUTES THE FAST FOURIER TRANSFORM OF QUARTER */
/* WAVE DATA. THAT IS , DCOSQB COMPUTES A SEQUENCE FROM ITS */
/* REPRESENTATION IN TERMS OF A COSINE SERIES WITH ODD WAVE NUMBERS. */
/* THE TRANSFORM IS DEFINED BELOW AT OUTPUT PARAMETER X. */

/* DCOSQB IS THE UNNORMALIZED INVERSE OF DCOSQF SINCE A CALL OF DCOSQB */
/* FOLLOWED BY A CALL OF DCOSQF WILL MULTIPLY THE INPUT SEQUENCE X */
/* BY 4*N. */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE DCOSQB MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE DCOSQI(N,WSAVE). */


/* INPUT PARAMETERS */

/* N       THE LENGTH OF THE ARRAY X TO BE TRANSFORMED.  THE METHOD */
/*         IS MOST EFFICIENT WHEN N IS A PRODUCT OF SMALL PRIMES. */

/* X       AN ARRAY WHICH CONTAINS THE SEQUENCE TO BE TRANSFORMED */

/* WSAVE   A WORK ARRAY THAT MUST BE DIMENSIONED AT LEAST 3*N+15 */
/*         IN THE PROGRAM THAT CALLS DCOSQB. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE DCOSQI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */

/* OUTPUT PARAMETERS */

/* X       FOR I=1,...,N */

/*              X(I)= THE SUM FROM K=1 TO K=N OF */

/*                4*X(K)*COS((2*K-1)*(I-1)*PI/(2*N)) */

/*              A CALL OF DCOSQB FOLLOWED BY A CALL OF */
/*              DCOSQF WILL MULTIPLY THE SEQUENCE X BY 4*N. */
/*              THEREFORE DCOSQF IS THE UNNORMALIZED INVERSE */
/*              OF DCOSQB. */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT */
/*         BE DESTROYED BETWEEN CALLS OF DCOSQB OR DCOSQF. */

/* ****************************************************************** */

/* SUBROUTINE ZFFTI(N,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE ZFFTI INITIALIZES THE ARRAY WSAVE WHICH IS USED IN */
/* BOTH ZFFTF AND ZFFTB. THE PRIME FACTORIZATION OF N TOGETHER WITH */
/* A TABULATION OF THE TRIGONOMETRIC FUNCTIONS ARE COMPUTED AND */
/* STORED IN WSAVE. */

/* INPUT PARAMETER */

/* N       THE LENGTH OF THE SEQUENCE TO BE TRANSFORMED */

/* OUTPUT PARAMETER */

/* WSAVE   A WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 4*N+15 */
/*         THE SAME WORK ARRAY CAN BE USED FOR BOTH ZFFTF AND ZFFTB */
/*         AS LONG AS N REMAINS UNCHANGED. DIFFERENT WSAVE ARRAYS */
/*         ARE REQUIRED FOR DIFFERENT VALUES OF N. THE CONTENTS OF */
/*         WSAVE MUST NOT BE CHANGED BETWEEN CALLS OF ZFFTF OR ZFFTB. */

/* ****************************************************************** */

/* SUBROUTINE ZFFTF(N,C,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE ZFFTF COMPUTES THE FORWARD COMPLEX DISCRETE FOURIER */
/* TRANSFORM (THE FOURIER ANALYSIS). EQUIVALENTLY , ZFFTF COMPUTES */
/* THE FOURIER COEFFICIENTS OF A COMPLEX PERIODIC SEQUENCE. */
/* THE TRANSFORM IS DEFINED BELOW AT OUTPUT PARAMETER C. */

/* THE TRANSFORM IS NOT NORMALIZED. TO OBTAIN A NORMALIZED TRANSFORM */
/* THE OUTPUT MUST BE DIVIDED BY N. OTHERWISE A CALL OF ZFFTF */
/* FOLLOWED BY A CALL OF ZFFTB WILL MULTIPLY THE SEQUENCE BY N. */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE ZFFTF MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE ZFFTI(N,WSAVE). */

/* INPUT PARAMETERS */


/* N      THE LENGTH OF THE COMPLEX SEQUENCE C. THE METHOD IS */
/*        MORE EFFICIENT WHEN N IS THE PRODUCT OF SMALL PRIMES. N */

/* C      A COMPLEX ARRAY OF LENGTH N WHICH CONTAINS THE SEQUENCE */

/* WSAVE   A REAL WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 4N+15 */
/*         IN THE PROGRAM THAT CALLS ZFFTF. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE ZFFTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */
/*         THE SAME WSAVE ARRAY CAN BE USED BY ZFFTF AND ZFFTB. */

/* OUTPUT PARAMETERS */

/* C      FOR J=1,...,N */

/*            C(J)=THE SUM FROM K=1,...,N OF */

/*                  C(K)*EXP(-I*(J-1)*(K-1)*2*PI/N) */

/*                        WHERE I=SQRT(-1) */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT BE */
/*         DESTROYED BETWEEN CALLS OF SUBROUTINE ZFFTF OR ZFFTB */

/* ****************************************************************** */

/* SUBROUTINE ZFFTB(N,C,WSAVE) */

/* ****************************************************************** */

/* SUBROUTINE ZFFTB COMPUTES THE BACKWARD COMPLEX DISCRETE FOURIER */
/* TRANSFORM (THE FOURIER SYNTHESIS). EQUIVALENTLY , ZFFTB COMPUTES */
/* A COMPLEX PERIODIC SEQUENCE FROM ITS FOURIER COEFFICIENTS. */
/* THE TRANSFORM IS DEFINED BELOW AT OUTPUT PARAMETER C. */

/* A CALL OF ZFFTF FOLLOWED BY A CALL OF ZFFTB WILL MULTIPLY THE */
/* SEQUENCE BY N. */

/* THE ARRAY WSAVE WHICH IS USED BY SUBROUTINE ZFFTB MUST BE */
/* INITIALIZED BY CALLING SUBROUTINE ZFFTI(N,WSAVE). */

/* INPUT PARAMETERS */


/* N      THE LENGTH OF THE COMPLEX SEQUENCE C. THE METHOD IS */
/*        MORE EFFICIENT WHEN N IS THE PRODUCT OF SMALL PRIMES. */

/* C      A COMPLEX ARRAY OF LENGTH N WHICH CONTAINS THE SEQUENCE */

/* WSAVE   A REAL WORK ARRAY WHICH MUST BE DIMENSIONED AT LEAST 4N+15 */
/*         IN THE PROGRAM THAT CALLS ZFFTB. THE WSAVE ARRAY MUST BE */
/*         INITIALIZED BY CALLING SUBROUTINE ZFFTI(N,WSAVE) AND A */
/*         DIFFERENT WSAVE ARRAY MUST BE USED FOR EACH DIFFERENT */
/*         VALUE OF N. THIS INITIALIZATION DOES NOT HAVE TO BE */
/*         REPEATED SO LONG AS N REMAINS UNCHANGED THUS SUBSEQUENT */
/*         TRANSFORMS CAN BE OBTAINED FASTER THAN THE FIRST. */
/*         THE SAME WSAVE ARRAY CAN BE USED BY ZFFTF AND ZFFTB. */

/* OUTPUT PARAMETERS */

/* C      FOR J=1,...,N */

/*            C(J)=THE SUM FROM K=1,...,N OF */

/*                  C(K)*EXP(I*(J-1)*(K-1)*2*PI/N) */

/*                        WHERE I=SQRT(-1) */

/* WSAVE   CONTAINS INITIALIZATION CALCULATIONS WHICH MUST NOT BE */
/*         DESTROYED BETWEEN CALLS OF SUBROUTINE ZFFTF OR ZFFTB */



/* ["SEND INDEX FOR VFFTPK" DESCRIBES A VECTORIZED VERSION OF FFTPACK] */



/* Subroutine */ int zfftb1_(integer *n, doublereal *c__, doublereal *ch, 
	doublereal *wa, integer *ifac)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k1, l1, l2, n2, na, nf, ip, iw, ix2, ix3, ix4, nac, 
	    ido, idl1, idot;
    extern /* Subroutine */ int dpassb_(integer *, integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), dpassb2_(integer *, 
	    integer *, doublereal *, doublereal *, doublereal *), dpassb3_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), dpassb4_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dpassb5_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --ifac;
    --wa;
    --ch;
    --c__;

    /* Function Body */
    nf = ifac[2];
    na = 0;
    l1 = 1;
    iw = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	l2 = ip * l1;
	ido = *n / l2;
	idot = ido + ido;
	idl1 = idot * l1;
	if (ip != 4) {
	    goto L103;
	}
	ix2 = iw + idot;
	ix3 = ix2 + idot;
	if (na != 0) {
	    goto L101;
	}
	dpassb4_(&idot, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3]);
	goto L102;
L101:
	dpassb4_(&idot, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3]);
L102:
	na = 1 - na;
	goto L115;
L103:
	if (ip != 2) {
	    goto L106;
	}
	if (na != 0) {
	    goto L104;
	}
	dpassb2_(&idot, &l1, &c__[1], &ch[1], &wa[iw]);
	goto L105;
L104:
	dpassb2_(&idot, &l1, &ch[1], &c__[1], &wa[iw]);
L105:
	na = 1 - na;
	goto L115;
L106:
	if (ip != 3) {
	    goto L109;
	}
	ix2 = iw + idot;
	if (na != 0) {
	    goto L107;
	}
	dpassb3_(&idot, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2]);
	goto L108;
L107:
	dpassb3_(&idot, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2]);
L108:
	na = 1 - na;
	goto L115;
L109:
	if (ip != 5) {
	    goto L112;
	}
	ix2 = iw + idot;
	ix3 = ix2 + idot;
	ix4 = ix3 + idot;
	if (na != 0) {
	    goto L110;
	}
	dpassb5_(&idot, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3], &
		wa[ix4]);
	goto L111;
L110:
	dpassb5_(&idot, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3], &
		wa[ix4]);
L111:
	na = 1 - na;
	goto L115;
L112:
	if (na != 0) {
	    goto L113;
	}
	dpassb_(&nac, &idot, &ip, &l1, &idl1, &c__[1], &c__[1], &c__[1], &ch[
		1], &ch[1], &wa[iw]);
	goto L114;
L113:
	dpassb_(&nac, &idot, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__[1],
		 &c__[1], &wa[iw]);
L114:
	if (nac != 0) {
	    na = 1 - na;
	}
L115:
	l1 = l2;
	iw += (ip - 1) * idot;
/* L116: */
    }
    if (na == 0) {
	return 0;
    }
    n2 = *n + *n;
    i__1 = n2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = ch[i__];
/* L117: */
    }
    return 0;
} /* zfftb1_ */

/* Subroutine */ int zfftb_(integer *n, doublereal *c__, doublereal *wsave)
{
    static integer iw1, iw2;
    extern /* Subroutine */ int zfftb1_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;
    --c__;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    iw1 = *n + *n + 1;
    iw2 = iw1 + *n + *n;
    zfftb1_(n, &c__[1], &wsave[1], &wsave[iw1], (integer*)&wsave[iw2]);
    return 0;
} /* zfftb_ */

/* Subroutine */ int zfftf1_(integer *n, doublereal *c__, doublereal *ch, 
	doublereal *wa, integer *ifac)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k1, l1, l2, n2, na, nf, ip, iw, ix2, ix3, ix4, nac, 
	    ido, idl1, idot;
    extern /* Subroutine */ int dpassf_(integer *, integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), dpassf2_(integer *, 
	    integer *, doublereal *, doublereal *, doublereal *), dpassf3_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), dpassf4_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dpassf5_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --ifac;
    --wa;
    --ch;
    --c__;

    /* Function Body */
    nf = ifac[2];
    na = 0;
    l1 = 1;
    iw = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	l2 = ip * l1;
	ido = *n / l2;
	idot = ido + ido;
	idl1 = idot * l1;
	if (ip != 4) {
	    goto L103;
	}
	ix2 = iw + idot;
	ix3 = ix2 + idot;
	if (na != 0) {
	    goto L101;
	}
	dpassf4_(&idot, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3]);
	goto L102;
L101:
	dpassf4_(&idot, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3]);
L102:
	na = 1 - na;
	goto L115;
L103:
	if (ip != 2) {
	    goto L106;
	}
	if (na != 0) {
	    goto L104;
	}
	dpassf2_(&idot, &l1, &c__[1], &ch[1], &wa[iw]);
	goto L105;
L104:
	dpassf2_(&idot, &l1, &ch[1], &c__[1], &wa[iw]);
L105:
	na = 1 - na;
	goto L115;
L106:
	if (ip != 3) {
	    goto L109;
	}
	ix2 = iw + idot;
	if (na != 0) {
	    goto L107;
	}
	dpassf3_(&idot, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2]);
	goto L108;
L107:
	dpassf3_(&idot, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2]);
L108:
	na = 1 - na;
	goto L115;
L109:
	if (ip != 5) {
	    goto L112;
	}
	ix2 = iw + idot;
	ix3 = ix2 + idot;
	ix4 = ix3 + idot;
	if (na != 0) {
	    goto L110;
	}
	dpassf5_(&idot, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3], &
		wa[ix4]);
	goto L111;
L110:
	dpassf5_(&idot, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3], &
		wa[ix4]);
L111:
	na = 1 - na;
	goto L115;
L112:
	if (na != 0) {
	    goto L113;
	}
	dpassf_(&nac, &idot, &ip, &l1, &idl1, &c__[1], &c__[1], &c__[1], &ch[
		1], &ch[1], &wa[iw]);
	goto L114;
L113:
	dpassf_(&nac, &idot, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__[1],
		 &c__[1], &wa[iw]);
L114:
	if (nac != 0) {
	    na = 1 - na;
	}
L115:
	l1 = l2;
	iw += (ip - 1) * idot;
/* L116: */
    }
    if (na == 0) {
	return 0;
    }
    n2 = *n + *n;
    i__1 = n2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = ch[i__];
/* L117: */
    }
    return 0;
} /* zfftf1_ */

/* Subroutine */ int zfftf_(integer *n, doublereal *c__, doublereal *wsave)
{
    static integer iw1, iw2;
    extern /* Subroutine */ int zfftf1_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;
    --c__;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    iw1 = *n + *n + 1;
    iw2 = iw1 + *n + *n;
    zfftf1_(n, &c__[1], &wsave[1], &wsave[iw1], (integer*)&wsave[iw2]);
    return 0;
} /* zfftf_ */

/* Subroutine */ int zffti1_(integer *n, doublereal *wa, integer *ifac)
{
    /* Initialized data */

    static integer ntryh[4] = { 3,4,2,5 };

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, i1, k1, l1, l2, ib;
    static doublereal fi;
    static integer ld, ii, nf, ip, nl, nq, nr;
    static doublereal arg;
    static integer ido, ipm;
    static doublereal tpi, argh;
    static integer idot, ntry;
    static doublereal argld;

    /* Parameter adjustments */
    --ifac;
    --wa;

    /* Function Body */
    nl = *n;
    nf = 0;
    j = 0;
L101:
    ++j;
    if (j - 4 <= 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    ntry = ntryh[j - 1];
    goto L104;
L103:
    ntry += 2;
L104:
    nq = nl / ntry;
    nr = nl - ntry * nq;
    if (nr != 0) {
	goto L101;
    } else {
	goto L105;
    }
L105:
    ++nf;
    ifac[nf + 2] = ntry;
    nl = nq;
    if (ntry != 2) {
	goto L107;
    }
    if (nf == 1) {
	goto L107;
    }
    i__1 = nf;
    for (i__ = 2; i__ <= i__1; ++i__) {
	ib = nf - i__ + 2;
	ifac[ib + 2] = ifac[ib + 1];
/* L106: */
    }
    ifac[3] = 2;
L107:
    if (nl != 1) {
	goto L104;
    }
    ifac[1] = *n;
    ifac[2] = nf;
    tpi = 6.2831853071795864769252867665590057;
    argh = tpi / (doublereal) (*n);
    i__ = 2;
    l1 = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	ld = 0;
	l2 = l1 * ip;
	ido = *n / l2;
	idot = ido + ido + 2;
	ipm = ip - 1;
	i__2 = ipm;
	for (j = 1; j <= i__2; ++j) {
	    i1 = i__;
	    wa[i__ - 1] = 1.;
	    wa[i__] = 0.;
	    ld += l1;
	    fi = 0.;
	    argld = (doublereal) ld * argh;
	    i__3 = idot;
	    for (ii = 4; ii <= i__3; ii += 2) {
		i__ += 2;
		fi += 1.;
		arg = fi * argld;
		wa[i__ - 1] = cos(arg);
		wa[i__] = sin(arg);
/* L108: */
	    }
	    if (ip <= 5) {
		goto L109;
	    }
	    wa[i1 - 1] = wa[i__ - 1];
	    wa[i1] = wa[i__];
L109:
	    ;
	}
	l1 = l2;
/* L110: */
    }
    return 0;
} /* zffti1_ */

/* Subroutine */ int zffti_(integer *n, doublereal *wsave)
{
    static integer iw1, iw2;
    extern /* Subroutine */ int zffti1_(integer *, doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    iw1 = *n + *n + 1;
    iw2 = iw1 + *n + *n;
    zffti1_(n, &wsave[iw1], (integer*)&wsave[iw2]);
    return 0;
} /* zffti_ */

/* Subroutine */ int dcosqb1_(integer *n, doublereal *x, doublereal *w, 
	doublereal *xh)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k, kc, np2, ns2;
    static doublereal xim1;
    static integer modn;
    extern /* Subroutine */ int dfftb_(integer *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --xh;
    --w;
    --x;

    /* Function Body */
    ns2 = (*n + 1) / 2;
    np2 = *n + 2;
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	xim1 = x[i__ - 1] + x[i__];
	x[i__] -= x[i__ - 1];
	x[i__ - 1] = xim1;
/* L101: */
    }
    x[1] += x[1];
    modn = *n % 2;
    if (modn == 0) {
	x[*n] += x[*n];
    }
    dfftb_(n, &x[1], &xh[1]);
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	xh[k] = w[k - 1] * x[kc] + w[kc - 1] * x[k];
	xh[kc] = w[k - 1] * x[k] - w[kc - 1] * x[kc];
/* L102: */
    }
    if (modn == 0) {
	x[ns2 + 1] = w[ns2] * (x[ns2 + 1] + x[ns2 + 1]);
    }
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	x[k] = xh[k] + xh[kc];
	x[kc] = xh[k] - xh[kc];
/* L103: */
    }
    x[1] += x[1];
    return 0;
} /* dcosqb1_ */

/* Subroutine */ int dcosqf1_(integer *n, doublereal *x, doublereal *w, 
	doublereal *xh)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k, kc, np2, ns2;
    static doublereal xim1;
    static integer modn;
    extern /* Subroutine */ int dfftf_(integer *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --xh;
    --w;
    --x;

    /* Function Body */
    ns2 = (*n + 1) / 2;
    np2 = *n + 2;
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	xh[k] = x[k] + x[kc];
	xh[kc] = x[k] - x[kc];
/* L101: */
    }
    modn = *n % 2;
    if (modn == 0) {
	xh[ns2 + 1] = x[ns2 + 1] + x[ns2 + 1];
    }
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	x[k] = w[k - 1] * xh[kc] + w[kc - 1] * xh[k];
	x[kc] = w[k - 1] * xh[k] - w[kc - 1] * xh[kc];
/* L102: */
    }
    if (modn == 0) {
	x[ns2 + 1] = w[ns2] * xh[ns2 + 1];
    }
    dfftf_(n, &x[1], &xh[1]);
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	xim1 = x[i__ - 1] - x[i__];
	x[i__] = x[i__ - 1] + x[i__];
	x[i__ - 1] = xim1;
/* L103: */
    }
    return 0;
} /* dcosqf1_ */

/* Subroutine */ int dcosqi_(integer *n, doublereal *wsave)
{
    /* Initialized data */

    static doublereal pih = 1.5707963267948966192313216916397514;

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double cos(doublereal);

    /* Local variables */
    static integer k;
    static doublereal fk, dt;
    extern /* Subroutine */ int dffti_(integer *, doublereal *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    dt = pih / (doublereal) (*n);
    fk = 0.;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	fk += 1.;
	wsave[k] = cos(fk * dt);
/* L101: */
    }
    dffti_(n, &wsave[*n + 1]);
    return 0;
} /* dcosqi_ */

/* Subroutine */ int dcost_(integer *n, doublereal *x, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k;
    static doublereal c1, t1, t2;
    static integer kc;
    static doublereal xi;
    static integer nm1, np1;
    static doublereal x1h;
    static integer ns2;
    static doublereal tx2, x1p3, xim2;
    static integer modn;
    extern /* Subroutine */ int dfftf_(integer *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    nm1 = *n - 1;
    np1 = *n + 1;
    ns2 = *n / 2;
    if ((i__1 = *n - 2) < 0) {
	goto L106;
    } else if (i__1 == 0) {
	goto L101;
    } else {
	goto L102;
    }
L101:
    x1h = x[1] + x[2];
    x[2] = x[1] - x[2];
    x[1] = x1h;
    return 0;
L102:
    if (*n > 3) {
	goto L103;
    }
    x1p3 = x[1] + x[3];
    tx2 = x[2] + x[2];
    x[2] = x[1] - x[3];
    x[1] = x1p3 + tx2;
    x[3] = x1p3 - tx2;
    return 0;
L103:
    c1 = x[1] - x[*n];
    x[1] += x[*n];
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np1 - k;
	t1 = x[k] + x[kc];
	t2 = x[k] - x[kc];
	c1 += wsave[kc] * t2;
	t2 = wsave[k] * t2;
	x[k] = t1 - t2;
	x[kc] = t1 + t2;
/* L104: */
    }
    modn = *n % 2;
    if (modn != 0) {
	x[ns2 + 1] += x[ns2 + 1];
    }
    dfftf_(&nm1, &x[1], &wsave[*n + 1]);
    xim2 = x[2];
    x[2] = c1;
    i__1 = *n;
    for (i__ = 4; i__ <= i__1; i__ += 2) {
	xi = x[i__];
	x[i__] = x[i__ - 2] - x[i__ - 1];
	x[i__ - 1] = xim2;
	xim2 = xi;
/* L105: */
    }
    if (modn != 0) {
	x[*n] = xim2;
    }
L106:
    return 0;
} /* dcost_ */

/* Subroutine */ int dzfft1_(integer *n, doublereal *wa, integer *ifac)
{
    /* Initialized data */

    static integer ntryh[4] = { 4,2,3,5 };
    static doublereal tpi = 6.2831853071795864769252867665590057;

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, k1, l1, l2, ib, ii, nf, ip, nl, is, nq, nr;
    static doublereal ch1, sh1;
    static integer ido, ipm;
    static doublereal dch1, ch1h, arg1, dsh1;
    static integer nfm1;
    static doublereal argh;
    static integer ntry;

    /* Parameter adjustments */
    --ifac;
    --wa;

    /* Function Body */
    nl = *n;
    nf = 0;
    j = 0;
L101:
    ++j;
    if (j - 4 <= 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    ntry = ntryh[j - 1];
    goto L104;
L103:
    ntry += 2;
L104:
    nq = nl / ntry;
    nr = nl - ntry * nq;
    if (nr != 0) {
	goto L101;
    } else {
	goto L105;
    }
L105:
    ++nf;
    ifac[nf + 2] = ntry;
    nl = nq;
    if (ntry != 2) {
	goto L107;
    }
    if (nf == 1) {
	goto L107;
    }
    i__1 = nf;
    for (i__ = 2; i__ <= i__1; ++i__) {
	ib = nf - i__ + 2;
	ifac[ib + 2] = ifac[ib + 1];
/* L106: */
    }
    ifac[3] = 2;
L107:
    if (nl != 1) {
	goto L104;
    }
    ifac[1] = *n;
    ifac[2] = nf;
    argh = tpi / (doublereal) (*n);
    is = 0;
    nfm1 = nf - 1;
    l1 = 1;
    if (nfm1 == 0) {
	return 0;
    }
    i__1 = nfm1;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	l2 = l1 * ip;
	ido = *n / l2;
	ipm = ip - 1;
	arg1 = (doublereal) l1 * argh;
	ch1 = 1.;
	sh1 = 0.;
	dch1 = cos(arg1);
	dsh1 = sin(arg1);
	i__2 = ipm;
	for (j = 1; j <= i__2; ++j) {
	    ch1h = dch1 * ch1 - dsh1 * sh1;
	    sh1 = dch1 * sh1 + dsh1 * ch1;
	    ch1 = ch1h;
	    i__ = is + 2;
	    wa[i__ - 1] = ch1;
	    wa[i__] = sh1;
	    if (ido < 5) {
		goto L109;
	    }
	    i__3 = ido;
	    for (ii = 5; ii <= i__3; ii += 2) {
		i__ += 2;
		wa[i__ - 1] = ch1 * wa[i__ - 3] - sh1 * wa[i__ - 2];
		wa[i__] = ch1 * wa[i__ - 2] + sh1 * wa[i__ - 3];
/* L108: */
	    }
L109:
	    is += ido;
/* L110: */
	}
	l1 = l2;
/* L111: */
    }
    return 0;
} /* dzfft1_ */

/* Subroutine */ int dcosqb_(integer *n, doublereal *x, doublereal *wsave)
{
    /* Initialized data */

    static doublereal tsqrt2 = 2.8284271247461900976033774484193961;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal x1;
    extern /* Subroutine */ int dcosqb1_(integer *, doublereal *, doublereal *
	    , doublereal *);

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    if ((i__1 = *n - 2) < 0) {
	goto L101;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L101:
    x[1] *= 4.;
    return 0;
L102:
    x1 = (x[1] + x[2]) * 4.;
    x[2] = tsqrt2 * (x[1] - x[2]);
    x[1] = x1;
    return 0;
L103:
    dcosqb1_(n, &x[1], &wsave[1], &wsave[*n + 1]);
    return 0;
} /* dcosqb_ */

/* Subroutine */ int dcosqf_(integer *n, doublereal *x, doublereal *wsave)
{
    /* Initialized data */

    static doublereal sqrt2 = 1.414213562373095048801688724209698;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal tsqx;
    extern /* Subroutine */ int dcosqf1_(integer *, doublereal *, doublereal *
	    , doublereal *);

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    if ((i__1 = *n - 2) < 0) {
	goto L102;
    } else if (i__1 == 0) {
	goto L101;
    } else {
	goto L103;
    }
L101:
    tsqx = sqrt2 * x[2];
    x[2] = x[1] - tsqx;
    x[1] += tsqx;
L102:
    return 0;
L103:
    dcosqf1_(n, &x[1], &wsave[1], &wsave[*n + 1]);
    return 0;
} /* dcosqf_ */

/* Subroutine */ int dcosti_(integer *n, doublereal *wsave)
{
    /* Initialized data */

    static doublereal pi = 3.1415926535897932384626433832795028;

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double sin(doublereal), cos(doublereal);

    /* Local variables */
    static integer k, kc;
    static doublereal fk, dt;
    static integer nm1, np1, ns2;
    extern /* Subroutine */ int dffti_(integer *, doublereal *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n <= 3) {
	return 0;
    }
    nm1 = *n - 1;
    np1 = *n + 1;
    ns2 = *n / 2;
    dt = pi / (doublereal) nm1;
    fk = 0.;
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np1 - k;
	fk += 1.;
	wsave[k] = sin(fk * dt) * 2.;
	wsave[kc] = cos(fk * dt) * 2.;
/* L101: */
    }
    dffti_(&nm1, &wsave[*n + 1]);
    return 0;
} /* dcosti_ */

/* Subroutine */ int dzfftb_(integer *n, doublereal *r__, doublereal *azero, 
	doublereal *a, doublereal *b, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, ns2;
    extern /* Subroutine */ int dfftb_(integer *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --wsave;
    --b;
    --a;
    --r__;

    /* Function Body */
    if ((i__1 = *n - 2) < 0) {
	goto L101;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L101:
    r__[1] = *azero;
    return 0;
L102:
    r__[1] = *azero + a[1];
    r__[2] = *azero - a[1];
    return 0;
L103:
    ns2 = (*n - 1) / 2;
    i__1 = ns2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	r__[i__ * 2] = a[i__] * .5;
	r__[(i__ << 1) + 1] = b[i__] * -.5;
/* L104: */
    }
    r__[1] = *azero;
    if (*n % 2 == 0) {
	r__[*n] = a[ns2 + 1];
    }
    dfftb_(n, &r__[1], &wsave[*n + 1]);
    return 0;
} /* dzfftb_ */

/* Subroutine */ int dzfftf_(integer *n, doublereal *r__, doublereal *azero, 
	doublereal *a, doublereal *b, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    static doublereal cf;
    static integer ns2;
    static doublereal cfm;
    static integer ns2m;
    extern /* Subroutine */ int dfftf_(integer *, doublereal *, doublereal *);


/*                       VERSION 3  JUNE 1979 */

    /* Parameter adjustments */
    --wsave;
    --b;
    --a;
    --r__;

    /* Function Body */
    if ((i__1 = *n - 2) < 0) {
	goto L101;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L101:
    *azero = r__[1];
    return 0;
L102:
    *azero = (r__[1] + r__[2]) * .5;
    a[1] = (r__[1] - r__[2]) * .5;
    return 0;
L103:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wsave[i__] = r__[i__];
/* L104: */
    }
    dfftf_(n, &wsave[1], &wsave[*n + 1]);
    cf = 2. / (doublereal) (*n);
    cfm = -cf;
    *azero = cf * .5 * wsave[1];
    ns2 = (*n + 1) / 2;
    ns2m = ns2 - 1;
    i__1 = ns2m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a[i__] = cf * wsave[i__ * 2];
	b[i__] = cfm * wsave[(i__ << 1) + 1];
/* L105: */
    }
    if (*n % 2 == 1) {
	return 0;
    }
    a[ns2] = cf * .5 * wsave[*n];
    b[ns2] = 0.;
    return 0;
} /* dzfftf_ */

/* Subroutine */ int dzffti_(integer *n, doublereal *wsave)
{
    extern /* Subroutine */ int dzfft1_(integer *, doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    dzfft1_(n, &wsave[(*n << 1) + 1], (integer*)&wsave[*n * 3 + 1]);
    return 0;
} /* dzffti_ */

/* Subroutine */ int dpassb_(integer *nac, integer *ido, integer *ip, integer 
	*l1, integer *idl1, doublereal *cc, doublereal *c1, doublereal *c2, 
	doublereal *ch, doublereal *ch2, doublereal *wa)
{
    /* System generated locals */
    integer ch_dim1, ch_dim2, ch_offset, cc_dim1, cc_dim2, cc_offset, c1_dim1,
	     c1_dim2, c1_offset, c2_dim1, c2_offset, ch2_dim1, ch2_offset, 
	    i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, l, jc, lc, ik, nt, idj, idl, inc, idp;
    static doublereal wai, war;
    static integer ipp2, idij, idlj, idot, ipph;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    c1_dim1 = *ido;
    c1_dim2 = *l1;
    c1_offset = 1 + c1_dim1 * (1 + c1_dim2);
    c1 -= c1_offset;
    cc_dim1 = *ido;
    cc_dim2 = *ip;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    ch2_dim1 = *idl1;
    ch2_offset = 1 + ch2_dim1;
    ch2 -= ch2_offset;
    c2_dim1 = *idl1;
    c2_offset = 1 + c2_dim1;
    c2 -= c2_offset;
    --wa;

    /* Function Body */
    idot = *ido / 2;
    nt = *ip * *idl1;
    ipp2 = *ip + 2;
    ipph = (*ip + 1) / 2;
    idp = *ip * *ido;

    if (*ido < *l1) {
	goto L106;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] + cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] - cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
/* L101: */
	    }
/* L102: */
	}
/* L103: */
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * cc_dim2 + 1) * 
		    cc_dim1];
/* L104: */
	}
/* L105: */
    }
    goto L112;
L106:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] + cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] - cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
/* L107: */
	    }
/* L108: */
	}
/* L109: */
    }
    i__1 = *ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * cc_dim2 + 1) * 
		    cc_dim1];
/* L110: */
	}
/* L111: */
    }
L112:
    idl = 2 - *ido;
    inc = 0;
    i__1 = ipph;
    for (l = 2; l <= i__1; ++l) {
	lc = ipp2 - l;
	idl += *ido;
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    c2[ik + l * c2_dim1] = ch2[ik + ch2_dim1] + wa[idl - 1] * ch2[ik 
		    + (ch2_dim1 << 1)];
	    c2[ik + lc * c2_dim1] = wa[idl] * ch2[ik + *ip * ch2_dim1];
/* L113: */
	}
	idlj = idl;
	inc += *ido;
	i__2 = ipph;
	for (j = 3; j <= i__2; ++j) {
	    jc = ipp2 - j;
	    idlj += inc;
	    if (idlj > idp) {
		idlj -= idp;
	    }
	    war = wa[idlj - 1];
	    wai = wa[idlj];
	    i__3 = *idl1;
	    for (ik = 1; ik <= i__3; ++ik) {
		c2[ik + l * c2_dim1] += war * ch2[ik + j * ch2_dim1];
		c2[ik + lc * c2_dim1] += wai * ch2[ik + jc * ch2_dim1];
/* L114: */
	    }
/* L115: */
	}
/* L116: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    ch2[ik + ch2_dim1] += ch2[ik + j * ch2_dim1];
/* L117: */
	}
/* L118: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *idl1;
	for (ik = 2; ik <= i__2; ik += 2) {
	    ch2[ik - 1 + j * ch2_dim1] = c2[ik - 1 + j * c2_dim1] - c2[ik + 
		    jc * c2_dim1];
	    ch2[ik - 1 + jc * ch2_dim1] = c2[ik - 1 + j * c2_dim1] + c2[ik + 
		    jc * c2_dim1];
	    ch2[ik + j * ch2_dim1] = c2[ik + j * c2_dim1] + c2[ik - 1 + jc * 
		    c2_dim1];
	    ch2[ik + jc * ch2_dim1] = c2[ik + j * c2_dim1] - c2[ik - 1 + jc * 
		    c2_dim1];
/* L119: */
	}
/* L120: */
    }
    *nac = 1;
    if (*ido == 2) {
	return 0;
    }
    *nac = 0;
    i__1 = *idl1;
    for (ik = 1; ik <= i__1; ++ik) {
	c2[ik + c2_dim1] = ch2[ik + ch2_dim1];
/* L121: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    c1[(k + j * c1_dim2) * c1_dim1 + 1] = ch[(k + j * ch_dim2) * 
		    ch_dim1 + 1];
	    c1[(k + j * c1_dim2) * c1_dim1 + 2] = ch[(k + j * ch_dim2) * 
		    ch_dim1 + 2];
/* L122: */
	}
/* L123: */
    }
    if (idot > *l1) {
	goto L127;
    }
    idij = 0;
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	idij += 2;
	i__2 = *ido;
	for (i__ = 4; i__ <= i__2; i__ += 2) {
	    idij += 2;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[
			i__ - 1 + (k + j * ch_dim2) * ch_dim1] - wa[idij] * 
			ch[i__ + (k + j * ch_dim2) * ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[i__ 
			+ (k + j * ch_dim2) * ch_dim1] + wa[idij] * ch[i__ - 
			1 + (k + j * ch_dim2) * ch_dim1];
/* L124: */
	    }
/* L125: */
	}
/* L126: */
    }
    return 0;
L127:
    idj = 2 - *ido;
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	idj += *ido;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    idij = idj;
	    i__3 = *ido;
	    for (i__ = 4; i__ <= i__3; i__ += 2) {
		idij += 2;
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[
			i__ - 1 + (k + j * ch_dim2) * ch_dim1] - wa[idij] * 
			ch[i__ + (k + j * ch_dim2) * ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[i__ 
			+ (k + j * ch_dim2) * ch_dim1] + wa[idij] * ch[i__ - 
			1 + (k + j * ch_dim2) * ch_dim1];
/* L128: */
	    }
/* L129: */
	}
/* L130: */
    }
    return 0;
} /* dpassb_ */

/* Subroutine */ int dpassb2_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1)
{
    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ti2, tr2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 3;
    cc -= cc_offset;
    --wa1;

    /* Function Body */
    if (*ido > 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[((k << 1) + 1) * cc_dim1 + 1] + 
		cc[((k << 1) + 2) * cc_dim1 + 1];
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cc[((k << 1) + 1) * cc_dim1 
		+ 1] - cc[((k << 1) + 2) * cc_dim1 + 1];
	ch[(k + ch_dim2) * ch_dim1 + 2] = cc[((k << 1) + 1) * cc_dim1 + 2] + 
		cc[((k << 1) + 2) * cc_dim1 + 2];
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = cc[((k << 1) + 1) * cc_dim1 
		+ 2] - cc[((k << 1) + 2) * cc_dim1 + 2];
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + ((k << 1) + 
		    1) * cc_dim1] + cc[i__ - 1 + ((k << 1) + 2) * cc_dim1];
	    tr2 = cc[i__ - 1 + ((k << 1) + 1) * cc_dim1] - cc[i__ - 1 + ((k <<
		     1) + 2) * cc_dim1];
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + ((k << 1) + 1) * 
		    cc_dim1] + cc[i__ + ((k << 1) + 2) * cc_dim1];
	    ti2 = cc[i__ + ((k << 1) + 1) * cc_dim1] - cc[i__ + ((k << 1) + 2)
		     * cc_dim1];
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * ti2 + 
		    wa1[i__] * tr2;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * tr2 
		    - wa1[i__] * ti2;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassb2_ */

/* Subroutine */ int dpassb3_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2)
{
    /* Initialized data */

    static doublereal taur = -.5;
    static doublereal taui = .86602540378443864676372317075293618;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + (cc_dim1 << 2);
    cc -= cc_offset;
    --wa1;
    --wa2;

    /* Function Body */
    if (*ido != 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr2 = cc[(k * 3 + 2) * cc_dim1 + 1] + cc[(k * 3 + 3) * cc_dim1 + 1];
	cr2 = cc[(k * 3 + 1) * cc_dim1 + 1] + taur * tr2;
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[(k * 3 + 1) * cc_dim1 + 1] + tr2;
	ti2 = cc[(k * 3 + 2) * cc_dim1 + 2] + cc[(k * 3 + 3) * cc_dim1 + 2];
	ci2 = cc[(k * 3 + 1) * cc_dim1 + 2] + taur * ti2;
	ch[(k + ch_dim2) * ch_dim1 + 2] = cc[(k * 3 + 1) * cc_dim1 + 2] + ti2;
	cr3 = taui * (cc[(k * 3 + 2) * cc_dim1 + 1] - cc[(k * 3 + 3) * 
		cc_dim1 + 1]);
	ci3 = taui * (cc[(k * 3 + 2) * cc_dim1 + 2] - cc[(k * 3 + 3) * 
		cc_dim1 + 2]);
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cr2 - ci3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cr2 + ci3;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = ci2 + cr3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = ci2 - cr3;
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    tr2 = cc[i__ - 1 + (k * 3 + 2) * cc_dim1] + cc[i__ - 1 + (k * 3 + 
		    3) * cc_dim1];
	    cr2 = cc[i__ - 1 + (k * 3 + 1) * cc_dim1] + taur * tr2;
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + (k * 3 + 1) *
		     cc_dim1] + tr2;
	    ti2 = cc[i__ + (k * 3 + 2) * cc_dim1] + cc[i__ + (k * 3 + 3) * 
		    cc_dim1];
	    ci2 = cc[i__ + (k * 3 + 1) * cc_dim1] + taur * ti2;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * 3 + 1) * 
		    cc_dim1] + ti2;
	    cr3 = taui * (cc[i__ - 1 + (k * 3 + 2) * cc_dim1] - cc[i__ - 1 + (
		    k * 3 + 3) * cc_dim1]);
	    ci3 = taui * (cc[i__ + (k * 3 + 2) * cc_dim1] - cc[i__ + (k * 3 + 
		    3) * cc_dim1]);
	    dr2 = cr2 - ci3;
	    dr3 = cr2 + ci3;
	    di2 = ci2 + cr3;
	    di3 = ci2 - cr3;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * di2 + 
		    wa1[i__] * dr2;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * dr2 
		    - wa1[i__] * di2;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * di3 + wa2[
		    i__] * dr3;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * dr3 - 
		    wa2[i__] * di3;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassb3_ */

/* Subroutine */ int dpassb4_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3)
{
    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, 
	    tr2, tr3, tr4;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 5;
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;

    /* Function Body */
    if (*ido != 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti1 = cc[((k << 2) + 1) * cc_dim1 + 2] - cc[((k << 2) + 3) * cc_dim1 
		+ 2];
	ti2 = cc[((k << 2) + 1) * cc_dim1 + 2] + cc[((k << 2) + 3) * cc_dim1 
		+ 2];
	tr4 = cc[((k << 2) + 4) * cc_dim1 + 2] - cc[((k << 2) + 2) * cc_dim1 
		+ 2];
	ti3 = cc[((k << 2) + 2) * cc_dim1 + 2] + cc[((k << 2) + 4) * cc_dim1 
		+ 2];
	tr1 = cc[((k << 2) + 1) * cc_dim1 + 1] - cc[((k << 2) + 3) * cc_dim1 
		+ 1];
	tr2 = cc[((k << 2) + 1) * cc_dim1 + 1] + cc[((k << 2) + 3) * cc_dim1 
		+ 1];
	ti4 = cc[((k << 2) + 2) * cc_dim1 + 1] - cc[((k << 2) + 4) * cc_dim1 
		+ 1];
	tr3 = cc[((k << 2) + 2) * cc_dim1 + 1] + cc[((k << 2) + 4) * cc_dim1 
		+ 1];
	ch[(k + ch_dim2) * ch_dim1 + 1] = tr2 + tr3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = tr2 - tr3;
	ch[(k + ch_dim2) * ch_dim1 + 2] = ti2 + ti3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = ti2 - ti3;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = tr1 + tr4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = tr1 - tr4;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = ti1 + ti4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = ti1 - ti4;
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    ti1 = cc[i__ + ((k << 2) + 1) * cc_dim1] - cc[i__ + ((k << 2) + 3)
		     * cc_dim1];
	    ti2 = cc[i__ + ((k << 2) + 1) * cc_dim1] + cc[i__ + ((k << 2) + 3)
		     * cc_dim1];
	    ti3 = cc[i__ + ((k << 2) + 2) * cc_dim1] + cc[i__ + ((k << 2) + 4)
		     * cc_dim1];
	    tr4 = cc[i__ + ((k << 2) + 4) * cc_dim1] - cc[i__ + ((k << 2) + 2)
		     * cc_dim1];
	    tr1 = cc[i__ - 1 + ((k << 2) + 1) * cc_dim1] - cc[i__ - 1 + ((k <<
		     2) + 3) * cc_dim1];
	    tr2 = cc[i__ - 1 + ((k << 2) + 1) * cc_dim1] + cc[i__ - 1 + ((k <<
		     2) + 3) * cc_dim1];
	    ti4 = cc[i__ - 1 + ((k << 2) + 2) * cc_dim1] - cc[i__ - 1 + ((k <<
		     2) + 4) * cc_dim1];
	    tr3 = cc[i__ - 1 + ((k << 2) + 2) * cc_dim1] + cc[i__ - 1 + ((k <<
		     2) + 4) * cc_dim1];
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = tr2 + tr3;
	    cr3 = tr2 - tr3;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = ti2 + ti3;
	    ci3 = ti2 - ti3;
	    cr2 = tr1 + tr4;
	    cr4 = tr1 - tr4;
	    ci2 = ti1 + ti4;
	    ci4 = ti1 - ti4;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * cr2 
		    - wa1[i__] * ci2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * ci2 + 
		    wa1[i__] * cr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * cr3 - 
		    wa2[i__] * ci3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * ci3 + wa2[
		    i__] * cr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * cr4 
		    - wa3[i__] * ci4;
	    ch[i__ + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * ci4 + 
		    wa3[i__] * cr4;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassb4_ */

/* Subroutine */ int dpassb5_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, 
	doublereal *wa4)
{
    /* Initialized data */

    static doublereal tr11 = .30901699437494742410229341718281905;
    static doublereal ti11 = .95105651629515357211643933337938214;
    static doublereal tr12 = -.80901699437494742410229341718281906;
    static doublereal ti12 = .58778525229247312916870595463907276;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, 
	    cr4, ti2, ti3, ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 6;
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;
    --wa4;

    /* Function Body */
    if (*ido != 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti5 = cc[(k * 5 + 2) * cc_dim1 + 2] - cc[(k * 5 + 5) * cc_dim1 + 2];
	ti2 = cc[(k * 5 + 2) * cc_dim1 + 2] + cc[(k * 5 + 5) * cc_dim1 + 2];
	ti4 = cc[(k * 5 + 3) * cc_dim1 + 2] - cc[(k * 5 + 4) * cc_dim1 + 2];
	ti3 = cc[(k * 5 + 3) * cc_dim1 + 2] + cc[(k * 5 + 4) * cc_dim1 + 2];
	tr5 = cc[(k * 5 + 2) * cc_dim1 + 1] - cc[(k * 5 + 5) * cc_dim1 + 1];
	tr2 = cc[(k * 5 + 2) * cc_dim1 + 1] + cc[(k * 5 + 5) * cc_dim1 + 1];
	tr4 = cc[(k * 5 + 3) * cc_dim1 + 1] - cc[(k * 5 + 4) * cc_dim1 + 1];
	tr3 = cc[(k * 5 + 3) * cc_dim1 + 1] + cc[(k * 5 + 4) * cc_dim1 + 1];
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[(k * 5 + 1) * cc_dim1 + 1] + tr2 
		+ tr3;
	ch[(k + ch_dim2) * ch_dim1 + 2] = cc[(k * 5 + 1) * cc_dim1 + 2] + ti2 
		+ ti3;
	cr2 = cc[(k * 5 + 1) * cc_dim1 + 1] + tr11 * tr2 + tr12 * tr3;
	ci2 = cc[(k * 5 + 1) * cc_dim1 + 2] + tr11 * ti2 + tr12 * ti3;
	cr3 = cc[(k * 5 + 1) * cc_dim1 + 1] + tr12 * tr2 + tr11 * tr3;
	ci3 = cc[(k * 5 + 1) * cc_dim1 + 2] + tr12 * ti2 + tr11 * ti3;
	cr5 = ti11 * tr5 + ti12 * tr4;
	ci5 = ti11 * ti5 + ti12 * ti4;
	cr4 = ti12 * tr5 - ti11 * tr4;
	ci4 = ti12 * ti5 - ti11 * ti4;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cr2 - ci5;
	ch[(k + ch_dim2 * 5) * ch_dim1 + 1] = cr2 + ci5;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = ci2 + cr5;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = ci3 + cr4;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cr3 - ci4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = cr3 + ci4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = ci3 - cr4;
	ch[(k + ch_dim2 * 5) * ch_dim1 + 2] = ci2 - cr5;
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    ti5 = cc[i__ + (k * 5 + 2) * cc_dim1] - cc[i__ + (k * 5 + 5) * 
		    cc_dim1];
	    ti2 = cc[i__ + (k * 5 + 2) * cc_dim1] + cc[i__ + (k * 5 + 5) * 
		    cc_dim1];
	    ti4 = cc[i__ + (k * 5 + 3) * cc_dim1] - cc[i__ + (k * 5 + 4) * 
		    cc_dim1];
	    ti3 = cc[i__ + (k * 5 + 3) * cc_dim1] + cc[i__ + (k * 5 + 4) * 
		    cc_dim1];
	    tr5 = cc[i__ - 1 + (k * 5 + 2) * cc_dim1] - cc[i__ - 1 + (k * 5 + 
		    5) * cc_dim1];
	    tr2 = cc[i__ - 1 + (k * 5 + 2) * cc_dim1] + cc[i__ - 1 + (k * 5 + 
		    5) * cc_dim1];
	    tr4 = cc[i__ - 1 + (k * 5 + 3) * cc_dim1] - cc[i__ - 1 + (k * 5 + 
		    4) * cc_dim1];
	    tr3 = cc[i__ - 1 + (k * 5 + 3) * cc_dim1] + cc[i__ - 1 + (k * 5 + 
		    4) * cc_dim1];
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + (k * 5 + 1) *
		     cc_dim1] + tr2 + tr3;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * 5 + 1) * 
		    cc_dim1] + ti2 + ti3;
	    cr2 = cc[i__ - 1 + (k * 5 + 1) * cc_dim1] + tr11 * tr2 + tr12 * 
		    tr3;
	    ci2 = cc[i__ + (k * 5 + 1) * cc_dim1] + tr11 * ti2 + tr12 * ti3;
	    cr3 = cc[i__ - 1 + (k * 5 + 1) * cc_dim1] + tr12 * tr2 + tr11 * 
		    tr3;
	    ci3 = cc[i__ + (k * 5 + 1) * cc_dim1] + tr12 * ti2 + tr11 * ti3;
	    cr5 = ti11 * tr5 + ti12 * tr4;
	    ci5 = ti11 * ti5 + ti12 * ti4;
	    cr4 = ti12 * tr5 - ti11 * tr4;
	    ci4 = ti12 * ti5 - ti11 * ti4;
	    dr3 = cr3 - ci4;
	    dr4 = cr3 + ci4;
	    di3 = ci3 + cr4;
	    di4 = ci3 - cr4;
	    dr5 = cr2 + ci5;
	    dr2 = cr2 - ci5;
	    di5 = ci2 - cr5;
	    di2 = ci2 + cr5;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * dr2 
		    - wa1[i__] * di2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * di2 + 
		    wa1[i__] * dr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * dr3 - 
		    wa2[i__] * di3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * di3 + wa2[
		    i__] * dr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * dr4 
		    - wa3[i__] * di4;
	    ch[i__ + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * di4 + 
		    wa3[i__] * dr4;
	    ch[i__ - 1 + (k + ch_dim2 * 5) * ch_dim1] = wa4[i__ - 1] * dr5 - 
		    wa4[i__] * di5;
	    ch[i__ + (k + ch_dim2 * 5) * ch_dim1] = wa4[i__ - 1] * di5 + wa4[
		    i__] * dr5;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassb5_ */

/* Subroutine */ int dpassf_(integer *nac, integer *ido, integer *ip, integer 
	*l1, integer *idl1, doublereal *cc, doublereal *c1, doublereal *c2, 
	doublereal *ch, doublereal *ch2, doublereal *wa)
{
    /* System generated locals */
    integer ch_dim1, ch_dim2, ch_offset, cc_dim1, cc_dim2, cc_offset, c1_dim1,
	     c1_dim2, c1_offset, c2_dim1, c2_offset, ch2_dim1, ch2_offset, 
	    i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, l, jc, lc, ik, nt, idj, idl, inc, idp;
    static doublereal wai, war;
    static integer ipp2, idij, idlj, idot, ipph;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    c1_dim1 = *ido;
    c1_dim2 = *l1;
    c1_offset = 1 + c1_dim1 * (1 + c1_dim2);
    c1 -= c1_offset;
    cc_dim1 = *ido;
    cc_dim2 = *ip;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    ch2_dim1 = *idl1;
    ch2_offset = 1 + ch2_dim1;
    ch2 -= ch2_offset;
    c2_dim1 = *idl1;
    c2_offset = 1 + c2_dim1;
    c2 -= c2_offset;
    --wa;

    /* Function Body */
    idot = *ido / 2;
    nt = *ip * *idl1;
    ipp2 = *ip + 2;
    ipph = (*ip + 1) / 2;
    idp = *ip * *ido;

    if (*ido < *l1) {
	goto L106;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] + cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] - cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
/* L101: */
	    }
/* L102: */
	}
/* L103: */
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * cc_dim2 + 1) * 
		    cc_dim1];
/* L104: */
	}
/* L105: */
    }
    goto L112;
L106:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] + cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = cc[i__ + (j + k * 
			cc_dim2) * cc_dim1] - cc[i__ + (jc + k * cc_dim2) * 
			cc_dim1];
/* L107: */
	    }
/* L108: */
	}
/* L109: */
    }
    i__1 = *ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * cc_dim2 + 1) * 
		    cc_dim1];
/* L110: */
	}
/* L111: */
    }
L112:
    idl = 2 - *ido;
    inc = 0;
    i__1 = ipph;
    for (l = 2; l <= i__1; ++l) {
	lc = ipp2 - l;
	idl += *ido;
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    c2[ik + l * c2_dim1] = ch2[ik + ch2_dim1] + wa[idl - 1] * ch2[ik 
		    + (ch2_dim1 << 1)];
	    c2[ik + lc * c2_dim1] = -wa[idl] * ch2[ik + *ip * ch2_dim1];
/* L113: */
	}
	idlj = idl;
	inc += *ido;
	i__2 = ipph;
	for (j = 3; j <= i__2; ++j) {
	    jc = ipp2 - j;
	    idlj += inc;
	    if (idlj > idp) {
		idlj -= idp;
	    }
	    war = wa[idlj - 1];
	    wai = wa[idlj];
	    i__3 = *idl1;
	    for (ik = 1; ik <= i__3; ++ik) {
		c2[ik + l * c2_dim1] += war * ch2[ik + j * ch2_dim1];
		c2[ik + lc * c2_dim1] -= wai * ch2[ik + jc * ch2_dim1];
/* L114: */
	    }
/* L115: */
	}
/* L116: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    ch2[ik + ch2_dim1] += ch2[ik + j * ch2_dim1];
/* L117: */
	}
/* L118: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *idl1;
	for (ik = 2; ik <= i__2; ik += 2) {
	    ch2[ik - 1 + j * ch2_dim1] = c2[ik - 1 + j * c2_dim1] - c2[ik + 
		    jc * c2_dim1];
	    ch2[ik - 1 + jc * ch2_dim1] = c2[ik - 1 + j * c2_dim1] + c2[ik + 
		    jc * c2_dim1];
	    ch2[ik + j * ch2_dim1] = c2[ik + j * c2_dim1] + c2[ik - 1 + jc * 
		    c2_dim1];
	    ch2[ik + jc * ch2_dim1] = c2[ik + j * c2_dim1] - c2[ik - 1 + jc * 
		    c2_dim1];
/* L119: */
	}
/* L120: */
    }
    *nac = 1;
    if (*ido == 2) {
	return 0;
    }
    *nac = 0;
    i__1 = *idl1;
    for (ik = 1; ik <= i__1; ++ik) {
	c2[ik + c2_dim1] = ch2[ik + ch2_dim1];
/* L121: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    c1[(k + j * c1_dim2) * c1_dim1 + 1] = ch[(k + j * ch_dim2) * 
		    ch_dim1 + 1];
	    c1[(k + j * c1_dim2) * c1_dim1 + 2] = ch[(k + j * ch_dim2) * 
		    ch_dim1 + 2];
/* L122: */
	}
/* L123: */
    }
    if (idot > *l1) {
	goto L127;
    }
    idij = 0;
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	idij += 2;
	i__2 = *ido;
	for (i__ = 4; i__ <= i__2; i__ += 2) {
	    idij += 2;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[
			i__ - 1 + (k + j * ch_dim2) * ch_dim1] + wa[idij] * 
			ch[i__ + (k + j * ch_dim2) * ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[i__ 
			+ (k + j * ch_dim2) * ch_dim1] - wa[idij] * ch[i__ - 
			1 + (k + j * ch_dim2) * ch_dim1];
/* L124: */
	    }
/* L125: */
	}
/* L126: */
    }
    return 0;
L127:
    idj = 2 - *ido;
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	idj += *ido;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    idij = idj;
	    i__3 = *ido;
	    for (i__ = 4; i__ <= i__3; i__ += 2) {
		idij += 2;
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[
			i__ - 1 + (k + j * ch_dim2) * ch_dim1] + wa[idij] * 
			ch[i__ + (k + j * ch_dim2) * ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[i__ 
			+ (k + j * ch_dim2) * ch_dim1] - wa[idij] * ch[i__ - 
			1 + (k + j * ch_dim2) * ch_dim1];
/* L128: */
	    }
/* L129: */
	}
/* L130: */
    }
    return 0;
} /* dpassf_ */

/* Subroutine */ int dpassf2_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1)
{
    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ti2, tr2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 3;
    cc -= cc_offset;
    --wa1;

    /* Function Body */
    if (*ido > 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[((k << 1) + 1) * cc_dim1 + 1] + 
		cc[((k << 1) + 2) * cc_dim1 + 1];
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cc[((k << 1) + 1) * cc_dim1 
		+ 1] - cc[((k << 1) + 2) * cc_dim1 + 1];
	ch[(k + ch_dim2) * ch_dim1 + 2] = cc[((k << 1) + 1) * cc_dim1 + 2] + 
		cc[((k << 1) + 2) * cc_dim1 + 2];
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = cc[((k << 1) + 1) * cc_dim1 
		+ 2] - cc[((k << 1) + 2) * cc_dim1 + 2];
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + ((k << 1) + 
		    1) * cc_dim1] + cc[i__ - 1 + ((k << 1) + 2) * cc_dim1];
	    tr2 = cc[i__ - 1 + ((k << 1) + 1) * cc_dim1] - cc[i__ - 1 + ((k <<
		     1) + 2) * cc_dim1];
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + ((k << 1) + 1) * 
		    cc_dim1] + cc[i__ + ((k << 1) + 2) * cc_dim1];
	    ti2 = cc[i__ + ((k << 1) + 1) * cc_dim1] - cc[i__ + ((k << 1) + 2)
		     * cc_dim1];
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * ti2 - 
		    wa1[i__] * tr2;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * tr2 
		    + wa1[i__] * ti2;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassf2_ */

/* Subroutine */ int dpassf3_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2)
{
    /* Initialized data */

    static doublereal taur = -.5;
    static doublereal taui = -.86602540378443864676372317075293618;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + (cc_dim1 << 2);
    cc -= cc_offset;
    --wa1;
    --wa2;

    /* Function Body */
    if (*ido != 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr2 = cc[(k * 3 + 2) * cc_dim1 + 1] + cc[(k * 3 + 3) * cc_dim1 + 1];
	cr2 = cc[(k * 3 + 1) * cc_dim1 + 1] + taur * tr2;
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[(k * 3 + 1) * cc_dim1 + 1] + tr2;
	ti2 = cc[(k * 3 + 2) * cc_dim1 + 2] + cc[(k * 3 + 3) * cc_dim1 + 2];
	ci2 = cc[(k * 3 + 1) * cc_dim1 + 2] + taur * ti2;
	ch[(k + ch_dim2) * ch_dim1 + 2] = cc[(k * 3 + 1) * cc_dim1 + 2] + ti2;
	cr3 = taui * (cc[(k * 3 + 2) * cc_dim1 + 1] - cc[(k * 3 + 3) * 
		cc_dim1 + 1]);
	ci3 = taui * (cc[(k * 3 + 2) * cc_dim1 + 2] - cc[(k * 3 + 3) * 
		cc_dim1 + 2]);
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cr2 - ci3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cr2 + ci3;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = ci2 + cr3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = ci2 - cr3;
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    tr2 = cc[i__ - 1 + (k * 3 + 2) * cc_dim1] + cc[i__ - 1 + (k * 3 + 
		    3) * cc_dim1];
	    cr2 = cc[i__ - 1 + (k * 3 + 1) * cc_dim1] + taur * tr2;
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + (k * 3 + 1) *
		     cc_dim1] + tr2;
	    ti2 = cc[i__ + (k * 3 + 2) * cc_dim1] + cc[i__ + (k * 3 + 3) * 
		    cc_dim1];
	    ci2 = cc[i__ + (k * 3 + 1) * cc_dim1] + taur * ti2;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * 3 + 1) * 
		    cc_dim1] + ti2;
	    cr3 = taui * (cc[i__ - 1 + (k * 3 + 2) * cc_dim1] - cc[i__ - 1 + (
		    k * 3 + 3) * cc_dim1]);
	    ci3 = taui * (cc[i__ + (k * 3 + 2) * cc_dim1] - cc[i__ + (k * 3 + 
		    3) * cc_dim1]);
	    dr2 = cr2 - ci3;
	    dr3 = cr2 + ci3;
	    di2 = ci2 + cr3;
	    di3 = ci2 - cr3;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * di2 - 
		    wa1[i__] * dr2;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * dr2 
		    + wa1[i__] * di2;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * di3 - wa2[
		    i__] * dr3;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * dr3 + 
		    wa2[i__] * di3;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassf3_ */

/* Subroutine */ int dpassf4_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3)
{
    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, 
	    tr2, tr3, tr4;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 5;
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;

    /* Function Body */
    if (*ido != 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti1 = cc[((k << 2) + 1) * cc_dim1 + 2] - cc[((k << 2) + 3) * cc_dim1 
		+ 2];
	ti2 = cc[((k << 2) + 1) * cc_dim1 + 2] + cc[((k << 2) + 3) * cc_dim1 
		+ 2];
	tr4 = cc[((k << 2) + 2) * cc_dim1 + 2] - cc[((k << 2) + 4) * cc_dim1 
		+ 2];
	ti3 = cc[((k << 2) + 2) * cc_dim1 + 2] + cc[((k << 2) + 4) * cc_dim1 
		+ 2];
	tr1 = cc[((k << 2) + 1) * cc_dim1 + 1] - cc[((k << 2) + 3) * cc_dim1 
		+ 1];
	tr2 = cc[((k << 2) + 1) * cc_dim1 + 1] + cc[((k << 2) + 3) * cc_dim1 
		+ 1];
	ti4 = cc[((k << 2) + 4) * cc_dim1 + 1] - cc[((k << 2) + 2) * cc_dim1 
		+ 1];
	tr3 = cc[((k << 2) + 2) * cc_dim1 + 1] + cc[((k << 2) + 4) * cc_dim1 
		+ 1];
	ch[(k + ch_dim2) * ch_dim1 + 1] = tr2 + tr3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = tr2 - tr3;
	ch[(k + ch_dim2) * ch_dim1 + 2] = ti2 + ti3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = ti2 - ti3;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = tr1 + tr4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = tr1 - tr4;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = ti1 + ti4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = ti1 - ti4;
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    ti1 = cc[i__ + ((k << 2) + 1) * cc_dim1] - cc[i__ + ((k << 2) + 3)
		     * cc_dim1];
	    ti2 = cc[i__ + ((k << 2) + 1) * cc_dim1] + cc[i__ + ((k << 2) + 3)
		     * cc_dim1];
	    ti3 = cc[i__ + ((k << 2) + 2) * cc_dim1] + cc[i__ + ((k << 2) + 4)
		     * cc_dim1];
	    tr4 = cc[i__ + ((k << 2) + 2) * cc_dim1] - cc[i__ + ((k << 2) + 4)
		     * cc_dim1];
	    tr1 = cc[i__ - 1 + ((k << 2) + 1) * cc_dim1] - cc[i__ - 1 + ((k <<
		     2) + 3) * cc_dim1];
	    tr2 = cc[i__ - 1 + ((k << 2) + 1) * cc_dim1] + cc[i__ - 1 + ((k <<
		     2) + 3) * cc_dim1];
	    ti4 = cc[i__ - 1 + ((k << 2) + 4) * cc_dim1] - cc[i__ - 1 + ((k <<
		     2) + 2) * cc_dim1];
	    tr3 = cc[i__ - 1 + ((k << 2) + 2) * cc_dim1] + cc[i__ - 1 + ((k <<
		     2) + 4) * cc_dim1];
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = tr2 + tr3;
	    cr3 = tr2 - tr3;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = ti2 + ti3;
	    ci3 = ti2 - ti3;
	    cr2 = tr1 + tr4;
	    cr4 = tr1 - tr4;
	    ci2 = ti1 + ti4;
	    ci4 = ti1 - ti4;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * cr2 
		    + wa1[i__] * ci2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * ci2 - 
		    wa1[i__] * cr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * cr3 + 
		    wa2[i__] * ci3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * ci3 - wa2[
		    i__] * cr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * cr4 
		    + wa3[i__] * ci4;
	    ch[i__ + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * ci4 - 
		    wa3[i__] * cr4;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassf4_ */

/* Subroutine */ int dpassf5_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, 
	doublereal *wa4)
{
    /* Initialized data */

    static doublereal tr11 = .30901699437494742410229341718281905;
    static doublereal ti11 = -.95105651629515357211643933337938214;
    static doublereal tr12 = -.80901699437494742410229341718281906;
    static doublereal ti12 = -.58778525229247312916870595463907276;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static doublereal ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, 
	    cr4, ti2, ti3, ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 6;
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;
    --wa4;

    /* Function Body */
    if (*ido != 2) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti5 = cc[(k * 5 + 2) * cc_dim1 + 2] - cc[(k * 5 + 5) * cc_dim1 + 2];
	ti2 = cc[(k * 5 + 2) * cc_dim1 + 2] + cc[(k * 5 + 5) * cc_dim1 + 2];
	ti4 = cc[(k * 5 + 3) * cc_dim1 + 2] - cc[(k * 5 + 4) * cc_dim1 + 2];
	ti3 = cc[(k * 5 + 3) * cc_dim1 + 2] + cc[(k * 5 + 4) * cc_dim1 + 2];
	tr5 = cc[(k * 5 + 2) * cc_dim1 + 1] - cc[(k * 5 + 5) * cc_dim1 + 1];
	tr2 = cc[(k * 5 + 2) * cc_dim1 + 1] + cc[(k * 5 + 5) * cc_dim1 + 1];
	tr4 = cc[(k * 5 + 3) * cc_dim1 + 1] - cc[(k * 5 + 4) * cc_dim1 + 1];
	tr3 = cc[(k * 5 + 3) * cc_dim1 + 1] + cc[(k * 5 + 4) * cc_dim1 + 1];
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[(k * 5 + 1) * cc_dim1 + 1] + tr2 
		+ tr3;
	ch[(k + ch_dim2) * ch_dim1 + 2] = cc[(k * 5 + 1) * cc_dim1 + 2] + ti2 
		+ ti3;
	cr2 = cc[(k * 5 + 1) * cc_dim1 + 1] + tr11 * tr2 + tr12 * tr3;
	ci2 = cc[(k * 5 + 1) * cc_dim1 + 2] + tr11 * ti2 + tr12 * ti3;
	cr3 = cc[(k * 5 + 1) * cc_dim1 + 1] + tr12 * tr2 + tr11 * tr3;
	ci3 = cc[(k * 5 + 1) * cc_dim1 + 2] + tr12 * ti2 + tr11 * ti3;
	cr5 = ti11 * tr5 + ti12 * tr4;
	ci5 = ti11 * ti5 + ti12 * ti4;
	cr4 = ti12 * tr5 - ti11 * tr4;
	ci4 = ti12 * ti5 - ti11 * ti4;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cr2 - ci5;
	ch[(k + ch_dim2 * 5) * ch_dim1 + 1] = cr2 + ci5;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 2] = ci2 + cr5;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = ci3 + cr4;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cr3 - ci4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = cr3 + ci4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = ci3 - cr4;
	ch[(k + ch_dim2 * 5) * ch_dim1 + 2] = ci2 - cr5;
/* L101: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; i__ += 2) {
	    ti5 = cc[i__ + (k * 5 + 2) * cc_dim1] - cc[i__ + (k * 5 + 5) * 
		    cc_dim1];
	    ti2 = cc[i__ + (k * 5 + 2) * cc_dim1] + cc[i__ + (k * 5 + 5) * 
		    cc_dim1];
	    ti4 = cc[i__ + (k * 5 + 3) * cc_dim1] - cc[i__ + (k * 5 + 4) * 
		    cc_dim1];
	    ti3 = cc[i__ + (k * 5 + 3) * cc_dim1] + cc[i__ + (k * 5 + 4) * 
		    cc_dim1];
	    tr5 = cc[i__ - 1 + (k * 5 + 2) * cc_dim1] - cc[i__ - 1 + (k * 5 + 
		    5) * cc_dim1];
	    tr2 = cc[i__ - 1 + (k * 5 + 2) * cc_dim1] + cc[i__ - 1 + (k * 5 + 
		    5) * cc_dim1];
	    tr4 = cc[i__ - 1 + (k * 5 + 3) * cc_dim1] - cc[i__ - 1 + (k * 5 + 
		    4) * cc_dim1];
	    tr3 = cc[i__ - 1 + (k * 5 + 3) * cc_dim1] + cc[i__ - 1 + (k * 5 + 
		    4) * cc_dim1];
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + (k * 5 + 1) *
		     cc_dim1] + tr2 + tr3;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * 5 + 1) * 
		    cc_dim1] + ti2 + ti3;
	    cr2 = cc[i__ - 1 + (k * 5 + 1) * cc_dim1] + tr11 * tr2 + tr12 * 
		    tr3;
	    ci2 = cc[i__ + (k * 5 + 1) * cc_dim1] + tr11 * ti2 + tr12 * ti3;
	    cr3 = cc[i__ - 1 + (k * 5 + 1) * cc_dim1] + tr12 * tr2 + tr11 * 
		    tr3;
	    ci3 = cc[i__ + (k * 5 + 1) * cc_dim1] + tr12 * ti2 + tr11 * ti3;
	    cr5 = ti11 * tr5 + ti12 * tr4;
	    ci5 = ti11 * ti5 + ti12 * ti4;
	    cr4 = ti12 * tr5 - ti11 * tr4;
	    ci4 = ti12 * ti5 - ti11 * ti4;
	    dr3 = cr3 - ci4;
	    dr4 = cr3 + ci4;
	    di3 = ci3 + cr4;
	    di4 = ci3 - cr4;
	    dr5 = cr2 + ci5;
	    dr2 = cr2 - ci5;
	    di5 = ci2 - cr5;
	    di2 = ci2 + cr5;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * dr2 
		    + wa1[i__] * di2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 1] * di2 - 
		    wa1[i__] * dr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * dr3 + 
		    wa2[i__] * di3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 1] * di3 - wa2[
		    i__] * dr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * dr4 
		    + wa3[i__] * di4;
	    ch[i__ + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 1] * di4 - 
		    wa3[i__] * dr4;
	    ch[i__ - 1 + (k + ch_dim2 * 5) * ch_dim1] = wa4[i__ - 1] * dr5 + 
		    wa4[i__] * di5;
	    ch[i__ + (k + ch_dim2 * 5) * ch_dim1] = wa4[i__ - 1] * di5 - wa4[
		    i__] * dr5;
/* L103: */
	}
/* L104: */
    }
    return 0;
} /* dpassf5_ */

/* Subroutine */ int dradb2_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1)
{
    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ti2, tr2;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 3;
    cc -= cc_offset;
    --wa1;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[((k << 1) + 1) * cc_dim1 + 1] + 
		cc[*ido + ((k << 1) + 2) * cc_dim1];
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cc[((k << 1) + 1) * cc_dim1 
		+ 1] - cc[*ido + ((k << 1) + 2) * cc_dim1];
/* L101: */
    }
    if ((i__1 = *ido - 2) < 0) {
	goto L107;
    } else if (i__1 == 0) {
	goto L105;
    } else {
	goto L102;
    }
L102:
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + ((k << 1) + 
		    1) * cc_dim1] + cc[ic - 1 + ((k << 1) + 2) * cc_dim1];
	    tr2 = cc[i__ - 1 + ((k << 1) + 1) * cc_dim1] - cc[ic - 1 + ((k << 
		    1) + 2) * cc_dim1];
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + ((k << 1) + 1) * 
		    cc_dim1] - cc[ic + ((k << 1) + 2) * cc_dim1];
	    ti2 = cc[i__ + ((k << 1) + 1) * cc_dim1] + cc[ic + ((k << 1) + 2) 
		    * cc_dim1];
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * tr2 
		    - wa1[i__ - 1] * ti2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * ti2 + 
		    wa1[i__ - 1] * tr2;
/* L103: */
	}
/* L104: */
    }
    if (*ido % 2 == 1) {
	return 0;
    }
L105:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[*ido + (k + ch_dim2) * ch_dim1] = cc[*ido + ((k << 1) + 1) * 
		cc_dim1] + cc[*ido + ((k << 1) + 1) * cc_dim1];
	ch[*ido + (k + (ch_dim2 << 1)) * ch_dim1] = -(cc[((k << 1) + 2) * 
		cc_dim1 + 1] + cc[((k << 1) + 2) * cc_dim1 + 1]);
/* L106: */
    }
L107:
    return 0;
} /* dradb2_ */

/* Subroutine */ int dradb3_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2)
{
    /* Initialized data */

    static doublereal taur = -.5;
    static doublereal taui = .86602540378443864676372317075293618;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + (cc_dim1 << 2);
    cc -= cc_offset;
    --wa1;
    --wa2;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr2 = cc[*ido + (k * 3 + 2) * cc_dim1] + cc[*ido + (k * 3 + 2) * 
		cc_dim1];
	cr2 = cc[(k * 3 + 1) * cc_dim1 + 1] + taur * tr2;
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[(k * 3 + 1) * cc_dim1 + 1] + tr2;
	ci3 = taui * (cc[(k * 3 + 3) * cc_dim1 + 1] + cc[(k * 3 + 3) * 
		cc_dim1 + 1]);
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cr2 - ci3;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cr2 + ci3;
/* L101: */
    }
    if (*ido == 1) {
	return 0;
    }
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    tr2 = cc[i__ - 1 + (k * 3 + 3) * cc_dim1] + cc[ic - 1 + (k * 3 + 
		    2) * cc_dim1];
	    cr2 = cc[i__ - 1 + (k * 3 + 1) * cc_dim1] + taur * tr2;
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + (k * 3 + 1) *
		     cc_dim1] + tr2;
	    ti2 = cc[i__ + (k * 3 + 3) * cc_dim1] - cc[ic + (k * 3 + 2) * 
		    cc_dim1];
	    ci2 = cc[i__ + (k * 3 + 1) * cc_dim1] + taur * ti2;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * 3 + 1) * 
		    cc_dim1] + ti2;
	    cr3 = taui * (cc[i__ - 1 + (k * 3 + 3) * cc_dim1] - cc[ic - 1 + (
		    k * 3 + 2) * cc_dim1]);
	    ci3 = taui * (cc[i__ + (k * 3 + 3) * cc_dim1] + cc[ic + (k * 3 + 
		    2) * cc_dim1]);
	    dr2 = cr2 - ci3;
	    dr3 = cr2 + ci3;
	    di2 = ci2 + cr3;
	    di3 = ci2 - cr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * dr2 
		    - wa1[i__ - 1] * di2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * di2 + 
		    wa1[i__ - 1] * dr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 2] * dr3 - 
		    wa2[i__ - 1] * di3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 2] * di3 + wa2[
		    i__ - 1] * dr3;
/* L102: */
	}
/* L103: */
    }
    return 0;
} /* dradb3_ */

/* Subroutine */ int dradb4_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3)
{
    /* Initialized data */

    static doublereal sqrt2 = 1.414213562373095048801688724209698;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, 
	    tr2, tr3, tr4;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 5;
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr1 = cc[((k << 2) + 1) * cc_dim1 + 1] - cc[*ido + ((k << 2) + 4) * 
		cc_dim1];
	tr2 = cc[((k << 2) + 1) * cc_dim1 + 1] + cc[*ido + ((k << 2) + 4) * 
		cc_dim1];
	tr3 = cc[*ido + ((k << 2) + 2) * cc_dim1] + cc[*ido + ((k << 2) + 2) *
		 cc_dim1];
	tr4 = cc[((k << 2) + 3) * cc_dim1 + 1] + cc[((k << 2) + 3) * cc_dim1 
		+ 1];
	ch[(k + ch_dim2) * ch_dim1 + 1] = tr2 + tr3;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = tr1 - tr4;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = tr2 - tr3;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = tr1 + tr4;
/* L101: */
    }
    if ((i__1 = *ido - 2) < 0) {
	goto L107;
    } else if (i__1 == 0) {
	goto L105;
    } else {
	goto L102;
    }
L102:
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    ti1 = cc[i__ + ((k << 2) + 1) * cc_dim1] + cc[ic + ((k << 2) + 4) 
		    * cc_dim1];
	    ti2 = cc[i__ + ((k << 2) + 1) * cc_dim1] - cc[ic + ((k << 2) + 4) 
		    * cc_dim1];
	    ti3 = cc[i__ + ((k << 2) + 3) * cc_dim1] - cc[ic + ((k << 2) + 2) 
		    * cc_dim1];
	    tr4 = cc[i__ + ((k << 2) + 3) * cc_dim1] + cc[ic + ((k << 2) + 2) 
		    * cc_dim1];
	    tr1 = cc[i__ - 1 + ((k << 2) + 1) * cc_dim1] - cc[ic - 1 + ((k << 
		    2) + 4) * cc_dim1];
	    tr2 = cc[i__ - 1 + ((k << 2) + 1) * cc_dim1] + cc[ic - 1 + ((k << 
		    2) + 4) * cc_dim1];
	    ti4 = cc[i__ - 1 + ((k << 2) + 3) * cc_dim1] - cc[ic - 1 + ((k << 
		    2) + 2) * cc_dim1];
	    tr3 = cc[i__ - 1 + ((k << 2) + 3) * cc_dim1] + cc[ic - 1 + ((k << 
		    2) + 2) * cc_dim1];
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = tr2 + tr3;
	    cr3 = tr2 - tr3;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = ti2 + ti3;
	    ci3 = ti2 - ti3;
	    cr2 = tr1 - tr4;
	    cr4 = tr1 + tr4;
	    ci2 = ti1 + ti4;
	    ci4 = ti1 - ti4;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * cr2 
		    - wa1[i__ - 1] * ci2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * ci2 + 
		    wa1[i__ - 1] * cr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 2] * cr3 - 
		    wa2[i__ - 1] * ci3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 2] * ci3 + wa2[
		    i__ - 1] * cr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 2] * cr4 
		    - wa3[i__ - 1] * ci4;
	    ch[i__ + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 2] * ci4 + 
		    wa3[i__ - 1] * cr4;
/* L103: */
	}
/* L104: */
    }
    if (*ido % 2 == 1) {
	return 0;
    }
L105:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti1 = cc[((k << 2) + 2) * cc_dim1 + 1] + cc[((k << 2) + 4) * cc_dim1 
		+ 1];
	ti2 = cc[((k << 2) + 4) * cc_dim1 + 1] - cc[((k << 2) + 2) * cc_dim1 
		+ 1];
	tr1 = cc[*ido + ((k << 2) + 1) * cc_dim1] - cc[*ido + ((k << 2) + 3) *
		 cc_dim1];
	tr2 = cc[*ido + ((k << 2) + 1) * cc_dim1] + cc[*ido + ((k << 2) + 3) *
		 cc_dim1];
	ch[*ido + (k + ch_dim2) * ch_dim1] = tr2 + tr2;
	ch[*ido + (k + (ch_dim2 << 1)) * ch_dim1] = sqrt2 * (tr1 - ti1);
	ch[*ido + (k + ch_dim2 * 3) * ch_dim1] = ti2 + ti2;
	ch[*ido + (k + (ch_dim2 << 2)) * ch_dim1] = -sqrt2 * (tr1 + ti1);
/* L106: */
    }
L107:
    return 0;
} /* dradb4_ */

/* Subroutine */ int dradb5_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, 
	doublereal *wa4)
{
    /* Initialized data */

    static doublereal tr11 = .30901699437494742410229341718281905;
    static doublereal ti11 = .95105651629515357211643933337938214;
    static doublereal tr12 = -.80901699437494742410229341718281906;
    static doublereal ti12 = .58778525229247312916870595463907276;

    /* System generated locals */
    integer cc_dim1, cc_offset, ch_dim1, ch_dim2, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, 
	    cr4, ti2, ti3, ti4, ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_offset = 1 + cc_dim1 * 6;
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;
    --wa4;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti5 = cc[(k * 5 + 3) * cc_dim1 + 1] + cc[(k * 5 + 3) * cc_dim1 + 1];
	ti4 = cc[(k * 5 + 5) * cc_dim1 + 1] + cc[(k * 5 + 5) * cc_dim1 + 1];
	tr2 = cc[*ido + (k * 5 + 2) * cc_dim1] + cc[*ido + (k * 5 + 2) * 
		cc_dim1];
	tr3 = cc[*ido + (k * 5 + 4) * cc_dim1] + cc[*ido + (k * 5 + 4) * 
		cc_dim1];
	ch[(k + ch_dim2) * ch_dim1 + 1] = cc[(k * 5 + 1) * cc_dim1 + 1] + tr2 
		+ tr3;
	cr2 = cc[(k * 5 + 1) * cc_dim1 + 1] + tr11 * tr2 + tr12 * tr3;
	cr3 = cc[(k * 5 + 1) * cc_dim1 + 1] + tr12 * tr2 + tr11 * tr3;
	ci5 = ti11 * ti5 + ti12 * ti4;
	ci4 = ti12 * ti5 - ti11 * ti4;
	ch[(k + (ch_dim2 << 1)) * ch_dim1 + 1] = cr2 - ci5;
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cr3 - ci4;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = cr3 + ci4;
	ch[(k + ch_dim2 * 5) * ch_dim1 + 1] = cr2 + ci5;
/* L101: */
    }
    if (*ido == 1) {
	return 0;
    }
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    ti5 = cc[i__ + (k * 5 + 3) * cc_dim1] + cc[ic + (k * 5 + 2) * 
		    cc_dim1];
	    ti2 = cc[i__ + (k * 5 + 3) * cc_dim1] - cc[ic + (k * 5 + 2) * 
		    cc_dim1];
	    ti4 = cc[i__ + (k * 5 + 5) * cc_dim1] + cc[ic + (k * 5 + 4) * 
		    cc_dim1];
	    ti3 = cc[i__ + (k * 5 + 5) * cc_dim1] - cc[ic + (k * 5 + 4) * 
		    cc_dim1];
	    tr5 = cc[i__ - 1 + (k * 5 + 3) * cc_dim1] - cc[ic - 1 + (k * 5 + 
		    2) * cc_dim1];
	    tr2 = cc[i__ - 1 + (k * 5 + 3) * cc_dim1] + cc[ic - 1 + (k * 5 + 
		    2) * cc_dim1];
	    tr4 = cc[i__ - 1 + (k * 5 + 5) * cc_dim1] - cc[ic - 1 + (k * 5 + 
		    4) * cc_dim1];
	    tr3 = cc[i__ - 1 + (k * 5 + 5) * cc_dim1] + cc[ic - 1 + (k * 5 + 
		    4) * cc_dim1];
	    ch[i__ - 1 + (k + ch_dim2) * ch_dim1] = cc[i__ - 1 + (k * 5 + 1) *
		     cc_dim1] + tr2 + tr3;
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * 5 + 1) * 
		    cc_dim1] + ti2 + ti3;
	    cr2 = cc[i__ - 1 + (k * 5 + 1) * cc_dim1] + tr11 * tr2 + tr12 * 
		    tr3;
	    ci2 = cc[i__ + (k * 5 + 1) * cc_dim1] + tr11 * ti2 + tr12 * ti3;
	    cr3 = cc[i__ - 1 + (k * 5 + 1) * cc_dim1] + tr12 * tr2 + tr11 * 
		    tr3;
	    ci3 = cc[i__ + (k * 5 + 1) * cc_dim1] + tr12 * ti2 + tr11 * ti3;
	    cr5 = ti11 * tr5 + ti12 * tr4;
	    ci5 = ti11 * ti5 + ti12 * ti4;
	    cr4 = ti12 * tr5 - ti11 * tr4;
	    ci4 = ti12 * ti5 - ti11 * ti4;
	    dr3 = cr3 - ci4;
	    dr4 = cr3 + ci4;
	    di3 = ci3 + cr4;
	    di4 = ci3 - cr4;
	    dr5 = cr2 + ci5;
	    dr2 = cr2 - ci5;
	    di5 = ci2 - cr5;
	    di2 = ci2 + cr5;
	    ch[i__ - 1 + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * dr2 
		    - wa1[i__ - 1] * di2;
	    ch[i__ + (k + (ch_dim2 << 1)) * ch_dim1] = wa1[i__ - 2] * di2 + 
		    wa1[i__ - 1] * dr2;
	    ch[i__ - 1 + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 2] * dr3 - 
		    wa2[i__ - 1] * di3;
	    ch[i__ + (k + ch_dim2 * 3) * ch_dim1] = wa2[i__ - 2] * di3 + wa2[
		    i__ - 1] * dr3;
	    ch[i__ - 1 + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 2] * dr4 
		    - wa3[i__ - 1] * di4;
	    ch[i__ + (k + (ch_dim2 << 2)) * ch_dim1] = wa3[i__ - 2] * di4 + 
		    wa3[i__ - 1] * dr4;
	    ch[i__ - 1 + (k + ch_dim2 * 5) * ch_dim1] = wa4[i__ - 2] * dr5 - 
		    wa4[i__ - 1] * di5;
	    ch[i__ + (k + ch_dim2 * 5) * ch_dim1] = wa4[i__ - 2] * di5 + wa4[
		    i__ - 1] * dr5;
/* L102: */
	}
/* L103: */
    }
    return 0;
} /* dradb5_ */

/* Subroutine */ int dradbg_(integer *ido, integer *ip, integer *l1, integer *
	idl1, doublereal *cc, doublereal *c1, doublereal *c2, doublereal *ch, 
	doublereal *ch2, doublereal *wa)
{
    /* Initialized data */

    static doublereal tpi = 6.2831853071795864769252867665590057;

    /* System generated locals */
    integer ch_dim1, ch_dim2, ch_offset, cc_dim1, cc_dim2, cc_offset, c1_dim1,
	     c1_dim2, c1_offset, c2_dim1, c2_offset, ch2_dim1, ch2_offset, 
	    i__1, i__2, i__3;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, k, l, j2, ic, jc, lc, ik, is;
    static doublereal dc2, ai1, ai2, ar1, ar2, ds2;
    static integer nbd;
    static doublereal dcp, arg, dsp, ar1h, ar2h;
    static integer idp2, ipp2, idij, ipph;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    c1_dim1 = *ido;
    c1_dim2 = *l1;
    c1_offset = 1 + c1_dim1 * (1 + c1_dim2);
    c1 -= c1_offset;
    cc_dim1 = *ido;
    cc_dim2 = *ip;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    ch2_dim1 = *idl1;
    ch2_offset = 1 + ch2_dim1;
    ch2 -= ch2_offset;
    c2_dim1 = *idl1;
    c2_offset = 1 + c2_dim1;
    c2 -= c2_offset;
    --wa;

    /* Function Body */
    arg = tpi / (doublereal) (*ip);
    dcp = cos(arg);
    dsp = sin(arg);
    idp2 = *ido + 2;
    nbd = (*ido - 1) / 2;
    ipp2 = *ip + 2;
    ipph = (*ip + 1) / 2;
    if (*ido < *l1) {
	goto L103;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * cc_dim2 + 1) * 
		    cc_dim1];
/* L101: */
	}
/* L102: */
    }
    goto L106;
L103:
    i__1 = *ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[i__ + (k + ch_dim2) * ch_dim1] = cc[i__ + (k * cc_dim2 + 1) * 
		    cc_dim1];
/* L104: */
	}
/* L105: */
    }
L106:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	j2 = j + j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[(k + j * ch_dim2) * ch_dim1 + 1] = cc[*ido + (j2 - 2 + k * 
		    cc_dim2) * cc_dim1] + cc[*ido + (j2 - 2 + k * cc_dim2) * 
		    cc_dim1];
	    ch[(k + jc * ch_dim2) * ch_dim1 + 1] = cc[(j2 - 1 + k * cc_dim2) *
		     cc_dim1 + 1] + cc[(j2 - 1 + k * cc_dim2) * cc_dim1 + 1];
/* L107: */
	}
/* L108: */
    }
    if (*ido == 1) {
	goto L116;
    }
    if (nbd < *l1) {
	goto L112;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		ic = idp2 - i__;
		ch[i__ - 1 + (k + j * ch_dim2) * ch_dim1] = cc[i__ - 1 + ((j 
			<< 1) - 1 + k * cc_dim2) * cc_dim1] + cc[ic - 1 + ((j 
			<< 1) - 2 + k * cc_dim2) * cc_dim1];
		ch[i__ - 1 + (k + jc * ch_dim2) * ch_dim1] = cc[i__ - 1 + ((j 
			<< 1) - 1 + k * cc_dim2) * cc_dim1] - cc[ic - 1 + ((j 
			<< 1) - 2 + k * cc_dim2) * cc_dim1];
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = cc[i__ + ((j << 1) - 
			1 + k * cc_dim2) * cc_dim1] - cc[ic + ((j << 1) - 2 + 
			k * cc_dim2) * cc_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = cc[i__ + ((j << 1) - 
			1 + k * cc_dim2) * cc_dim1] + cc[ic + ((j << 1) - 2 + 
			k * cc_dim2) * cc_dim1];
/* L109: */
	    }
/* L110: */
	}
/* L111: */
    }
    goto L116;
L112:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[i__ - 1 + (k + j * ch_dim2) * ch_dim1] = cc[i__ - 1 + ((j 
			<< 1) - 1 + k * cc_dim2) * cc_dim1] + cc[ic - 1 + ((j 
			<< 1) - 2 + k * cc_dim2) * cc_dim1];
		ch[i__ - 1 + (k + jc * ch_dim2) * ch_dim1] = cc[i__ - 1 + ((j 
			<< 1) - 1 + k * cc_dim2) * cc_dim1] - cc[ic - 1 + ((j 
			<< 1) - 2 + k * cc_dim2) * cc_dim1];
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = cc[i__ + ((j << 1) - 
			1 + k * cc_dim2) * cc_dim1] - cc[ic + ((j << 1) - 2 + 
			k * cc_dim2) * cc_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = cc[i__ + ((j << 1) - 
			1 + k * cc_dim2) * cc_dim1] + cc[ic + ((j << 1) - 2 + 
			k * cc_dim2) * cc_dim1];
/* L113: */
	    }
/* L114: */
	}
/* L115: */
    }
L116:
    ar1 = 1.;
    ai1 = 0.;
    i__1 = ipph;
    for (l = 2; l <= i__1; ++l) {
	lc = ipp2 - l;
	ar1h = dcp * ar1 - dsp * ai1;
	ai1 = dcp * ai1 + dsp * ar1;
	ar1 = ar1h;
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    c2[ik + l * c2_dim1] = ch2[ik + ch2_dim1] + ar1 * ch2[ik + (
		    ch2_dim1 << 1)];
	    c2[ik + lc * c2_dim1] = ai1 * ch2[ik + *ip * ch2_dim1];
/* L117: */
	}
	dc2 = ar1;
	ds2 = ai1;
	ar2 = ar1;
	ai2 = ai1;
	i__2 = ipph;
	for (j = 3; j <= i__2; ++j) {
	    jc = ipp2 - j;
	    ar2h = dc2 * ar2 - ds2 * ai2;
	    ai2 = dc2 * ai2 + ds2 * ar2;
	    ar2 = ar2h;
	    i__3 = *idl1;
	    for (ik = 1; ik <= i__3; ++ik) {
		c2[ik + l * c2_dim1] += ar2 * ch2[ik + j * ch2_dim1];
		c2[ik + lc * c2_dim1] += ai2 * ch2[ik + jc * ch2_dim1];
/* L118: */
	    }
/* L119: */
	}
/* L120: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    ch2[ik + ch2_dim1] += ch2[ik + j * ch2_dim1];
/* L121: */
	}
/* L122: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[(k + j * ch_dim2) * ch_dim1 + 1] = c1[(k + j * c1_dim2) * 
		    c1_dim1 + 1] - c1[(k + jc * c1_dim2) * c1_dim1 + 1];
	    ch[(k + jc * ch_dim2) * ch_dim1 + 1] = c1[(k + j * c1_dim2) * 
		    c1_dim1 + 1] + c1[(k + jc * c1_dim2) * c1_dim1 + 1];
/* L123: */
	}
/* L124: */
    }
    if (*ido == 1) {
	goto L132;
    }
    if (nbd < *l1) {
	goto L128;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		ch[i__ - 1 + (k + j * ch_dim2) * ch_dim1] = c1[i__ - 1 + (k + 
			j * c1_dim2) * c1_dim1] - c1[i__ + (k + jc * c1_dim2) 
			* c1_dim1];
		ch[i__ - 1 + (k + jc * ch_dim2) * ch_dim1] = c1[i__ - 1 + (k 
			+ j * c1_dim2) * c1_dim1] + c1[i__ + (k + jc * 
			c1_dim2) * c1_dim1];
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = c1[i__ + (k + j * 
			c1_dim2) * c1_dim1] + c1[i__ - 1 + (k + jc * c1_dim2) 
			* c1_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = c1[i__ + (k + j * 
			c1_dim2) * c1_dim1] - c1[i__ - 1 + (k + jc * c1_dim2) 
			* c1_dim1];
/* L125: */
	    }
/* L126: */
	}
/* L127: */
    }
    goto L132;
L128:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[i__ - 1 + (k + j * ch_dim2) * ch_dim1] = c1[i__ - 1 + (k + 
			j * c1_dim2) * c1_dim1] - c1[i__ + (k + jc * c1_dim2) 
			* c1_dim1];
		ch[i__ - 1 + (k + jc * ch_dim2) * ch_dim1] = c1[i__ - 1 + (k 
			+ j * c1_dim2) * c1_dim1] + c1[i__ + (k + jc * 
			c1_dim2) * c1_dim1];
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = c1[i__ + (k + j * 
			c1_dim2) * c1_dim1] + c1[i__ - 1 + (k + jc * c1_dim2) 
			* c1_dim1];
		ch[i__ + (k + jc * ch_dim2) * ch_dim1] = c1[i__ + (k + j * 
			c1_dim2) * c1_dim1] - c1[i__ - 1 + (k + jc * c1_dim2) 
			* c1_dim1];
/* L129: */
	    }
/* L130: */
	}
/* L131: */
    }
L132:
    if (*ido == 1) {
	return 0;
    }
    i__1 = *idl1;
    for (ik = 1; ik <= i__1; ++ik) {
	c2[ik + c2_dim1] = ch2[ik + ch2_dim1];
/* L133: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    c1[(k + j * c1_dim2) * c1_dim1 + 1] = ch[(k + j * ch_dim2) * 
		    ch_dim1 + 1];
/* L134: */
	}
/* L135: */
    }
    if (nbd > *l1) {
	goto L139;
    }
    is = -(*ido);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	is += *ido;
	idij = is;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    idij += 2;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[
			i__ - 1 + (k + j * ch_dim2) * ch_dim1] - wa[idij] * 
			ch[i__ + (k + j * ch_dim2) * ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[i__ 
			+ (k + j * ch_dim2) * ch_dim1] + wa[idij] * ch[i__ - 
			1 + (k + j * ch_dim2) * ch_dim1];
/* L136: */
	    }
/* L137: */
	}
/* L138: */
    }
    goto L143;
L139:
    is = -(*ido);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	is += *ido;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    idij = is;
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		idij += 2;
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[
			i__ - 1 + (k + j * ch_dim2) * ch_dim1] - wa[idij] * 
			ch[i__ + (k + j * ch_dim2) * ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = wa[idij - 1] * ch[i__ 
			+ (k + j * ch_dim2) * ch_dim1] + wa[idij] * ch[i__ - 
			1 + (k + j * ch_dim2) * ch_dim1];
/* L140: */
	    }
/* L141: */
	}
/* L142: */
    }
L143:
    return 0;
} /* dradbg_ */

/* Subroutine */ int dradf2_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1)
{
    /* System generated locals */
    integer ch_dim1, ch_offset, cc_dim1, cc_dim2, cc_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ti2, tr2;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_offset = 1 + ch_dim1 * 3;
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_dim2 = *l1;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    --wa1;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[((k << 1) + 1) * ch_dim1 + 1] = cc[(k + cc_dim2) * cc_dim1 + 1] + 
		cc[(k + (cc_dim2 << 1)) * cc_dim1 + 1];
	ch[*ido + ((k << 1) + 2) * ch_dim1] = cc[(k + cc_dim2) * cc_dim1 + 1] 
		- cc[(k + (cc_dim2 << 1)) * cc_dim1 + 1];
/* L101: */
    }
    if ((i__1 = *ido - 2) < 0) {
	goto L107;
    } else if (i__1 == 0) {
	goto L105;
    } else {
	goto L102;
    }
L102:
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    tr2 = wa1[i__ - 2] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * cc_dim1] 
		    + wa1[i__ - 1] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1];
	    ti2 = wa1[i__ - 2] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1] - 
		    wa1[i__ - 1] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * 
		    cc_dim1];
	    ch[i__ + ((k << 1) + 1) * ch_dim1] = cc[i__ + (k + cc_dim2) * 
		    cc_dim1] + ti2;
	    ch[ic + ((k << 1) + 2) * ch_dim1] = ti2 - cc[i__ + (k + cc_dim2) *
		     cc_dim1];
	    ch[i__ - 1 + ((k << 1) + 1) * ch_dim1] = cc[i__ - 1 + (k + 
		    cc_dim2) * cc_dim1] + tr2;
	    ch[ic - 1 + ((k << 1) + 2) * ch_dim1] = cc[i__ - 1 + (k + cc_dim2)
		     * cc_dim1] - tr2;
/* L103: */
	}
/* L104: */
    }
    if (*ido % 2 == 1) {
	return 0;
    }
L105:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[((k << 1) + 2) * ch_dim1 + 1] = -cc[*ido + (k + (cc_dim2 << 1)) * 
		cc_dim1];
	ch[*ido + ((k << 1) + 1) * ch_dim1] = cc[*ido + (k + cc_dim2) * 
		cc_dim1];
/* L106: */
    }
L107:
    return 0;
} /* dradf2_ */

/* Subroutine */ int dradf3_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2)
{
    /* Initialized data */

    static doublereal taur = -.5;
    static doublereal taui = .86602540378443864676372317075293618;

    /* System generated locals */
    integer ch_dim1, ch_offset, cc_dim1, cc_dim2, cc_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ci2, di2, di3, cr2, dr2, dr3, ti2, ti3, tr2, tr3;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_offset = 1 + (ch_dim1 << 2);
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_dim2 = *l1;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    --wa1;
    --wa2;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	cr2 = cc[(k + (cc_dim2 << 1)) * cc_dim1 + 1] + cc[(k + cc_dim2 * 3) * 
		cc_dim1 + 1];
	ch[(k * 3 + 1) * ch_dim1 + 1] = cc[(k + cc_dim2) * cc_dim1 + 1] + cr2;
	ch[(k * 3 + 3) * ch_dim1 + 1] = taui * (cc[(k + cc_dim2 * 3) * 
		cc_dim1 + 1] - cc[(k + (cc_dim2 << 1)) * cc_dim1 + 1]);
	ch[*ido + (k * 3 + 2) * ch_dim1] = cc[(k + cc_dim2) * cc_dim1 + 1] + 
		taur * cr2;
/* L101: */
    }
    if (*ido == 1) {
	return 0;
    }
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    dr2 = wa1[i__ - 2] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * cc_dim1] 
		    + wa1[i__ - 1] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1];
	    di2 = wa1[i__ - 2] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1] - 
		    wa1[i__ - 1] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * 
		    cc_dim1];
	    dr3 = wa2[i__ - 2] * cc[i__ - 1 + (k + cc_dim2 * 3) * cc_dim1] + 
		    wa2[i__ - 1] * cc[i__ + (k + cc_dim2 * 3) * cc_dim1];
	    di3 = wa2[i__ - 2] * cc[i__ + (k + cc_dim2 * 3) * cc_dim1] - wa2[
		    i__ - 1] * cc[i__ - 1 + (k + cc_dim2 * 3) * cc_dim1];
	    cr2 = dr2 + dr3;
	    ci2 = di2 + di3;
	    ch[i__ - 1 + (k * 3 + 1) * ch_dim1] = cc[i__ - 1 + (k + cc_dim2) *
		     cc_dim1] + cr2;
	    ch[i__ + (k * 3 + 1) * ch_dim1] = cc[i__ + (k + cc_dim2) * 
		    cc_dim1] + ci2;
	    tr2 = cc[i__ - 1 + (k + cc_dim2) * cc_dim1] + taur * cr2;
	    ti2 = cc[i__ + (k + cc_dim2) * cc_dim1] + taur * ci2;
	    tr3 = taui * (di2 - di3);
	    ti3 = taui * (dr3 - dr2);
	    ch[i__ - 1 + (k * 3 + 3) * ch_dim1] = tr2 + tr3;
	    ch[ic - 1 + (k * 3 + 2) * ch_dim1] = tr2 - tr3;
	    ch[i__ + (k * 3 + 3) * ch_dim1] = ti2 + ti3;
	    ch[ic + (k * 3 + 2) * ch_dim1] = ti3 - ti2;
/* L102: */
	}
/* L103: */
    }
    return 0;
} /* dradf3_ */

/* Subroutine */ int dradf4_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3)
{
    /* Initialized data */

    static doublereal hsqt2 = .70710678118654752440084436210484904;

    /* System generated locals */
    integer cc_dim1, cc_dim2, cc_offset, ch_dim1, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ci2, ci3, ci4, cr2, cr3, cr4, ti1, ti2, ti3, ti4, tr1, 
	    tr2, tr3, tr4;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_offset = 1 + ch_dim1 * 5;
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_dim2 = *l1;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr1 = cc[(k + (cc_dim2 << 1)) * cc_dim1 + 1] + cc[(k + (cc_dim2 << 2))
		 * cc_dim1 + 1];
	tr2 = cc[(k + cc_dim2) * cc_dim1 + 1] + cc[(k + cc_dim2 * 3) * 
		cc_dim1 + 1];
	ch[((k << 2) + 1) * ch_dim1 + 1] = tr1 + tr2;
	ch[*ido + ((k << 2) + 4) * ch_dim1] = tr2 - tr1;
	ch[*ido + ((k << 2) + 2) * ch_dim1] = cc[(k + cc_dim2) * cc_dim1 + 1] 
		- cc[(k + cc_dim2 * 3) * cc_dim1 + 1];
	ch[((k << 2) + 3) * ch_dim1 + 1] = cc[(k + (cc_dim2 << 2)) * cc_dim1 
		+ 1] - cc[(k + (cc_dim2 << 1)) * cc_dim1 + 1];
/* L101: */
    }
    if ((i__1 = *ido - 2) < 0) {
	goto L107;
    } else if (i__1 == 0) {
	goto L105;
    } else {
	goto L102;
    }
L102:
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    cr2 = wa1[i__ - 2] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * cc_dim1] 
		    + wa1[i__ - 1] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1];
	    ci2 = wa1[i__ - 2] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1] - 
		    wa1[i__ - 1] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * 
		    cc_dim1];
	    cr3 = wa2[i__ - 2] * cc[i__ - 1 + (k + cc_dim2 * 3) * cc_dim1] + 
		    wa2[i__ - 1] * cc[i__ + (k + cc_dim2 * 3) * cc_dim1];
	    ci3 = wa2[i__ - 2] * cc[i__ + (k + cc_dim2 * 3) * cc_dim1] - wa2[
		    i__ - 1] * cc[i__ - 1 + (k + cc_dim2 * 3) * cc_dim1];
	    cr4 = wa3[i__ - 2] * cc[i__ - 1 + (k + (cc_dim2 << 2)) * cc_dim1] 
		    + wa3[i__ - 1] * cc[i__ + (k + (cc_dim2 << 2)) * cc_dim1];
	    ci4 = wa3[i__ - 2] * cc[i__ + (k + (cc_dim2 << 2)) * cc_dim1] - 
		    wa3[i__ - 1] * cc[i__ - 1 + (k + (cc_dim2 << 2)) * 
		    cc_dim1];
	    tr1 = cr2 + cr4;
	    tr4 = cr4 - cr2;
	    ti1 = ci2 + ci4;
	    ti4 = ci2 - ci4;
	    ti2 = cc[i__ + (k + cc_dim2) * cc_dim1] + ci3;
	    ti3 = cc[i__ + (k + cc_dim2) * cc_dim1] - ci3;
	    tr2 = cc[i__ - 1 + (k + cc_dim2) * cc_dim1] + cr3;
	    tr3 = cc[i__ - 1 + (k + cc_dim2) * cc_dim1] - cr3;
	    ch[i__ - 1 + ((k << 2) + 1) * ch_dim1] = tr1 + tr2;
	    ch[ic - 1 + ((k << 2) + 4) * ch_dim1] = tr2 - tr1;
	    ch[i__ + ((k << 2) + 1) * ch_dim1] = ti1 + ti2;
	    ch[ic + ((k << 2) + 4) * ch_dim1] = ti1 - ti2;
	    ch[i__ - 1 + ((k << 2) + 3) * ch_dim1] = ti4 + tr3;
	    ch[ic - 1 + ((k << 2) + 2) * ch_dim1] = tr3 - ti4;
	    ch[i__ + ((k << 2) + 3) * ch_dim1] = tr4 + ti3;
	    ch[ic + ((k << 2) + 2) * ch_dim1] = tr4 - ti3;
/* L103: */
	}
/* L104: */
    }
    if (*ido % 2 == 1) {
	return 0;
    }
L105:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti1 = -hsqt2 * (cc[*ido + (k + (cc_dim2 << 1)) * cc_dim1] + cc[*ido + 
		(k + (cc_dim2 << 2)) * cc_dim1]);
	tr1 = hsqt2 * (cc[*ido + (k + (cc_dim2 << 1)) * cc_dim1] - cc[*ido + (
		k + (cc_dim2 << 2)) * cc_dim1]);
	ch[*ido + ((k << 2) + 1) * ch_dim1] = tr1 + cc[*ido + (k + cc_dim2) * 
		cc_dim1];
	ch[*ido + ((k << 2) + 3) * ch_dim1] = cc[*ido + (k + cc_dim2) * 
		cc_dim1] - tr1;
	ch[((k << 2) + 2) * ch_dim1 + 1] = ti1 - cc[*ido + (k + cc_dim2 * 3) *
		 cc_dim1];
	ch[((k << 2) + 4) * ch_dim1 + 1] = ti1 + cc[*ido + (k + cc_dim2 * 3) *
		 cc_dim1];
/* L106: */
    }
L107:
    return 0;
} /* dradf4_ */

/* Subroutine */ int dradf5_(integer *ido, integer *l1, doublereal *cc, 
	doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, 
	doublereal *wa4)
{
    /* Initialized data */

    static doublereal tr11 = .30901699437494742410229341718281905;
    static doublereal ti11 = .95105651629515357211643933337938214;
    static doublereal tr12 = -.80901699437494742410229341718281906;
    static doublereal ti12 = .58778525229247312916870595463907276;

    /* System generated locals */
    integer cc_dim1, cc_dim2, cc_offset, ch_dim1, ch_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k, ic;
    static doublereal ci2, di2, ci4, ci5, di3, di4, di5, ci3, cr2, cr3, dr2, 
	    dr3, dr4, dr5, cr5, cr4, ti2, ti3, ti5, ti4, tr2, tr3, tr4, tr5;
    static integer idp2;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_offset = 1 + ch_dim1 * 6;
    ch -= ch_offset;
    cc_dim1 = *ido;
    cc_dim2 = *l1;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    --wa1;
    --wa2;
    --wa3;
    --wa4;

    /* Function Body */
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	cr2 = cc[(k + cc_dim2 * 5) * cc_dim1 + 1] + cc[(k + (cc_dim2 << 1)) * 
		cc_dim1 + 1];
	ci5 = cc[(k + cc_dim2 * 5) * cc_dim1 + 1] - cc[(k + (cc_dim2 << 1)) * 
		cc_dim1 + 1];
	cr3 = cc[(k + (cc_dim2 << 2)) * cc_dim1 + 1] + cc[(k + cc_dim2 * 3) * 
		cc_dim1 + 1];
	ci4 = cc[(k + (cc_dim2 << 2)) * cc_dim1 + 1] - cc[(k + cc_dim2 * 3) * 
		cc_dim1 + 1];
	ch[(k * 5 + 1) * ch_dim1 + 1] = cc[(k + cc_dim2) * cc_dim1 + 1] + cr2 
		+ cr3;
	ch[*ido + (k * 5 + 2) * ch_dim1] = cc[(k + cc_dim2) * cc_dim1 + 1] + 
		tr11 * cr2 + tr12 * cr3;
	ch[(k * 5 + 3) * ch_dim1 + 1] = ti11 * ci5 + ti12 * ci4;
	ch[*ido + (k * 5 + 4) * ch_dim1] = cc[(k + cc_dim2) * cc_dim1 + 1] + 
		tr12 * cr2 + tr11 * cr3;
	ch[(k * 5 + 5) * ch_dim1 + 1] = ti12 * ci5 - ti11 * ci4;
/* L101: */
    }
    if (*ido == 1) {
	return 0;
    }
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    dr2 = wa1[i__ - 2] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * cc_dim1] 
		    + wa1[i__ - 1] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1];
	    di2 = wa1[i__ - 2] * cc[i__ + (k + (cc_dim2 << 1)) * cc_dim1] - 
		    wa1[i__ - 1] * cc[i__ - 1 + (k + (cc_dim2 << 1)) * 
		    cc_dim1];
	    dr3 = wa2[i__ - 2] * cc[i__ - 1 + (k + cc_dim2 * 3) * cc_dim1] + 
		    wa2[i__ - 1] * cc[i__ + (k + cc_dim2 * 3) * cc_dim1];
	    di3 = wa2[i__ - 2] * cc[i__ + (k + cc_dim2 * 3) * cc_dim1] - wa2[
		    i__ - 1] * cc[i__ - 1 + (k + cc_dim2 * 3) * cc_dim1];
	    dr4 = wa3[i__ - 2] * cc[i__ - 1 + (k + (cc_dim2 << 2)) * cc_dim1] 
		    + wa3[i__ - 1] * cc[i__ + (k + (cc_dim2 << 2)) * cc_dim1];
	    di4 = wa3[i__ - 2] * cc[i__ + (k + (cc_dim2 << 2)) * cc_dim1] - 
		    wa3[i__ - 1] * cc[i__ - 1 + (k + (cc_dim2 << 2)) * 
		    cc_dim1];
	    dr5 = wa4[i__ - 2] * cc[i__ - 1 + (k + cc_dim2 * 5) * cc_dim1] + 
		    wa4[i__ - 1] * cc[i__ + (k + cc_dim2 * 5) * cc_dim1];
	    di5 = wa4[i__ - 2] * cc[i__ + (k + cc_dim2 * 5) * cc_dim1] - wa4[
		    i__ - 1] * cc[i__ - 1 + (k + cc_dim2 * 5) * cc_dim1];
	    cr2 = dr2 + dr5;
	    ci5 = dr5 - dr2;
	    cr5 = di2 - di5;
	    ci2 = di2 + di5;
	    cr3 = dr3 + dr4;
	    ci4 = dr4 - dr3;
	    cr4 = di3 - di4;
	    ci3 = di3 + di4;
	    ch[i__ - 1 + (k * 5 + 1) * ch_dim1] = cc[i__ - 1 + (k + cc_dim2) *
		     cc_dim1] + cr2 + cr3;
	    ch[i__ + (k * 5 + 1) * ch_dim1] = cc[i__ + (k + cc_dim2) * 
		    cc_dim1] + ci2 + ci3;
	    tr2 = cc[i__ - 1 + (k + cc_dim2) * cc_dim1] + tr11 * cr2 + tr12 * 
		    cr3;
	    ti2 = cc[i__ + (k + cc_dim2) * cc_dim1] + tr11 * ci2 + tr12 * ci3;
	    tr3 = cc[i__ - 1 + (k + cc_dim2) * cc_dim1] + tr12 * cr2 + tr11 * 
		    cr3;
	    ti3 = cc[i__ + (k + cc_dim2) * cc_dim1] + tr12 * ci2 + tr11 * ci3;
	    tr5 = ti11 * cr5 + ti12 * cr4;
	    ti5 = ti11 * ci5 + ti12 * ci4;
	    tr4 = ti12 * cr5 - ti11 * cr4;
	    ti4 = ti12 * ci5 - ti11 * ci4;
	    ch[i__ - 1 + (k * 5 + 3) * ch_dim1] = tr2 + tr5;
	    ch[ic - 1 + (k * 5 + 2) * ch_dim1] = tr2 - tr5;
	    ch[i__ + (k * 5 + 3) * ch_dim1] = ti2 + ti5;
	    ch[ic + (k * 5 + 2) * ch_dim1] = ti5 - ti2;
	    ch[i__ - 1 + (k * 5 + 5) * ch_dim1] = tr3 + tr4;
	    ch[ic - 1 + (k * 5 + 4) * ch_dim1] = tr3 - tr4;
	    ch[i__ + (k * 5 + 5) * ch_dim1] = ti3 + ti4;
	    ch[ic + (k * 5 + 4) * ch_dim1] = ti4 - ti3;
/* L102: */
	}
/* L103: */
    }
    return 0;
} /* dradf5_ */

/* Subroutine */ int dradfg_(integer *ido, integer *ip, integer *l1, integer *
	idl1, doublereal *cc, doublereal *c1, doublereal *c2, doublereal *ch, 
	doublereal *ch2, doublereal *wa)
{
    /* Initialized data */

    static doublereal tpi = 6.2831853071795864769252867665590057;

    /* System generated locals */
    integer ch_dim1, ch_dim2, ch_offset, cc_dim1, cc_dim2, cc_offset, c1_dim1,
	     c1_dim2, c1_offset, c2_dim1, c2_offset, ch2_dim1, ch2_offset, 
	    i__1, i__2, i__3;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, k, l, j2, ic, jc, lc, ik, is;
    static doublereal dc2, ai1, ai2, ar1, ar2, ds2;
    static integer nbd;
    static doublereal dcp, arg, dsp, ar1h, ar2h;
    static integer idp2, ipp2, idij, ipph;

    /* Parameter adjustments */
    ch_dim1 = *ido;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2);
    ch -= ch_offset;
    c1_dim1 = *ido;
    c1_dim2 = *l1;
    c1_offset = 1 + c1_dim1 * (1 + c1_dim2);
    c1 -= c1_offset;
    cc_dim1 = *ido;
    cc_dim2 = *ip;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2);
    cc -= cc_offset;
    ch2_dim1 = *idl1;
    ch2_offset = 1 + ch2_dim1;
    ch2 -= ch2_offset;
    c2_dim1 = *idl1;
    c2_offset = 1 + c2_dim1;
    c2 -= c2_offset;
    --wa;

    /* Function Body */
    arg = tpi / (doublereal) (*ip);
    dcp = cos(arg);
    dsp = sin(arg);
    ipph = (*ip + 1) / 2;
    ipp2 = *ip + 2;
    idp2 = *ido + 2;
    nbd = (*ido - 1) / 2;
    if (*ido == 1) {
	goto L119;
    }
    i__1 = *idl1;
    for (ik = 1; ik <= i__1; ++ik) {
	ch2[ik + ch2_dim1] = c2[ik + c2_dim1];
/* L101: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[(k + j * ch_dim2) * ch_dim1 + 1] = c1[(k + j * c1_dim2) * 
		    c1_dim1 + 1];
/* L102: */
	}
/* L103: */
    }
    if (nbd > *l1) {
	goto L107;
    }
    is = -(*ido);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	is += *ido;
	idij = is;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    idij += 2;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[i__ - 1 + (k + j * ch_dim2) * ch_dim1] = wa[idij - 1] * c1[
			i__ - 1 + (k + j * c1_dim2) * c1_dim1] + wa[idij] * 
			c1[i__ + (k + j * c1_dim2) * c1_dim1];
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = wa[idij - 1] * c1[i__ 
			+ (k + j * c1_dim2) * c1_dim1] - wa[idij] * c1[i__ - 
			1 + (k + j * c1_dim2) * c1_dim1];
/* L104: */
	    }
/* L105: */
	}
/* L106: */
    }
    goto L111;
L107:
    is = -(*ido);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	is += *ido;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    idij = is;
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		idij += 2;
		ch[i__ - 1 + (k + j * ch_dim2) * ch_dim1] = wa[idij - 1] * c1[
			i__ - 1 + (k + j * c1_dim2) * c1_dim1] + wa[idij] * 
			c1[i__ + (k + j * c1_dim2) * c1_dim1];
		ch[i__ + (k + j * ch_dim2) * ch_dim1] = wa[idij - 1] * c1[i__ 
			+ (k + j * c1_dim2) * c1_dim1] - wa[idij] * c1[i__ - 
			1 + (k + j * c1_dim2) * c1_dim1];
/* L108: */
	    }
/* L109: */
	}
/* L110: */
    }
L111:
    if (nbd < *l1) {
	goto L115;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = ch[i__ - 1 + (k + 
			j * ch_dim2) * ch_dim1] + ch[i__ - 1 + (k + jc * 
			ch_dim2) * ch_dim1];
		c1[i__ - 1 + (k + jc * c1_dim2) * c1_dim1] = ch[i__ + (k + j *
			 ch_dim2) * ch_dim1] - ch[i__ + (k + jc * ch_dim2) * 
			ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = ch[i__ + (k + j * 
			ch_dim2) * ch_dim1] + ch[i__ + (k + jc * ch_dim2) * 
			ch_dim1];
		c1[i__ + (k + jc * c1_dim2) * c1_dim1] = ch[i__ - 1 + (k + jc 
			* ch_dim2) * ch_dim1] - ch[i__ - 1 + (k + j * ch_dim2)
			 * ch_dim1];
/* L112: */
	    }
/* L113: */
	}
/* L114: */
    }
    goto L121;
L115:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		c1[i__ - 1 + (k + j * c1_dim2) * c1_dim1] = ch[i__ - 1 + (k + 
			j * ch_dim2) * ch_dim1] + ch[i__ - 1 + (k + jc * 
			ch_dim2) * ch_dim1];
		c1[i__ - 1 + (k + jc * c1_dim2) * c1_dim1] = ch[i__ + (k + j *
			 ch_dim2) * ch_dim1] - ch[i__ + (k + jc * ch_dim2) * 
			ch_dim1];
		c1[i__ + (k + j * c1_dim2) * c1_dim1] = ch[i__ + (k + j * 
			ch_dim2) * ch_dim1] + ch[i__ + (k + jc * ch_dim2) * 
			ch_dim1];
		c1[i__ + (k + jc * c1_dim2) * c1_dim1] = ch[i__ - 1 + (k + jc 
			* ch_dim2) * ch_dim1] - ch[i__ - 1 + (k + j * ch_dim2)
			 * ch_dim1];
/* L116: */
	    }
/* L117: */
	}
/* L118: */
    }
    goto L121;
L119:
    i__1 = *idl1;
    for (ik = 1; ik <= i__1; ++ik) {
	c2[ik + c2_dim1] = ch2[ik + ch2_dim1];
/* L120: */
    }
L121:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    c1[(k + j * c1_dim2) * c1_dim1 + 1] = ch[(k + j * ch_dim2) * 
		    ch_dim1 + 1] + ch[(k + jc * ch_dim2) * ch_dim1 + 1];
	    c1[(k + jc * c1_dim2) * c1_dim1 + 1] = ch[(k + jc * ch_dim2) * 
		    ch_dim1 + 1] - ch[(k + j * ch_dim2) * ch_dim1 + 1];
/* L122: */
	}
/* L123: */
    }

    ar1 = 1.;
    ai1 = 0.;
    i__1 = ipph;
    for (l = 2; l <= i__1; ++l) {
	lc = ipp2 - l;
	ar1h = dcp * ar1 - dsp * ai1;
	ai1 = dcp * ai1 + dsp * ar1;
	ar1 = ar1h;
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    ch2[ik + l * ch2_dim1] = c2[ik + c2_dim1] + ar1 * c2[ik + (
		    c2_dim1 << 1)];
	    ch2[ik + lc * ch2_dim1] = ai1 * c2[ik + *ip * c2_dim1];
/* L124: */
	}
	dc2 = ar1;
	ds2 = ai1;
	ar2 = ar1;
	ai2 = ai1;
	i__2 = ipph;
	for (j = 3; j <= i__2; ++j) {
	    jc = ipp2 - j;
	    ar2h = dc2 * ar2 - ds2 * ai2;
	    ai2 = dc2 * ai2 + ds2 * ar2;
	    ar2 = ar2h;
	    i__3 = *idl1;
	    for (ik = 1; ik <= i__3; ++ik) {
		ch2[ik + l * ch2_dim1] += ar2 * c2[ik + j * c2_dim1];
		ch2[ik + lc * ch2_dim1] += ai2 * c2[ik + jc * c2_dim1];
/* L125: */
	    }
/* L126: */
	}
/* L127: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    ch2[ik + ch2_dim1] += c2[ik + j * c2_dim1];
/* L128: */
	}
/* L129: */
    }

    if (*ido < *l1) {
	goto L132;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    cc[i__ + (k * cc_dim2 + 1) * cc_dim1] = ch[i__ + (k + ch_dim2) * 
		    ch_dim1];
/* L130: */
	}
/* L131: */
    }
    goto L135;
L132:
    i__1 = *ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    cc[i__ + (k * cc_dim2 + 1) * cc_dim1] = ch[i__ + (k + ch_dim2) * 
		    ch_dim1];
/* L133: */
	}
/* L134: */
    }
L135:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	j2 = j + j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    cc[*ido + (j2 - 2 + k * cc_dim2) * cc_dim1] = ch[(k + j * ch_dim2)
		     * ch_dim1 + 1];
	    cc[(j2 - 1 + k * cc_dim2) * cc_dim1 + 1] = ch[(k + jc * ch_dim2) *
		     ch_dim1 + 1];
/* L136: */
	}
/* L137: */
    }
    if (*ido == 1) {
	return 0;
    }
    if (nbd < *l1) {
	goto L141;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	j2 = j + j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		ic = idp2 - i__;
		cc[i__ - 1 + (j2 - 1 + k * cc_dim2) * cc_dim1] = ch[i__ - 1 + 
			(k + j * ch_dim2) * ch_dim1] + ch[i__ - 1 + (k + jc * 
			ch_dim2) * ch_dim1];
		cc[ic - 1 + (j2 - 2 + k * cc_dim2) * cc_dim1] = ch[i__ - 1 + (
			k + j * ch_dim2) * ch_dim1] - ch[i__ - 1 + (k + jc * 
			ch_dim2) * ch_dim1];
		cc[i__ + (j2 - 1 + k * cc_dim2) * cc_dim1] = ch[i__ + (k + j *
			 ch_dim2) * ch_dim1] + ch[i__ + (k + jc * ch_dim2) * 
			ch_dim1];
		cc[ic + (j2 - 2 + k * cc_dim2) * cc_dim1] = ch[i__ + (k + jc *
			 ch_dim2) * ch_dim1] - ch[i__ + (k + j * ch_dim2) * 
			ch_dim1];
/* L138: */
	    }
/* L139: */
	}
/* L140: */
    }
    return 0;
L141:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	j2 = j + j;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		cc[i__ - 1 + (j2 - 1 + k * cc_dim2) * cc_dim1] = ch[i__ - 1 + 
			(k + j * ch_dim2) * ch_dim1] + ch[i__ - 1 + (k + jc * 
			ch_dim2) * ch_dim1];
		cc[ic - 1 + (j2 - 2 + k * cc_dim2) * cc_dim1] = ch[i__ - 1 + (
			k + j * ch_dim2) * ch_dim1] - ch[i__ - 1 + (k + jc * 
			ch_dim2) * ch_dim1];
		cc[i__ + (j2 - 1 + k * cc_dim2) * cc_dim1] = ch[i__ + (k + j *
			 ch_dim2) * ch_dim1] + ch[i__ + (k + jc * ch_dim2) * 
			ch_dim1];
		cc[ic + (j2 - 2 + k * cc_dim2) * cc_dim1] = ch[i__ + (k + jc *
			 ch_dim2) * ch_dim1] - ch[i__ + (k + j * ch_dim2) * 
			ch_dim1];
/* L142: */
	    }
/* L143: */
	}
/* L144: */
    }
    return 0;
} /* dradfg_ */

/* Subroutine */ int dfftb1_(integer *n, doublereal *c__, doublereal *ch, 
	doublereal *wa, integer *ifac)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k1, l1, l2, na, nf, ip, iw, ix2, ix3, ix4, ido, idl1;
    extern /* Subroutine */ int dradb2_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *), dradb3_(integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dradb4_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dradb5_(integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dradbg_(integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --ifac;
    --wa;
    --ch;
    --c__;

    /* Function Body */
    nf = ifac[2];
    na = 0;
    l1 = 1;
    iw = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	l2 = ip * l1;
	ido = *n / l2;
	idl1 = ido * l1;
	if (ip != 4) {
	    goto L103;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	if (na != 0) {
	    goto L101;
	}
	dradb4_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3]);
	goto L102;
L101:
	dradb4_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3]);
L102:
	na = 1 - na;
	goto L115;
L103:
	if (ip != 2) {
	    goto L106;
	}
	if (na != 0) {
	    goto L104;
	}
	dradb2_(&ido, &l1, &c__[1], &ch[1], &wa[iw]);
	goto L105;
L104:
	dradb2_(&ido, &l1, &ch[1], &c__[1], &wa[iw]);
L105:
	na = 1 - na;
	goto L115;
L106:
	if (ip != 3) {
	    goto L109;
	}
	ix2 = iw + ido;
	if (na != 0) {
	    goto L107;
	}
	dradb3_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2]);
	goto L108;
L107:
	dradb3_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2]);
L108:
	na = 1 - na;
	goto L115;
L109:
	if (ip != 5) {
	    goto L112;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	ix4 = ix3 + ido;
	if (na != 0) {
	    goto L110;
	}
	dradb5_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3], &wa[
		ix4]);
	goto L111;
L110:
	dradb5_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3], &wa[
		ix4]);
L111:
	na = 1 - na;
	goto L115;
L112:
	if (na != 0) {
	    goto L113;
	}
	dradbg_(&ido, &ip, &l1, &idl1, &c__[1], &c__[1], &c__[1], &ch[1], &ch[
		1], &wa[iw]);
	goto L114;
L113:
	dradbg_(&ido, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__[1], &c__[
		1], &wa[iw]);
L114:
	if (ido == 1) {
	    na = 1 - na;
	}
L115:
	l1 = l2;
	iw += (ip - 1) * ido;
/* L116: */
    }
    if (na == 0) {
	return 0;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = ch[i__];
/* L117: */
    }
    return 0;
} /* dfftb1_ */

/* Subroutine */ int dfftb_(integer *n, doublereal *r__, doublereal *wsave)
{
    extern /* Subroutine */ int dfftb1_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;
    --r__;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    dfftb1_(n, &r__[1], &wsave[1], &wsave[*n + 1], (integer*)&wsave[(*n << 1) 
	    + 1]);
    return 0;
} /* dfftb_ */

/* Subroutine */ int dfftf1_(integer *n, doublereal *c__, doublereal *ch, 
	doublereal *wa, integer *ifac)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k1, l1, l2, na, kh, nf, ip, iw, ix2, ix3, ix4, ido, 
	    idl1;
    extern /* Subroutine */ int dradf2_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *), dradf3_(integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dradf4_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dradf5_(integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dradfg_(integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --ifac;
    --wa;
    --ch;
    --c__;

    /* Function Body */
    nf = ifac[2];
    na = 1;
    l2 = *n;
    iw = *n;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	kh = nf - k1;
	ip = ifac[kh + 3];
	l1 = l2 / ip;
	ido = *n / l2;
	idl1 = ido * l1;
	iw -= (ip - 1) * ido;
	na = 1 - na;
	if (ip != 4) {
	    goto L102;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	if (na != 0) {
	    goto L101;
	}
	dradf4_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3]);
	goto L110;
L101:
	dradf4_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3]);
	goto L110;
L102:
	if (ip != 2) {
	    goto L104;
	}
	if (na != 0) {
	    goto L103;
	}
	dradf2_(&ido, &l1, &c__[1], &ch[1], &wa[iw]);
	goto L110;
L103:
	dradf2_(&ido, &l1, &ch[1], &c__[1], &wa[iw]);
	goto L110;
L104:
	if (ip != 3) {
	    goto L106;
	}
	ix2 = iw + ido;
	if (na != 0) {
	    goto L105;
	}
	dradf3_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2]);
	goto L110;
L105:
	dradf3_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2]);
	goto L110;
L106:
	if (ip != 5) {
	    goto L108;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	ix4 = ix3 + ido;
	if (na != 0) {
	    goto L107;
	}
	dradf5_(&ido, &l1, &c__[1], &ch[1], &wa[iw], &wa[ix2], &wa[ix3], &wa[
		ix4]);
	goto L110;
L107:
	dradf5_(&ido, &l1, &ch[1], &c__[1], &wa[iw], &wa[ix2], &wa[ix3], &wa[
		ix4]);
	goto L110;
L108:
	if (ido == 1) {
	    na = 1 - na;
	}
	if (na != 0) {
	    goto L109;
	}
	dradfg_(&ido, &ip, &l1, &idl1, &c__[1], &c__[1], &c__[1], &ch[1], &ch[
		1], &wa[iw]);
	na = 1;
	goto L110;
L109:
	dradfg_(&ido, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__[1], &c__[
		1], &wa[iw]);
	na = 0;
L110:
	l2 = l1;
/* L111: */
    }
    if (na == 1) {
	return 0;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = ch[i__];
/* L112: */
    }
    return 0;
} /* dfftf1_ */

/* Subroutine */ int dfftf_(integer *n, doublereal *r__, doublereal *wsave)
{
    extern /* Subroutine */ int dfftf1_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;
    --r__;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    dfftf1_(n, &r__[1], &wsave[1], &wsave[*n + 1], (integer*)&wsave[(*n << 1) 
	    + 1]);
    return 0;
} /* dfftf_ */

/* Subroutine */ int dffti1_(integer *n, doublereal *wa, integer *ifac)
{
    /* Initialized data */

    static integer ntryh[4] = { 4,2,3,5 };

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, k1, l1, l2, ib;
    static doublereal fi;
    static integer ld, ii, nf, ip, nl, is, nq, nr;
    static doublereal arg;
    static integer ido, ipm;
    static doublereal tpi;
    static integer nfm1;
    static doublereal argh;
    static integer ntry;
    static doublereal argld;

    /* Parameter adjustments */
    --ifac;
    --wa;

    /* Function Body */
    nl = *n;
    nf = 0;
    j = 0;
L101:
    ++j;
    if (j - 4 <= 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    ntry = ntryh[j - 1];
    goto L104;
L103:
    ntry += 2;
L104:
    nq = nl / ntry;
    nr = nl - ntry * nq;
    if (nr != 0) {
	goto L101;
    } else {
	goto L105;
    }
L105:
    ++nf;
    ifac[nf + 2] = ntry;
    nl = nq;
    if (ntry != 2) {
	goto L107;
    }
    if (nf == 1) {
	goto L107;
    }
    i__1 = nf;
    for (i__ = 2; i__ <= i__1; ++i__) {
	ib = nf - i__ + 2;
	ifac[ib + 2] = ifac[ib + 1];
/* L106: */
    }
    ifac[3] = 2;
L107:
    if (nl != 1) {
	goto L104;
    }
    ifac[1] = *n;
    ifac[2] = nf;
    tpi = 6.2831853071795864769252867665590057;
    argh = tpi / (doublereal) (*n);
    is = 0;
    nfm1 = nf - 1;
    l1 = 1;
    if (nfm1 == 0) {
	return 0;
    }
    i__1 = nfm1;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = ifac[k1 + 2];
	ld = 0;
	l2 = l1 * ip;
	ido = *n / l2;
	ipm = ip - 1;
	i__2 = ipm;
	for (j = 1; j <= i__2; ++j) {
	    ld += l1;
	    i__ = is;
	    argld = (doublereal) ld * argh;
	    fi = 0.;
	    i__3 = ido;
	    for (ii = 3; ii <= i__3; ii += 2) {
		i__ += 2;
		fi += 1.;
		arg = fi * argld;
		wa[i__ - 1] = cos(arg);
		wa[i__] = sin(arg);
/* L108: */
	    }
	    is += ido;
/* L109: */
	}
	l1 = l2;
/* L110: */
    }
    return 0;
} /* dffti1_ */

/* Subroutine */ int dffti_(integer *n, doublereal *wsave)
{
    extern /* Subroutine */ int dffti1_(integer *, doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    dffti1_(n, &wsave[*n + 1], (integer*)&wsave[(*n << 1) + 1]);
    return 0;
} /* dffti_ */

/* Subroutine */ int dsinqb_(integer *n, doublereal *x, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k, kc, ns2;
    static doublereal xhold;
    extern /* Subroutine */ int dcosqb_(integer *, doublereal *, doublereal *)
	    ;

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    if (*n > 1) {
	goto L101;
    }
    x[1] *= 4.;
    return 0;
L101:
    ns2 = *n / 2;
    i__1 = *n;
    for (k = 2; k <= i__1; k += 2) {
	x[k] = -x[k];
/* L102: */
    }
    dcosqb_(n, &x[1], &wsave[1]);
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = *n - k;
	xhold = x[k];
	x[k] = x[kc + 1];
	x[kc + 1] = xhold;
/* L103: */
    }
    return 0;
} /* dsinqb_ */

/* Subroutine */ int dsinqf_(integer *n, doublereal *x, doublereal *wsave)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer k, kc, ns2;
    static doublereal xhold;
    extern /* Subroutine */ int dcosqf_(integer *, doublereal *, doublereal *)
	    ;

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    if (*n == 1) {
	return 0;
    }
    ns2 = *n / 2;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = *n - k;
	xhold = x[k];
	x[k] = x[kc + 1];
	x[kc + 1] = xhold;
/* L101: */
    }
    dcosqf_(n, &x[1], &wsave[1]);
    i__1 = *n;
    for (k = 2; k <= i__1; k += 2) {
	x[k] = -x[k];
/* L102: */
    }
    return 0;
} /* dsinqf_ */

/* Subroutine */ int dsinqi_(integer *n, doublereal *wsave)
{
    extern /* Subroutine */ int dcosqi_(integer *, doublereal *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    dcosqi_(n, &wsave[1]);
    return 0;
} /* dsinqi_ */

/* Subroutine */ int dsint1_(integer *n, doublereal *war, doublereal *was, 
	doublereal *xh, doublereal *x, integer *ifac)
{
    /* Initialized data */

    static doublereal sqrt3 = 1.7320508075688772935274463415058723;

    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k;
    static doublereal t1, t2;
    static integer kc, np1, ns2, modn;
    static doublereal xhold;
    extern /* Subroutine */ int dfftf1_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *);

    /* Parameter adjustments */
    --ifac;
    --x;
    --xh;
    --was;
    --war;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xh[i__] = war[i__];
	war[i__] = x[i__];
/* L100: */
    }
    if ((i__1 = *n - 2) < 0) {
	goto L101;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L101:
    xh[1] += xh[1];
    goto L106;
L102:
    xhold = sqrt3 * (xh[1] + xh[2]);
    xh[2] = sqrt3 * (xh[1] - xh[2]);
    xh[1] = xhold;
    goto L106;
L103:
    np1 = *n + 1;
    ns2 = *n / 2;
    x[1] = 0.;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = np1 - k;
	t1 = xh[k] - xh[kc];
	t2 = was[k] * (xh[k] + xh[kc]);
	x[k + 1] = t1 + t2;
	x[kc + 1] = t2 - t1;
/* L104: */
    }
    modn = *n % 2;
    if (modn != 0) {
	x[ns2 + 2] = xh[ns2 + 1] * 4.;
    }
    dfftf1_(&np1, &x[1], &xh[1], &war[1], &ifac[1]);
    xh[1] = x[1] * .5;
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	xh[i__ - 1] = -x[i__];
	xh[i__] = xh[i__ - 2] + x[i__ - 1];
/* L105: */
    }
    if (modn != 0) {
	goto L106;
    }
    xh[*n] = -x[*n + 1];
L106:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	x[i__] = war[i__];
	war[i__] = xh[i__];
/* L107: */
    }
    return 0;
} /* dsint1_ */

/* Subroutine */ int dsint_(integer *n, doublereal *x, doublereal *wsave)
{
    static integer np1, iw1, iw2, iw3;
    extern /* Subroutine */ int dsint1_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, integer *);

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    np1 = *n + 1;
    iw1 = *n / 2 + 1;
    iw2 = iw1 + np1;
    iw3 = iw2 + np1;
    dsint1_(n, &x[1], &wsave[1], &wsave[iw1], &wsave[iw2], (integer*)&wsave[
	    iw3]);
    return 0;
} /* dsint_ */

/* Subroutine */ int dsinti_(integer *n, doublereal *wsave)
{
    /* Initialized data */

    static doublereal pi = 3.1415926535897932384626433832795028;

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double sin(doublereal);

    /* Local variables */
    static integer k;
    static doublereal dt;
    static integer np1, ns2;
    extern /* Subroutine */ int dffti_(integer *, doublereal *);

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    if (*n <= 1) {
	return 0;
    }
    ns2 = *n / 2;
    np1 = *n + 1;
    dt = pi / (doublereal) np1;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	wsave[k] = sin(k * dt) * 2.;
/* L101: */
    }
    dffti_(&np1, &wsave[ns2 + 1]);
    return 0;
} /* dsinti_ */

