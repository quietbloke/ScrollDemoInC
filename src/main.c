#include <stdlib.h>
#include <stddef.h>
#include <input.h>
#include <arch/zxn.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <im2.h>
#include <z80.h>
#include <intrinsic.h>

#include "lores.h"
#include "sprite.h"
#include "layer2.h"
#include "copper.h"
#include "scroller.h"

#define RUSTY_PIXEL_SPRITE 0
static signed char sinOffsetX[] = {
	1 , 1 , 2 , 2 , 3 , 4 , 4 , 5,
	5 , 6 , 6 , 7 , 8 , 8 , 9 , 9,
	10 , 10 , 11 , 11 , 12 , 12 , 13 , 13,
	14 , 14 , 15 , 15 , 16 , 16 , 17 , 17,
	17 , 18 , 18 , 19 , 19 , 19 , 20 , 20,
	20 , 21 , 21 , 21 , 21 , 22 , 22 , 22,
	22 , 23 , 23 , 23 , 23 , 23 , 23 , 24,
	24 , 24 , 24 , 24 , 24 , 24 , 24 , 24,
	24 , 24 , 24 , 24 , 24 , 24 , 24 , 24,
	23 , 23 , 23 , 23 , 23 , 23 , 22 , 22,
	22 , 22 , 21 , 21 , 21 , 21 , 20 , 20,
	20 , 19 , 19 , 19 , 18 , 18 , 17 , 17,
	17 , 16 , 16 , 15 , 15 , 14 , 14 , 13,
	13 , 12 , 12 , 11 , 11 , 10 , 10 , 9,
	9 , 8 , 8 , 7 , 6 , 6 , 5 , 5,
	4 , 4 , 3 , 2 , 2 , 1 , 1 , 0,
	-1 ,-1 ,-2 ,-2 ,-3 ,-4 ,-4 ,-5,
	-5 ,-6 ,-6 ,-7 ,-8 ,-8 ,-9 ,-9,
	-10 ,-10 ,-11 ,-11 ,-12 ,-12 ,-13 ,-13,
	-14 ,-14 ,-15 ,-15 ,-16 ,-16 ,-17 ,-17,
	-17 ,-18 ,-18 ,-19 ,-19 ,-19 ,-20 ,-20,
	-20 ,-21 ,-21 ,-21 ,-21 ,-22 ,-22 ,-22,
	-22 ,-23 ,-23 ,-23 ,-23 ,-23 ,-23 ,-24,
	-24 ,-24 ,-24 ,-24 ,-24 ,-24 ,-24 ,-24,
	-24 ,-24 ,-24 ,-24 ,-24 ,-24 ,-24 ,-24,
	-23 ,-23 ,-23 ,-23 ,-23 ,-23 ,-22 ,-22,
	-22 ,-22 ,-21 ,-21 ,-21 ,-21 ,-20 ,-20,
	-20 ,-19 ,-19 ,-19 ,-18 ,-18 ,-17 ,-17,
	-17 ,-16 ,-16 ,-15 ,-15 ,-14 ,-14 ,-13,
	-13 ,-12 ,-12 ,-11 ,-11 ,-10 ,-10 ,-9,
	-9 ,-8 ,-8 ,-7 ,-6 ,-6 ,-5 ,-5,
	-4 ,-4 ,-3 ,-2 ,-2 ,-1 ,-1 , 0
};

static signed char sinOffsetY[] = {
	 1 , 1 , 2 , 3 , 3 , 4 , 5 , 5,
	 6 , 7 , 7 , 8 , 9 , 9 , 10 , 11,
	 11 , 12 , 12 , 13 , 14 , 14 , 15 , 15,
	 16 , 16 , 17 , 17 , 18 , 18 , 19 , 19,
	 19 , 20 , 20 , 21 , 21 , 21 , 22 , 22,
	 22 , 22 , 23 , 23 , 23 , 23 , 23 , 24,
	 24 , 24 , 24 , 24 , 24 , 24 , 24 , 24,
	 24 , 24 , 24 , 24 , 24 , 24 , 23 , 23,
	 23 , 23 , 23 , 22 , 22 , 22 , 22 , 21,
	 21 , 21 , 20 , 20 , 19 , 19 , 19 , 18,
	 18 , 17 , 17 , 16 , 16 , 15 , 15 , 14,
	 14 , 13 , 12 , 12 , 11 , 11 , 10 , 9,
	 9 , 8 , 7 , 7 , 6 , 5 , 5 , 4,
	 3 , 3 , 2 , 1 , 1 , 0 ,-1 ,-1,
	-2 ,-3 ,-3 ,-4 ,-5 ,-5 ,-6 ,-7,
	-7 ,-8 ,-9 ,-9 ,-10 ,-11 ,-11 ,-12,
	-12 ,-13 ,-14 ,-14 ,-15 ,-15 ,-16 ,-16,
	-17 ,-17 ,-18 ,-18 ,-19 ,-19 ,-19 ,-20,
	-20 ,-21 ,-21 ,-21 ,-22 ,-22 ,-22 ,-22,
	-23 ,-23 ,-23 ,-23 ,-23 ,-24 ,-24 ,-24,
	-24 ,-24 ,-24 ,-24 ,-24 ,-24 ,-24 ,-24,
	-24 ,-24 ,-24 ,-24 ,-23 ,-23 ,-23 ,-23,
	-23 ,-22 ,-22 ,-22 ,-22 ,-21 ,-21 ,-21,
	-20 ,-20 ,-19 ,-19 ,-19 ,-18 ,-18 ,-17,
	-17 ,-16 ,-16 ,-15 ,-15 ,-14 ,-14 ,-13,
	-12 ,-12 ,-11 ,-11 ,-10 ,-9 ,-9 ,-8,
	-7 ,-7 ,-6 ,-5 ,-5 ,-4 ,-3 ,-3,
	-2 ,-1 ,-1 , 0
};

unsigned char loresAngleSin = 0;
unsigned char loresAngleCos = 64;

unsigned char spriteAngleSin = 0;
unsigned char spriteAngleCos = 0;

unsigned char stage = 0;
unsigned char stage_counter = 0;

/* --------------------------------- */

static void initialise();
static void CreateRustyPixelSprite();

/* --------------------------------- */

IM2_DEFINE_ISR_8080(isr)
{
     unsigned char save;
   
   // save nextreg register
//    zx_border(INK_BLACK);
   
   save = IO_NEXTREG_REG;

  // Temp code to slow things down.
//  for ( char p =0; p < 2; p++)
//  {
//    unsigned int x = rand()%128;
//    unsigned int y = rand()%96;
//    loResPlot(x, y, x);
//  }

  // restore nextreg register

//    Scroller_Update();
   
  IO_NEXTREG_REG = save;

//  zx_border(INK_BLUE);
}

int main(void)
{
  ZXN_WRITE_REG(REG_TURBO_MODE, 2);


  initialise();

  loResSetInitPallete();

//  zx_border(INK_BLUE);

  loResSetClipWindow ( 0, 255, 255, 255); // hide the bg 

  if ( !loResLoadImage("bg.bin"))
  {
//    zx_border(7);
  }

  if ( !sprites_load_patterns("sprites.spr"))
  {
//    zx_border(7);
  }

  CreateRustyPixelSprite();

  // Draw the 4 rounded corner sprites
  set_sprite(32,32+192-16, 32);
  set_sprite(32+256-16,32+192-16, 33);
  set_sprite(32, 32, 34);
  set_sprite(32+256-16, 32, 35);

  loResSetOffsetX(sinOffsetX[loresAngleSin]);
  loResSetOffsetY(sinOffsetX[loresAngleCos]);

  set_sprite_pattern_index(0);
  set_sprite(255, 255, 0);

  // NOTE : we need to move the code org to 0x8184 (zxpragma.inc)
  // because the interrupt table and code is in 0x8000 - 0x8183

  im2_init((void *)0x8000);
  memset((void *)0x8000, 0x81, 257);
  z80_bpoke(0x8181, 0xc3);   // z80 JP instruction
  z80_wpoke(0x8182, (unsigned int)isr);

  // enable interrupts
  intrinsic_ei();

  // keep going till space key is pressed
  while(!in_key_pressed(IN_KEY_SCANCODE_SPACE) )
  {
    // wait until line 192 is active

    // REG_ACTIVE_VIDEO_LINE_H
    IO_NEXTREG_REG = REG_ACTIVE_VIDEO_LINE_L;
    while (IO_NEXTREG_DAT != 192);

    zx_border(INK_BLUE);

    Update();
    Render();

    if ( stage == 0)
    {
      stage_counter++;
      if ( stage_counter == 30)
      {
        stage++;
        stage_counter = 0;
      }
    }

    if ( stage == 1)
    {
      loResSetClipWindow ( 0, 255, 0, stage_counter); // hide the bg 
      stage_counter++;
      if ( stage_counter == 196)
      {
        stage++;
        stage_counter = 0;
      }
    }

    if ( stage > 0)
    {
      Scroller_Update();
      loResSetOffsetX(sinOffsetX[loresAngleSin]);
      loResSetOffsetY(sinOffsetX[loresAngleCos]);
      loresAngleSin += 1;
      loresAngleCos += 1;
    }

    copperRun();

    if ( stage > 1)
    {
      set_sprite_pattern_index(0);
      set_sprite(sinOffsetX[spriteAngleSin] + 128, sinOffsetY[spriteAngleCos] + 52, 0);

      spriteAngleSin += 1;
      spriteAngleCos += 1;

      if ( spriteAngleCos > 219)
      {
        spriteAngleCos = 0;
      }
    }
    Scroller_Render();

    zx_border(INK_BLACK);

  }

  return 0;
}

/* --------------------------------- */

static void Update()
{

}

static void Render()
{

}

static void initialise()
{
//  zx_border(INK_BLUE);

  // Enable the lowres screen, show sprites
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_LSU; 

  layer2Initialise();
  layer2Clear(0xe3);

  layer2WriteText(2, 10, "SCROLLNUTTER");
  layer2WriteText(3, 11, "NEXT  DEMO");

  Scroller_Init();
//  layer2WriteBigText(0, 0, "HELLO", fontBankStart);
//  layer2WriteBigText(0, 1, "WORLD!", font2BankStart);
//  layer2WriteBigText(0, 2, "FONT 3", font3BankStart);
//  layer2WriteBigText(0, 3, "FONT 2", font2BankStart);
//  layer2WriteBigText(0, 4, "FONT 1", fontBankStart);

  layer2Show();
}

static void CreateRustyPixelSprite()
{
  set_sprite(128, 64, 0);

  unsigned char spritesize = 16;

  // data is 
  //  sprite size offset x
  //  sprite size offset y
  signed char childsprites[] = {
     1,0, 2,0, 3,0,
     0,1, 1,1, 2,1, 3,1,
     0,2, 1,2, 2,2, 3,2,
     0,3, 1,3, 2,3, 3,3,
     0,4, 1,4, 2,4, 3,4,
    -1,5, 0,5, 1,5, 2,5, 3,5, 4,5,
    -1,6, 0,6, 1,6, 2,6, 3,6, 4,6
  };
  
  for (unsigned char childSprite = 0; childSprite < sizeof(childsprites); childSprite+=2)
  {
    set_sprite_composite(childsprites[childSprite] * spritesize, childsprites[childSprite + 1] * spritesize, childSprite / 2 + 1);
  }
}