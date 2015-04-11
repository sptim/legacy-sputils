# Makefile for GNU Make & Open Watcom 1.1

OBJECTS=registry.obj

all: $(OBJECTS) .SYMBOLIC

.c.obj:
	$(CC) $[* $(CFLAGS)

wlink.lnk:	.SYMBOLIC
	for %i in ($(OBJECTS)) do %append $(LNKFILE) file $(PREFIX)\%i
	%append $(LNKFILE) import _O32_RegOpenKeyEx		REGISTRY.RegOpenKeyExA
	%append $(LNKFILE) import _O32_RegQueryValueEx	REGISTRY.RegQueryValueExA
	%append $(LNKFILE) import _O32_RegCloseKey		REGISTRY.RegCloseKey
	%append $(LNKFILE) import _O32_RegCreateKeyEx	REGISTRY.RegCreateKeyExA
	%append $(LNKFILE) import _O32_RegSetValueEx	REGISTRY.RegSetValueExA
	%append $(LNKFILE) import _O32_RegDeleteValue	REGISTRY.RegDeleteValueA
	%append $(LNKFILE) import _O32_RegDeleteKey		REGISTRY.RegDeleteKeyA
	%append $(LNKFILE) import _O32_RegEnumKeyExA	REGISTRY.RegEnumKeyExA
	%append $(LNKFILE) import _O32_RegEnumValue		REGISTRY.RegEnumValueA

rxfh2tbl:	.SYMBOLIC
	$(SCRIPTDIR)\$^@ $(TARGET) $(OBJECTS)

clean:	.SYMBOLIC
	-$(RM) *.obj *.dll *.err *.map *.sym *.exe

