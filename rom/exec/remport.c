/*
    $Id$
    $Log$
    Revision 1.1  1996/07/28 16:37:24  digulla
    Initial revision

    Desc:
    Lang: english
*/
#include <exec/ports.h>
#include <exec/execbase.h>
#include <aros/libcall.h>

/*****************************************************************************

    NAME */
	#include <clib/exec_protos.h>

	__AROS_LH1(void, RemPort,

/*  SYNOPSIS */
	__AROS_LA(struct MsgPort *, port, A1),

/*  LOCATION */
	struct ExecBase *, SysBase, 60, Exec)

/*  FUNCTION
	Remove a public port from the public port list to make it private
	again. Any further attempts to find this port in the public port
	list will fail.

    INPUTS
	port - Pointer to messageport structure.

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY

******************************************************************************/
{
    __AROS_FUNC_INIT

    /* Arbitrate for the list of message ports.*/
    Forbid();

    /* Remove the current port. */
    Remove(&port->mp_Node);

    /* All done. */
    Permit();
    __AROS_FUNC_EXIT
} /* RemPort */



