/*
    (C) 1995-96 AROS - The Amiga Replacement OS
    $Id$
    $Log$
    Revision 1.1  1996/11/14 08:53:30  aros
    First attempt for a real fastfilesystem
    (only directoryscans for now)


    Desc:
    Lang:
*/
#ifndef RAM_HANDLER_GCC_H
#define RAM_HANDLER_GCC_H
#include <aros/libcall.h>
#include <exec/execbase.h>
#include <exec/io.h>
#include <exec/devices.h>
#include <dos/dos.h>
#include <dos/filesystem.h>

struct ffsbase
{
    struct Device device;
    struct ExecBase *sysbase;
    struct DosLibrary *dosbase;
    BPTR seglist;
    ULONG unitcount;
    struct MsgPort port;		/* Port to put IORequests to */
    struct MsgPort dport;		/* reply port for exec device */
    struct SignalSemaphore sigsem;	/* Semaphore for rport */
    struct MsgPort rport;		/* Message reply port */
    struct IOFileSys *iofs;		/* IORequest to be aborted or NULL */
    LONG dlflag;			/* have to change dos list */
    struct MinList inserted;
    struct MinList mounted;
    struct MinList removed;
};

#define expunge() \
AROS_LC0(BPTR, expunge, struct rambase *, ffsbase, 3, ffs)

#define SysBase ffsbase->sysbase
#define DOSBase ffsbase->dosbase

#endif

