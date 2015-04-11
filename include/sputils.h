/******************************************************************************
*                                                                             *
*  spUtils                                                                    *
*                                                                             *
******************************************************************************/

#ifndef _SPUTILS_H
	#define _SPUTILS_H
	
	/* Rexx Macros */
	#define BUILDRXSTRING(t, s) {\
			strcpy((t)->strptr,(s));\
			(t)->strlength = strlen((s));\
		}
	
	#define BUILDNRXSTRING(t, s, n) {\
			if((n)>RXAUTOBUFLEN)\
				(t)->strptr=(char*)malloc((n));\
			(t)->strlength=(n);\
			memcpy((t)->strptr,(s),n)\
		}
	
	#define RXRESIZE(t, n) {\
			if((n)>RXAUTOBUFLEN)\
				(t)->strptr=(char*)malloc((n));\
			(t)->strlength=(n);\
		}
	
	#define RXRETURNUL(v) {\
			ultoa(v,retstr->strptr,10);\
			retstr->strlength=strlen(retstr->strptr);\
		}

	#define RXFUNC(f) ULONG f (PUCHAR name, ULONG numargs,\
			RXSTRING args[], PSZ queuename, RXSTRING* retstr)

	#ifndef INVALID_ROUTINE
		#define	INVALID_ROUTINE 40
	#endif
	#ifndef NO_ERROR
		#define	NO_ERROR	0
	#endif
	#ifndef	OUT_OF_MEMORY
		#define	OUT_OF_MEMORY	5
	#endif
	
	#define MAX_WINDOW_TEXT_LENGTH 409600
	
	#ifndef RXAUTOBUFLEN
		#define RXAUTOBUFLEN 256L
	#endif
	
	#define CHKPARAM(min,max) if(!chkParam(numargs,args,min,max))\
			return INVALID_ROUTINE;
	
	#define RXRET autoSleep(); return NO_ERROR;

	#define CIntFunctionHandler int
	#define CLongFunctionHandler long

	/* Function Prototypes for helper functions */
	UCHAR	setRexxVar(char*,char*);
	USHORT	getRexxVar(char*,char*,USHORT);
	PVOID	allocAndShare(ULONG,HWND);
	BOOL	chkParam(ULONG,RXSTRING*,ULONG,ULONG);
	
#endif

