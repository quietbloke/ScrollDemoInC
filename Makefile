APP := scrolldemo
ECHO := echo
FIND := find

SRCDIR := src
OBJDIR := obj
LIB := sdcc_iy

EMU := ../../tools/CSpect/CSpect.exe -zxnext -s14 -w4 -r -vsync -mmc=./
MAPGRABBER := ../../tools/MapGrabber 
GFXSRC := GFX/Bg.bmp GFX/Sprites.bmp
GFXOBJECTS := obj/bg.bin obj/sprites.spr

CSOURCES != find src -type f -name "*.c" | sort
ASMSOURCES != find src -type f -name "*.asm" | sort

COBJECTS = $(CSOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o) 

$(APP).sna : $(COBJECTS) $(ASMOBJECTS) $(GFXOBJECTS)
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $(COBJECTS) $(ASMSOURCES) -o $(APP) -pragma-include:zpragma.inc -create-app -Cz"--sna"

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $< -o $@ -c -pragma-include:zpragma.inc

 $(GFXOBJECTS): $(GFXSRC)
	$(MAPGRABBER) GFX/Bg b
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

run: $(APP).sna
	$(EMU) $(APP).sna

clean:
	rm *.bin
	rm *.map
	rm *.sna	
	rm *.spr
	rm *.nxt
	rm *.pt3
	rm obj/*.o

print:
	@$(ECHO) "****** Make variables ******"
	@$(ECHO) APP: $(APP)
	@$(ECHO) Source C   : $(CSOURCES)
	@$(ECHO) Source ASM : $(ASMSOURCES)
	@$(ECHO) Objects C  : $(COBJECTS)
	@$(ECHO) Objects ASM: $(ASMOBJECTS)
	