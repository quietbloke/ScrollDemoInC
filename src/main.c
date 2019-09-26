#include <stdlib.h>
#include <stddef.h>
#include <input.h>
#include <intrinsic.h>
#include <arch/zxn.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//#include <math.h>
#include "lores.h"
#include "sprite.h"

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

/* --------------------------------- */

static void initialise();
static void CreateRustyPixelSprite();

/* --------------------------------- */

int main(void)
{
  unsigned char loresAngleSin = 0;
  unsigned char loresAngleCos = 64;

  unsigned char spriteAngleSin = 0;
  unsigned char spriteAngleCos = 0;

  unsigned char stage = 0;
  unsigned char stage_counter = 0;

  initialise();

  loResSetInitPallete();

  zx_border(INK_BLUE);

  loResSetClipWindow ( 0, 255, 255, 255); // hide the bg 
  // hide the sprites

  if ( !loResLoadImage("bg.bin"))
  {
    zx_border(7);
  }

  if ( !sprites_load_patterns("sprites.spr"))
  {
    zx_border(7);
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


// keep going till space key is pressed

  while(!in_key_pressed(IN_KEY_SCANCODE_SPACE) ){

// Temp code to slow things down.
//    for ( char p =0; p < 4; p++)
//    {
//      unsigned int x = rand()%128;
//      unsigned int y = rand()%96;
//      loResPlot(x, y, x);
//    }

  if ( stage == 0)
  {
    stage_counter++;
    if ( stage_counter == 240)
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
    loResSetOffsetX(sinOffsetX[loresAngleSin]);
    loResSetOffsetY(sinOffsetX[loresAngleCos]);
    loresAngleSin += 1;
    loresAngleCos += 1;
  }

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
  zx_border(1);

  intrinsic_halt();
  zx_border(0);
}

  // wait till no key is pressed then a key is pressed before we quit for real
  in_wait_nokey();
  in_wait_key();

  return 0;
}

/* --------------------------------- */

static void initialise()
{
  zx_border(INK_BLUE);

  // Enable the lowres screen
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE; 
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