/* wininfo.cmd (spUtils Example Code)                                    */
/* waits for focus change and displays infos about the new focus window  */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs
CALL spSetAutoSleep 1000

hwnd=spQueryFocus()
SAY 'Change Focus to window control to be inspekted.'
do while hwnd=spQueryFocus()
	nop
end
SAY 'Focus change detected'
CALL spSetAutoSleep 50

hwnd=spQueryFocus()
CALL ShowInfo 'Focus window',hwnd
CALL ShowInfo 'Parent window',spQueryWindow(hwnd,'QW_PARENT')
CALL ShowInfo 'Owner window',spQueryWindow(hwnd,'QW_OWNER')
CALL ShowInfo 'Frame owner',spQueryWindow(hwnd,'QW_FRAMEOWNER')

RETURN

ShowInfo: PROCEDURE
	sp=LEFT('',20,' ')

	IF \spIsWindow(ARG(2)) THEN RETURN

	id=spQueryWindowID(ARG(2))
	IF spIsWindowEnabled(ARG(2)) THEN stat='enabled'; ELSE stat='disabled'
	IF spIsWindowShowing(ARG(2)) THEN showing='yes'; ELSE showing='no'
	IF spIsWindowVisible(ARG(2)) THEN visible='yes'; ELSE visible='no'

	SAY ''
	SAY LEFT(ARG(1),20,' ')||,
	        'text:    "'||spQueryWindowText(ARG(2))||'"'
	SAY sp||'handle:  '||ARG(2)||' (0x'||spD2X(ARG(2),'l')||')'
	SAY sp||'id:      '||id||' (0x'||D2X(id)||')'
	SAY sp||'state:   '||stat
	SAY sp||'showing: '||showing
	SAY sp||'visible: '||visible

	RETURN
