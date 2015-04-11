/* startlpx.cmd (spUtils Example Code)                                   */
/* start wps object <LAUNCHPAD_x> where x is current screen width        */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs

CALL RXFUNCADD 'SysSetObjectData','RexxUtil','SysSetObjectData'

obj='<LAUNCHPAD_'||spQuerySysValue('SV_CXSCREEN')||'>'
CALL SysSetObjectData obj,'OPEN=DEFAULT'

