/*
    (C) 1995-2000 AROS - The Amiga Research OS
    $Id$

    Desc: Create a new OOP object
    Lang: english
*/
#define AROS_ALMOST_COMPATIBLE
#include <exec/lists.h>
#include <proto/exec.h>
#include "intern.h"
#define MD(x) ((struct metadata *)x)

/*****************************************************************************

    NAME */
#include <proto/oop.h>

	AROS_LH1(VOID, OOP_DisposeObject,

/*  SYNOPSIS */
	AROS_LHA(OOP_Object  *, obj, A0),

/*  LOCATION */
	struct Library *, OOPBase, 10, OOP)

/*  FUNCTION
	Delete an object that was previously allocated with NewObjectA().

    INPUTS
    	obj	- pointer to object to dispose.

    RESULT
    	None.

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
    	OOP_NewObjectA()

    INTERNALS

    HISTORY
	29-10-95    digulla automatically created from
			    intuition_lib.fd and clib/intuition_protos.h

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct Library*,OOPBase)
    
    ULONG mid = OOP_GetMethodID(IID_Root, moRoot_Dispose);
    
    EnterFunc(bug("OOP_DisposeObject(classID=%s)\n",
    		OCLASS(obj)->ClassNode.ln_Name));
		
    if (obj == NULL) return;



    MD(OOP_OCLASS(obj))->objectcount --;
		

    OOP_DoMethod(obj, (OOP_Msg)&mid);

        
    ReturnVoid("OOP_DisposeObject");
    
    AROS_LIBFUNC_EXIT
} /* OOP_DisposeObject */
