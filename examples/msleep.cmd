/* msleep.cmd (spUtils Example Code)                                     */
/* Waits the given number of milliseconds                                */

CALL RXFUNCADD 'spSleep','spUtils','spSleep'
PARSE ARG ms rest

CALL spSleep ms
