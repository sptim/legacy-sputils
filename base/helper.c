#define INCL_DOSMEMMGR
#define INCL_WINWINDOWMGR
#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#include "sputils.h"

extern int AutoSleepTime;

void autoSleep() {
	DosSleep(AutoSleepTime);
}

BOOL chkParam(ULONG numargs, RXSTRING args[], ULONG min, ULONG max) {
	ULONG i;
	
	if(numargs<min || numargs>max) return FALSE;
	for(i=0;i<numargs;i++)
		if(!RXVALIDSTRING(args[i])) return FALSE;
	return TRUE;
}

PVOID allocAndShare(ULONG size,HWND hwnd) {
	PVOID pBuffer;
	PID pid;
	TID tid;
	
	if(DosAllocSharedMem(&pBuffer,NULL,size,PAG_COMMIT|
											OBJ_GIVEABLE|
											OBJ_GETTABLE|
											OBJ_TILE|
											PAG_READ|
											PAG_WRITE)!=0) return NULL;
	if(!WinQueryWindowProcess(hwnd, &pid, &tid)) {
		DosFreeMem(pBuffer);
		return NULL;
	}
	if(DosGiveSharedMem(pBuffer,pid,PAG_READ|PAG_WRITE)!=0) {
		DosFreeMem(pBuffer);
		return NULL;
	}
	
	return pBuffer;
}

UCHAR setRexxVar(char* name,char* value) {
	SHVBLOCK	shvb;
	strupr(name);
	shvb.shvnext=NULL;
	shvb.shvname.strptr=name;
	shvb.shvname.strlength=strlen(name);
	shvb.shvvalue.strptr=value;
	shvb.shvvalue.strlength=strlen(value);
	shvb.shvnamelen=strlen(name);
	shvb.shvvaluelen=strlen(value);
	shvb.shvcode = RXSHV_SET;
	shvb.shvret=(UCHAR)0;
	return RexxVariablePool(&shvb);
}

USHORT getRexxVar(char* name,char* value,USHORT length) {
	SHVBLOCK	shvb;
	strupr(name);
	shvb.shvnext=NULL;
	shvb.shvname.strptr=name;
	shvb.shvname.strlength=strlen(name);
	shvb.shvvalue.strptr=value;
	shvb.shvvalue.strlength=length;
	shvb.shvnamelen=strlen(name);
	shvb.shvvaluelen=length;
	shvb.shvcode = RXSHV_FETCH;
	RexxVariablePool(&shvb);
	return shvb.shvvalue.strlength;
}

