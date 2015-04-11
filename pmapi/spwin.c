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
static const char *id = "$Id: spwin.c,v 1.2 2004-02-15 03:45:37 spTim Exp $";

/* #define INCL_WINSWITCHLIST */
//#define INCL_NLS
#define INCL_WIN
//#define INCL_PM
//#define INCL_DOS
//#define INCL_BASE
#define  INCL_REXXSAA
//#define  INCL_ERRORS
#include <os2.h>
#include <rexxsaa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sputils.h"

typedef struct _window {
		PSZ		pszClassname;			/* in/out - classname */
		USHORT	usClassnameLength;		/* in/out - max length of classname (in)
									            real length of classname (out) */
		USHORT	usId;				/* in/out - window control id */
		PSZ		pszText;			/* in/out - window text */
		USHORT	usTextLength;		/* in/out - max length of text (in)
									            real length of text (out) */
		ULONG	ulPid;				/* in/out - pid */
		ULONG	ulTid;				/* in/out - tid */
		PSZ		pszFilename;		/* in/out - filename */
		USHORT	usFilenameLength;	/* in/out - max length of filename (in)
									            real length of filename (out) */
	} WINDOW;

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

/* Functions related to Window Functions */
RexxFunctionHandler spFindWindowHandle;
RexxFunctionHandler spFindWindowHandles;
RexxFunctionHandler spMPFROM2SHORT;
RexxFunctionHandler spMPFROMSH2CH;
RexxFunctionHandler spQueryWindowID;
/*RexxFunctionHandler spSwitchListTitles;*/
CIntFunctionHandler queryWindow(HWND, WINDOW*);
CIntFunctionHandler findWindowHandles(HWND, WINDOW*, HWND*,int);

/******************************************************************************
* hwnd=spFindWindowHandle(hwndParent,class,id,title,pid,exefile)              *
******************************************************************************/
RXFUNC(spFindWindowHandle) {
	HWND hwndParent,
		hwnd;
	char buffer[10];
	WINDOW wnd;

	BUILDRXSTRING(retstr,"0");

	wnd.pszClassname=NULL;
	wnd.usId=0;
	wnd.pszText=NULL;
	wnd.ulPid=0;
	wnd.ulTid=0;
	wnd.pszFilename=NULL;

	if(numargs<1 || numargs>6) return INVALID_ROUTINE;
	if(!RXVALIDSTRING(args[0]) || args[0].strlength==0) {
		hwndParent=HWND_DESKTOP;
	}
	else {
		hwndParent=strtoul(args[0].strptr,NULL,10);
	}
	if(numargs>1 && RXVALIDSTRING(args[1]) && args[1].strlength!=0) wnd.pszClassname=args[1].strptr;
	if(numargs>2 && RXVALIDSTRING(args[2]) && args[2].strlength!=0) wnd.usId=atol(args[2].strptr);
	if(numargs>3 && RXVALIDSTRING(args[3]) && args[3].strlength!=0) wnd.pszText=args[3].strptr;
	if(numargs>4 && RXVALIDSTRING(args[4]) && args[4].strlength!=0) wnd.ulPid=atol(args[3].strptr);
	if(numargs>5 && RXVALIDSTRING(args[5]) && args[5].strlength!=0) wnd.pszFilename=args[4].strptr;

	if(findWindowHandles(hwndParent,&wnd,&hwnd,1)) {
		ultoa((ULONG)hwnd,buffer,10);
		BUILDRXSTRING(retstr,buffer);
	}

	RXRET;
}

/******************************************************************************
* title=spFindWindowHandles(stem,hwndParent,class,id,title,pid,exefile)       *
******************************************************************************/
RXFUNC(spFindWindowHandles) {
	HWND hwndParent,
		*buffer;
	char *varnamebuf,
		valbuf[13];
	WINDOW wnd;
	int i,
		num;

	BUILDRXSTRING(retstr,"0");

	wnd.pszClassname=NULL;
	wnd.usId=0;
	wnd.pszText=NULL;
	wnd.ulPid=0;
	wnd.ulTid=0;
	wnd.pszFilename=NULL;

	if(numargs<1 || numargs>7) return INVALID_ROUTINE;
	if(!RXVALIDSTRING(args[0]) || args[0].strlength==0) return INVALID_ROUTINE;
	if(!RXVALIDSTRING(args[1]) || args[1].strlength==0) {
		hwndParent=HWND_DESKTOP;
	}
	else {
		hwndParent=strtoul(args[1].strptr,NULL,10);
	}
	if(numargs>2 && RXVALIDSTRING(args[2]) && args[2].strlength!=0) wnd.pszClassname=args[2].strptr;
	if(numargs>3 && RXVALIDSTRING(args[3]) && args[3].strlength!=0) wnd.usId=atol(args[3].strptr);
	if(numargs>4 && RXVALIDSTRING(args[4]) && args[4].strlength!=0) wnd.pszText=args[4].strptr;
	if(numargs>5 && RXVALIDSTRING(args[5]) && args[5].strlength!=0) wnd.ulPid=atol(args[5].strptr);
	if(numargs>6 && RXVALIDSTRING(args[6]) && args[6].strlength!=0) wnd.pszFilename=args[6].strptr;

	num=findWindowHandles(hwndParent,&wnd,NULL,0);
	if(num==0) {
		varnamebuf=malloc(strlen(args[0].strptr)+6);
		sprintf(varnamebuf,"%s.0",args[0].strptr);
		setRexxVar(varnamebuf,"0");
		free(varnamebuf);
		BUILDRXSTRING(retstr,"0");
		RXRET;

	}
	buffer=malloc(num*sizeof(hwndParent));
	num=findWindowHandles(hwndParent,&wnd,(HWND*)buffer,num);

	varnamebuf=malloc(strlen(args[0].strptr)+6);
	for(i=0;i<num;i++) {
		sprintf(varnamebuf,"%s.%d",args[0].strptr,i+1);
		ultoa(buffer[i],valbuf,10);
		if(setRexxVar(varnamebuf,valbuf)==RXSHV_BADN) {
			free(varnamebuf);
			free(buffer);
			return INVALID_ROUTINE;
		}
	}

	sprintf(valbuf,"%d",num);
	sprintf(varnamebuf,"%s.0",args[0].strptr);
	BUILDRXSTRING(retstr,valbuf);
	setRexxVar(varnamebuf,valbuf);

	free(varnamebuf);
	free(buffer);

	BUILDRXSTRING(retstr,"1");
	RXRET;
}

/******************************************************************************
* int queryWindow(HWND hwnd, WINDOW *wnd)                                     *
* params: HWND hwnd   - window handle                                         *
*         WINDOW* wnd - window details structure                              *
* returns: TRUE  - successful operation                                       *
*          FALSE - window handle invalid                                      *
******************************************************************************/
int queryWindow(HWND hwnd, WINDOW *wnd) {
	HAB		hab;
	PSZ		pszBuffer;
	SHORT	sBufferLength;

	/* check if handle is valid */
	hab=WinInitialize(0);
	if(!WinIsWindow(hab,hwnd)) {
		return FALSE;
	}
	WinTerminate(hab);

	/* window class */
	sBufferLength=0;
	if((wnd->pszClassname!=NULL) && (wnd->usClassnameLength!=0)) {
		sBufferLength=WinQueryClassName(hwnd,
				wnd->usClassnameLength,
				wnd->pszClassname)-250;
	}
	do {
		sBufferLength+=256;
		if((pszBuffer=malloc(sBufferLength))==NULL) {
			wnd->usClassnameLength=0;
			break;
		}
		wnd->usClassnameLength=WinQueryClassName(hwnd,
				sBufferLength,
				pszBuffer)+1;
		free(pszBuffer);
	} while(wnd->usClassnameLength<sBufferLength);

	/* window id */
	wnd->usId=WinQueryWindowUShort(hwnd, QWS_ID);

	/* window text */
	wnd->usTextLength=queryWindowText(hwnd,wnd->usTextLength,wnd->pszText);

	/* pid, tid */
	WinQueryWindowProcess(hwnd,&wnd->ulPid,&wnd->ulTid);

	/* filename */
	if((wnd->pszFilename!=NULL) && (wnd->usFilenameLength!=0))
		wnd->usFilenameLength=filenameFromPid(wnd->ulPid,
				wnd->pszFilename,
				wnd->usFilenameLength);
	else
		wnd->usFilenameLength=filenameFromPid(wnd->ulPid,NULL,0);

	/* return true */
	return TRUE;
}

/******************************************************************************
* int findWindowHandles(HWND hwndParent,WINDOW *wndFilter,HWND *buff,int num) *
* params:                                                                     *
* returns: int - number of window handles found                               *
******************************************************************************/
int findWindowHandles(HWND hwndParent,WINDOW *wndFilter,HWND *buffer,int num) {
	HENUM henum;
	HWND hwnd;
	WINDOW wnd;
	char pszBuf[FILENAME_MAX];
	int found;
	char cZero='\0';

	found=0;
	if(num==0) buffer=NULL;
	if(wndFilter->pszFilename!=NULL) {
		strcpy(pszBuf,wndFilter->pszFilename);
		strupr(pszBuf);
	}
	henum=WinBeginEnumWindows(hwndParent);
	hwnd=WinGetNextWindow(henum);
	while(hwnd!=NULLHANDLE) {

		wnd.usClassnameLength=0;
		wnd.usTextLength=0;
		wnd.usFilenameLength=0;
		queryWindow(hwnd,&wnd);

		wnd.pszClassname=(wnd.usClassnameLength)?
				malloc(wnd.usClassnameLength):&cZero;
		wnd.pszText=(wnd.usTextLength)?
				malloc(wnd.usTextLength):&cZero;
		wnd.pszFilename=(wnd.usFilenameLength)?
				malloc(wnd.usFilenameLength):&cZero;
		if(		(wnd.pszClassname==NULL) ||
				(wnd.pszText==NULL) ||
				(wnd.pszFilename==NULL)) {
			WinEndEnumWindows(henum);
			return -1;
		}
		queryWindow(hwnd,&wnd);

		if(		(		(wndFilter->pszClassname==NULL)						||
						(!strcmp(wndFilter->pszClassname,wnd.pszClassname))
				) && (	(wndFilter->usId==0)								||
						(wndFilter->usId==wnd.usId)
				) && (	(wndFilter->pszText==NULL)							||
						(!strcmp(wndFilter->pszText,wnd.pszText))
				) && (	(wndFilter->ulPid==0)								||
						(wndFilter->ulPid==wnd.ulPid)
				) && (	(wndFilter->pszFilename==NULL)						||
						(strstr(wnd.pszFilename,pszBuf)!=NULL)
				)) {
			if((buffer!=NULL) && (found<num)) buffer[found]=hwnd;
			found++;
		}

		free(wnd.pszClassname);
		free(wnd.pszText);
		free(wnd.pszFilename);

		if(buffer!=NULL && found==num) break;

		hwnd=WinGetNextWindow(henum);
	}

	WinEndEnumWindows(henum);

	return found;
}

/******************************************************************************
* rc=spMPFROM2SHORT(USHORT,SHORT)                                             *
******************************************************************************/
RXFUNC(spMPFROM2SHORT) {
	char buffer[11];

	CHKPARAM(2,2);

	ultoa(MAKEULONG(strtoul(args[0].strptr,NULL,10),strtoul(args[1].strptr,NULL,10)),buffer,10);
	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* rc=spMPFROMSH2CH(USHORT,UCHAR,UCHAR)                                        *
******************************************************************************/
RXFUNC(spMPFROMSH2CH) {
	char buffer[11];

	CHKPARAM(3,3);

	ultoa(MAKEULONG(strtoul(args[0].strptr,NULL,10),
					MAKEUSHORT(	strtoul(args[1].strptr,NULL,10),
								strtoul(args[2].strptr,NULL,10))),buffer,10);

	BUILDRXSTRING(retstr,buffer);

	RXRET;
}

/******************************************************************************
* rc=spQueryWindowId(hwnd)                                                    *
******************************************************************************/
RXFUNC(spQueryWindowID) {
	char buffer[12];

	CHKPARAM(1,1);

	BUILDRXSTRING(retstr,ultoa(((ULONG)WinQueryWindowUShort(
			(HWND)strtoul(args[0].strptr,NULL,10),QWS_ID)),buffer,10));

	RXRET;
}

/*
ULONG spSwitchListTitles(PUCHAR name,
						ULONG numargs,
						RXSTRING args[],
						PSZ queuename,
						RXSTRING* retstr) {
	ULONG	titles,
			i;
	char	*title,
			*buffer,
			*varnamebuf,
			valuebuf[10];
	int	varnamelength;

	BUILDRXSTRING(retstr,"0");

	if(numargs!=1) return INVALID_ROUTINE;
	if(!RXVALIDSTRING(args[0])) return INVALID_ROUTINE;

	for(i=titles,varnamelength=strlen(args[0].strptr)+2;i>0;i/=10) varnamelength++;
	varnamebuf=(char*) malloc((size_t)varnamelength);

	titles=getSwitchListTitles(NULL,0);
	if(titles!=0) {
		buffer=(char*) malloc(titles*SWITCHLISTTITLELENGTH);
		titles=getSwitchListTitles(buffer,titles);
		for(i=0;i<titles;i++) {
			title=&buffer[i*SWITCHLISTTITLELENGTH];
			sprintf(varnamebuf,"%s.%d",args[0].strptr,i+1);
			if(setRexxVar(varnamebuf,title)==RXSHV_BADN) {
				free(buffer);
				free(varnamebuf);
				return INVALID_ROUTINE;
			}
		}
		free(buffer);
	}
	sprintf(varnamebuf,"%s.0",args[0].strptr);
	sprintf(valuebuf,"%d",titles);
	if(setRexxVar(varnamebuf,valuebuf)==RXSHV_BADN) {
		free(varnamebuf);
		return INVALID_ROUTINE;

	}
	free(varnamebuf);
	DosSleep(50);
	return NO_ERROR;
}

ULONG getSwitchListTitles(char* buffer,ULONG titles) {
	ULONG	cbItems,
			doItems,
			ulBufSize,
			i;
	PSWBLOCK	pswblk;
	HAB	hab;

	hab=WinInitialize(0);
	cbItems = WinQuerySwitchList(hab, NULL, 0);
	if(buffer==NULL) return cbItems;

	ulBufSize = (cbItems * sizeof(SWENTRY)) + sizeof(HSWITCH);
	pswblk = (PSWBLOCK) malloc(ulBufSize);
	cbItems = WinQuerySwitchList(hab, pswblk, ulBufSize);

	doItems=(cbItems<titles)?cbItems:titles;

	for(i=0;i<doItems;i++) {
		strcpy(&buffer[i*SWITCHLISTTITLELENGTH],pswblk->aswentry[i].swctl.szSwtitle);
	}
	free(pswblk);
	WinTerminate(hab);
	return doItems;
}

HWND getWindowHandle(char *wintitle,int num) {
	ULONG	cbItems,
			ulBufSize;
	PSWBLOCK	pswblk;
	HAB hab;
	int i;

	hab=WinInitialize(0);
	cbItems=WinQuerySwitchList(hab,NULL,0);
	ulBufSize = (cbItems * sizeof(SWENTRY)) + sizeof(HSWITCH);
	pswblk=(PSWBLOCK)malloc(ulBufSize);
	cbItems=WinQuerySwitchList(hab,pswblk,ulBufSize);
	for(i=0;i<cbItems;i++) {
		if(!strcmp(pswblk->aswentry[i].swctl.szSwtitle,wintitle)) {
			if(!num) {
				free(pswblk);
				WinTerminate(hab);
				return pswblk->aswentry[i].swctl.hwnd;
			}
			else {
				num--;
			}
		}
	}
	free(pswblk);
	WinTerminate(hab);
	return (HWND)0;
}
*/

#ifdef BUILDEXE
/*int printSwitchListTitles() {
	ULONG	titles,
			i;
	char *buffer;

	titles=getSwitchListTitles(NULL,0);
	printf("Titles in Switch List: %d\n",titles);
	if(titles==0) return 0;
	buffer=(char*) malloc(titles*SWITCHLISTTITLELENGTH);
	titles=getSwitchListTitles(buffer,titles);
	for(i=0;i<titles;i++) {
		printf("%s\n",&buffer[i*SWITCHLISTTITLELENGTH]);
	}
	free(buffer);
	return 0;
}

int main(int argc,char** argv) {
	WINDOW wnd;
	wnd.class[0]=0;
	wnd.id=0;
	wnd.pid=0;
	strcpy(wnd.file,"med.exe");


	findWindowHandle(HWND_DESKTOP,&wnd);
	printf("done...found '%s'\n",wnd.file);
	return 0;
}
*/
#endif

