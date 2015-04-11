/* findinf.cmd (spUtils Example Code)                                    */
/* Starts PMSEEK to find all *.INF files on the boot drive containing    */
/* the String passed as argument and sets VIEW.EXE as editor.            */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs

PARSE ARG searchstr
filemask=spGetBootdrive()||'*.INF'

CALL spSetAutoSleep 1000

'START PMSEEK 'filemask' 'searchstr

DO i=1 TO 2
	IF \spFindWindowHandles('hwnd',,,X2D('FA'),,,) THEN ITERATE
	DO j=1 TO hwnd.0
		hwndInput=spWindowFromId(hwnd.j,X2D('64'))
		IF hwndInput=0 THEN ITERATE
		IF spQueryWindowText(hwndInput)=filemask THEN DO
			CALL spSetDlgItemText hwnd.j,X2D('6E'),'VIEW.EXE'
			RETURN
		END
	END
END
SAY 'PMSEEK window not found'
