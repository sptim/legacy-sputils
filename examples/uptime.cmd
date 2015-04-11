/* uptime.cmd (spUtils Example Code)                                     */
/* writes the time since last IPL to stdout                              */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs

sec=spQuerySysInfo('QSV_MS_COUNT')%1000
SAY sec%86400      || ' day(s) '    ||,
    (sec%3600)//24 || ' hour(s) '   ||,
    (sec%60)//60   || ' minute(s) ' ||,
    sec//60        || ' second(s)'



