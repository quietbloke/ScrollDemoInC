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

#include "defines.h"
#include "lores.h"
#include "sprite.h"
#include "layer2.h"
#include "copper.h"
#include "scroller.h"
#include "colourbars.h"
#include "vt_sound.h"
#include "music.h"

extern uint8_t music_module[];

unsigned char Layer2YClip         = 160;      // y clip on layer2
unsigned int  Layer2YClipDelay	  = 220;      // delay for base text to appear
unsigned char SpriteYClip         =  0;       // y clip on sprite layer
unsigned int  SpriteYClipDelay	  = 720;      // delay for logo to appear
unsigned int  LoResClipDelay      = 1550;     // delay for background to appear
unsigned char LoResClip           = 0;        // loRes clip value
unsigned int  ColourBarsDelay     = 2406;     // delay to trigger colour bars

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

unsigned char borderColour = 0;

/* --------------------------------- */

static void initialise();
static void CreateRustyPixelSprite();
static void SetBorder(unsigned char border);
static void Update();
static void UpdateHW();

/* --------------------------------- */

IM2_DEFINE_ISR_8080(isr)
{
  unsigned char save;
  unsigned char borderSave;
   
   // save nextreg register
  save = IO_NEXTREG_REG;
  borderSave = borderColour;
  SetBorder(INK_MAGENTA);

  // swap in the music bank
  ZXN_WRITE_MMU2(musicBankStart);

  vt_play_isr();

  // swap it back out again
  ZXN_WRITE_MMU2(0xff);

  // Temp code to slow things down.
  // for ( char p =0; p < 7; p++)
  // {
  //   unsigned int x = rand()%128;
  //   unsigned int y = rand()%96;
  //   loResPlot(x, y, x);
  // }

  // restore nextreg register

  IO_NEXTREG_REG = save;

  SetBorder(borderSave);
}

void SetBorder(unsigned char border)
{
  zx_border(border);
  borderColour = border;
}

int main(void)
{
  zx_border(INK_BLACK);
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

  loadMusic();

  ZXN_WRITE_MMU2(musicBankStart);
  vt_init(0x4000);
  ZXN_WRITE_MMU2(0xff);

  im2_init((void *)0x8000);
  memset((void *)0x8000, 0x81, 257);
  z80_bpoke(0x8181, 0xc3);   // z80 JP instruction
  z80_wpoke(0x8182, (unsigned int)isr);

  // enable interrupts
  intrinsic_ei();

  // keep going till space key is pressed
  while(!in_key_pressed(IN_KEY_SCANCODE_SPACE) )
  {
    // REG_ACTIVE_VIDEO_LINE_H
    // For now the music playing happens just before we have reached end of screen rendering 
    IO_NEXTREG_REG = REG_ACTIVE_VIDEO_LINE_L;
//    while (IO_NEXTREG_DAT != 182);

//    SetBorder(INK_GREEN);

//    ZXN_WRITE_MMU2(musicBankStart);
//    vt_play();
//    ZXN_WRITE_MMU2(0xff);

    SetBorder(INK_BLACK);

    // wait until line 192 is active
    while (IO_NEXTREG_DAT != 192);

    UpdateHW();

    Update();

    SetBorder(INK_BLACK);
  }

  return 0;
}

/* --------------------------------- */

static void Update()
{
  SetBorder(INK_WHITE);
  spriteAngleSin += 1;
  spriteAngleCos += 1;

  if ( spriteAngleCos > 219)
  {
    spriteAngleCos = 0;
  }

  if ( ColourBarsDelay == 0 )
  {
    ColourBars_Update();
  }
  else
  {
    ColourBarsDelay--;
  }

  loresAngleSin += 1;
  loresAngleCos += 1;

  // Layer2 Clip window
  if (Layer2YClip != 191)
  {
    if ( Layer2YClipDelay > 1)
    {
      Layer2YClipDelay--;
    }
    else
    {
      Layer2YClip++;
    }
  }

  // Sprite Clip window
  if (SpriteYClip != 191)
  {
    if ( SpriteYClipDelay > 1)
    {
      SpriteYClipDelay--;
    }
    else
    {
      SpriteYClip++;
      SpriteSetClipWindow(0, 255, 0, SpriteYClip);
    }
  }

  // Lowres Clip window
  if (LoResClip != 191)
  {
    if ( LoResClipDelay > 1)
    {
      LoResClipDelay--;
    }
    else
    {
      LoResClip++;
    }
  }

  Scroller_Update();
  SetBorder(INK_BLACK);
}

static void UpdateHW()
{
   SetBorder(INK_BLUE);

  layer2SetClipWindow(0, 255, 0, Layer2YClip);

  if ( LoResClip != 0)
  {
    loResSetClipWindow(0, 255, 0, LoResClip);
  }

  loResSetOffsetX(sinOffsetX[loresAngleSin]);
  loResSetOffsetY(sinOffsetX[loresAngleCos]);

  set_sprite_pattern_index(0);
  set_sprite(sinOffsetX[spriteAngleSin] + 128, sinOffsetY[spriteAngleCos] + 52, 0);

  SetBorder(INK_GREEN);
  copperRun();

  // plotting the verticals of the scrollers are last as they can be done
  // just in time while the screen is rendering
  SetBorder(INK_YELLOW);
  Scroller_Render();
  SetBorder(INK_BLACK);
}

static void initialise()
{
  // Enable the lowres screen, show sprites
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_LSU; 

  layer2Initialise();
  layer2Clear(0xe3);
  layer2SetClipWindow(0, 255, 0, Layer2YClip);
  SpriteSetClipWindow(0, 255, 0, SpriteYClip);

  layer2WriteText(2, 10, "SCROLLNUTTER");
  layer2WriteText(3, 11, "NEXT  DEMO");

  Scroller_Init();
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