/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: pmapi/winsys.c                                                      *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/PMAPI/WINSYS";
static const char *id = "$Id: sys.c,v 1.2 2004-02-15 03:46:16 spTim Exp $";

#define INCL_WINSYS
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"
#include "sysvals.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

RexxFunctionHandler spQuerySysValue;

/******************************************************************************
* value=spQuerySysValue( hwnd, key )                                          *
******************************************************************************/
RXFUNC(spQuerySysValue) {
	char buffer[12];
	HWND hwnd;
	ULONG sysval;
	
	CHKPARAM(1,2);
	
	if(numargs==1) {
		hwnd=HWND_DESKTOP;
		str2sysval(args[0].strptr);
	}
	else if(!RXVALIDSTRING(args[0]) || args[0].strlength<=0) {
		hwnd=HWND_DESKTOP;
		str2sysval(args[1].strptr);
	}
	else {
		hwnd=(HWND)strtoul(args[0].strptr,NULL,10);
		str2sysval(args[1].strptr);
	}
	
	ultoa((ULONG)WinQuerySysValue(hwnd,sysval),buffer,10);
	BUILDRXSTRING(retstr,buffer);
	
	RXRET;
}



