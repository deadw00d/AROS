#include <dos/bptr.h>
#include <proto/exec.h>

#include <kernel_base.h>
#include <kernel_tagitems.h>

#include "debug_intern.h"

/*****************************************************************************

    NAME */
#include <proto/kernel.h>

AROS_LH2(int, KrnDecodeLocationA,

/*  SYNOPSIS */
	AROS_LHA(void *, addr, A0),
	AROS_LHA(struct TagItem *, tags, A1),

/*  LOCATION */
	struct KernelBase *, KernelBase, 24, Kernel)

/*  FUNCTION
	Locate the given address in the list of registered modules and return
	information about it.

    INPUTS
	addr - An address to resolve
	tags - An optional taglist. ti_Tag can be one of the following tags and
	       ti_Data is always a pointer to a storage of specified type.
	       Resulting values will be placed into specified locations if the
	       function succeeds.

	    KDL_ModuleName     (char *) - Module name
	    KDL_SegmentName    (char *) - Segment name. Can be NULL if there were
					  no segment names provided for the module.
	    KDL_SegmentPointer (BPTR)   - DOS pointer to the corresponding segment.
					  Note that it will be different from
					  KDL_SegmentStart value
	    
	    KDL_SegmentNumber  (unsigned int) - Order number of the segment in the
						module
	    KDL_SegmentStart   (void *) - Start address of actual segment contents
					  in memory.
	    KDL_SegmentEnd     (void *) - End address of actual segment contents
					  in memory.
	
	    The following tags may return NULL values if there was no corresponding
	    information provided for the module:

	    KDL_SymbolName     (char *) - Symbol name (function or variable name)
	    KDL_SymbolStart    (void *) - Start address of contents described by this
					  symbol.
	    KDL_SymbolEnd      (void *) - End address of contents described by this
					  symbol.

    RESULT
	Zero if lookup failed and no corresponding module found, nonzero
	otherwise.

    NOTES
	If the function fails values pointed to by taglist will not be changed.

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

******************************************************************************/
{
    AROS_LIBFUNC_INIT

    struct segment *seg;
    symbol_t *sym;
    void *dummy;
    char **module   = &dummy;
    char **segment  = &dummy;
    char **function = &dummy;
    void **secstart = &dummy;
    void **secend   = &dummy;
    void **funstart = &dummy;
    void **funend   = &dummy;
    BPTR  *secptr   = &dummy;
    unsigned int *secnum = &dummy;
    struct TagItem *tstate = tags;
    struct TagItem *tag;
    BOOL want_sym = FALSE;
    int ret = 0;

    /* Parse TagList */
    while ((tag = krnNextTagItem(&tstate)))
    {
	switch (tag->ti_Tag) {
	case KDL_ModuleName:
	    module = (char **)tag->ti_Data;
	    break;

	case KDL_SegmentName:
	    segment = (char **)tag->ti_Data;
	    break;

	case KDL_SegmentPointer:
	    secptr = (BPTR *)tag->ti_Data;
	    break;

	case KDL_SegmentNumber:
	    secnum = (unsigned int *)tag->ti_Data;
	    break;

	case KDL_SegmentStart:
	    secstart = (void **)tag->ti_Data;
	    break;

	case KDL_SegmentEnd:
	    secend = (void **)tag->ti_Data;
	    break;

	case KDL_SymbolName:
	    function = (char **)tag->ti_Data;
	    want_sym = TRUE;
	    break;

	case KDL_SymbolStart:
	    funstart = (void **)tag->ti_Data;
	    want_sym = TRUE;
	    break;

	case KDL_SymbolEnd:
	    funend = (void **)tag->ti_Data;
	    want_sym = TRUE;
	    break;
	}
    }

    /* We can be called in supervisor mode. No semaphores in the case! */
    if (!KrnIsSuper())
	ObtainSemaphoreShared(&KernelBase->kb_ModSem);

    ForeachNode(&KernelBase->kb_Modules, seg)
    {
	/* if address suits the segment bounds, you got it */
	if ((seg->s_lowest <= addr) && (seg->s_highest >= addr))
	{
	    *module = seg->s_mod->m_name;

	    *segment  = seg->s_name;
	    *secptr   = seg->s_seg;
	    *secnum   = seg->s_num;
	    *secstart = seg->s_lowest;
	    *secend   = seg->s_highest;

	    /* Now look up the function if requested */
	    if (want_sym) {
		ForeachNode(&seg->s_mod->m_symbols, sym)
		{
		    if (sym->s_lowest <= addr && sym->s_highest >= addr) {
			*function = sym->s_name;
			*funstart = sym->s_lowest;
			*funend   = sym->s_highest;

			break;
		    }
		}

		if (!sym->s_node.mln_Succ)
		{
		    /* Indicate that symbol not found */
		    *function = NULL;
		    *funstart = NULL;
		    *funend   = NULL;
		}

	    }
	    ret = 1;
	    break;
	}
    }

    if (!KrnIsSuper())
	ReleaseSemaphore(&KernelBase->kb_ModSem);

    return ret;

    AROS_LIBFUNC_EXIT
}
