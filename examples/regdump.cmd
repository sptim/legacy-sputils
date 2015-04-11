/* regdump.cmd (spUtils Example Code)                                    */
/* Prints out a registry key with all values and subvalues, like the     */
/* Registry Export function of Regedit                                   */

PARSE ARG args
IF RIGHT(args,1)='\' THEN args=LEFT(args,LENGTH(args)-1)

SAY 'REGEDIT4'
SAY ''
CALL RegDump args
RETURN

RegDump:	PROCEDURE
	SAY '['||ARG(1)||']'

	CALL KeyDump ARG(1)

	CALL spRegistry ARG(1),'SUBKEYS:','s'
	DO i=1 TO s.0
		CALL RegDump ARG(1)||'\'||s.i
	END

	RETURN

KeyDump:	PROCEDURE
	CALL spRegistry ARG(1),'VALUES:','v'
	DO i=1 to v.0

		CALL ValDump ARG(1), v.i

	END
	CALL LINEOUT ,''
	RETURN

ValDump:	PROCEDURE
	IF ARG(2)='' THEN DO
		val=spRegistry(ARG(1),'DEFAULT:')
		type=''
		out='@='
	END
	ELSE DO
		val=spRegistry(ARG(1),ARG(2))
		type=spRegistry(ARG(1),'TYPE:',ARG(2))
		out='"'
		DO i=1 TO LENGTH(ARG(2))
			c=SUBSTR(ARG(2),i,1)
			IF c='\' | c='"' THEN out=out||'\'
			out=out||c
		END
		out=out||'"='type
	END
	SELECT
		WHEN type='dword:' THEN CALL DWordDump  val
		WHEN type='hex:'   THEN CALL HexDump    val
		WHEN type=''       THEN CALL StringDump val
		OTHERWISE EXIT 1
	END
	RETURN

DWordDump:	PROCEDURE EXPOSE out
	DO i=1 TO LENGTH(out)
		c=SUBSTR(out,i,1)
	CALL LINEOUT ,out||spD2X(ARG(1),'l')
	RETURN

HexDump:	PROCEDURE EXPOSE out
	DO i=1 TO LENGTH(ARG(1))
		IF i>1 & LENGTH(out)>3 THEN out=out||','
		out=out||spD2X(C2D(SUBSTR(ARG(1),i,1)),'b')

		IF LENGTH(out)>75 & i<=LENGTH(ARG(1)) THEN DO
			CALL LINEOUT ,out||',\'
			out='  '
		END
	END
	IF out\='' THEN CALL LINEOUT ,out
	RETURN

StringDump:	PROCEDURE EXPOSE out
	CALL CHAROUT ,out||'"'
	posx=LENGTH(out)+1
	DO i=1 TO LENGTH(ARG(1))
		IF i>1 & posx>76 & i<LENGTH(ARG(1)) THEN DO
			CALL LINEOUT ,'"\'
			CALL CHAROUT ,'"'
			posx=1
		END
		c=SUBSTR(ARG(1),i,1)
		IF c='\' | c='"' THEN DO
			CALL CHAROUT ,'\'
			posx=posx+1
		END
		CALL CHAROUT ,c
		posx=posx+1
	END
	CALL LINEOUT ,'"'
	RETURN

