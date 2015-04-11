/* */
PARSE ARG filename variables

prefix=''
suffix=''
vendor=''
version=''
datetime=left(' 'date()' 'time(), 26)
hostname=strip(substr(VALUE('HOSTNAME',,'OS2ENVIRONMENT'), 1, 11));
asdfeatureid=''
lang=''
country=''
miniver=''
fixpackver=''
description=''

INTERPRET(TRANSLATE(variables,';',':'))

Vendor         = strip(substr(Vendor, 1, 31));
if (substr(DateTime, 1, 1) <> ' ') then DateTime = ' ' || DateTime;
DateTime       = left(DateTime, 26);
Hostname       = strip(substr(Hostname, 1, 11));
MiniVer        = strip(substr(MiniVer, 1, 11));
Description    = strip(substr(Description, 1, 80));
Country        = strip(substr(Country, 1, 4));
Language       = strip(substr(Lang, 1, 4));
ASDFeatureId   = strip(substr(ASDFeatureId, 1, 11));
FixPakVer      = strip(substr(FixPakVer, 1, 11));


CALL STREAM filename,'C','OPEN WRITE'
CALL LINEOUT filename,prefix || '@#' || vendor || ':' || version || '#@##1##' || datetime || hostname || ':' || asdfeatureid || ':' || lang || ':' || country || ':' || miniver || '::' || fixpackver || '@@' || description || suffix
CALL STREAM filename,'C','CLOSE'


