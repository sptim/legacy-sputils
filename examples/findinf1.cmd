/* findinf1.cmd (spUtils Example Code)                                   */
/* Starts PMSEEK to find all *.INF files on the boot drive containing    */
/* the String passed as argument and writes a summery of the results to  */
/* stdout.                                                               */
CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs
PARSE ARG searchstr
'START PMSEEK '||spGetBootdrive()||'*.INF '||searchstr
CALL spSleep 1000
hwnd=spFindWindowHandle(,,X2D('FA'),,,'PMSEEK.EXE')
IF hwnd=0 THEN DO
	SAY 'PMSeek window not found'
	RETURN
END
DO FOREVER
	text=spQueryDlgItemText(hwnd,X2D('3C'))
	IF LEFT(text,15)='Search Complete' THEN LEAVE
END
SAY text
SAY LEFT('',spQueryDlgItemTextLength(hwnd,X2D('3C')),'-')