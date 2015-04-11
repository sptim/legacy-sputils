/******************************************************************************
* CALL spLoadFuncs                                                            *
* ToDo: CheckButton, CheckMenuItem, DeleteLboxItem, DismissDlg, FocusChange,  *
*       IsControlEnabled, IsMenuItemChecked, IsMenuItemEnabled,               *
*       IsMenuItemValid, IsWindow, IsWindowEnabled, IsWindowShowing,          *
*       IsWindowVisible, QueryDlgItemText, QueryDlgItemTextLength, QueryFocus *
*       QueryLboxCount, QueryLboxItemText, QueryLboxItemTextLength,           *
*       QueryLboxSelectedItem, QuerySessionTitle, QuerySwitchEntry,           *
*       QuerySysValue, SetLboxItemText
******************************************************************************/

static const char *MODULE = "SPUTILS/DOSAPI/CTRLPROG";
static const char *id = "$Id: ctrlprog.c,v 1.2 2004-02-15 03:09:43 spTim Exp $";

#define INCL_DOS
#define INCL_BASE
#define INCL_ERRORS
#define INCL_REXXSAA
#define INCL_DOSEXCEPTIONS
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"
#include "qsysinfval.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

/* DOS API Control Program Functions */
RexxFunctionHandler spQuerySysInfo;
RexxFunctionHandler spSleep;
RexxFunctionHandler spSendSignalException;
RexxFunctionHandler spKillProcess;

/******************************************************************************
* val=spQuerySysInfo(key)                                                     *
******************************************************************************/
RXFUNC(spQuerySysInfo) {
	ULONG val;
	ULONG qsv;
	char buffer[12];
	
	CHKPARAM(1,1);
	
	str2qsv(args[0].strptr);
	DosQuerySysInfo(qsv,qsv,&val,sizeof(val));
	BUILDRXSTRING(retstr,ultoa(val,buffer,10));
	return NO_ERROR;
}
/******************************************************************************
* CALL spSleep msec                                                           *
******************************************************************************/
RXFUNC(spSleep) {
	CHKPARAM(1,1);
	
	DosSleep(strtoul(args[0].strptr,NULL,10));
	BUILDRXSTRING(retstr,"");
	return NO_ERROR;
}
/******************************************************************************
* success=spSendSignalException(pid, exception)                               *
******************************************************************************/
RXFUNC(spSendSignalException) {
	PID pid;
	ULONG ulException;
	ULONG ulRC;
	char buffer[11];

	CHKPARAM(1,2);
	pid=strtoul(args[0].strptr,NULL,10);
	if(numargs==1 ||
			!stricmp("XCPT_SIGNAL_INTR",args[1].strptr) ||
			!stricmp("SIGNAL_INTR",args[1].strptr) ||
			!stricmp("INTRL",args[1].strptr) ||
			!stricmp("SIGINT",args[1].strptr) ||
			!stricmp("INT",args[1].strptr) ||
			!stricmp("CTRL-C",args[1].strptr) ||
			!stricmp("CTRLC",args[1].strptr) ||
			!stricmp("I",args[1].strptr) ||
			!stricmp("1",args[1].strptr)) {
		ulException=XCPT_SIGNAL_INTR;
	}
	else if(!stricmp("XCPT_SIGNAL_BREAK",args[2].strptr) ||
			!stricmp("SIGNAL_BREAK",args[2].strptr) ||
			!stricmp("BREAK",args[2].strptr) ||
			!stricmp("SIGBREAK",args[2].strptr) ||
			!stricmp("CTRL-BREAK",args[2].strptr) ||
			!stricmp("CTRLBREAK",args[2].strptr) ||
			!stricmp("B",args[2].strptr) ||
			!stricmp("4",args[2].strptr)) {
		ulException=XCPT_SIGNAL_BREAK;
	}
	else {
		return INVALID_ROUTINE;
	}
	if((ulRC=DosSendSignalException(pid,ulException))!=NO_ERROR) {;
		ultoa(ulRC,buffer,10);
		setRexxVar("RC",buffer);
		return 0;
	}
	return 1;
}

/******************************************************************************
* success=spKillProcess(action,pid)                                           *
* success=spKillProcess(pid)                                                  *
******************************************************************************/
RXFUNC(spKillProcess) {
	ULONG ulPid;
	ULONG ulAction;
	APIRET rc;
	
	CHKPARAM(1,2);
	
	if(numargs==1) {
		ulPid=strtoul(args[0].strptr,NULL,10);
		ulAction=DKP_PROCESS;
	}
	else {
		ulPid=strtoul(args[1].strptr,NULL,10);
		if(!args[0].strlength ||
				!stricmp(args[0].strptr,"DKP_PROCESS") ||
				!stricmp(args[0].strptr,"PROCESS") ||
				!stricmp(args[0].strptr,"P")) {
			ulAction=DKP_PROCESS;
		}
		else if(!stricmp(args[0].strptr,"DKP_PROCESSTREE") ||
				!stricmp(args[0].strptr,"PROCESSTREE") ||
				!stricmp(args[0].strptr,"T")) {
			ulAction=DKP_PROCESSTREE;
		}
		else {
			return INVALID_ROUTINE;
		}
	}
	
	ultoa(DosKillProcess(ulAction,ulPid),retstr->strptr,10);
	retstr->strlength=strlen(retstr->strptr);
	return NO_ERROR;
}

