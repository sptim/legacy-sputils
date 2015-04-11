/* ps.cmd (spUtils Example Code)                                         */
/* List all currently active processes with pid and module name,         */
/* optionally a filter substring can be passed as argument.              */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs

CALL spGetPidList 'p',ARG(1)
DO i=1 to p.0
	SAY RIGHT(p.i,5,' ') || ' ' || spFilenameFromPid(p.i)
END
