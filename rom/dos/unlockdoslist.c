/*
    $Id$
    $Log$
    Revision 1.1  1996/07/28 16:37:23  digulla
    Initial revision

    Desc:
    Lang: english
*/
#include <dos/dosextens.h>
#include <clib/exec_protos.h>

/*****************************************************************************

    NAME */
	#include <clib/dos_protos.h>

	__AROS_LH1(void, UnLockDosList,

/*  SYNOPSIS */
	__AROS_LA(ULONG, flags, D1),

/*  LOCATION */
	struct DosLibrary *, DOSBase, 110, Dos)

/*  FUNCTION
	Frees a lock on the dos lists given by LockDosList().

    INPUTS
	flags - the same value as given to LockDosList().

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY
	29-10-95    digulla automatically created from
			    dos_lib.fd and clib/dos_protos.h

*****************************************************************************/
{
    __AROS_FUNC_INIT
    __AROS_BASE_EXT_DECL(struct DosLibrary *,DOSBase)
    ReleaseSemaphore(&DOSBase->dl_DosListLock);
    __AROS_FUNC_EXIT
} /* UnLockDosList */
