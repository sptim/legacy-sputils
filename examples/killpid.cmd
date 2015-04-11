/* killpid.cmd (spUtils Example Code)                                    */
/* Kills the process with the pid passed as argument                     */

CALL RXFUNCADD 'spKillProcess','spUtils','spKillProcess'
PARSE ARG pid rest

IF spKillProcess(pid)\=0 THEN DO
	SAY 'Failed to kill pid ' || pid
END
ELSE DO
	SAY 'Pid ' || pid || ' has been killed.'
END

