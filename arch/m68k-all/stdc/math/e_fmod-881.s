#    Copyright � 2018, The AROS Development Team. All rights reserved.
#    $Id$
#include "aros/m68k/asm.h"

	.text
	.balign 4
	.globl	AROS_SLIB_ENTRY(fmod881, STDC, 166)
	_FUNCTION(AROS_SLIB_ENTRY(fmod881, STDC, 166))
	
AROS_SLIB_ENTRY(fmod881, STDC, 166):
	fmove.s	%d0,%fp0
	fmove.s	%d1,%fp1
	fmod.x	%fp1,%fp0
	fmove.s	%fp0,%d0
	rts
