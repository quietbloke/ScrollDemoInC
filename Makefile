APP := scrolldemo
LIB := sdcc_iy
SRC := src/main.c src/lores.c src/sprite.c src/layer2.c src/copper.c src/scroller.c src/colourbars.c src/music.c src/dma.c Audio/PT3PROM.asm Audio/vt_sound.asm 
EMU := ../../tools/CSpect/CSpect.exe -zxnext -s14 -w4 -r -vsync -mmc=./

MAPGRABBER := ../../tools/MapGrabber 
GFXSRC := GFX/Bg.bmp GFX/Sprites.bmp

$(APP).sna : $(SRC) $(GFXSRC)
	zcc +zxn -vn -m -startup=31 -clib=$(LIB) $(SRC) -o $(APP) -pragma-include:zpragma.inc -create-app -Cz"--sna"

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
	rm *.sna	
	rm *.spr
	rm *.nxt
	rm *.pt3
	