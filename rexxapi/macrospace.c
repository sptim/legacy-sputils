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

static const char *MODULE = "SPUTILS/REXXAPI/MACROSPACE";
static const char *id = "$Id: macrospace.c,v 1.1 2003-12-17 01:52:02 spTim Exp $";

#define  INCL_REXXSAA
#define  INCL_ERRORS
#include <os2.h>
#include <rexxsaa.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "sputils.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

/* Rexx API Macrospace Functions */
RexxFunctionHandler spAddMacro;
RexxFunctionHandler spDropMacro;
RexxFunctionHandler spClearMacroSpace;
RexxFunctionHandler spSaveMacroSpace;
RexxFunctionHandler spLoadMacroSpace;
RexxFunctionHandler spQueryMacro;
RexxFunctionHandler spReorderMacro;

/******************************************************************************
* rc=spAddMacro([funcname],filename[,'B'|'A']                                 *
******************************************************************************/
ULONG spAddMacro(PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	ULONG	position,
			rc;
	char	funcname[FILENAME_MAX];
	struct	stat statbuf;
	
	if(numargs<2||numargs>3) return INVALID_ROUTINE;
	if(!RXVALIDSTRING(args[1])) return INVALID_ROUTINE;
	if(numargs>2 && !RXVALIDSTRING(args[2])) return INVALID_ROUTINE;
	
	if(stat(args[1].strptr,&statbuf)) return INVALID_ROUTINE;
	if(!(statbuf.st_mode&&S_IRUSR)) return INVALID_ROUTINE;
	printf("(1)\n");
	
	if(!RXVALIDSTRING(args[0]) || args[0].strlength==0) {
		strcpy(funcname,args[1].strptr);
		*strchr(funcname,'.')='\0';
	}
	else {
		strcpy(funcname, args[0].strptr);
	}
	if(numargs!=3) {
		position=RXMACRO_SEARCH_AFTER;
	}
	else if(!stricmp(args[2].strptr,"B") || !stricmp(args[2].strptr,"BEFORE")) {
		position=RXMACRO_SEARCH_BEFORE;
	}
	else if(!stricmp(args[2].strptr,"A") || !stricmp(args[2].strptr,"AFTER")) {
		position=RXMACRO_SEARCH_AFTER;
	}
	else return INVALID_ROUTINE;
	
	switch (RexxAddMacro(funcname,args[1].strptr,position)) {
		case RXMACRO_OK:
			BUILDRXSTRING(retstr,"");
			rc=NO_ERROR;
			break;
		case RXMACRO_NO_STORAGE:
			BUILDRXSTRING(retstr,"NO_STORAGE");
			rc=NO_ERROR;
			break;
		case RXMACRO_SOURCE_NOT_FOUND:
			printf("(2)\n");
			BUILDRXSTRING(retstr,"SOURCE_NOT_FOUND");
			rc=INVALID_ROUTINE;
			break;
		default:
			printf("(3:%d=0x%x)\n",position,position);
			BUILDRXSTRING(retstr,"UNKNOWN_ERROR");
			rc=INVALID_ROUTINE;
	}
	return rc;
}

/******************************************************************************
* rc=spDropMacro(funcname)                                                    *
******************************************************************************/
ULONG spDropMacro(	PUCHAR name,
					ULONG numargs,
					RXSTRING args[],
					PSZ queuename,
					RXSTRING* retstr) {
	BUILDRXSTRING(retstr,"");
	
	if(numargs!=1 || !RXVALIDSTRING(args[0])) return INVALID_ROUTINE;
	if(RexxDropMacro(args[0].strptr)==RXMACRO_OK) {
		return NO_ERROR;
	}
	else {
		return INVALID_ROUTINE;
	}
}

/******************************************************************************
* rc=spClearMacroSpace()                                                      *
******************************************************************************/
ULONG spClearMacroSpace(PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	BUILDRXSTRING(retstr,"");
	
	if(numargs!=0) return INVALID_ROUTINE;
	RexxClearMacroSpace();
	return NO_ERROR;
}

/******************************************************************************
* rc=spSaveMacroSpace(filename[,stem])                                        *
******************************************************************************/
ULONG spSaveMacroSpace(	PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	BUILDRXSTRING(retstr,"");
	
	if(numargs<1 || numargs>2 || !RXVALIDSTRING(args[0])) return INVALID_ROUTINE;
	if(numargs==2 && !RXVALIDSTRING(args[1])) return INVALID_ROUTINE;
	
	if(numargs==1 || args[1].strlength==0) {
		RexxSaveMacroSpace(0,NULL,args[0].strptr);
		return NO_ERROR;
	}
	return INVALID_ROUTINE;
}

/******************************************************************************
* rc=spLoadMacroSpace(filename[,stem])                                        *
******************************************************************************/
ULONG spLoadMacroSpace(	PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	BUILDRXSTRING(retstr,"");
	
	if(numargs<1 || numargs>2 || !RXVALIDSTRING(args[0])) return INVALID_ROUTINE;
	if(numargs==2 && !RXVALIDSTRING(args[1])) return INVALID_ROUTINE;
	
	if(numargs==1 || args[1].strlength==0) {
		RexxLoadMacroSpace(0,NULL,args[0].strptr);
		return NO_ERROR;
	}
	return INVALID_ROUTINE;
}

/******************************************************************************
* rc=spQueryMacro(funcname)                                                   *
******************************************************************************/
ULONG spQueryMacro(	PUCHAR name,
					ULONG numargs,
					RXSTRING args[],
					PSZ queuename,
					RXSTRING* retstr) {
	USHORT position;
	
	BUILDRXSTRING(retstr,"");
	
	if(numargs!=1 || !RXVALIDSTRING(args[0])) return INVALID_ROUTINE;
	
	if(RexxQueryMacro(args[0].strptr,&position)) {
		if(position==RXMACRO_SEARCH_BEFORE) {
			BUILDRXSTRING(retstr,"BEFORE");
		}
		else if(position==RXMACRO_SEARCH_AFTER) {
			BUILDRXSTRING(retstr,"AFTER");
		}
		else {
			return INVALID_ROUTINE;
		}
	}
	return NO_ERROR;
}

/******************************************************************************
* rc=spReorderMacro(funcname)                                                 *
******************************************************************************/
ULONG spReorderMacro(	PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	ULONG position;
	ULONG rc;
	
	BUILDRXSTRING(retstr,"");
	if(numargs!=2||!RXVALIDSTRING(args[0])||!RXVALIDSTRING(args[1])) return INVALID_ROUTINE;
	
	if(!stricmp(args[1].strptr,"B") || !stricmp(args[1].strptr,"BEFORE")) {
		position=RXMACRO_SEARCH_BEFORE;
	}
	else if(!stricmp(args[1].strptr,"A") || !stricmp(args[1].strptr,"AFTER")) {
		position=RXMACRO_SEARCH_AFTER;
	}
	else return INVALID_ROUTINE;
	if(RexxReorderMacro(args[0].strptr,position)!=RXMACRO_OK) return INVALID_ROUTINE;
	return NO_ERROR;
}


