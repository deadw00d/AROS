/* From: @(#)k_cos.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Copyright (c) 2008 Steven G. Kargl, David Schultz, Bruce D. Evans.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

//__FBSDID("$FreeBSD: src/lib/msun/ld80/k_cosl.c,v 1.1 2008/02/17 07:32:14 das Exp $");

/*
 * ld80 version of k_cos.c.  See ../src/k_cos.c for most comments.
 */

#include "math_private.h"

/*
 * Domain [-0.7854, 0.7854], range ~[-2.43e-23, 2.425e-23]:
 * |cos(x) - c(x)| < 2**-75.1
 *
 * The coefficients of c(x) were generated by a pari-gp script using
 * a Remez algorithm that searches for the best higher coefficients
 * after rounding leading coefficients to a specified precision.
 *
 * Simpler methods like Chebyshev or basic Remez barely suffice for
 * cos() in 64-bit precision, because we want the coefficient of x^2
 * to be precisely -0.5 so that multiplying by it is exact, and plain
 * rounding of the coefficients of a good polynomial approximation only
 * gives this up to about 64-bit precision.  Plain rounding also gives
 * a mediocre approximation for the coefficient of x^4, but a rounding
 * error of 0.5 ulps for this coefficient would only contribute ~0.01
 * ulps to the final error, so this is unimportant.  Rounding errors in
 * higher coefficients are even less important.
 *
 * In fact, coefficients above the x^4 one only need to have 53-bit
 * precision, and this is more efficient.  We get this optimization
 * almost for free from the complications needed to search for the best
 * higher coefficients.
 */
static const double
one = 1.0;

#if defined(__amd64__) || defined(__i386__)
/* Long double constants are slow on these arches, and broken on i386. */
static const volatile double
C1hi = 0.041666666666666664,		/*  0x15555555555555.0p-57 */
C1lo = 2.2598839032744733e-18;		/*  0x14d80000000000.0p-111 */
#define	C1	((long double)C1hi + C1lo)
#else
static const long double
C1 =  0.0416666666666666666136L;	/*  0xaaaaaaaaaaaaaa9b.0p-68 */
#endif

static const double
C2 = -0.0013888888888888874,		/* -0x16c16c16c16c10.0p-62 */
C3 =  0.000024801587301571716,		/*  0x1a01a01a018e22.0p-68 */
C4 = -0.00000027557319215507120,	/* -0x127e4fb7602f22.0p-74 */
C5 =  0.0000000020876754400407278,	/*  0x11eed8caaeccf1.0p-81 */
C6 = -1.1470297442401303e-11,		/* -0x19393412bd1529.0p-89 */
C7 =  4.7383039476436467e-14;		/*  0x1aac9d9af5c43e.0p-97 */

long double
__kernel_cosl(long double x, long double y)
{
	long double hz,z,r,w;

	z  = x*x;
	r  = z*(C1+z*(C2+z*(C3+z*(C4+z*(C5+z*(C6+z*C7))))));
	hz = 0.5*z;
	w  = one-hz;
	return w + (((one-w)-hz) + (z*r-x*y));
}
