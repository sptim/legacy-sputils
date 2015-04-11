/* killall.cmd (spUtils Example Code)                                    */
/* Kills all processes which run the specified exe file.                 */

CALL RXFUNCADD 'spKillAll','spUtils','spKillAll    '
PARSE UPPER ARG exe rest
IF POS('\',exe)=0 THEN exe='\' || exe
IF RIGHT(exe,4)\='.EXE' THEN exe=exe || '.EXE'

SAY spKillAll(exe) || ' process(es) killed'


