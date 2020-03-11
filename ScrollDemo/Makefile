APP := scrolldemo
STARTUP := 31

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

EMU := $(MONO) ../../../../tools/CSpect/CSpect.exe -zxnext -s14 -w3 -r -vsync -mmc=./
MAPGRABBER := $(WINE) ../../../tools/MapGrabber

GFXSRC := GFX/Bg.bmp GFX/Sprites.bmp GFX/Balls01.bmp GFX/Balls02.bmp
GFXOBJECTS := Bg.bin Sprites.spr balls01.spr balls02.spr font.nxt font2.nxt font3.nxt font4.nxt music.pt3

AUDIOSRC := Audio/Music2.pt3
AUDIOOBJECTS := music.pt3

CSOURCES != find src -type f -name "*.c" | sort
ASMSOURCES != find src -type f -name "*.asm" | sort

COBJECTS = $(CSOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ASMOBJECTS = $(ASMSOURCES:$(SRCDIR)/%.asm=$(OBJDIR)/%.o)

$(APP).nex : $(GFXOBJECTS) $(AUDIOOBJECTS) $(ASMOBJECTS) $(COBJECTS)
	zcc +zxn -vn -m -startup=$(STARTUP) -clib=$(LIB) $(COBJECTS) $(ASMSOURCES) -o $(APP) -pragma-include:zpragma.inc -subtype=nex -Cz"--fullsize --clean" -create-app

	# copy all the files needed into the out folder
	cp $(APP).nex out/$(APP).nex

	# Also copy the output file into the sdcard image
	$(WINE) ../../../../tools/hdfmonkey.exe put ../../../../tools/CSpect/cspect-next-2gb.img $(APP).nex $(APP).nex 

$(GFXOBJECTS): $(GFXSRC)
	$(MAPGRABBER) GFX/Bg bitmap
	mv GFX/Bg.nxi Bg.bin
	split --bytes=6144 --suffix-length=1 --additional-suffix=.bin Bg.bin bg

	$(MAPGRABBER) GFX/Sprites spr s16 r0
	mv GFX/Sprites.spr Sprites.spr
	split --bytes=8192 --suffix-length=1 --additional-suffix=.spr Sprites.spr sprites

	$(MAPGRABBER) GFX/Font s32 nomap r0
	mv GFX/Font.nxt font.nxt
	split --bytes=8192 --suffix-length=1 --additional-suffix=.nxt font.nxt font

	$(MAPGRABBER) GFX/Font2 s32 nomap r0
	mv GFX/Font2.nxt font2.nxt
	split --bytes=8192 --suffix-length=1 --additional-suffix=.nxt font2.nxt font2

	$(MAPGRABBER) GFX/Font3 s32 nomap r0
	mv GFX/Font3.nxt font3.nxt
	split --bytes=8192 --suffix-length=1 --additional-suffix=.nxt font3.nxt font3

	$(MAPGRABBER) GFX/Font4 s16 nomap r0
	mv GFX/Font4.nxt font4.nxt
	split --bytes=8192 --suffix-length=1 --additional-suffix=.nxt font4.nxt font4

	$(MAPGRABBER) GFX/Balls01 spr s16 r0
	mv GFX/Balls01.spr balls01.spr
	split --bytes=8192 --suffix-length=1 --additional-suffix=.spr balls01.spr balls01

	$(MAPGRABBER) GFX/Balls02 spr s16 r0
	mv GFX/Balls02.spr balls02.spr
	split --bytes=8192 --suffix-length=1 --additional-suffix=.spr balls02.spr balls02

$(AUDIOOBJECTS): $(AUDIOSRC)
	cp Audio/Music2.pt3 music.pt3

$(OBJDIR)/%.o : $(SRCDIR)/%.asm
	zcc +zxn -vn -m -startup=$(STARTUP) -clib=$(LIB) $< -o $@ -c -pragma-include:zpragma.inc

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	zcc +zxn -vn -m -startup=$(STARTUP) -clib=$(LIB) $< -o $@ -c -pragma-include:zpragma.inc

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
	