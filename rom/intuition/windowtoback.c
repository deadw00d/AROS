/*
    (C) 1995-96 AROS - The Amiga Replacement OS
    $Id$
    $Log$
    Revision 1.1  1996/10/21 17:56:23  aros
    Two new functions


    Desc:
    Lang: english
*/
#include "intuition_intern.h"

extern void intui_WindowToBack (struct Window * window);

/*****************************************************************************

    NAME */
	#include <intuition/intuition.h>
	#include <clib/intuition_protos.h>

	__AROS_LH1(void, WindowToBack,

/*  SYNOPSIS */
	__AROS_LHA(struct Window *, window, A0),

/*  LOCATION */
	struct IntuitionBase *, IntuitionBase, 51, Intuition)

/*  FUNCTION
	Bring a window to the back (ie. behind any other window).

    INPUTS
	window - Which window

    RESULT
	None.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY
	29-10-95    digulla automatically created from
			    intuition_lib.fd and clib/intuition_protos.h

*****************************************************************************/
{
    __AROS_FUNC_INIT
    __AROS_BASE_EXT_DECL(struct IntuitionBase *,IntuitionBase)

    intui_WindowToBack (window);

    __AROS_FUNC_EXIT
} /* WindowToBack */
