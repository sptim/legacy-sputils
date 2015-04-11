/******************************************************************************
* spUtils                                                                     *
* -------                                                                     *
* Module: winapi/registry.c                                                   *
* Author: spTim@mecking.net                                                   *
******************************************************************************/

static const char *MODULE = "SPUTILS/WINAPI/REGISTRY";
static const char *id = "$Id: registry.c,v 1.2 2004-02-15 03:50:07 spTim Exp $";

#define INCL_REXXSAA
#include <os2.h>
#include <rexxsaa.h>
#define OS2DEF_INCLUDED
#include <win32api.h>
#include <string.h>
#include "sputils.h"

#define SPREG_ACTION_QUERY			0x0001
#define SPREG_ACTION_SET			0x0002
#define SPREG_ACTION_DELETEVALUE	0x0004
#define SPREG_ACTION_DELETEKEY		0x0008
#define SPREG_ACTION_ENUM_VALUES	0x0010
#define SPREG_ACTION_ENUM_SUBKEYS	0x0020
#define SPREG_ACTION_QUERYTYPE		0x0040

#define RegOpenKeyEx	_O32_RegOpenKeyEx
#define RegQueryValueEx	_O32_RegQueryValueEx
#define RegSetValueEx	_O32_RegSetValueEx
#define RegCreateKeyEx	_O32_RegCreateKeyEx
#define RegCloseKey		_O32_RegCloseKey
#define RegDeleteValue	_O32_RegDeleteValue
#define RegDeleteKey	_O32_RegDeleteKey
#define RegEnumKeyExA	_O32_RegEnumKeyExA
#define RegEnumValue	_O32_RegEnumValue

#define ERROR_SUCCESS			ERROR_SUCCESS_W
#define ERROR_FILE_NOT_FOUND	ERROR_FILE_NOT_FOUND_W
#define ERROR_NO_MORE_ITEMS		ERROR_NO_MORE_ITEMS_W

#define _O32_HKEY_LOCAL_MACHINE		0xFFFFFFEFL
#define _O32_HKEY_CURRENT_USER		0xFFFFFFEEL
#define _O32_HKEY_USERS				0xFFFFFFEDL
#define _O32_HKEY_CLASSES_ROOT		0xFFFFFFECL

/******************************************************************************
* Function Prototypes                                                         *
******************************************************************************/
RexxFunctionHandler spRegistry;

/* Helper Function Prototype */
BOOL parseKey(PUCHAR szKey, HKEY *pHKey, PUCHAR *pszSubkey);
LONG RecursiveDeleteKey(HKEY hParentKey, PUCHAR szKey);

/******************************************************************************
* Exported Functions                                                          *
******************************************************************************/

RXFUNC(spRegistry) {
	HKEY	hKeyRoot;
	HKEY	hKey;
	PUCHAR	szSubkey;
	UCHAR	szName[256];
	ULONG	ulSize;
	ULONG	ulType;
	PUCHAR	pValue;
	ULONG	ulValue;
	ULONG	ulDisposition;
	LONG	err;
	int		iAction;
	UCHAR	cSwap;
	int		i;
	FILETIME	ftime;


	CHKPARAM(1,3);

	setRexxVar("RC","0");

	if(!parseKey(args[0].strptr,&hKeyRoot,&szSubkey)) {
		BUILDRXSTRING(retstr,"ERROR:");
		return NO_ERROR;
	}

	szName[0]='\0';
	switch(numargs) {
		case 1:
			iAction=SPREG_ACTION_QUERY;
			break;
		case 2:
			if(stricmp(args[1].strptr,"DELETE:")) {
				iAction=SPREG_ACTION_QUERY;
				if(stricmp(args[1].strptr,"DEFAULT:")) {
					strncpy(szName,args[1].strptr,args[1].strlength+1);
				}
			}
			else {
				iAction=SPREG_ACTION_DELETEKEY;
			}
			break;
		case 3:
			if(stricmp(args[2].strptr,"DELETE:")) {
				iAction=SPREG_ACTION_SET;
				ulType=REG_SZ;
			}
			else {
				iAction=SPREG_ACTION_DELETEVALUE;
			}

			if(!stricmp(args[1].strptr,"VALUES:")) {
				iAction=SPREG_ACTION_ENUM_VALUES;
			}
			else if(!stricmp(args[1].strptr,"SUBKEYS:")) {
				iAction=SPREG_ACTION_ENUM_SUBKEYS;
			}
			else if(!stricmp(args[1].strptr,"TYPE:")) {
				iAction=SPREG_ACTION_QUERYTYPE;
				strncpy(szName,args[2].strptr,args[2].strlength+1);
			}
			else if(stricmp(args[1].strptr,"DEFAULT:")) {
				strncpy(szName,args[1].strptr,args[1].strlength+1);
			}
			break;
		case 4:
			if(!stricmp(args[4].strptr,"dword:")) {
				ulType=REG_DWORD;
			}
			else if(!stricmp(args[4].strptr,"hex:")) {
				ulType=REG_DWORD;
			}
			else if(args[4].strptr[0]=='\0') {
				ulType=REG_SZ;
			}
			else return INVALID_ROUTINE;
			iAction=SPREG_ACTION_SET;
			break;
		default:
			return INVALID_ROUTINE;
	}

	switch(iAction) {
		case	SPREG_ACTION_QUERYTYPE:
			if((err=RegOpenKeyEx(hKeyRoot,		// Root hKey
					szSubkey,					// Subkey
					0,							// Options (Reserved)
					KEY_QUERY_VALUE,			// Sam Desired
					&hKey))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			ulSize=0L;
			ulType=0L;
			if((err=RegQueryValueEx(hKey,		// hKey
					szName,						// Value Name
					NULL,						// Reserved
					&ulType,					// Value Type out
					NULL,						// Value out
					&ulSize))!=ERROR_SUCCESS) {	// Value Size in/out
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				RegCloseKey(hKey);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			switch(ulType) {
				case REG_DWORD_BIG_ENDIAN:
				case REG_DWORD:
					BUILDRXSTRING(retstr,"dword:");
					break;
				case REG_SZ:
				case REG_EXPAND_SZ:
					BUILDRXSTRING(retstr,"");
					break;
				case REG_BINARY:
					BUILDRXSTRING(retstr,"hex:");
					break;
				default:
					BUILDRXSTRING(retstr,"ERROR:");
			}
			RegCloseKey(hKey);
			return NO_ERROR;
		case	SPREG_ACTION_QUERY:
			if((err=RegOpenKeyEx(hKeyRoot,		// Root hKey
					szSubkey,					// Subkey
					0,							// Options (Reserved)
					KEY_QUERY_VALUE,			// Sam Desired
					&hKey))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}

			ulSize=0L;
			if((err=RegQueryValueEx(hKey,		// hKey
					szName,						// Value Name
					NULL,						// Reserved
					NULL,					// Value Type out
					NULL,						// Value out
					&ulSize))!=ERROR_SUCCESS) {	// Value Size in/out
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				RegCloseKey(hKey);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}

			if((pValue=(PVOID)malloc(ulSize))==NULL) {
				RegCloseKey(hKey);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}

			if((err=RegQueryValueEx(hKey,		// hKey
					szName,						// Value Name
					NULL,						// Reserved
					&ulType,					// Value Type out
					pValue,					// Value out
					&ulSize))!=ERROR_SUCCESS) {	// Value Size in/out
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				free(pValue);
				RegCloseKey(hKey);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			switch(ulType) {
				case REG_DWORD_BIG_ENDIAN:
					cSwap=((PUCHAR)pValue)[0];
					((PUCHAR)pValue)[0]=((PUCHAR)pValue)[3];
					((PUCHAR)pValue)[3]=cSwap;
					cSwap=((PUCHAR)pValue)[1];
					((PUCHAR)pValue)[1]=((PUCHAR)pValue)[2];
					((PUCHAR)pValue)[2]=cSwap;
				case REG_DWORD:
					RXRETURNUL(*((PULONG)pValue));
					break;
				case REG_SZ:
				case REG_EXPAND_SZ:
					if(ulSize) {
						RXRESIZE(retstr,ulSize);
						memcpy((PVOID)(retstr->strptr),pValue,(size_t)(ulSize));
						retstr->strptr[ulSize-1]='\0';
						retstr->strlength=strlen(retstr->strptr);
					}
					else {
						retstr->strlength=0;
					}
					break;
				default:
					if(ulSize) {
						RXRESIZE(retstr,ulSize);
						memcpy((PVOID)(retstr->strptr),pValue,(size_t)(ulSize));
					}
					else {
						retstr->strlength=0;
					}
					/*
					RXRESIZE(retstr,ulSize*2+1);
					for(i=0;i<ulSize;i++) {
						retstr->strptr[i*2]=(((PUCHAR)pValue)[i]>>4)+'0';
						if(retstr->strptr[i*2]>'9') retstr->strptr[i*2]+=7;

						retstr->strptr[(i*2)+1]=(((PUCHAR)pValue)[i]&0x0f)+'0';
						if(retstr->strptr[(i*2)+1]>'9') retstr->strptr[(i*2)+1]+=7;
					}
					*/
			}
			free(pValue);
			RegCloseKey(hKey);
			return NO_ERROR;

		case	SPREG_ACTION_SET:
			if((err=RegCreateKeyEx(hKeyRoot,			// Root hKey
					szSubkey,							// Subkey
					0,									// Options (Reserved)
					NULL,								// Class
					//REG_OPTION_NON_VOLATILE,			// Options
					0,									// Options
					KEY_SET_VALUE|KEY_CREATE_SUB_KEY,	// Sam Desired
					NULL,								// Security Attributes
					&hKey,
					&ulDisposition))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			if(!strnicmp(args[2].strptr,"DWORD:",6)) {
				ulValue=strtoul(&args[2].strptr[6],NULL,16);
				pValue=(PVOID)&ulValue;
				ulSize=sizeof(ULONG);
				ulType=REG_DWORD;
			}
			else if (!strnicmp(args[2].strptr,"HEX:",4)) {
				if((pValue=(PUCHAR)malloc(strlen(args[2].strptr)))==NULL) {
					return INVALID_ROUTINE;
				}
				i=0;
				while(args[2].strptr[4+i*3]!='\0') {
					if((args[2].strptr[4+i*3]>='0') && (args[2].strptr[4+i*3]<='9')) {
						((PUCHAR)pValue)[i]=(args[2].strptr[4+i*3]-'0')*16;
					}
					else if((args[2].strptr[4+i*3]>='a') && (args[2].strptr[4+i*3]<='f')) {
						((PUCHAR)pValue)[i]=(args[2].strptr[4+i*3]-'a'+10)*16;
					}
					else if((args[2].strptr[4+i*3]>='A') && (args[2].strptr[4+i*3]<='F')) {
						((PUCHAR)pValue)[i]=(args[2].strptr[4+i*3]-'A'+10)*16;
					}
					else return INVALID_ROUTINE;
					if((args[2].strptr[5+i*3]>='0') && (args[2].strptr[5+i*3]<='9')) {
						((PUCHAR)pValue)[i]+=(args[2].strptr[5+i*3]-'0');
					}
					else if((args[2].strptr[5+i*3]>='a') && (args[2].strptr[5+i*3]<='f')) {
						((PUCHAR)pValue)[i]+=(args[2].strptr[5+i*3]-'a'+10);
					}
					else if((args[2].strptr[5+i*3]>='A') && (args[2].strptr[5+i*3]<='F')) {
						((PUCHAR)pValue)[i]+=(args[2].strptr[5+i*3]-'A'+10);
					}
					else return INVALID_ROUTINE;
					if(args[2].strptr[6+i*3]=='\0') break;
					if(args[2].strptr[6+i*3]!=',') return INVALID_ROUTINE;
					i++;
				}
				ulSize=++i;
				ulType=REG_BINARY;
			}
			else {
				pValue=args[2].strptr;
				ulSize=strlen(args[2].strptr);
				ulType=REG_SZ;
			}
			if((err=RegSetValueEx(hKey,			// hKey
					szName,						// Value Name
					0,							// Reserved
					ulType,						// Type
					pValue,						// Size of Data
					ulSize))!=ERROR_SUCCESS) {
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
			}
			else {
				retstr->strlength=0;
			}
			RegCloseKey(hKey);
			return NO_ERROR;

		case	SPREG_ACTION_DELETEVALUE:
			retstr->strlength=0;
			if((err=RegOpenKeyEx(hKeyRoot,		// Root hKey
					szSubkey,					// Subkey
					0,							// Options (Reserved)
					KEY_SET_VALUE,				// Sam Desired
					&hKey))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			if((err=RegDeleteValue(hKey,		// hKey
					szName))!=ERROR_SUCCESS && err!=ERROR_FILE_NOT_FOUND) {	// Value Name
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
			}
			RegCloseKey(hKey);
			return NO_ERROR;

		case	SPREG_ACTION_DELETEKEY:
			retstr->strlength=0;
			if((err=RecursiveDeleteKey(hKeyRoot,szSubkey))!=ERROR_SUCCESS) {
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
			}
			/*if((err=RegOpenKeyEx(hKeyRoot,		// Root hKey
					szSubkey,					// Subkey
					0,							// Options (Reserved)
					KEY_WRITE,					// Sam Desired
					&hKey))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			if((err=RegDeleteKeyEx(hKeyRoot,	// Root hKey
					szSubkey,
					0,
					0))!=ERROR_SUCCESS) {
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
			}
			*/
			return NO_ERROR;

		case	SPREG_ACTION_ENUM_VALUES:
			retstr->strlength=0;
			if((err=RegOpenKeyEx(hKeyRoot,		// Root hKey
					szSubkey,					// Subkey
					0,							// Options (Reserved)
					KEY_QUERY_VALUE,				// Sam Desired
					&hKey))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			if((pValue=(PVOID)malloc(261))==NULL) {
				RegCloseKey(hKey);
				BUILDRXSTRING(retstr,"ERROR:");
				setRexxVar("RC","-1");
				return NO_ERROR;
			}
			ulSize=261;
			ulValue=0;
			while((err=RegEnumValue(hKey,		// hKey
					ulValue++,					// Index
					pValue,						// Value Name
					&ulSize,					// Value Name Length
					NULL,						// Reserved
					NULL,						// Value Type
					NULL,						// Value
					NULL))==ERROR_SUCCESS) {	// Value Length
				sprintf(szName,"%s.%d",args[2].strptr,ulValue);
				setRexxVar(szName,pValue);
				ulSize=261;
			}
			if(err!=ERROR_NO_MORE_ITEMS) {
				RegCloseKey(hKey);
				free(pValue);
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			ultoa(ulValue-1,pValue,10);
			sprintf(szName,"%s.0",args[2].strptr);
			setRexxVar(szName,pValue);
			BUILDRXSTRING(retstr,pValue);
			free(pValue);
			RegCloseKey(hKey);
			return NO_ERROR;

		case	SPREG_ACTION_ENUM_SUBKEYS:
			retstr->strlength=0;
			if((err=RegOpenKeyEx(hKeyRoot,		// Root hKey
					szSubkey,					// Subkey
					0,							// Options (Reserved)
					KEY_ENUMERATE_SUB_KEYS,		// Sam Desired
					&hKey))!=ERROR_SUCCESS) {	// Result hKey
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			if((pValue=(PVOID)malloc(261))==NULL) {
				RegCloseKey(hKey);
				BUILDRXSTRING(retstr,"ERROR:");
				setRexxVar("RC","-1");
				return NO_ERROR;
			}
			ulValue=0;
			ulSize=256;
			while((err=RegEnumKeyExA(hKey,
					ulValue++,
					pValue,
					&ulSize,
					NULL,
					NULL,
					NULL,
					&ftime))==ERROR_SUCCESS) {
				sprintf(szName,"%s.%d",args[2].strptr,ulValue);
				setRexxVar(szName,pValue);
				ulSize=256;
			}
			if(err!=ERROR_NO_MORE_ITEMS) {
				RegCloseKey(hKey);
				free(pValue);
				ltoa(err,szName,10);
				setRexxVar("RC",szName);
				BUILDRXSTRING(retstr,"ERROR:");
				return NO_ERROR;
			}
			ultoa(ulValue-1,pValue,10);
			sprintf(szName,"%s.0",args[2].strptr);
			setRexxVar(szName,pValue);
			BUILDRXSTRING(retstr,pValue);
			free(pValue);
			RegCloseKey(hKey);
			return NO_ERROR;
	}
	return INVALID_ROUTINE;
}

BOOL parseKey(UCHAR* szKey, HKEY *pHKey, PUCHAR *pszSubKey) {
	if(		!strnicmp(szKey,"HKLM\\",5) ||
			!strnicmp(szKey,"HKEY_LOCAL_MACHINE\\",19) ||
			!strnicmp(szKey,"LOCAL_MACHINE\\",14) ||
			!stricmp (szKey,"HKLM") ||
			!stricmp (szKey,"HKEY_LOCAL_MACHINE") ||
			!stricmp (szKey,"LOCAL_MACHINE")) {
		*pHKey=_O32_HKEY_LOCAL_MACHINE;
	}
	else if(!strnicmp(szKey,"HKCU\\",5) ||
			!strnicmp(szKey,"HKEY_CURRENT_USER\\",18) ||
			!strnicmp(szKey,"CURRENT_USER\\",13) ||
			!stricmp (szKey,"HKCU") ||
			!stricmp (szKey,"HKEY_CURRENT_USER") ||
			!stricmp (szKey,"CURRENT_USER")) {
		*pHKey=_O32_HKEY_CURRENT_USER;
	}
	else if(!strnicmp(szKey,"HKCR\\",5) ||
			!strnicmp(szKey,"HKEY_CLASSES_ROOT\\",18) ||
			!strnicmp(szKey,"CLASSES_ROOT\\",13) ||
			!stricmp (szKey,"HKCR") ||
			!stricmp (szKey,"HKEY_CLASSES_ROOT") ||
			!stricmp (szKey,"CLASSES_ROOT")) {
		*pHKey=_O32_HKEY_CLASSES_ROOT;
	}
	else if(!strnicmp(szKey,"HKU\\",4) ||
			!strnicmp(szKey,"HKEY_USERS\\",11) ||
			!strnicmp(szKey,"USERS\\",6) ||
			!stricmp (szKey,"HKU") ||
			!stricmp (szKey,"HKEY_USERS") ||
			!stricmp (szKey,"USERS")) {
		*pHKey=_O32_HKEY_USERS;
	}
	else {
		return FALSE;
	}
	if((*pszSubKey=strchr(szKey,'\\'))!=NULL) {
		(*pszSubKey)++;
	}
	return TRUE;
}

LONG RecursiveDeleteKey(HKEY hParentKey, PUCHAR szKey) {
	PUCHAR	szBuffer;
	PUCHAR	szSubkey;
	LONG	err;
	ULONG	ulSize;
	HKEY	hKey;
	FILETIME	ftime;
	int		i=0;

	if((szBuffer=(PVOID)malloc(256))==NULL) return -1;
	if((err=RegOpenKeyEx(hParentKey,		// Root hKey
			szKey,							// Subkey
			0,								// Options (Reserved)
			KEY_ALL_ACCESS,					// Sam Desired
			&hKey))!=ERROR_SUCCESS) {		// Result hKey
		free(szBuffer);
		return err;
	}
	while((err=RegEnumKeyExA(hKey,
			i++,
			szBuffer,
			&ulSize,
			NULL,
			NULL,
			NULL,
			&ftime))==ERROR_SUCCESS) {
		if((szSubkey=(PVOID)malloc(ulSize))==NULL) {
			RegCloseKey(hKey);
			return -1;
		}
		strcpy(szSubkey,szBuffer);
		free(szBuffer);
		if((err=RecursiveDeleteKey(hKey,szSubkey))!=ERROR_SUCCESS) {
			free(szSubkey);
			RegCloseKey(hKey);
			return err;
		}
		free(szSubkey);
		if((szBuffer=(PVOID)malloc(256))==NULL) {
			RegCloseKey(hKey);
			return -1;
		}
	}
	free(szBuffer);
	RegCloseKey(hKey);
	if(err!=ERROR_NO_MORE_ITEMS) return err;
	return RegDeleteKey(hParentKey,szKey);
}

