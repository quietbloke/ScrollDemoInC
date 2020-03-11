APP := copperdemo
ECHO := echo
FIND := find

SRCDIR := src
OBJDIR := obj
LIB := sdcc_iy

# Detect Operating System
ifeq ($(OS),Windows_NT)
    OSNAME := WIN32
    MONO := 
    WINE := 
else
    OSNAME != uname -s
    MONO := mono
    WINE := wine
endif

EMU := $(MONO) ../../../../../tools/CSpect/CSpect.exe -zxnext -s14 -w3 -r -vsync -mmc=./
MAPGRABBER := $(WINE) ../../../../tools/MapGrabber

GFXSRC := 
GFXOBJECTS := 

CSOURCES != find src -type f -name "*.c" | sort
ASMSOURCES != find src -type f -name "*.asm" | sort

COBJECTS = $(CSOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o) 

$(APP).nex : $(COBJECTS) $(ASMOBJECTS) $(GFXOBJECTS) $(AUDIOOBJECTS)
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $(COBJECTS) $(ASMSOURCES) -o $(APP) -pragma-include:zpragma.inc -create-app -subtype=nex -Cz"--clean"

	# copy all the files needed into the out folder
	cp $(APP).nex out/$(APP).nex

	# Also copy the output file into the sdcard image
	$(WINE) ../../../../tools/hdfmonkey.exe put ../../../../tools/CSpect/cspect-next-2gb.img $(APP).nex $(APP).nex 

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $< -o $@ -c -pragma-include:zpragma.inc

 $(GFXOBJECTS): $(GFXSRC)
	$(MAPGRABBER) GFX/Bg bitmap
	mv GFX/bg.nxi bg.bin

	$(MAPGRABBER) GFX/Sprites spr s16 r0
	mv GFX/sprites.spr sprites.spr

	$(MAPGRABBER) GFX/Font s32 nomap r0
	mv GFX/Font.nxt font.nxt

	$(MAPGRABBER) GFX/Font2 s32 nomap r0
	mv GFX/Font2.nxt font2.nxt

	$(MAPGRABBER) GFX/Font3 s32 nomap r0
	mv GFX/Font3.nxt font3.nxt

	$(MAPGRABBER) GFX/Font4 s16 nomap r0
	mv GFX/Font4.nxt font4.nxt

	cp Audio/music2.pt3 music.pt3

run: $(APP).nex
	cd out; \
	$(EMU) $(APP).nex

clean:
	rm *.bin -f
	rm *.map -f
	rm *.nex -f
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
