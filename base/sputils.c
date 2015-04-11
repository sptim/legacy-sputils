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

const char *MODULE = "SPUTILS/BASE/SPUTILS";
const char *id = "$Id: sputils.c,v 1.3 2004-02-15 12:04:17 spTim Exp $";

#define  INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"
#include "rxfnctbl.h"

int AutoSleepTime=50;

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

/* spUtils Loading/Unloading/Version */
RexxFunctionHandler spVersion;
RexxFunctionHandler spLoadFuncs;
RexxFunctionHandler spDropFuncs;
RexxFunctionHandler spSetAutoSleep;
RexxFunctionHandler spD2X;
/*RexxFunctionHandler spX2D;*/
/******************************************************************************
* CALL spLoadFuncs                                                            *
******************************************************************************/
ULONG spLoadFuncs(	PUCHAR name,
					ULONG numargs,
					RXSTRING args[],
					PSZ queuename,
					RXSTRING* retstr) {
	int i,
		num;

	if(numargs!=0) return INVALID_ROUTINE;
	num=sizeof(RexxFunctions)/sizeof(PSZ);
	for(i=0;i<num;i++) {
		RexxRegisterFunctionDll(RexxFunctions[i],"SPUTILS",RexxFunctions[i]);
	}
	return NO_ERROR;
}

/******************************************************************************
* CALL spDropFuncs                                                            *
******************************************************************************/
ULONG spDropFuncs(	PUCHAR name,
					ULONG numargs,
					RXSTRING args[],
					PSZ queuename,
					RXSTRING* retstr) {
	int i,
		num;

	if(numargs!=0) return INVALID_ROUTINE;
	num=sizeof(RexxFunctions)/sizeof(PSZ);
	for(i=0;i<num;i++) {
		RexxDeregisterFunction(RexxFunctions[i]);
	}
	return NO_ERROR;
}

/******************************************************************************
* CALL spVersion                                                              *
******************************************************************************/
RXFUNC(spVersion) {
	if(numargs!=0) return INVALID_ROUTINE;
	BUILDRXSTRING(retstr,VERSION);
	return NO_ERROR;
}

/******************************************************************************
* CALL spSetAutoSleep time                                                    *
******************************************************************************/
RXFUNC(spSetAutoSleep) {
	CHKPARAM(1,1);

	AutoSleepTime=strtoul(args[0].strptr,NULL,10);
	return NO_ERROR;
}

/******************************************************************************
* hexval=spD2X(dezval[,type])                                                 *
******************************************************************************/
RXFUNC(spD2X) {
	char szFormat[4];
	char buffer[9];
	ULONG ulMask;
	ULONG ulValue;

	CHKPARAM(1,2);

	ulValue=strtoul(args[0].strptr,NULL,10);

	if((numargs==2) && (args[1].strlength>0)) {
		if		(!strcmp(args[1].strptr,"L"))	sprintf(buffer,"%08X",ulValue);
		else if (!strcmp(args[1].strptr,"l"))	sprintf(buffer,"%08x",ulValue);
		else if (!strcmp(args[1].strptr,"S") &&
				 !(ulValue&0xffff0000))			sprintf(buffer,"%04X",ulValue);
		else if (!strcmp(args[1].strptr,"s") &&
				 !(ulValue&0xffff0000))			sprintf(buffer,"%04x",ulValue);
		else if (!strcmp(args[1].strptr,"B") &&
				 !(ulValue&0xffffff00))			sprintf(buffer,"%02X",ulValue);
		else if (!strcmp(args[1].strptr,"b") &&
				 !(ulValue&0xffffff00))			sprintf(buffer,"%02x",ulValue);
		else if (!strcmp(args[1].strptr,"N") &&
				 !(ulValue&0xfffffff0))			sprintf(buffer,"%01X",ulValue);
		else if (!strcmp(args[1].strptr,"N") &&
			 	!(ulValue&0xfffffff0))			sprintf(buffer,"%01x",ulValue);
		else return INVALID_ROUTINE;
	}
	else {
		ultoa(ulValue,buffer,16);
	}
	BUILDRXSTRING(retstr,buffer);
	return NO_ERROR;
}

/******************************************************************************
* dezval=spX2D(hexval[,type])                                                 *
******************************************************************************/
/*RXFUNC(spX2D) {
	char szFormat[4];
	char buffer[9];
	ULONG ulMask;
	ULONG ulValue;

	CHKPARAM(1,2);

	ulValue=strtoul(args[0].strptr,NULL,16);

	if((numargs==2) && (args[1].strlength>0)) {
		if (	(!stricmp(args[1].strptr,"S") && (ulValue&0xffff0000)) ||
				(!stricmp(args[1].strptr,"B") && (ulValue&0xffffff00)) ||
				(!stricmp(args[1].strptr,"b") && (ulValue&0xffffff00)) ||
				(!stricmp(args[1].strptr,"N") && (ulValue&0xfffffff0))) {
			return INVALID_ROUTINE;
		}
	}
	ultoa(ulValue,buffer,10);
	BUILDRXSTRING(retstr,buffer);
	return NO_ERROR;
}
*/
