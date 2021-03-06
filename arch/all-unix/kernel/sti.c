/*
    Copyright (C) 1995-2021, The AROS Development Team. All rights reserved.
*/

#include <aros/libcall.h>

#include "kernel_base.h"
#include "kernel_intern.h"
#include "kernel_unix.h"

#include <signal.h>

AROS_LH0(void, KrnSti,
          struct KernelBase *, KernelBase, 10, Kernel)
{
    AROS_LIBFUNC_INIT

    if (!UKB(KernelBase)->SupervisorCount)
    {
        if (KernelBase->kb_PlatformData->iface)
        {
            KernelBase->kb_PlatformData->iface->sigprocmask(SIG_UNBLOCK, &KernelBase->kb_PlatformData->sig_int_mask, NULL);
            AROS_HOST_BARRIER
        }
    }

    AROS_LIBFUNC_EXIT
}
