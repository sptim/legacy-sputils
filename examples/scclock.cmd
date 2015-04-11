/* scclock.cmd (spUtils Example Code)                                    */
/* Switches the Smartcenter (aka WarpCenter or eComCenter) clock by      */
/* simulating a button click on the clock                                */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs
CALL spSetAutoSleep 0
focus=spQueryFocus()
PARSE VALUE spQueryPointerPos() WITH xptrpos yptrpos

hwnd=spFindWindowHandle(,,,'SmartCenter',,'\PMSHELL.EXE')
IF hwnd=0 THEN DO
	SAY 'Smartcenter (WarpCenter or eComCenter) not found'
	RETURN
END

xoffset=spQueryWindowPos(hwnd,'x')
yoffset=spQueryWindowPos(hwnd,'y')
xpos=spQueryWindowPos(hwnd,'cx')-5
ypos=5

CALL spSetPointerPos xoffset+xpos,yoffset+ypos
CALL spPostMsg hwnd,'WM_BUTTON1DOWN',spMPFROM2SHORT(xpos,ypos)
CALL spPostMsg hwnd,'WM_BUTTON1UP',spMPFROM2SHORT(xpos,ypos)
CALL spPostMsg focus,'WM_BUTTON1DOWN',spMPFROM2SHORT(0,0)
CALL spPostMsg focus,'WM_BUTTON1UP',spMPFROM2SHORT(0,0)
CALL spSetPointerPos xptrpos,yptrpos