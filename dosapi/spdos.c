/******************************************************************************
* CALL spDos                                                                  *
******************************************************************************/

static const char *MODULE = "SPUTILS/DOSAPI/SPDOS";
static const char *id = "$Id: spdos.c,v 1.2 2004-02-15 03:11:40 spTim Exp $";

#define INCL_DOSPROCESS
#define INCL_DOS
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers/procstat.h"
#include "sputils.h"

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/
RexxFunctionHandler spGetPidList;
RexxFunctionHandler spKillAll;
RexxFunctionHandler spFilenameFromPid;
RexxFunctionHandler spGetBootdrive;
CIntFunctionHandler filenameFromPid(ULONG, PSZ, USHORT);

/******************************************************************************
* success=spGetPidList(stem, filter)                                          *
******************************************************************************/
RXFUNC(spGetPidList) {
	PQTOPLEVEL32	pqTopLevel;
	PQPROCESS32		pqProcess32;
/*	PQMODULE32		pqModule32;
*/	APIRET			arc;
	PUCHAR			pszFilter;
	USHORT			usFound;
	PUCHAR			pszVarName;
	UCHAR			szVarValue[12];
	UCHAR			pszFilename[FILENAME_MAX];

	if(numargs<1 || numargs>2 || !RXVALIDSTRING(args[0])) return INVALID_ROUTINE;
	//CHKPARAM(1,2);

	if((pszVarName=malloc(strlen(args[0].strptr)+13))==NULL) {
		return INVALID_ROUTINE;
	}
	if(numargs==1 || strlen(args[1].strptr)==0) {
		pszFilter=NULL;
	}
	else {
		if((pszFilter=malloc(strlen(args[1].strptr)+1))==NULL) {
			free(pszVarName);
			return INVALID_ROUTINE;
		}
		strcpy(pszFilter,args[1].strptr);
		strupr(pszFilter);
	}

	usFound=0;
	pqTopLevel=prc32GetInfo(&arc);
	pqProcess32=pqTopLevel->pProcessData;
	while (pqProcess32 && pqProcess32->ulRecType == 1) {
		PQTHREAD32  t = pqProcess32->pThreads;
		/*if(((pqModule32=prc32FindModule(pqTopLevel,pqProcess32->usHModule))!=NULL) &&
				(pqModule32->pcName!=NULL)) {
			if((pszFilter==NULL) ||
					(strstr(pqModule32->pcName,pszFilter)!=NULL)) {
		*/
		if(		(pszFilter==NULL) ||
				((DosQueryModuleName(pqProcess32->usHModule,
						FILENAME_MAX,
						pszFilename)==0) &&
				(strstr(pszFilename,pszFilter)!=NULL))) {
			sprintf(pszVarName,"%s.%d",args[0].strptr,++usFound);
			utoa(pqProcess32->usPID,szVarValue,10);
			setRexxVar(pszVarName,szVarValue);
		}

		pqProcess32=(PQPROCESS32)(pqProcess32->pThreads+pqProcess32->usThreadCount);
	}
	sprintf(pszVarName,"%s.0",args[0].strptr);
	utoa(usFound,szVarValue,10);
	setRexxVar(pszVarName,szVarValue);
	free(pszFilter);
	free(pszVarName);
	prc32FreeInfo(pqTopLevel);

	BUILDRXSTRING(retstr,(usFound>0)?"1":"0");

	return NO_ERROR;
}

/******************************************************************************
* numkilled=spKillAll(action, filename)                                       *
******************************************************************************/
RXFUNC(spKillAll) {
	PQTOPLEVEL32	pqTopLevel;
	PQPROCESS32		pqProcess32;
	/*
	PQMODULE32		pqModule32;
	*/
	char			pszModuleName[FILENAME_MAX];
	APIRET			arc;
	PSZ				pszFilename;
	ULONG			ulAction;
	ULONG			usNumKilled=0;
	APIRET			rc;
	char			szRc[13];

	CHKPARAM(1,2);

	if(numargs==1) {
		pszFilename=malloc(args[0].strlength+1);
		strncpy(pszFilename,args[0].strptr,args[0].strlength+1);

		ulAction=DKP_PROCESS;
	}
	else {
		if(!stricmp(args[0].strptr,"DKP_PROCESS") ||
				!stricmp(args[0].strptr,"PROCESS") ||
				!stricmp(args[0].strptr,"P") ||
				!stricmp(args[0].strptr,"")) {
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
		pszFilename=malloc(args[0].strlength+1);
		strncpy(pszFilename,args[0].strptr,args[0].strlength+1);
	}
	strupr(pszFilename);

	pqTopLevel=prc32GetInfo(&arc);
	pqProcess32=pqTopLevel->pProcessData;
	while(pqProcess32 && pqProcess32->ulRecType == 1) {
		PQTHREAD32  t = pqProcess32->pThreads;
		if((!DosQueryModuleName(pqProcess32->usHModule,
						FILENAME_MAX,
						pszModuleName)) &&
				(strstr(pszModuleName,pszFilename)!=NULL)) {
/*		if(((pqModule32=prc32FindModule(pqTopLevel,pqProcess32->usHModule))!=NULL) &&
				(pqModule32->pcName!=NULL)) {
			if(strstr(pqModule32->pcName,pszFilename)!=NULL) {
*/
				if(DosKillProcess(ulAction,pqProcess32->usPID)==0) usNumKilled++;
/*			}
*/		}
		pqProcess32=(PQPROCESS32)(pqProcess32->pThreads+pqProcess32->usThreadCount);
	}

	ultoa(usNumKilled,szRc,10);
	BUILDRXSTRING(retstr,szRc);
	free(pszFilename);
	return NO_ERROR;
}

/******************************************************************************
* filename=spFilenameFromPid(pid)                                             *
******************************************************************************/
RXFUNC(spFilenameFromPid) {
	CHKPARAM(1,1);

	//RXRESIZE(retstr,filenameFromPid(strtoul(args[0].strptr,NULL,10),NULL,0)+1);
	filenameFromPid(strtoul(args[0].strptr,NULL,10),
			retstr->strptr,
			RXAUTOBUFLEN); //retstr->strlength);
	retstr->strlength=strlen(retstr->strptr);
	return NO_ERROR;

/*	pqTopLevel=prc32GetInfo(&arc);
	pqProcess32=pqTopLevel->pProcessData;
	while(pqProcess32 && pqProcess32->ulRecType == 1) {
		PQTHREAD32  t = pqProcess32->pThreads;
		if((ulPid==pqProcess32->usPID) &&
				((pqModule32=prc32FindModule(pqTopLevel,pqProcess32->usHModule))!=NULL) &&
				(pqModule32->pcName!=NULL)) {
			BUILDRXSTRING(retstr,pqModule32->pcName);
			return NO_ERROR;
		}
		pqProcess32=(PQPROCESS32)(pqProcess32->pThreads+pqProcess32->usThreadCount);
	}

	BUILDRXSTRING(retstr,"");
	return NO_ERROR;
*/
}

/******************************************************************************
* int filenameFromPid(ULONG ulPid, PSZ pszFilename, USHORT usMaxLenFilename)  *
* params: ULONG  ulPid            - process identifier                        *
*         PSZ    pszFilename      - address of the storage area where this    *
*                                   function returns the filename.            *
*                                   If NULL, this function does not return    *
*                                   the filename.                             *
*         USHORT usMaxLenFilename - The length, in bytes, of pszFilename      *
* returns: int - real length of filename                                      *
******************************************************************************/
int filenameFromPid(ULONG ulPid, PSZ pszFilename, USHORT usMaxLenFilename) {
	PQTOPLEVEL32	pqTopLevel;
	PQPROCESS32		pqProcess;
	ULONG			rc;
	ULONG			ulMaxPathLength;
	char*			pszModuleName;

	DosQuerySysInfo(QSV_MAX_PATH_LENGTH,
			QSV_MAX_PATH_LENGTH,
			(PVOID)&ulMaxPathLength,
			sizeof(ulMaxPathLength));
	if((pszModuleName=malloc(ulMaxPathLength))==NULL) return 0;

	pqTopLevel=prc32GetInfo(&rc);
	pqProcess=prc32FindProcessFromPID(pqTopLevel,ulPid);
	rc=DosQueryModuleName(pqProcess->usHModule,
			ulMaxPathLength,
			pszModuleName);
	prc32FreeInfo(pqTopLevel);

	if(rc==0) {
		strncpy(pszFilename,pszModuleName,usMaxLenFilename);
		rc=strlen(pszModuleName);
	}
	else {
		if(usMaxLenFilename>0) pszFilename[0]='\0';
		rc=0;
	}

	free(pszModuleName);
	return rc;
}

/******************************************************************************
******************************************************************************/
RXFUNC(spGetBootdrive) {
	ULONG ulVar;

	CHKPARAM(0,0);

	DosQuerySysInfo(QSV_BOOT_DRIVE,QSV_BOOT_DRIVE,&ulVar,sizeof(ulVar));
	BUILDRXSTRING(retstr,"@:");
	retstr->strptr[0]+=ulVar;

	return NO_ERROR;
}

