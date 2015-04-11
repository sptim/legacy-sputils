/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: pmapi/windialogs.c                                                  *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/PMAPI/WINDIALOGS";
static const char *id = "$Id: dialogs.c,v 1.2 2004-02-15 04:01:53 spTim Exp $";

#define INCL_DOSPROCESS
#define INCL_WINDIALOGS
#define INCL_WINPROGRAMLIST
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

RexxFunctionHandler spQueryDlgItemText;
RexxFunctionHandler spQueryDlgItemTextLength;
RexxFunctionHandler spSetDlgItemText;

/******************************************************************************
* title=spQueryDlgItemText(hwnd)                                              *
******************************************************************************/
ULONG spQueryDlgItemText(PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	HAB		hab;
	HMQ		hmq;
	QMSG	qmsg;
	PIB		*ppib;
	TIB		*ptib;
	HWND	hwnd,
			hwndFrame,
			hwndClient;
	char	*pBuffer;
	ULONG	ulID,
			ulFCF,
			ulpmtype;

	CHKPARAM(2,2);

	hwnd=(HWND)strtoul(args[0].strptr,NULL,10);
	ulID=(ULONG)strtoul(args[1].strptr,NULL,10);

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
	WinQueryDlgItemText(hwnd,ulID,MAX_WINDOW_TEXT_LENGTH,pBuffer);

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
* length=spQueryDlgItemTextLength(hwnd,id)                                    *
******************************************************************************/
ULONG spQueryDlgItemTextLength(	PUCHAR name,
								ULONG numargs,
								RXSTRING args[],
								PSZ queuename,
								RXSTRING* retstr) {
	HAB		hab;
	HMQ		hmq;
	QMSG	qmsg;
	PIB		*ppib;
	TIB		*ptib;
	ULONG	ulPMType;
	ULONG	ulFCF;
	HWND	hwndFrame;
	HWND	hwndClient;
	char	buffer[12];

	CHKPARAM(2,2);

	/* Change program type if necessary */
	DosGetInfoBlocks(&ptib,&ppib);
	ulPMType=ppib->pib_ultype;
	if( (ulPMType==PROG_FULLSCREEN) || (ulPMType==PROG_WINDOWABLEVIO) ) ppib->pib_ultype=PROG_PM;

	/* Create PM Window */
	hab=WinInitialize(0);
	hmq=WinCreateMsgQueue(hab,0);
	ulFCF=0;
	hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&ulFCF,WC_FRAME,"spQueryWindowTitle",0,NULLHANDLE,100,&hwndClient);

	/* Query Text Length */
	ultoa((ULONG)WinQueryDlgItemTextLength(
			(HWND)strtoul(args[0].strptr,NULL,10),
			(USHORT)strtoul(args[1].strptr,NULL,10)),buffer,10);

	/* Close & Destroy PM Window */
	WinPostMsg(hwndFrame,WM_CLOSE,0,0);
	while (WinGetMsg(hab, &qmsg, NULL, 0, 0))
		WinDispatchMsg(hab, &qmsg);
	WinDestroyWindow(hwndFrame);
	WinDestroyMsgQueue(hmq);
	WinTerminate(hab);

	/* Change back program type */
	ppib->pib_ultype=ulPMType;

	BUILDRXSTRING(retstr,buffer);
	RXRET;
}

/******************************************************************************
* CALL spSetDlgItemText hwnd,title                                            *
******************************************************************************/
ULONG spSetDlgItemText(	PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	HAB		hab;
	HMQ		hmq;
	QMSG	qmsg;
	PIB		*ppib;
	TIB		*ptib;
	HWND	hwnd,
			hwndFrame,
			hwndClient;
	char	*pBuffer;
	ULONG	ulID,
			ulFCF,
			ulpmtype;
	BOOL	rc;

	CHKPARAM(3,3);

	hwnd=(HWND)strtoul(args[0].strptr,NULL,10);
	ulID=(ULONG)strtoul(args[1].strptr,NULL,10);

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
	hwndFrame=WinCreateStdWindow(HWND_DESKTOP,0,&ulFCF,WC_FRAME,"spSetDlgItemText",0,NULLHANDLE,100,&hwndClient);

	/* Set Window Text */
	strncpy(pBuffer,args[2].strptr,MAX_WINDOW_TEXT_LENGTH);
	rc=WinSetDlgItemText(hwnd, ulID, pBuffer);

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
	BUILDRXSTRING(retstr,rc?"1":"0");
	DosFreeMem(pBuffer);

	RXRET;
}

