#include <aros/libcall.h>
#include "machine.h"
#include "memory.h"

/*****************************************************************************

    NAME */
	#include <clib/exec_protos.h>

	__AROS_LH2(APTR, AllocVec,

/*  SYNOPSIS */
	__AROS_LA(ULONG, byteSize,     D0),
	__AROS_LA(ULONG, requirements, D1),

/*  LOCATION */
	struct ExecBase *, SysBase, 114, Exec)

/*  FUNCTION
	Allocate some memory from the sytem memory pool with the given
	requirements and without the need to memorize the actual size
	of the block.

    INPUTS
	byteSize     - Number of bytes you want to get
	requirements - Type of memory

    RESULT
	A pointer to the number of bytes you wanted or NULL if the memory
	couldn't be allocated

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
	FreeVec()

    INTERNALS

    HISTORY
	8-10-95    created by m. fleischer
       16-10-95    increased portability

******************************************************************************/
{
    __AROS_FUNC_INIT

    UBYTE *ret;

    /* Add room for stored size. */
    byteSize+=ALLOCVEC_TOTAL;

    /* Get the memory. */
    ret=(UBYTE *)AllocMem(byteSize,requirements);

    /* If there's not enough memory left return immediately. */
    if(ret==NULL)
	return NULL;

    /* Store size */
    *(ULONG *)ret=byteSize;

    /* return free space */
    return ret+ALLOCVEC_TOTAL;
    __AROS_FUNC_EXIT
} /* AllocVec */

