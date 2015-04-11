/* bootdrv.cmd (spUtils Example Code)                                    */
/* The env variable specified as argument is set to the drive letter of  */
/* the boot volume, or if called without parameter, writes it to stdout  */

CALL RXFUNCADD 'spLoadFuncs','spUtils','spLoadFuncs'
CALL spLoadFuncs

PARSE ARG varname rest

IF varname='' THEN SAY spGetBootdrive()
ELSE CALL VALUE varname,spGetBootdrive(),'OS2ENVIRONMENT'

CALL spDropFuncs
