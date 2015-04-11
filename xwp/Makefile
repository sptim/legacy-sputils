# Makefile for GNU Make & Open Watcom 1.1

OBJECTS=procstat.obj

all: $(OBJECTS) .SYMBOLIC

.c.obj:
	$(CC) $[* $(CFLAGS)

wlink.lnk:	.SYMBOLIC
	for %i in ($(OBJECTS)) do %append $(LNKFILE) file $(PREFIX)\%i

rxfh2tbl:	.SYMBOLIC
	$(SCRIPTDIR)\$^@ $(TARGET) $(OBJECTS)

clean:	.SYMBOLIC
	-$(RM) *.obj *.dll *.err *.map *.sym *.exe

