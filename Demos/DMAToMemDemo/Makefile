APP := dmatomemdemo
ECHO := echo
FIND := find

SRCDIR := src
OBJDIR := obj
LIB := sdcc_iy

EMU := ../../../../tools/CSpect/CSpect.exe -zxnext -s14 -w4 -r -vsync -mmc=./
MAPGRABBER := ../../../../tools/MapGrabber 

GFXSRC := 
GFXOBJECTS := 

AUDIOSRC := 
AUDIOOBJECTS := 

CSOURCES != find src -type f -name "*.c" | sort
ASMSOURCES != find src -type f -name "*.asm" | sort

COBJECTS = $(CSOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o) 

$(APP).nex : $(COBJECTS) $(ASMOBJECTS) $(GFXOBJECTS) $(AUDIOOBJECTS)
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $(COBJECTS) $(ASMSOURCES) -o $(APP) -pragma-include:zpragma.inc -create-app -subtype=nex -Cz"--clean"

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $< -o $@ -c -pragma-include:zpragma.inc

run: $(APP).nex
	$(EMU) $(APP).nex

clean:
	rm *.bin -f
	rm *.map -f
	rm *.sna -f
	rm *.spr -f
	rm *.nxt -f
	rm *.pt3 -f
	rm obj/*.o -f

print:
	@$(ECHO) "****** Make variables ******"
	@$(ECHO) APP: $(APP)
	@$(ECHO) Source C   : $(CSOURCES)
	@$(ECHO) Source ASM : $(ASMSOURCES)
	@$(ECHO) Objects C  : $(COBJECTS)
	@$(ECHO) Objects ASM: $(ASMOBJECTS)
	