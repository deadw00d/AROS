#ifndef __EXEC_PLATFORM_H
#define __EXEC_PLATFORM_H

#if defined(_WANT_UCONTEXT) && defined(_HAVE_CONFIG_H)
#include <aros/host-conf.h>
#endif

#define SCHEDQUANTUM_VALUE      4

#ifdef HOST_OS_android
/* Android is not a true Linux ;-) */
#undef HOST_OS_linux

#else

#ifndef _XOPEN_SOURCE
/* On Darwin this definition is required by ucontext.h (which is marked as deprecated) 
 * At least on Debian 8, _XOPEN_SOURCE is already defined in /usr/include/features.h 
 */
#define _XOPEN_SOURCE
#endif

#if defined(HAVE_UCONTEXT_H)
#define _HAVE_UCONTEXT
#include <ucontext.h>
#else
#if defined(HAVE_SYS_UCONTEXT_H)
#define _HAVE_UCONTEXT
#include <sys/ucontext.h>
#endif
#endif
#endif

#if defined(_WANT_UCONTEXT) && !defined(_HAVE_UCONTEXT)
#warning "missing support for ucontext_t on this platform"
#endif

#ifdef HOST_OS_linux
#define LIBC_NAME "libc.so.6"
#endif

#ifdef HOST_OS_darwin
#define LIBC_NAME "libSystem.dylib"
#endif

#ifndef LIBC_NAME
#define LIBC_NAME "libc.so"
#endif

struct LibCInterface
{
    void (*exit)(int status);
#ifdef HAVE_SWAPCONTEXT
    int  (*getcontext)(ucontext_t *ucp);
    void (*makecontext)(ucontext_t *ucp, void *func(), int argc, ...);
    int  (*swapcontext)(ucontext_t *oucp, ucontext_t *ucp);
#endif
};

struct Exec_PlatformData
{
    APTR HostLibBase;
    struct LibCInterface *SysIFace;
};

#ifndef __AROS_KERNEL__
#define HostLibBase PD(SysBase).HostLibBase
#endif

#if defined(__AROSPLATFORM_SMP__)

/*
 * __AROSPLATFORM_SMP__     changes externally exposed data structures for SMP
 * __AROSEXEC_SMP__         enables SMP in Exec
 */

#if defined(__AROSEXEC_SMP__)

/* NO IMPLEMENTED */

#else /* __AROSEXEC_SMP__ */

#ifdef AROS_NO_ATOMIC_OPERATIONS
#define IDNESTCOUNT_INC                 SysBase->Private4++
#define IDNESTCOUNT_DEC                 SysBase->Private4--
#define TDNESTCOUNT_INC                 SysBase->Private5++
#define TDNESTCOUNT_DEC                 SysBase->Private5--
#define FLAG_SCHEDQUANTUM_CLEAR         SysBase->SysFlags &= ~SFF_QuantumOver
#define FLAG_SCHEDQUANTUM_SET           SysBase->SysFlags |= SFF_QuantumOver
#define FLAG_SCHEDSWITCH_CLEAR          SysBase->AttnResched &= ~ARF_AttnSwitch
#define FLAG_SCHEDSWITCH_SET            SysBase->AttnResched |= ARF_AttnSwitch
#define FLAG_SCHEDDISPATCH_CLEAR        SysBase->AttnResched &= ~ARF_AttnDispatch
#define FLAG_SCHEDDISPATCH_SET          SysBase->AttnResched |= ARF_AttnDispatch
#else
#define IDNESTCOUNT_INC                 AROS_ATOMIC_INC(SysBase->Private4)
#define IDNESTCOUNT_DEC                 AROS_ATOMIC_DEC(SysBase->Private4)
#define TDNESTCOUNT_INC                 AROS_ATOMIC_INC(SysBase->Private5)
#define TDNESTCOUNT_DEC                 AROS_ATOMIC_DEC(SysBase->Private5)
#define FLAG_SCHEDQUANTUM_CLEAR         AROS_ATOMIC_AND(SysBase->SysFlags, ~SFF_QuantumOver)
#define FLAG_SCHEDQUANTUM_SET           AROS_ATOMIC_OR(SysBase->SysFlags, SFF_QuantumOver)
#define FLAG_SCHEDSWITCH_CLEAR          AROS_ATOMIC_AND(SysBase->AttnResched, ~ARF_AttnSwitch)
#define FLAG_SCHEDSWITCH_SET            AROS_ATOMIC_OR(SysBase->AttnResched, ARF_AttnSwitch)
#define FLAG_SCHEDDISPATCH_CLEAR        AROS_ATOMIC_AND(SysBase->AttnResched, ~ARF_AttnDispatch)
#define FLAG_SCHEDDISPATCH_SET          AROS_ATOMIC_OR(SysBase->AttnResched, ARF_AttnDispatch)
#endif
#define SCHEDQUANTUM_SET(val)           (SysBase->Private2=(val))
#define SCHEDQUANTUM_GET                (SysBase->Private2)
#define SCHEDELAPSED_SET(val)           (SysBase->Private3=(val))
#define SCHEDELAPSED_GET                (SysBase->Private3)
#define IDNESTCOUNT_GET                 (SysBase->Private4)
#define IDNESTCOUNT_SET(val)            (SysBase->Private4=(val))
#define TDNESTCOUNT_GET                 (SysBase->Private5)
#define TDNESTCOUNT_SET(val)            (SysBase->Private5=(val))
#define FLAG_SCHEDQUANTUM_ISSET         (SysBase->SysFlags & SFF_QuantumOver)
#define FLAG_SCHEDSWITCH_ISSET          (SysBase->AttnResched & ARF_AttnSwitch)
#define FLAG_SCHEDDISPATCH_ISSET        (SysBase->AttnResched & ARF_AttnDispatch)

#define GET_THIS_TASK                   ((struct Task *)(SysBase->Private1))
#define SET_THIS_TASK(x)                (SysBase->Private1=(IPTR)(x))

#endif

#else /* __AROSPLATFORM_SMP__ */

#ifdef AROS_NO_ATOMIC_OPERATIONS
#define IDNESTCOUNT_INC                 SysBase->IDNestCnt++
#define IDNESTCOUNT_DEC                 SysBase->IDNestCnt--
#define TDNESTCOUNT_INC                 SysBase->TDNestCnt++
#define TDNESTCOUNT_DEC                 SysBase->TDNestCnt--
#define FLAG_SCHEDQUANTUM_CLEAR         SysBase->SysFlags &= ~SFF_QuantumOver
#define FLAG_SCHEDQUANTUM_SET           SysBase->SysFlags |= SFF_QuantumOver
#define FLAG_SCHEDSWITCH_CLEAR          SysBase->AttnResched &= ~ARF_AttnSwitch
#define FLAG_SCHEDSWITCH_SET            SysBase->AttnResched |= ARF_AttnSwitch
#define FLAG_SCHEDDISPATCH_CLEAR        SysBase->AttnResched &= ~ARF_AttnDispatch
#define FLAG_SCHEDDISPATCH_SET          SysBase->AttnResched |= ARF_AttnDispatch
#else
#define IDNESTCOUNT_INC                 AROS_ATOMIC_INC(SysBase->IDNestCnt)
#define IDNESTCOUNT_DEC                 AROS_ATOMIC_DEC(SysBase->IDNestCnt)
#define TDNESTCOUNT_INC                 AROS_ATOMIC_INC(SysBase->TDNestCnt)
#define TDNESTCOUNT_DEC                 AROS_ATOMIC_DEC(SysBase->TDNestCnt)
#define FLAG_SCHEDQUANTUM_CLEAR         AROS_ATOMIC_AND(SysBase->SysFlags, ~SFF_QuantumOver)
#define FLAG_SCHEDQUANTUM_SET           AROS_ATOMIC_OR(SysBase->SysFlags, SFF_QuantumOver)
#define FLAG_SCHEDSWITCH_CLEAR          AROS_ATOMIC_AND(SysBase->AttnResched, ~ARF_AttnSwitch)
#define FLAG_SCHEDSWITCH_SET            AROS_ATOMIC_OR(SysBase->AttnResched, ARF_AttnSwitch)
#define FLAG_SCHEDDISPATCH_CLEAR        AROS_ATOMIC_AND(SysBase->AttnResched, ~ARF_AttnDispatch)
#define FLAG_SCHEDDISPATCH_SET          AROS_ATOMIC_OR(SysBase->AttnResched, ARF_AttnDispatch)
#endif
#define SCHEDQUANTUM_SET(val)           (SysBase->Quantum=(val))
#define SCHEDQUANTUM_GET                (SysBase->Quantum)
#define SCHEDELAPSED_SET(val)           (SysBase->Elapsed=(val))
#define SCHEDELAPSED_GET                (SysBase->Elapsed)
#define IDNESTCOUNT_GET                 (SysBase->IDNestCnt)
#define IDNESTCOUNT_SET(val)            (SysBase->IDNestCnt=(val))
#define TDNESTCOUNT_GET                 (SysBase->TDNestCnt)
#define TDNESTCOUNT_SET(val)            (SysBase->TDNestCnt=(val))
#define FLAG_SCHEDQUANTUM_ISSET         (SysBase->SysFlags & SFF_QuantumOver)
#define FLAG_SCHEDSWITCH_ISSET          (SysBase->AttnResched & ARF_AttnSwitch)
#define FLAG_SCHEDDISPATCH_ISSET        (SysBase->AttnResched & ARF_AttnDispatch)

#define GET_THIS_TASK                   (SysBase->ThisTask)
#define SET_THIS_TASK(x)                (SysBase->ThisTask=(x))

#endif

#endif /* __EXEC_PLATFORM_H */
