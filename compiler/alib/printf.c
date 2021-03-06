/*
    Copyright (C) 1995-2017, The AROS Development Team. All rights reserved.

    Desc: Varargs stub for Printf()
*/

#include <dos/dos.h>
#include <proto/dos.h>

LONG Printf(STRPTR fmt, ...) __stackparm;

LONG Printf(STRPTR fmt, ...)
{
    LONG retval;

    AROS_SLOWSTACKFORMAT_PRE(fmt);
    retval = VFPrintf(Output(), fmt, AROS_SLOWSTACKFORMAT_ARG(fmt));
    AROS_SLOWSTACKFORMAT_POST(fmt);

    return retval;
}
