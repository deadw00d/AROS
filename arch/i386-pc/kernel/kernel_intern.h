/*
    Copyright � 1995-2017, The AROS Development Team. All rights reserved.
    $Id$

    Desc: kernel_intern.h
    Lang: english
*/

#ifndef KERNEL_INTERN_H_
#define KERNEL_INTERN_H_

#include <asm/cpu.h>
#include <hardware/vbe.h>

#include <inttypes.h>

#include "apic.h"

#define STACK_SIZE 8192
#define PAGE_SIZE	0x1000
#define PAGE_MASK	0x0FFF

struct PlatformData
{
    struct List         kb_SysCallHandlers;
    APTR	     kb_APIC_TrampolineBase;
    struct ACPIData *kb_ACPI;
    struct APICData *kb_APIC;
    struct IOAPICData   *kb_IOAPIC;
};

#define IDT_SIZE                sizeof(long long) * 256
#define GDT_SIZE                sizeof(long long) * 8
#define TLS_SIZE                sizeof(struct tss)
#define TLS_ALIGN               64

#define krnSysCallCPUWake(data) 				        \
({								        \
    int __value;						        \
    __asm__ __volatile__ ("int $0x80":"=a"(__value):"a"(SC_X86CPUWAKE),"b"(data):"memory");	\
    __value;						                \
})

extern struct segment_desc *GDT;

void vesahack_Init(char *cmdline, struct vbe_mode *vmode);
void core_Unused_Int(void);
void core_Reboot(void);

/** CPU Functions **/
#if (0)
void core_SetupIDT(struct KernBootPrivate *, apicid_t, APTR);
void core_SetupGDT(struct KernBootPrivate *, apicid_t, APTR, APTR, APTR);
void core_SetupMMU(struct KernBootPrivate *, IPTR memtop);
#endif

void core_CPUSetup(apicid_t, APTR, IPTR);

void ictl_Initialize(struct KernelBase *KernelBase);

struct ExceptionContext;

/* Interrupt processing */
void core_LeaveInterrupt(struct ExceptionContext *);
void core_Supervisor(struct ExceptionContext *);

void PlatformPostInit(void);

#endif /* KERNEL_INTERN_H_ */
