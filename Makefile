APP := scrolldemo
LIB := sdcc_iy
SRC1 := src/main.c src/lores.c src/sprite.c src/layer2.c src/copper.c src/scroller.c src/colourbars.c src/PT3PROM.asm src/vt_demo_module.asm  src/vt_sound.asm
SRC := src/main.c src/lores.c src/sprite.c src/layer2.c src/copper.c src/scroller.c src/colourbars.c src/music.c src/PT3PROM.asm src/vt_sound.asm  
EMU1 := ../../tools/CSpect/CSpect.exe -zxnext -s14 -w4 -r  -60 -vsync -sound -mmc=./
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




run: $(APP).sna
	$(EMU) $(APP).sna

clean:
	rm *.bin
	rm *.sna	
	rm *.spr
	rm *.nxt
	