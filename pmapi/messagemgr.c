/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: pmapi/winmessagemgr.c                                               *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/PMAPI/WINMESSAGEMGR";
static const char *id = "$Id: messagemgr.c,v 1.2 2004-02-15 03:36:29 spTim Exp $";

#define INCL_WIN
#define INCL_NLS
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"
#include "messages.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

RexxFunctionHandler spPostMsg;

/******************************************************************************
* rc=spPostMsg(hwnd,msg,mp1,mp2)                                              *
******************************************************************************/
RXFUNC(spPostMsg) {
	HWND hwnd;
	ULONG	msg,
			ulmp1,
			ulmp2;
	
	CHKPARAM(2,4);
	
	hwnd=(args[0].strlength==0)?(HWND)NULL:(HWND)strtoul(args[0].strptr,NULL,10);
	if(hwnd==(HWND)0) hwnd=(HWND)NULL;
	if(numargs>2 && args[2].strlength!=0) {
		ulmp1=(ULONG)strtoul(args[2].strptr,NULL,10);
	}
	else {
		ulmp1=0;
	}
	if(numargs>3 && args[3].strlength!=0) {
		ulmp2=(ULONG)strtoul(args[3].strptr,NULL,10);
	}
	else {
		ulmp2=0;
	}
	
	str2msg(args[1].strptr);
	
	BUILDRXSTRING(retstr,(WinPostMsg(hwnd,msg,(MPARAM)ulmp1,(MPARAM)ulmp2))?"1":"0");
	
	RXRET;
}


