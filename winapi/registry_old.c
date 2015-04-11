/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: winapi/registry.c                                                   *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/WINAPI/REGISTRY";
static const char *id = "$Id: registry_old.c,v 1.1 2003-12-17 01:52:02 spTim Exp $";

//#define _O32_RegOpenKey			RegOpenKey
//#define _O32_RegOpenKeyEx		RegOpenKeyEx
//#define _O32_RegCreateKeyEx		RegCreateKeyEx
//#define _O32_RegCloseKey		RegCloseKey
//#define _O32_RegQueryValueEx	RegQueryValueEx
//#define _O32_RegSetValueEx		RegSetValueEx

#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#define OS2DEF_INCLUDED
#include <win32api.h>
#include <winconst.h>
//#include <os2win.h>
#include <stdlib.h>
#include "sputils.h"
#include <stdio.h>
#include <string.h>

//#define RegOpenKey			_O32_RegOpenKey
//#define RegOpenKeyEx		_O32_RegOpenKeyEx
//#define RegCreateKeyEx		_O32_RegCreateKeyEx
//#define RegCloseKey			_O32_RegCloseKey
//#define RegQueryValueEx		_O32_RegQueryValueEx
//#define RegSetValueEx		_O32_RegSetValueEx

/* From Wine winreg.h */
#define REG_OPTION_RESERVED				0x00000000
#define REG_OPTION_NON_VOLATILE			0x00000000
#define REG_OPTION_VOLATILE				0x00000001
#define REG_OPTION_CREATE_LINK			0x00000002
#define REG_OPTION_BACKUP_RESTORE		0x00000004 /* FIXME */
#define REG_OPTION_TAINTED				0x80000000 /* Internal? */

/* Found on the Web */
#define REG_CREATED_NEW_KEY				0x00000001

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/

RexxFunctionHandler spRegOpenKey;
RexxFunctionHandler spRegCreateKey;
// RexxFunctionHandler spRegConnectRegistry;
RexxFunctionHandler spRegCloseKey;
RexxFunctionHandler spRegQueryValue;
// RexxFunctionHandler spRegQueryValueType;
RexxFunctionHandler spRegSetValue;
// RexxFunctionHandler spRegDeleteKey;
// RexxFunctionHandler spRegDeleteValue;
// RexxFunctionHandler spRegEnumKey;
// RexxFunctionHandler spRegEnumValue;
// RexxFunctionHandler spRegFlushKey;
// RexxFunctionHandler spRegGetKeySecDesc;
// RexxFunctionHandler spRegSetKeySecDesc;
// RexxFunctionHandler spRegQueryInfoKey;
// RexxFunctionHandler spRegSaveKey;
// RexxFunctionHandler spRegRestoreKey;
// RexxFunctionHandler spRegLoadKey;
// RexxFunctionHandler spRegUnloadKey;
// RexxFunctionHandler spExpandEnvironmentStrings;

/* Helper Functions */
static HKEY getHKEY(char*);
ULONG getRegTypeUl(char*);

/*RXFUNC(spRegTest) {
	char buffer[255];
	
	CHKPARAM(0,0);
	
	if(RegCreateKey(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\Windows",&hKey
*/
/******************************************************************************
* Syntax: hKey = spRegOpenKey ( key, [subkey] )                               *
* Desc: Opens a sub-key of an already-open key, or opens a new handle to an   *
*       already-open key. <i>key</i> can be one of the special key names      *
*       listed above, or the return code from a previous call to one of the   *
*       key opening routines. If <i>subkeyname</i> is not specified, a new    *
*       handle to the <i>key</i> specified by key will be returned.           *
*       <br>                                                                  *
*       If the function is not successful, it returns 0.                      *
******************************************************************************/
RXFUNC(spRegOpenKey) {
	HKEY hKey;
	ULONG err;
	char errbuff[13];
	
	CHKPARAM(1,2);
	
	ultoa(getHKEY(args[0].strptr),errbuff,16);
	setRexxVar("HK",errbuff);
	setRexxVar("SUBKEY",(numargs>1 && args[1].strlength>0)?args[1].strptr:"NULL");
	if((err=RegOpenKeyEx(getHKEY(args[0].strptr),
			(numargs>1 && args[1].strlength>0)?args[1].strptr:NULL,
			0,
			KEY_READ, //MAXIMUM_ALLOWED_W,
			&hKey))!=ERROR_SUCCESS_W) {

//	if((err=RegOpenKey(HKEY_CURRENT_USER,(numargs>1 && args[1].strlength>0)?args[1].strptr:NULL,&hKey))!=ERROR_SUCCESS_W) {
		BUILDRXSTRING(retstr,"0");
	}
	else {
		RXRETURNUL(hKey);
	}
	
	setRexxVar("RC",ultoa(err,errbuff,10));
	return NO_ERROR;
	
}

/******************************************************************************
* Syntax: hKey = spRegCreateKey ( key, subkey [,created] )                    *
* Desc: Creates or opens a sub-key of an already-open key. <i>key</i> can be  *
*       one of the special key names listed in section 4 above, or the return *
*       code from a previous call to one of the key opening routines. If the  *
*       sub-key already exists, it is opened, rather than created.            *
*       <br>                                                                  *
*       If the third argument is specified, it must evaluate to a variable    *
*       name. If the key has been created, "1" will be written to that        *
*       variable, otherwise, if the key existed before, "0" will be written.  *
*       <br>                                                                  *
*       If the function is not successful, it returns 0.                      *
******************************************************************************/
RXFUNC(spRegCreateKey) {
	HKEY hKey;
	ULONG disp;
	ULONG err;
	char errbuff[13];
	
	CHKPARAM(2,3);
	
	ultoa(getHKEY(args[0].strptr),errbuff,10);
	setRexxVar("HK",errbuff);
	if((err=RegCreateKeyEx(getHKEY(args[0].strptr),
			args[1].strptr,
			0,
			"",
			REG_OPTION_NON_VOLATILE,
			MAXIMUM_ALLOWED_W,
			NULL,
			&hKey,
			&disp))!=ERROR_SUCCESS_W) {
		BUILDRXSTRING(retstr,"0");
	}
	else {
		RXRETURNUL(hKey);
		if(numargs==3 && args[2].strlength>0)
			setRexxVar(args[0].strptr,(disp==REG_CREATED_NEW_KEY)?"1":"0");
	}
	
	ultoa(err,errbuff,10);
	setRexxVar("ERR",errbuff);
	return NO_ERROR;
	
}

/******************************************************************************
* Syntax: ret = spRegCloseKey ( hKey )                                        *
* Desc: Closes a previously opened registry key. <i>key</i> must be the       *
*       return code from a previous call to one of the key opening routines.  *
*       If the function is not successful, it returns 1, otherwise, it        *
*       returns 0.                                                            *
******************************************************************************/
RXFUNC(spRegCloseKey) {
	
	CHKPARAM(1,1);
	
//	BUILDRXSTRING(retstr,
//			(RegCloseKey(strtoul(args[0].strptr,NULL,10))==ERROR_SUCCESS_W)?"1":"0");
	
	return NO_ERROR;
}

/******************************************************************************
* Syntax: value = spRegQueryValue ( hKey, entry [, type] )                    *
* Desc: Retrieves the value of the value named entry from the key identified  *
*       by key. If the third argument is specified, it must evaluate to a     *
*       variable name, and the type of the value will be written to that      *
*       variable. See <a href="#spRegQueryValueType">spRegQueryValueType</a>  *
*       for a list of the possible types.                                     *
*       <br>                                                                  *
*       Values are always returned as strings, regardless of the type of data *
*       in the registry. For instance, a REG DWORD value will be returned as  *
*       an ordinary rexx integer, rather than as a 4-byte binary value.       *
*       <br>                                                                  *
*       If there is no such entry in the key, sets RC to 1 and returns the    *
*       empty string. Otherwise it sets RC to 0.                              *
******************************************************************************/

RXFUNC(spRegQueryValue) {
	HKEY	hKey;
	ULONG	ulType;
	ULONG	cbData;
	PUCHAR	pData;
	UCHAR	buffer[13];
	ULONG	i;
	
	CHKPARAM(2,3);
	
	hKey=getHKEY(args[0].strptr);
	
	if(RegQueryValueEx(hKey,
			args[1].strptr,
			NULL,
			&ulType,
			NULL,
			&cbData)!=ERROR_SUCCESS_W) {
		BUILDRXSTRING(retstr,"");
		setRexxVar("RC","1");
		return NO_ERROR;
	}
	
	if((pData=malloc((size_t)cbData))==NULL) {
		return OUT_OF_MEMORY;
	}
	
	if(RegQueryValueEx(hKey,
			args[1].strptr,
			NULL,
			&ulType,
			pData,
			&cbData)!=ERROR_SUCCESS_W) {
			free(pData);
		return INVALID_ROUTINE;
	}
	
	switch(ulType) {
		case REG_DWORD_BIG_ENDIAN:
			buffer[0]=pData[3];
			pData[3]=pData[0];
			pData[0]=buffer[0];
			buffer[0]=pData[2];
			pData[2]=pData[1];
			pData[1]=buffer[0];
		case REG_DWORD:
			RXRETURNUL(*((PULONG)pData));
			break;
		case REG_EXPAND_SZ:
		case REG_SZ:
			if(cbData) {
				RXRESIZE(retstr,cbData-1);
				memcpy(retstr,pData,cbData-1);
			}
			else {
				retstr->strlength=0;
			}
			break;
		default:
			RXRESIZE(retstr,cbData*2+1);
			for(i=0;i<cbData;i++) {
				sprintf(&pData[i*2],"%02x",pData[i]);
			}
	}
	
	free(pData);
	setRexxVar("RC","0");
	return NO_ERROR;
}

/******************************************************************************
* Syntax: value = spRegSetValue ( hKey, entry, type, data )                   *
* Desc:                                                                       *
******************************************************************************/

RXFUNC(spRegSetValue) {
	HKEY	hKey;
	ULONG	ulType;
	PUCHAR	pData;
	ULONG	cbData;
	ULONG	ulData;
	UCHAR	cBuf;
	ULONG	i;
	
	CHKPARAM(4,4);
	
	hKey=getHKEY(args[0].strptr);
	ulType=getRegTypeUl(args[2].strptr);
	
	switch(ulType) {
		case REG_DWORD:
			ulData=strtoul(args[3].strptr,NULL,10);
			pData=(PUCHAR)&ulData;
			cbData=4;
			break;
		case REG_DWORD_BIG_ENDIAN:
			ulData=strtoul(args[3].strptr,NULL,10);
			pData=(PUCHAR)&ulData;
			cBuf=pData[0];
			pData[0]=pData[3];
			pData[3]=cBuf;
			cBuf=pData[1];
			pData[2]=pData[1];
			pData[1]=cBuf;
			cbData=4;
			break;
		case REG_EXPAND_SZ:
		case REG_SZ:
			cbData=args[3].strlength+1;
			pData=(PUCHAR)args[3].strptr;
			break;
		case 0:
			BUILDRXSTRING(retstr,"1");
			return NO_ERROR;
		default:
			if(args[3].strlength%2!=0) {
				BUILDRXSTRING(retstr,"1");
				return NO_ERROR;
			}
			cbData=args[3].strlength/2;
			if((pData=malloc(cbData))==NULL) {
				BUILDRXSTRING(retstr,"1");
				return OUT_OF_MEMORY;
			}
			for(i=0;i<cbData;i++) {
				pData[i]=(args[3].strptr[(2*i)]-'0')*16+args[3].strptr[(2*i)+1];
			}
			break;
	}
	
	BUILDRXSTRING(retstr,(RegSetValueEx(hKey,
			args[1].strptr,
			0,
			ulType,
			pData,
			cbData)==ERROR_SUCCESS_W)?"0":"1");
	
	if(ulType!=REG_DWORD &&
			ulType!=REG_DWORD_BIG_ENDIAN &&
			ulType!=REG_EXPAND_SZ &&
			ulType!=REG_SZ &&
			ulType!=0) {
		free(pData);
	}
	
	return NO_ERROR;
}

/******************************************************************************
* Helper Functions                                                            *
******************************************************************************/
static HKEY getHKEY(char* szKey) {
	
	if(strlen(szKey)<5) {
		return strtoul(szKey,NULL,10);
	}
	else if(!stricmp(szKey,"HKEY_CLASSES_ROOT") ||
			!stricmp(szKey,"CLASSES_ROOT")) {
		return HKEY_CLASSES_ROOT;
	}
	else if(!stricmp(szKey,"HKEY_CURRENT_USER") ||
			!stricmp(szKey,"CURRENT_USER")) {
		return HKEY_CURRENT_USER;
}
	else if(!stricmp(szKey,"HKEY_LOCAL_MACHINE") ||
			!stricmp(szKey,"LOCAL_MACHINE")) {
		return HKEY_LOCAL_MACHINE;
	}
	else if(!stricmp(szKey,"HKEY_USERS") ||
			!stricmp(szKey,"USERS")) {
		return HKEY_USERS;
	}
	return strtoul(szKey,NULL,10);
}

char* getRegTypeString(ULONG ulType) {
	char buffer[13];
	
	if(ulType==REG_BINARY)					return "REG_BINARY";
	else if(ulType==REG_DWORD)				return "REG_DWORD";
	else if(ulType==REG_DWORD_BIG_ENDIAN)	return "REG_DWORD_BIG_ENDIAN";
	else if(ulType==REG_EXPAND_SZ)			return "REG_EXPAND_SZ";
	else if(ulType==REG_LINK)				return "REG_LINK";
	else if(ulType==REG_MULTI_SZ)			return "REG_MULTI_SZ";
	else if(ulType==REG_NONE)				return "REG_NONE";
	else if(ulType==REG_RESOURCE_LIST)		return "REG_RESOURCE_LIST";
	else if(ulType==REG_SZ)					return "REG_SZ";
	else									return ultoa(ulType,buffer,10);
}

ULONG getRegTypeUl(char* szType) {
	
	if(	!stricmp(szType,"REG_BINARY") ||
			!stricmp(szType,"BINARY"))				return REG_BINARY;
	else if(!stricmp(szType,"REG_DWORD") ||
			!stricmp(szType,	"DWORD"))			return REG_DWORD;
	else if(!stricmp(szType,"REG_DWORD_BIG_ENDIAN") ||
			!stricmp(szType,	"DWORD_BIG_ENDIAN"))return REG_DWORD_BIG_ENDIAN;
	else if(!stricmp(szType,"REG_EXPAND_SZ") ||
			!stricmp(szType,	"EXPAND_SZ"))		return REG_EXPAND_SZ;
	else if(!stricmp(szType,"REG_LINK") ||
			!stricmp(szType,	"LINK"))			return REG_LINK;
	else if(!stricmp(szType,"REG_MULTI_SZ") ||
			!stricmp(szType,	"MULTI_SZ"))		return REG_MULTI_SZ;
	else if(!stricmp(szType,"REG_NONE") ||
			!stricmp(szType,	"NONE"))			return REG_NONE;
	else if(!stricmp(szType,"REG_RESOURCE_LIST") ||
			!stricmp(szType,	"RESOURCE_LIST"))	return REG_RESOURCE_LIST;
	else if(!stricmp(szType,"REG_SZ") ||
			!stricmp(szType,	"SZ"))				return REG_SZ;
	else return 0xffffffff;
}
