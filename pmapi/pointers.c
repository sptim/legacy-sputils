/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: pmapi/winpointers.c                                                 *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/PMAPI/WINPOINTERS";
static const char *id = "$Id: pointers.c,v 1.2 2004-02-15 03:43:44 spTim Exp $";

#define INCL_WINPOINTERS
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

RexxFunctionHandler spSetPointerPos;
RexxFunctionHandler spQueryPointerPos;

/******************************************************************************
* hwnd=spSetPointerPos(hwnd,x,y)                                              *
******************************************************************************/
RXFUNC(spSetPointerPos) {

	CHKPARAM(2,3);

	if(numargs!=3) {
		BUILDRXSTRING(retstr,WinSetPointerPos(
				HWND_DESKTOP,
				strtoul(args[0].strptr,NULL,10),
				strtoul(args[1].strptr,NULL,10))?"1":"0");
	}
	else {
		BUILDRXSTRING(retstr,WinSetPointerPos(
				((!RXVALIDSTRING(args[0]))||(args[0].strlength==0))?HWND_DESKTOP:strtoul(args[0].strptr,NULL,10),
				strtoul(args[1].strptr,NULL,10),
				strtoul(args[2].strptr,NULL,10))?"1":"0");
	}

	RXRET;
}

/******************************************************************************
* hwnd=spQueryPointerPos(hwnd,code)                                           *
******************************************************************************/
RXFUNC(spQueryPointerPos) {
	POINTL	plPos;
	char	szBuffer[21];

	CHKPARAM(0,2);

	if(!WinQueryPointerPos(
			(numargs==2)?strtoul(args[0].strptr,NULL,10):HWND_DESKTOP,
			&plPos)) {
		szBuffer[0]='\0';
	}
	else if((numargs==0) || (!stricmp(args[(numargs==2)?1:0].strptr,"b"))) {
		sprintf(szBuffer,"%d %d",plPos.x,plPos.y);
	}
	else if(!stricmp(args[(numargs==2)?1:0].strptr,"x")) {
		sprintf(szBuffer,"%d",plPos.x);
	}
	else if(!stricmp(args[(numargs==2)?1:0].strptr,"y")) {
		sprintf(szBuffer,"%d",plPos.y);
	}
	BUILDRXSTRING(retstr,szBuffer);

	RXRET;
}
