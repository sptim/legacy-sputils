# Makefile for GNU Make & Open Watcom 1.1

OBJECTS=pointers.obj dialogs.obj
OBJECTS+= messagemgr.obj input.obj sys.obj
OBJECTS+= windowmgr.obj
OBJECTS+= spwin.obj

all: $(OBJECTS) .SYMBOLIC

.c.obj:
	$(CC) $[* $(CFLAGS)

wlink.lnk:	.SYMBOLIC
	for %i in ($(OBJECTS)) do %append $(LNKFILE) file $(PREFIX)\%i

rxfh2tbl:	.SYMBOLIC
	$(SCRIPTDIR)\$^@ $(TARGET) $(OBJECTS)

clean:	.SYMBOLIC
	-$(RM) *.obj *.dll *.err *.map *.sym *.exe

