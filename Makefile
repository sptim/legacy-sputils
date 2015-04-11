# Makefile for GNU Make & Open Watcom 1.1

!include config.mif

BLDLEVEL_VENDOR=mecking.net
BLDLEVEL_VERSION=0.1
BLDLEVEL_MINIVER=1
BLDLEVEL_INFO=spUtils - spTims Rexx Utility Package
BLDLEVEL_MODULE=SPUTILS.DLL

VERSION="$(BLDLEVEL_VERSION) (Preview Release 1)"

CC=wcc386
LD=wlink
CL=wcl386
RM=del
CP=copy
MAKE=wmake -h

CFLAGS=-bd -zq -zp4 -5s -os -fp5 -s -DVERSION="\"$(VERSION)\"" -DPROCSTAT32 -D_Optlink -i"$(%CWD)\include" -i"$(%CWD)\_xwp" -i"$(XWPHELPERS)\include" -i"$(ODIN32)\include"
#-i"$(%XWPHELPERS)\include\helpers"

SUBDIRS=base rexxapi dosapi pmapi winapi xwp

all:	.SYMBOLIC
	-$(RM) "$(%CWD)\include\rxfnctbl.h"
	for %i in ($(SUBDIRS)) do cd %i && $(MAKE) rxfh2tbl SCRIPTDIR="$(%CWD)\scripts" TARGET="$(%CWD)\include\rxfnctbl.h"
	%append include\rxfnctbl.h };
	for %i in ($(SUBDIRS)) do cd %i && $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)"
	$(MAKE) sputils.dll CC="$(CC)" CFLAGS="$(CFLAGS)"

sputils.lnk: .SYMBOLIC
	%create $^@
	%append $^@ system os2v2 dll initinstance terminstance
	%append $^@ name spUtils.dll
	%append $^@ library rexx
	%append $^@ library $(OS2TK)\lib\os2386.lib
	# %append $^@ library $(OS2TK)\lib\pmwinx.lib
	# %append $^@ library $(ODIN32)\lib\release\advapi32.lib
	$(%CWD)\scripts\adddescription.cmd $^@ PREFIX="option description '":SUFFIX="'":VENDOR="$(BLDLEVEL_VENDOR)":VERSION="$(BLDLEVEL_VERSION)":MINIVER="$(BLDLEVEL_MINIVER)":HOSTNAME="$(%HOSTNAME)":DESCRIPTION="$(BLDLEVEL_INFO)"
	# %append $^@ option description '$$@$#$(BLDLEVEL_VENDOR):$(BLDLEVEL_VERSION) ($(BLDLEVEL_MODULE))$#@$(BLDLEVEL_INFO)'
	%append $^@ option modname=SPUTILS
	%append $^@ option caseexact
	%append $^@ option map=spUtils.map
	# %append $^@ option symfile
	%append $^@ option manyautodata
	%append $^@ option quiet
	%append $^@ segment class 'code' loadoncall
	%append $^@ segment class 'data' loadoncall nonshared readwrite
	for %i in ($(SUBDIRS)) do cd %i && $(MAKE) wlink.lnk LNKFILE="$(%CWD)\$^@" PREFIX="%i"
	scripts\rxfh2lnk $^@

sputils.dll:	$(OBJECTS) sputils.lnk
	$(LD) @sputils.lnk

sputils.zip:	sputils.dll doc\sputils.pdf
	-$(RM) sputils.zip
	cd doc && zip -9 ..\$@ sputils.pdf
	zip -9 $@ sputils.dll examples\*

clean:	.SYMBOLIC
	-$(RM) "$(%CWD)\include\rxfnctbl.h"
	-$(RM) *.obj *.dll *.err *.map *.sym *.exe *.lnk
	for %i in ($(SUBDIRS)) do cd %i && $(MAKE) clean RM="$(RM)"

#$(MODULE).obj:	$(MODULE).c msg/messages.c
#	$(CC) -bd -zq -j -zp4 -5s -os -fp5 -s -D__MIG_LIB__ -D_Optlink $(MODULE).c
#
#$(ADDITIONAL).obj:	$(ADDITIONAL).c
#	$(CC) -iF:\DEVEL\NETLABS\XWPHELPERS\include -bd -zq -j -zp4 -5s -os -fp5 -s -DPROCSTAT32 -D__MIG_LIB__ -D_Optlink $(ADDITIONAL).c
#
#$(MODULE).dll:	$(MODULE).obj $(ADDITIONAL).obj
#	$(LD) @$(MODULE)
#
#$(MODULE)-exe.obj:	$(MODULE).c
#	$(CC) -dBUILDEXE -fo=$@ $<
#
#$(MODULE).exe:	$(MODULE).c $(ADDITIONAL).c
#	$(CL) -i=F:\DEVEL\NETLABS\XWPHELPERS\include -bw -dBUILDEXE -dPROCSTAT32 $^ rexx.lib
#
#msg/messages.c:
#	cd msg && wmake
#
#install:	.SYMBOLIC $(MODULE).dll
#	$(CP) $< $(TARGET)
#
#clean:	.SYMBOLIC
#	-$(RM) *.obj *.dll *.err *.map *.sym *.exe
#	cd msg && wmake clean
