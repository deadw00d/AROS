/*
    Copyright � 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: DOS function LoadSeg()
    Lang: english
*/

#include <dos/dos.h>
#include <dos/dosextens.h>
#include <proto/dos.h>
#include <aros/debug.h>
#include "dos_intern.h"

#ifdef AROS_MODULES_DEBUG
#include <exec/nodes.h>
#include <exec/lists.h>
#include <exec/memory.h>
#include <string.h>
struct MinList debug_seglist =
{
    (struct MinNode *)&debug_seglist.mlh_Tail,
    NULL,
    (struct MinNode *)&debug_seglist
};
#endif

/*****************************************************************************

    NAME */
#include <proto/dos.h>

        AROS_LH1(BPTR, LoadSeg,

/*  SYNOPSIS */
        AROS_LHA(STRPTR, name, D1),

/*  LOCATION */
        struct DosLibrary *, DOSBase, 25, Dos)

/*  FUNCTION
        Loads an executable file into memory. Each hunk of the loadfile
        is loaded into his own memory section and a handle on all of them
        is returned. The segments can be freed with UnLoadSeg().

    INPUTS
        name - NUL terminated name of the file.

    RESULT
        Handle to the loaded executable or 0 if the load failed.
q        IoErr() gives additional information in that case.

    NOTES
        This function is built on top of InternalLoadSeg()

    EXAMPLE

    BUGS

    SEE ALSO
        UnLoadSeg()

    INTERNALS

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct DosLibrary *,DOSBase)

    void (* FunctionArray[3])();
    BPTR file, segs=0;

    FunctionArray[0] = __AROS_GETVECADDR(DOSBase,7);
    FunctionArray[1] = __AROS_GETVECADDR(SysBase,33);
    FunctionArray[2] = __AROS_GETVECADDR(SysBase,35);

    /* Open the file */
    file = Open (name, FMF_READ);

    if (file)
    {
	D(bug("Loading \"%s\"...\n", name));

	segs = InternalLoadSeg (file, NULL, (void *)FunctionArray, NULL);

	if (segs)
        {
#if AROS_MODULES_DEBUG
            struct debug_segnode *segnode;
            segnode = AllocMem(sizeof(struct debug_segnode), MEMF_ANY);

            if (segnode)
            {
                NameFromFH(file, segnode->name, sizeof(segnode->name));

                segnode->seglist = segs;

                ADDTAIL(&debug_seglist, segnode);
            }
#endif

            SetIoErr (0);
        }
#if DEBUG > 1
	else
 	    bug ("Loading failed\n");
#endif
        Close(file);
    }

    /* And return */
    return segs;

    AROS_LIBFUNC_EXIT
} /* LoadSeg */
