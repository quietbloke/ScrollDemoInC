#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>

#include "scroller.h"
#include "colourbars.h"

#define COPPER_WAIT 0x80;

static signed wibble[] = {
	 2 , 3 , 5 , 6 , 8 , 9 , 11 , 12,
	 14 , 15 , 16 , 18 , 19 , 20 , 21 , 23,
	 24 , 25 , 26 , 27 , 27 , 28 , 29 , 30,
	 30 , 31 , 31 , 31 , 32 , 32 , 32 , 32,
	 32 , 32 , 32 , 31 , 31 , 31 , 30 , 30,
	 29 , 28 , 27 , 27 , 26 , 25 , 24 , 23,
	 21 , 20 , 19 , 18 , 16 , 15 , 14 , 12,
	 11 , 9 , 8 , 6 , 5 , 3 , 2 , 0,
	-2 ,-3 ,-5 ,-6 ,-8 ,-9 ,-11 ,-12,
	-14 ,-15 ,-16 ,-18 ,-19 ,-20 ,-21 ,-23,
	-24 ,-25 ,-26 ,-27 ,-27 ,-28 ,-29 ,-30,
	-30 ,-31 ,-31 ,-31 ,-32 ,-32 ,-32 ,-32,
	-32 ,-32 ,-32 ,-31 ,-31 ,-31 ,-30 ,-30,
	-29 ,-28 ,-27 ,-27 ,-26 ,-25 ,-24 ,-23,
	-21 ,-20 ,-19 ,-18 ,-16 ,-15 ,-14 ,-12,
	-11 ,-9 ,-8 ,-6 ,-5 ,-3 ,-2 , 0,
	 2 , 4 , 6 , 8 , 10 , 11 , 13 , 14,
	 15 , 15 , 16 , 16 , 16 , 15 , 15 , 14,
	 13 , 11 , 10 , 8 , 6 , 4 , 2 , 0,
	-2 ,-4 ,-6 ,-8 ,-10 ,-11 ,-13 ,-14,
	-15 ,-15 ,-16 ,-16 ,-16 ,-15 ,-15 ,-14,
	-13 ,-11 ,-10 ,-8 ,-6 ,-4 ,-2 , 0,
	 2 , 3 , 4 , 6 , 7 , 7 , 8 , 8,
	 8 , 7 , 7 , 6 , 4 , 3 , 2 , 0,
	-2 ,-3 ,-4 ,-6 ,-7 ,-7 ,-8 ,-8,
	-8 ,-7 ,-7 ,-6 ,-4 ,-3 ,-2 , 0,
	 2 , 4 , 6 , 8 , 10 , 11 , 13 , 14,
	 15 , 15 , 16 , 16 , 16 , 15 , 15 , 14,
	 13 , 11 , 10 , 8 , 6 , 4 , 2 , 0,
	-2 ,-4 ,-6 ,-8 ,-10 ,-11 ,-13 ,-14,
	-15 ,-15 ,-16 ,-16 ,-16 ,-15 ,-15 ,-14,
	-13 ,-11 ,-10 ,-8 ,-6 ,-4 ,-2 , 0
};

unsigned char wibbleOffset = 0;

void copperRun()
{
    // Start Send
    ZXN_WRITE_REG(REG_COPPER_CONTROL_H, 0);
    ZXN_WRITE_REG(REG_COPPER_CONTROL_L, 0);

    // Select copper data port
    IO_NEXTREG_REG = REG_COPPER_DATA;

    // wait till scanline 0;
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 0;

	// change order of layers
    IO_NEXTREG_DAT = REG_SPRITE_LAYER_SYSTEM;
    IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_SLU;

	// offset for first scroller
	Scroller* scroller = Scroller_Get(0);

	IO_NEXTREG_DAT = REG_LAYER_2_OFFSET_X;
	IO_NEXTREG_DAT = scroller->scrXPos;

//	ColourBars_Render(0);
//	ColourBars_Update_Test(0);

    // wait till scanline 32 - second scroller;
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 32;

    IO_NEXTREG_DAT = REG_SPRITE_LAYER_SYSTEM;
    IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_LSU;

	// offset for second scroller
	scroller = Scroller_Get(1);

	IO_NEXTREG_DAT = REG_LAYER_2_OFFSET_X;
	IO_NEXTREG_DAT = scroller->scrXPos;

//	ColourBars_Render(1);
	ColourBars_Update_Test(1);

    // wait till scanline 64 - second scroller;
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 64;

	// offset for third scroller
	scroller = Scroller_Get(2);

	IO_NEXTREG_DAT = REG_LAYER_2_OFFSET_X;
	IO_NEXTREG_DAT = scroller->scrXPos;

//	ColourBars_Render(2);
	ColourBars_Update_Test(2);

    // wait till scanline 96 - second scroller;
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 96;

	// offset for third scroller
	scroller = Scroller_Get(3);
	IO_NEXTREG_DAT = REG_LAYER_2_OFFSET_X;
	IO_NEXTREG_DAT = scroller->scrXPos;

	ColourBars_Update_Test(3);

    // wait till scanline 128 - second scroller;
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 128;

	// offset for fourth scroller
	scroller = Scroller_Get(4);
	IO_NEXTREG_DAT = REG_LAYER_2_OFFSET_X;
	IO_NEXTREG_DAT = scroller->scrXPos;

    IO_NEXTREG_DAT = REG_SPRITE_LAYER_SYSTEM;
    IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_SLU;

//	ColourBars_Render(3);
	ColourBars_Update_Test(4);

    unsigned char wibblePos = wibbleOffset;
    for ( unsigned char scanline = 160; scanline < 192; scanline++)
    {
        // wait till scanline 160;
        IO_NEXTREG_DAT = COPPER_WAIT;
        IO_NEXTREG_DAT = scanline;

        // Change layer2 x-offset
        IO_NEXTREG_DAT = REG_LAYER_2_OFFSET_X;
        IO_NEXTREG_DAT = wibble[wibblePos];
        wibblePos++;
    }
    wibbleOffset++;


	// done
    IO_NEXTREG_DAT = 0xff;
    IO_NEXTREG_DAT = 0xff;

    // Start

    ZXN_WRITE_REG(REG_COPPER_CONTROL_H, 0xc0);
    ZXN_WRITE_REG(REG_COPPER_CONTROL_L, 0);
}