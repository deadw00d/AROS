/*
    $Id$
    $Log$
    Revision 1.1  1996/07/28 16:37:24  digulla
    Initial revision

    Desc:
    Lang: english
*/
#include <exec/execbase.h>
#include <aros/libcall.h>

/*****************************************************************************

    NAME */
	#include <clib/exec_protos.h>

	__AROS_LH2(void, Signal,

/*  SYNOPSIS */
	__AROS_LA(struct Task *,     task,      A1),
	__AROS_LA(ULONG,             signalSet, D0),

/*  LOCATION */
	struct ExecBase *, SysBase, 54, Exec)

/*  FUNCTION
	Send some signals to a given task. If the task is currently waiting
	on these signals, has a higher priority as the current one and if
	taskswitches are allowed the new task begins to run immediately.

    INPUTS
	task	  - Pointer to task structure.
	signalSet - The set of signals to send to the task.

    RESULT

    NOTES
	This function may be used from interrupts.

    EXAMPLE

    BUGS

    SEE ALSO
	AllocSignal(), FreeSignal(), Wait(), SetSignal(), SetExcept()

    INTERNALS

    HISTORY

******************************************************************************/
{
    __AROS_FUNC_INIT

    /* Protect the task lists against other tasks that may use Signal(). */
    Disable();

    /* Set the signals in the task structure. */
    task->tc_SigRecvd|=signalSet;

    /* Do those bits raise exceptions? */
    if(task->tc_SigExcept&task->tc_SigRecvd)
    {
	/* Yes. Set the exception flag. */
	task->tc_Flags|=TF_EXCEPT;

	/* task is running? Raise the exception or defer it for later. */
	if(task->tc_State==TS_RUN)
	{
	    /* Are taskswitches allowed? (Don't count own Disable() here) */
	    if(SysBase->TDNestCnt>=0||SysBase->IDNestCnt>0)
		/* No. Store it for later. */
		SysBase->AttnResched|=0x80;
	    else
	    {
		/* Switches are allowed. Move the current task away. */
		SysBase->ThisTask->tc_State=TS_READY;
		Enqueue(&SysBase->TaskReady,&SysBase->ThisTask->tc_Node);

		/* And force a rescedule. */
		Switch();
	    }

	    /* All done. */
	    Enable();
	    return;
	}
    }

    /*
	Is the task receiving the signals waiting on them
	(or on a exception) ?
    */
    if(task->tc_State==TS_WAIT&&
       (task->tc_SigRecvd&(task->tc_SigWait|task->tc_SigExcept)))
    {
	/* Yes. Move him to the ready list. */
	task->tc_State=TS_READY;
	Remove(&task->tc_Node);
	Enqueue(&SysBase->TaskReady,&task->tc_Node);

	/* Has it a higher priority as the current one? */
	if(task->tc_Node.ln_Pri>SysBase->ThisTask->tc_Node.ln_Pri)
	    /*
		Yes. A taskswitch is necessary. Prepare one if possible.
		(If the current task is not running it is already moved)
	    */
	    if(SysBase->ThisTask->tc_State==TS_RUN)
	    {
		/* Are taskswitches allowed? */
		if(SysBase->TDNestCnt>=0||SysBase->IDNestCnt>0)
		    /* No. Store it for later. */
		    SysBase->AttnResched|=0x80;
		else
		{
		    /* Switches are allowed. Move the current task away. */
		    SysBase->ThisTask->tc_State=TS_READY;
		    Enqueue(&SysBase->TaskReady,&SysBase->ThisTask->tc_Node);

		    /* And force a rescedule. */
		    Switch();
		}
	    }
    }

    Enable();
    __AROS_FUNC_EXIT
}

