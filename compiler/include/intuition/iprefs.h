#ifndef INTUITION_IPREFS_H
#define INTUITION_IPREFS_H

/*
    Copyright � 1995-2010, The AROS Development Team. All rights reserved.
    $Id$

    Desc: PRIVATE/TOP SECRET!!! Communication between IPrefs program and Intuition
    Lang: english
*/

#ifndef EXEC_TYPES_H
#   include <exec/types.h>
#endif

#define IPREFS_TYPE_ICONTROL   0
#define IPREFS_TYPE_SCREENMODE 1
#define IPREFS_TYPE_POINTER    2

struct IScreenModePrefs
{
    ULONG smp_DisplayID;
    UWORD smp_Width;
    UWORD smp_Height;
    UWORD smp_Depth;
    UWORD smp_Control;
};

struct IIControlPrefs
{
    UWORD ic_TimeOut;
    WORD  ic_MetaDrag;
    ULONG ic_Flags;
    UBYTE ic_WBtoFront;
    UBYTE ic_FrontToBack;
    UBYTE ic_ReqTrue;
    UBYTE ic_ReqFalse;
};

struct IPointerPrefs
{
    struct BitMap *BitMap;
    WORD  XOffset;
    WORD  YOffset;
    UWORD BytesPerRow;
    UWORD Size;
    UWORD YSize;
    UWORD Which;
    ULONG Zero;
};

#endif /* GRAPHICS_SCREENS_H */
