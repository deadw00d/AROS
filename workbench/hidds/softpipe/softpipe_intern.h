#ifndef _SOFTPIPE_INTERN_H
#define _SOFTPIPE_INTERN_H

/*
    Copyright 2010-2021, The AROS Development Team. All rights reserved.
    $Id$
*/

#include <stdint.h>
#include <stdbool.h>

#include "pipe/p_shader_tokens.h"
#if defined(TGSI_SEMANTIC_VIEWPORT_MASK)
#include "frontend/sw_winsys.h"
#else
#include "state_tracker/sw_winsys.h"
#endif

#include LC_LIBDEFS_FILE

#define CLID_Hidd_Gallium_Softpipe  "hidd.gallium.softpipe"

// The object instance data is used as our winsys wrapper
struct HiddGalliumSoftpipeData
{
    struct sw_winsys softpipe_winsys;
    OOP_Object *softpipe_obj;
};

struct softpipestaticdata 
{
    OOP_Class       *galliumclass;
    OOP_AttrBase    hiddGalliumAB;
    struct Library  *CyberGfxBase;
    struct Library  *UtilityBase;
};

LIBBASETYPE 
{
    struct Library              LibNode;
    struct softpipestaticdata   sd;
};

#define METHOD(base, id, name) \
  base ## __ ## id ## __ ## name (OOP_Class *cl, OOP_Object *o, struct p ## id ## _ ## name *msg)

#define BASE(lib) ((LIBBASETYPEPTR)(lib))

#define SD(cl) (&BASE(cl->UserData)->sd)

#endif
