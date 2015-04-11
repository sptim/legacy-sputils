/* spversion.cmd (spUtils Example Code)                                  */
/* Writes the version of the sputils.dll to stdout                       */

CALL RXFUNCADD 'spVersion','spUtils','spVersion'
SAY spVersion()

