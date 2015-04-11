/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: pmapi/wininput.c                                                    *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/PMAPI/WININPUT";
static const char *id = "$Id: input.c,v 1.2 2004-02-15 03:30:14 spTim Exp $";

#define INCL_WININPUT
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

RexxFunctionHandler spQueryCapture;
RexxFunctionHandler spQueryFocus;
RexxFunctionHandler spSetFocus;

/******************************************************************************
* hwnd=spQueryCapture(hwnd)                                                   *
******************************************************************************/
RXFUNC(spQueryCapture) {
	char buffer[12];

	CHKPARAM(0,1);

	ultoa((ULONG)WinQueryCapture(
			((numargs==0)?HWND_DESKTOP:(HWND)strtoul(args[0].strptr,NULL,10))),
			buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* length=spQueryFocus(hwnd)                                                   *
******************************************************************************/
RXFUNC(spQueryFocus) {
	char buffer[12];

	CHKPARAM(0,1);

	ultoa((ULONG)WinQueryFocus(
			(numargs==0?HWND_DESKTOP:(HWND)strtoul(args[0].strptr,NULL,10))),
			buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* CALL spSetFocus([[hwndDesktop],]hwnd)                                         *
******************************************************************************/
RXFUNC(spSetFocus) {
	CHKPARAM(1,2);

	if(numargs==1) {
		BUILDRXSTRING(retstr,(WinSetFocus(
				HWND_DESKTOP,strtoul(args[0].strptr,NULL,10)))?"1":"0");
	}
	else {
		BUILDRXSTRING(retstr,(WinSetFocus((args[0].strlength==0)?HWND_DESKTOP:
				strtoul(args[0].strptr,NULL,10),
				strtoul(args[1].strptr,NULL,10)))?"1":"0");
	}

	RXRET;
}
