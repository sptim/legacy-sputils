REM Extract Query SysInfo Value IDs from Toolkit include file bsedos.h

SET LANG=de_DE
PERL EXTRACT_SYSINFO_CONV1.PL \devel\os2tk45\h\BSEDOS.H | SORT | PERL EXTRACT_SYSINFO_CONV2.PL >qsysinfval.h
