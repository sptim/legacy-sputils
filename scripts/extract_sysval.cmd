REM Extract System Value IDs from Toolkit include file pmwin.h

SET LANG=de_DE
PERL EXTRACT_SYSVAL_CONV1.PL PMWIN.H | SORT | PERL EXTRACT_SYSVAL_CONV2.PL >sysvals.h
