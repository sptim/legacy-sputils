/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: pmapi/windowmgr.c                                                   *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/PMAPI/WINDOWMGR";
static const char *id = "$Id: windowmgr.c,v 1.2 2004-02-15 03:47:26 spTim Exp $";

#define  INCL_DOSPROCESS
#define  INCL_WIN
#define  INCL_WINWINDOWMGR
#define  INCL_WINBUTTONS
#define  INCL_WINPROGRAMLIST
#define  INCL_REXXSAA
#define  INCL_ERRORS
#include <os2.h>
#include <rexxsaa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sputils.h"
#include "sysvals.h"
#include "querywindow.h"
#include "helpers\procstat.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

/* PM API Window Functions */

RexxFunctionHandler spIsChild;
RexxFunctionHandler spIsControlEnabled;
RexxFunctionHandler spIsMenuItemChecked;
RexxFunctionHandler spIsMenuItemEnabled;
RexxFunctionHandler spIsMenuItemValid;
RexxFunctionHandler spIsWindow;
RexxFunctionHandler spIsWindowEnabled;
RexxFunctionHandler spIsWindowShowing;
RexxFunctionHandler spIsWindowVisible;
RexxFunctionHandler spQueryActiveWindow;
RexxFunctionHandler spQueryButtonCheckstate;
RexxFunctionHandler spQueryLboxCount;
RexxFunctionHandler spQueryLboxSelectedItem;
RexxFunctionHandler spQueryWindow;
RexxFunctionHandler spQueryWindowPos;
RexxFunctionHandler spQueryWindowText;
RexxFunctionHandler spSetWindowText;
RexxFunctionHandler spWindowFromID;
CLongFunctionHandler queryWindowText(HWND,long,PSZ);

/******************************************************************************
* rc=spIsChild(hwnd,hwndParent)                                               *
******************************************************************************/
RXFUNC(spIsChild) {

	CHKPARAM(2,2);

	BUILDRXSTRING(retstr,(WinIsChild(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(HWND)strtoul(args[1].strptr,NULL,10) ))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsControlEnabled(hwnd,id)                                              *
******************************************************************************/
RXFUNC (spIsControlEnabled) {

	CHKPARAM(2,2);

	BUILDRXSTRING(retstr,(WinIsControlEnabled(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(USHORT)strtoul(args[1].strptr,NULL,10) ))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsMenuItemChecked(hwnd,id)                                             *
******************************************************************************/
RXFUNC(spIsMenuItemChecked) {

	CHKPARAM(2,2);

	BUILDRXSTRING(retstr,(WinIsMenuItemChecked(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(USHORT)strtoul(args[1].strptr,NULL,10) ))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsMenuItemEnabled(hwnd,id)                                             *
******************************************************************************/
RXFUNC(spIsMenuItemEnabled) {

	CHKPARAM(2,2);

	BUILDRXSTRING(retstr,(WinIsMenuItemEnabled(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(USHORT)strtoul(args[1].strptr,NULL,10) ))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsMenuItemValid(hwnd,id)                                               *
******************************************************************************/
RXFUNC(spIsMenuItemValid) {

	CHKPARAM(2,2);

	BUILDRXSTRING(retstr,(WinIsMenuItemValid(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(USHORT)strtoul(args[1].strptr,NULL,10) ))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsWindow(hwnd)                                                         *
******************************************************************************/
RXFUNC(spIsWindow) {
	HAB		hab;

	CHKPARAM(1,1);

	hab=WinInitialize(0);

	BUILDRXSTRING(retstr,(WinIsWindow(hab,
			(HWND)strtoul(args[0].strptr,NULL,10)))?"1":"0");

	WinTerminate(hab);
	RXRET;
}

/******************************************************************************
* rc=spIsWindowEnabled(hwnd)                                                  *
******************************************************************************/
RXFUNC(spIsWindowEnabled) {

	CHKPARAM(1,1);

	BUILDRXSTRING(retstr,(WinIsWindowEnabled(
			(HWND)strtoul(args[0].strptr,NULL,10)))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsWindowShowing(hwnd)                                                  *
******************************************************************************/
RXFUNC(spIsWindowShowing) {

	CHKPARAM(1,1);

	BUILDRXSTRING(retstr,(WinIsWindowShowing(
			(HWND)strtoul(args[0].strptr,NULL,10)))?"1":"0");

	RXRET;
}

/******************************************************************************
* rc=spIsWindowVisible(hwnd)                                                  *
******************************************************************************/
RXFUNC(spIsWindowVisible) {
	HWND hwnd;

	if(numargs!=1 || !RXVALIDSTRING(args[0])) return INVALID_ROUTINE;
	hwnd=(HWND)strtoul(args[0].strptr,NULL,10);

	BUILDRXSTRING(retstr,(WinIsWindowVisible(hwnd))?"1":"0");

	RXRET;
}

/******************************************************************************
* hwnd=spQueryActiveWindow(hwnd)                                              *
******************************************************************************/
RXFUNC(spQueryActiveWindow) {
	char buffer[12];

	CHKPARAM(0,1);

	ultoa(((ULONG)WinQueryActiveWindow(
			(numargs==0)?HWND_DESKTOP:(HWND)strtoul(args[0].strptr,NULL,10))),
			buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* state=spQueryButtonCheckstate(hwnd,id)                                      *
******************************************************************************/
RXFUNC(spQueryButtonCheckstate) {
	char buffer[12];

	CHKPARAM(2,2);

	ultoa((ULONG)WinQueryButtonCheckstate(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(USHORT)strtoul(args[1].strptr,NULL,10)),buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* length=spQueryLboxCount(hwnd)                                               *
******************************************************************************/
RXFUNC(spQueryLboxCount) {
	char buffer[12];

	CHKPARAM(1,1);

	ultoa((ULONG)WinQueryLboxCount(
			(HWND)strtoul(args[0].strptr,NULL,10)),buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* length=spQueryLboxSelectedItem(hwnd)                                        *
******************************************************************************/
RXFUNC(spQueryLboxSelectedItem) {
	char buffer[12];

	CHKPARAM(1,1);

	ultoa((ULONG)WinQueryLboxSelectedItem(
			(HWND)strtoul(args[0].strptr,NULL,10)),buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* hwnd=spQueryWindow( hwnd, code )                                            *
******************************************************************************/
RXFUNC(spQueryWindow) {
	char buffer[12];

	CHKPARAM(2,2);

	ultoa((ULONG)WinQueryWindow(
			(HWND)strtoul(args[0].strptr,NULL,10),
			getLCode(args[1].strptr)),buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* hwnd=spQueryWIndowPos( hwnd, key )                                          *
******************************************************************************/
RXFUNC(spQueryWindowPos) {
	HWND hwnd;
	SWP	swp;
	char buffer[11];

	CHKPARAM(1,2);

	if((numargs==1) || (!RXVALIDSTRING(args[0])) || (args[0].strlength==0)) {
		hwnd=HWND_DESKTOP;
	}
	else {
		hwnd=strtoul(args[0].strptr,NULL,10);
	}

	if(WinQueryWindowPos(hwnd,&swp)) {
		if     ((!stricmp(args[(numargs==1)?0:1].strptr,"fl")) ||
				(!stricmp(args[(numargs==1)?0:1].strptr,"flags")) ||
				(!stricmp(args[(numargs==1)?0:1].strptr,"options"))) {
			BUILDRXSTRING(retstr,ultoa(swp.fl,buffer,10));
		}
		else if((!stricmp(args[(numargs==1)?0:1].strptr,"cy")) ||
				(!stricmp(args[(numargs==1)?0:1].strptr,"height"))) {
			BUILDRXSTRING(retstr,ultoa(swp.cy,buffer,10));
		}
		else if((!stricmp(args[(numargs==1)?0:1].strptr,"cx")) ||
				(!stricmp(args[(numargs==1)?0:1].strptr,"width"))) {
			BUILDRXSTRING(retstr,ultoa(swp.cx,buffer,10));
		}
		else if((!stricmp(args[(numargs==1)?0:1].strptr,"y")) ||
				(!stricmp(args[(numargs==1)?0:1].strptr,"bottom"))) {
			BUILDRXSTRING(retstr,ultoa(swp.y,buffer,10));
		 }
		else if((!stricmp(args[(numargs==1)?0:1].strptr,"x")) ||
				(!stricmp(args[(numargs==1)?0:1].strptr,"left"))) {
			BUILDRXSTRING(retstr,ultoa(swp.x,buffer,10));
		}
		else if(!stricmp(args[(numargs==1)?0:1].strptr,"hwndInsertBehind")) {
			BUILDRXSTRING(retstr,ultoa(swp.hwndInsertBehind,buffer,10));
		}
		else if(!stricmp(args[(numargs==1)?0:1].strptr,"hwnd")) {
			BUILDRXSTRING(retstr,ultoa(swp.hwnd,buffer,10));
		}
		else {
			return INVALID_ROUTINE;
		}
		RXRET;
	}

	return INVALID_ROUTINE;
}

/******************************************************************************
* title=spQueryWindowText(hwnd)                                               *
******************************************************************************/
RXFUNC(spQueryWindowText) {
	HAB		hab;
	HMQ		hmq;
	QMSG	qmsg;
	PIB		*ppib;
	TIB		*ptib;
	HWND	hwnd,
			hwndFrame,
			hwndClient;
	char	*pBuffer;
	ULONG	ulFCF,
			ulpmtype;

	CHKPARAM(1,1);

	hwnd=(HWND)strtoul(args[0].strptr,NULL,10);

	/* Allocate Shared Memory and Share With Hwnd */
	if((pBuffer=allocAndShare(MAX_WINDOW_TEXT_LENGTH,hwnd))==NULL) return INVALID_ROUTINE;

	/* Change program type if necessary */
	DosGetInfoBlocks(&ptib,&ppib);
	ulpmtype=ppib->pib_ultype;
	if( (ulpmtype==PROG_FULLSCREEN) || (ulpmtype==PROG_WINDOWABLEVIO) ) ppib->pib_ultype=PROG_PM;

	/* Create PM Window */
	hab=WinInitialize(0);
	hmq=WinCreateMsgQueue(hab,0);
	ulFCF=0;
	hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&ulFCF,WC_FRAME,"spQueryWindowTitle",0,NULLHANDLE,100,&hwndClient);

	/* Query Window Text */
	WinQueryWindowText(hwnd,MAX_WINDOW_TEXT_LENGTH,pBuffer);

	/* Close & Destroy PM Window */
	WinPostMsg(hwndFrame,WM_CLOSE,0,0);
	while (WinGetMsg(hab, &qmsg, NULL, 0, 0))
		WinDispatchMsg(hab, &qmsg);
	WinDestroyWindow(hwndFrame);
	WinDestroyMsgQueue(hmq);
	WinTerminate(hab);

	/* Change back program type */
	ppib->pib_ultype=ulpmtype;

	/* return result */
	BUILDRXSTRING(retstr,pBuffer);
	DosFreeMem(pBuffer);

	RXRET;
}


/******************************************************************************
* CALL spSetWindowText hwnd,title                                             *
******************************************************************************/
RXFUNC(spSetWindowText) {
	HAB		hab;
	HMQ		hmq;
	QMSG	qmsg;
	PIB		*ppib;
	TIB		*ptib;
	HWND	hwnd,
			hwndFrame,
			hwndClient;
	char	*pBuffer;
	ULONG	ulFCF,
			ulpmtype;
	BOOL	rc;

	CHKPARAM(2,2);

	hwnd=(HWND)strtoul(args[0].strptr,NULL,10);

	/* Allocate Shared Memory and Share With Hwnd */
	if((pBuffer=allocAndShare(MAX_WINDOW_TEXT_LENGTH,hwnd))==NULL) return INVALID_ROUTINE;

	/* Change program type if necessary */
	DosGetInfoBlocks(&ptib,&ppib);
	ulpmtype=ppib->pib_ultype;
	if( (ulpmtype==PROG_FULLSCREEN) || (ulpmtype==PROG_WINDOWABLEVIO) ) ppib->pib_ultype=PROG_PM;

	/* Create PM Window */
	hab=WinInitialize(0);
	hmq=WinCreateMsgQueue(hab,0);
	ulFCF=0;
	hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&ulFCF,WC_FRAME,"spSetWindowTitle",0,NULLHANDLE,100,&hwndClient);

	/* Query Window Text */
	strncpy(pBuffer,args[1].strptr,MAX_WINDOW_TEXT_LENGTH);
	rc=WinSetWindowText(hwnd,pBuffer);

	/* Close & Destroy PM Window */
	WinPostMsg(hwndFrame,WM_CLOSE,0,0);
	while (WinGetMsg(hab, &qmsg, NULL, 0, 0))
		WinDispatchMsg(hab, &qmsg);
	WinDestroyWindow(hwndFrame);
	WinDestroyMsgQueue(hmq);
	WinTerminate(hab);

	/* Change back program type */
	ppib->pib_ultype=ulpmtype;

	/* return result */
	BUILDRXSTRING(retstr,(rc?"1":"0"));
	DosFreeMem(pBuffer);

	RXRET;
}

/******************************************************************************
* long queryWindowText(HWND hwnd,LONG lBufferLength, PSZ pszBuffer)           *
* params: HWND hwnd          - window handle                                  *
*         LONG lBufferLength - size of buffer                                 *
*         PSZ pszBuffer      - pointer to buffer                              *
* returns: long - real length of text                                         *
******************************************************************************/
long queryWindowText(HWND hwnd,LONG lBufferLength, PSZ pszBuffer) {
	HAB		hab;
	HMQ		hmq;
	QMSG	qmsg;
	HWND	hwndFrame;
	HWND	hwndClient;
	LONG	lTextLength;
	ULONG	ulFCF;
	ULONG	ulPMType;
	PIB*	ppib;
	TIB*	ptib;
	PSZ		pSharedBuffer;

	/* Change program type if necessary */
	DosGetInfoBlocks(&ptib,&ppib);
	ulPMType=ppib->pib_ultype;
	if( (ulPMType==PROG_FULLSCREEN) || (ulPMType==PROG_WINDOWABLEVIO) ) ppib->pib_ultype=PROG_PM;

	/* Create PM Window */
	hab=WinInitialize(0);
	hmq=WinCreateMsgQueue(hab,0);
	ulFCF=0;
	hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&ulFCF,WC_FRAME,"queryWindowTextLength",0,NULLHANDLE,100,&hwndClient);

	/* Query Window Text Length */
	lTextLength=WinQueryWindowTextLength(hwnd)+1;

	/* Close & Destroy PM Window */
	WinPostMsg(hwndFrame,WM_CLOSE,0,0);
	while (WinGetMsg(hab, &qmsg, NULL, 0, 0))
		WinDispatchMsg(hab, &qmsg);
	WinDestroyWindow(hwndFrame);
	WinDestroyMsgQueue(hmq);

	/* Allocate Shared Memory and Share With Hwnd */
	if((lBufferLength>0) && (pszBuffer!=NULL)) {
		if((pSharedBuffer=allocAndShare(lTextLength,hwnd))==NULL) {
			WinTerminate(hab);
			ppib->pib_ultype=ulPMType;
			return -1;
		}

		/* Create PM Window */
//		hab=WinInitialize(0);
		hmq=WinCreateMsgQueue(hab,0);
		ulFCF=0;
		hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&ulFCF,WC_FRAME,"queryWindowTextLength",0,NULLHANDLE,100,&hwndClient);

		/* Query Window Text */
		WinQueryWindowText(hwnd,lTextLength,pSharedBuffer);

		/* Close & Destroy PM Window */
		WinPostMsg(hwndFrame,WM_CLOSE,0,0);
		while (WinGetMsg(hab, &qmsg, NULL, 0, 0))
			WinDispatchMsg(hab, &qmsg);
		WinDestroyWindow(hwndFrame);
		WinDestroyMsgQueue(hmq);

		/* Copy Window Text & free shared mem */
		strncpy(pszBuffer,pSharedBuffer,lTextLength);
		DosFreeMem(pSharedBuffer);
//		WinTerminate(hab);
	}

	/* Terminate PM */
	WinTerminate(hab);

	/* Change back program type */
	ppib->pib_ultype=ulPMType;

	return lTextLength;
}
/******************************************************************************
* hwnd=spWindowFromID(hwnd,id)                                                *
******************************************************************************/
RXFUNC(spWindowFromID) {
	HWND hwnd;
	ULONG ulID;
	char buffer[10];

	CHKPARAM(2,2);
	hwnd=(HWND)strtoul(args[0].strptr,NULL,10);
	ulID=(ULONG)strtoul(args[1].strptr,NULL,10);

	ultoa(((ULONG)WinWindowFromID(hwnd,ulID)),buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

