/* rxfh2tbl.cmd - RexxFunctionHandler-to-functiontable */

PARSE ARG resultfile sourcefiles

/* Open Resultfile */
i=1
IF STREAM(resultfile,'C','QUERY EXISTS')='' THEN i=0
CALL STREAM resultfile,'C','OPEN WRITE'

IF sourefiles='CLOSETABLE' THEN DO
	CALL LINEOUT resultfile,X2C('09')||'};'
END
ELSE DO
	DO WHILE sourcefiles\=''
		PARSE VALUE sourcefiles WITH sourcefile sourcefiles
		sourcefile=LEFT(TRANSLATE(sourcefile),LASTPOS('.',sourcefile))||'C'
		CALL STREAM sourcefile,'C','OPEN READ'
		j=i
		CALL CHAROUT, sourcefile': '
		DO WHILE LINES(sourcefile)>0
			PARSE VALUE LINEIN(sourcefile) WITH key val';'
			IF STRIP(key)='RexxFunctionHandler' THEN DO
				IF i\=0 THEN DO
					CALL LINEOUT resultfile,","
				END
				ELSE DO
					CALL LINEOUT resultfile,"static PSZ RexxFunctions[] = {"
				END
				IF i=j THEN DO
					CALL LINEOUT resultfile,''
					CALL LINEOUT resultfile,X2C('09')||'/* '||substr(directory(),lastpos('\',directory())+1)||'\'||sourcefile||' */'
					CALL LINEOUT resultfile,''
				END
				CALL CHAROUT resultfile,X2C('09')||'"'||STRIP(val)||'"'
				i=i+1
			END
		END
		CALL STREAM sourcefile,'C','CLOSE'
		CALL LINEOUT ,(i-j)||' Rexx Functions'
	END
END

/* Close Resultfile */
CALL STREAM resultfile,'C','CLOSE'
