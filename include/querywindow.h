#define getLCode(str) (\
		(!stricmp(str,"QW_NEXT")		||!stricmp(str,"NEXT"))?		QW_NEXT:\
		(!stricmp(str,"QW_PREV")		||!stricmp(str,"PREV"))?		QW_PREV:\
		(!stricmp(str,"QW_TOP")			||!stricmp(str,"TOP"))?			QW_TOP:\
		(!stricmp(str,"QW_BOTTOM")		||!stricmp(str,"BOTTOM"))?		QW_BOTTOM:\
		(!stricmp(str,"QW_OWNER")		||!stricmp(str,"OWNER"))?		QW_OWNER:\
		(!stricmp(str,"QW_PARENT")		||!stricmp(str,"PARENT"))?		QW_PARENT:\
		(!stricmp(str,"QW_NEXTTOP")		||!stricmp(str,"NEXTTOP"))?		QW_NEXTTOP:\
		(!stricmp(str,"QW_PREVTOP")		||!stricmp(str,"PREVTOP"))?		QW_PREVTOP:\
		(!stricmp(str,"QW_FRAMEOWNER")	||!stricmp(str,"FRAMEOWNER"))?	QW_FRAMEOWNER:\
		(LONG)strtoul(str,NULL,10))
