/*
    (C) 1995-97 AROS - The Amiga Replacement OS
    $Id$

    Desc: Get the name of the current program.
    Lang: english
*/
#include <proto/exec.h>
#include <dos/dos.h>
#include "dos_intern.h"

/*****************************************************************************

    NAME */
#include <proto/dos.h>

	AROS_LH2(BOOL, GetProgramName,

/*  SYNOPSIS */
	AROS_LHA(STRPTR, buf, D1),
	AROS_LHA(LONG  , len, D2),

/*  LOCATION */
	struct DosLibrary *, DOSBase, 96, Dos)

/*  FUNCTION
	Copies the name of the current program from the CLI structure
	into the buffer. If the buffer is too small the name is truncated,
	and a failure is returned. If the current process doesn't have
	a CLI structure, a 0 length string is put into the buffer and a
	failure is returned.

    INPUTS
	buf - Buffer for the name.
	len - Size of the buffer in bytes.

    RESULT
	!=0 on success, 0 on failure. IoErr() gives additional information
	in that case.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
	SetProgramName()

    INTERNALS

    HISTORY
	27-11-96    digulla automatically created from
			    dos_lib.fd and clib/dos_protos.h

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct DosLibrary *,DOSBase)

    struct Process *me = (struct Process *)FindTask(NULL);
    struct CommandLineInterface *cli = BADDR(me->pr_CLI);
    STRPTR cname;
    ULONG clen;
    BOOL ret = DOSTRUE;

    if (cli == NULL)
    {
	*buf = '\0';
	me->pr_Result2 = ERROR_OBJECT_WRONG_TYPE;
	return DOSFALSE;
    }

    cname = AROS_BSTR_ADDR(cli->cli_CommandName);
    clen = (ULONG)AROS_BSTR_strlen(cli->cli_CommandName);
    if (clen >= (len-1))
    {
	clen = len-1;
	me->pr_Result2 = ERROR_LINE_TOO_LONG;
	ret = DOSFALSE;
    }
    CopyMem(cname, buf, clen);
    buf[clen] = '\0';

    return ret;
    AROS_LIBFUNC_EXIT
} /* GetProgramName */
