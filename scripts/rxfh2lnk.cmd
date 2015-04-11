/* Rxfh2lnk RexxFunctionHandler-to-Linkerfile */

PARSE ARG args

lnkfile=STRIP(args)
SAY lnkfile
CALL STREAM lnkfile,'C','OPEN READ'
i=1
DO WHILE LINES(lnkfile)>0
	PARSE VALUE LINEIN(lnkfile) WITH key file.i
	IF STRIP(TRANSLATE(key))='FILE' THEN i=i+1
END
file.0=i-1
CALL STREAM lnkfile,'C','CLOSE'

count=0
CALL STREAM lnkfile,'C','OPEN WRITE'
DO i=1 TO file.0
	f=LEFT(TRANSLATE(file.i,'/','\'),LASTPOS('.',file.i))||'c'
	SAY f
	CALL STREAM f,'C','OPEN READ'
	DO WHILE LINES(f)>0
		PARSE VALUE LINEIN(f) WITH w1 w2
		IF STRIP(w1)='RexxFunctionHandler' THEN DO
			PARSE VALUE w2 WITH w2';'w3
			CALL LINEOUT lnkfile,LEFT('export '||w2,32,' ')||'.'||count
			CALL LINEOUT lnkfile,LEFT('export '||TRANSLATE(w2),32,' ')||'= '||w2
			count=count+1
		END
		IF STRIP(w1)='CIntFunctionHandler' THEN DO
			PARSE VALUE w2 WITH w2'('w3
			CALL LINEOUT lnkfile,LEFT('export '||w2,32,' ')||'.'||count
			count=count+1
		END
		IF STRIP(w1)='CLongFunctionHandler' THEN DO
			PARSE VALUE w2 WITH w2'('w3
			CALL LINEOUT lnkfile,LEFT('export '||w2,32,' ')||'.'||count
			count=count+1
		END
		
	END
	CALL STREAM f,'C','CLOSE'
END
CALL STREAM lnkfile,'C','CLOSE'
