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
#include "dma.h"

#define MEMORY_BLOCK_START_ADDRESS 0x2000

extern uint8_t music_module[];

uint8_t   Layer2YClip         = 160;      // y clip on layer2
uint16_t  Layer2YClipDelay	  = 220;      // delay for base text to appear
uint8_t   SpriteYClip         = 0;        // y clip on sprite layer
uint16_t  SpriteYClipDelay	  = 720;      // delay for logo to appear
uint16_t  LoResClipDelay      = 1550;     // delay for background to appear
uint8_t   LoResClip           = 0;        // loRes clip value
uint16_t  ColourBarsDelay     = 2406;     // delay to trigger colour bars

uint8_t ballFrame = 0;
uint8_t ballFramePause = 0;

uint8_t spriteIndex = 0; // index of next available sprite to allocate

uint8_t rustyAnchorSprite = 0;
//uint8_t ballAnchorSpriteBack = 0;
//uint8_t ballAnchorSpriteFront = 0;

static int8_t sinOffsetX[] = {
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

static int8_t sinOffsetY[] = {
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

uint8_t loresAngleSin = 0;
uint8_t loresAngleCos = 64;

uint8_t spriteAngleSin = 0;
uint8_t spriteAngleCos = 0;

uint8_t stage = 0;
uint8_t stage_counter = 0;

uint8_t borderColour = 0;


#define MAX_BALLS   4
typedef struct BallSprite
{
  uint16_t  visibleDelay;
  uint8_t   ballAnchorSprite;
  uint16_t  xPos;
  int8_t    xVel;
  uint8_t   yPos;     // ypos is actually a fixed float, 8 bits int, 8 bits fraction
  int8_t    yVel;     // the velocity will be -32 to +32, but we will use yVel / 8 when updating the yPos
  bool      ballFront;
} BallSprite;

BallSprite ballSprites[MAX_BALLS * 2]; // we need two ball sprites per ball, 1 behind and 1 in front of the rusty sprite

/* --------------------------------- */

static void     initialise();
static uint8_t  CreateRustyPixelSprite();
static uint8_t  CreateBallSprite();
static void     SetBorder(uint8_t border);
static void     Update();
static void     UpdateHW();
static void     CreateBallPatterns();

/* --------------------------------- */

IM2_DEFINE_ISR_8080(isr)
{
  uint8_t save;
  uint8_t borderSave;
   
   // save nextreg register
  save = IO_NEXTREG_REG;

  // remember the current border colour
  borderSave = borderColour;

  SetBorder(INK_MAGENTA);

  // swap in the music bank
  ZXN_WRITE_MMU2(musicBankStart);

  vt_play_isr();

  // swap it back out again
  ZXN_WRITE_MMU2(0xff);

  // restore nextreg register
  IO_NEXTREG_REG = save;

  // restore the border colour
  SetBorder(borderSave);
}

void SetBorder(uint8_t border)
{
  // For texting switch the border setting on.
  // zx_border(border);    zx_border(INK_RED);
    borderColour = border;
}

int main(void)
{
  zx_border(INK_BLACK);
  ZXN_WRITE_REG(REG_TURBO_MODE, 2);

  initialise();
  loResSetInitPallete();

  loResSetClipWindow ( 0, 255, 255, 255); // hide the bg 
  layer2SetClipWindow ( 0, 255, 255, 255); // hide the sprite window

  loResDrawImage();

  create_sprite_patterns();

  // Draw the 4 rounded corner sprites
  set_sprite(32,32+(192-16), 32, true);
  set_sprite(32+(256-16),32+(192-16), 33, true);
  set_sprite(32, 32, 34, true);
  set_sprite(32+(256-16), 32, 35, true);
  spriteIndex += 4;

  // create the balls sprites behind the rusty pixel sprite
  for ( uint8_t ballIndex = 0;  ballIndex < MAX_BALLS; ballIndex++)
  {
    BallSprite* ballSprite = &ballSprites[ballIndex];

    ballSprite->ballAnchorSprite = CreateBallSprite();
  }

  rustyAnchorSprite = CreateRustyPixelSprite();

  // create the balls sprites in front of the rusty pixel sprite
  for ( uint8_t ballIndex = MAX_BALLS;  ballIndex < MAX_BALLS * 2; ballIndex++)
  {
    BallSprite* ballSprite = &ballSprites[ballIndex];

    ballSprite->ballAnchorSprite = CreateBallSprite();
  }

  loResSetOffsetX(sinOffsetX[loresAngleSin]);
  loResSetOffsetY(sinOffsetX[loresAngleCos]);

//  if(!loadMusic())
//  {
//    return 1;
//  }

  // NOTE : we need to move the code org to 0x8184 (zxpragma.inc)
  // because the interrupt table and code is in 0x8000 - 0x8183

  ZXN_WRITE_MMU2(musicBankStart);
  vt_init(0x4000);
  ZXN_WRITE_MMU2(0xff);

  im2_init((void *)0x8000);
  memset((void *)0x8000, 0x81, 257);
  z80_bpoke(0x8181, 0xc3);   // z80 JP instruction
  z80_wpoke(0x8182, (uint16_t)isr);

  // enable interrupts
  intrinsic_ei();

  // keep going till space key is pressed
  while(!in_key_pressed(IN_KEY_SCANCODE_SPACE) )
  {
    IO_NEXTREG_REG = REG_ACTIVE_VIDEO_LINE_L;

    // If you would rather play the music within the main loop
    // then remove the interrupt code and uncomment the following
    // which will play the music just before the screen rendering finishes.

    // while (IO_NEXTREG_DAT != 182);  
    // SetBorder(INK_GREEN);

    // ZXN_WRITE_MMU2(musicBankStart);
    // vt_play();
    // ZXN_WRITE_MMU2(0xff);

    // SetBorder(INK_BLACK);

    // wait until line 192 is active ( wee have finished rendering the screen)
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

  for ( uint8_t ballIndex = 0;  ballIndex < MAX_BALLS; ballIndex++)
  {
    BallSprite* ballSprite = &ballSprites[ballIndex];
    if ( ballSprite->visibleDelay == 0)
    {
      ballSprite->xPos += ballSprite->xVel;
      if (ballSprite->xPos > 255 || ballSprite->xPos < 48)
      {
        ballSprite->xVel = -ballSprite->xVel;
      }

      if ( ballSprite->yVel < 29 )
      {
        ballSprite->yVel += 1;
      }
      ballSprite->yPos += ballSprite->yVel >> 2;
      if ( ballSprite->yPos > 192-32)
      {
        ballSprite->yPos = 192-32;
        ballSprite->yVel = -ballSprite->yVel;
        if ( ballSprite->ballFront == true)
        {
          ballSprite->ballFront = false;
        }
        else
        {
          ballSprite->ballFront = true;
        }
        
      }
    }
    else
    {
      ballSprite->visibleDelay--;
    }
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
    if ( SpriteYClipDelay > 0)
    {
      SpriteYClipDelay--;
    }
    else
    {
      SpriteYClip++;
      SpriteSetClipWindow(0, 255, 0, SpriteYClip);
    }
  }

  // only move the sprite if the sprite window is visible
  if ( SpriteYClipDelay == 0)
  {
    spriteAngleSin += 1;
    spriteAngleCos += 1;

    if ( spriteAngleCos > 219)
    {
      spriteAngleCos = 0;
    }
  }

  // Lowres Clip window
  if (LoResClip != 191)
  {
    if ( LoResClipDelay > 0)
    {
      LoResClipDelay--;
    }
    else
    {
      LoResClip++;
    }
  }

  ballFramePause++;
  if ( ballFramePause > 0)
  {
    ballFrame++;
    if ( ballFrame > 10)
    {
      ballFrame = 0;
    }
    ballFramePause = 0;
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

  SetBorder(INK_GREEN);
  copperRun();

  SetBorder(INK_RED);
  CreateBallPatterns();

  SetBorder(INK_CYAN);
  for ( uint8_t ballIndex = 0;  ballIndex < MAX_BALLS; ballIndex++)
  {
    BallSprite* ballSprite = &ballSprites[ballIndex];
    if ( ballSprite->visibleDelay == 0)
    {
      set_sprite_pattern_index(ballSprite->ballAnchorSprite);
      set_sprite(ballSprite->xPos, ballSprite->yPos, 52, true);

      BallSprite* ballSpriteFront = &ballSprites[ballIndex + MAX_BALLS];
      set_sprite_pattern_index(ballSpriteFront->ballAnchorSprite);

      if ( ballSprite->ballFront )
      {
        set_sprite(ballSprite->xPos, ballSprite->yPos, 52, true);
      }
      else
      {
        set_sprite(ballSprite->xPos, ballSprite->yPos, 52, false);
      }
      
    }

  }

  set_sprite_pattern_index(rustyAnchorSprite);
  set_sprite(sinOffsetX[spriteAngleSin] + 128, sinOffsetY[spriteAngleCos] + 52, 0, true);

  SetBorder(INK_YELLOW);
  Scroller_Render();

  SetBorder(INK_BLACK);
}

static void initialise()
{
  for( uint8_t ballIndex = 0; ballIndex < MAX_BALLS; ballIndex++)
  {
    BallSprite* ballSprite    = &ballSprites[ballIndex];
    ballSprite->visibleDelay  = 0x1df6 + ballIndex * 0x06fa;
    ballSprite->xPos          = 64;
    ballSprite->yPos          = 0;
    ballSprite->xVel          = 1;

    ballSprite = &ballSprites[ballIndex + MAX_BALLS];
    ballSprite->visibleDelay  = 0x1df6 + ballIndex * 0x06fa;
  }

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

// 32 sprites have up the rusty pixel sprite
int8_t childPixelSprites[] = {
     1,0, 2,0, 3,0,
     0,1, 1,1, 2,1, 3,1,
     0,2, 1,2, 2,2, 3,2,
     0,3, 1,3, 2,3, 3,3,
     0,4, 1,4, 2,4, 3,4,
    -1,5, 0,5, 1,5, 2,5, 3,5, 4,5,
    -1,6, 0,6, 1,6, 2,6, 3,6, 4,6
};
  
uint8_t CreateRustyPixelSprite()
{
  uint8_t anchorSpriteIndex = spriteIndex;

  set_sprite_pattern_index(anchorSpriteIndex);
  set_sprite(sinOffsetX[spriteAngleSin] + 128, sinOffsetY[spriteAngleCos] + 52, 0, true);
  spriteIndex++;
  uint8_t spritesize = 16;

  // data is 
  //  sprite size offset x
  //  sprite size offset y
  for (uint8_t childSprite = 0; childSprite < sizeof(childPixelSprites); childSprite+=2)
  {
    set_sprite_composite(childPixelSprites[childSprite] * spritesize, childPixelSprites[childSprite + 1] * spritesize, childSprite / 2 + 1);
    spriteIndex++;
  }

  return anchorSpriteIndex;
}

int8_t childBallSprites[] = {
    -1,-1,48, 0,-1,49, 1,-1,50,
    -1,0,51, 1,0,53,
    -1,1,54, 0,1,55, 1,1,56
};

uint8_t CreateBallSprite()
{
  uint8_t anchorSpriteIndex = spriteIndex;

  set_sprite_pattern_index(anchorSpriteIndex);
  set_sprite(0, 0, 52, false);
  spriteIndex++;

  uint8_t spritesize = 16;

  // data is 
  //  sprite size offset x
  //  sprite size offset y
  //  sprite texture
  for (uint8_t childSprite = 0; childSprite < sizeof(childBallSprites); childSprite+=3)
  {
    set_sprite_composite(childBallSprites[childSprite] * spritesize, childBallSprites[childSprite + 1] * spritesize, childBallSprites[childSprite + 2]);
    spriteIndex++;
  }

  return anchorSpriteIndex;
}

static void CreateBallPatterns()
{
  // decide what frame of animation ( block of 9 textures ) to use
  // hard coded extended memory page
  uint8_t textureFrame = ballFrame;

  // map the texture block from extended memory into main memory
  if ( ballFrame > 6)
  {
    ZXN_WRITE_MMU1(ballsBank2Start);
    ZXN_WRITE_MMU2(ballsBank2Start+1);
    textureFrame -= 7;
  }
  else
  {
    ZXN_WRITE_MMU1(ballsBank1Start);
    ZXN_WRITE_MMU2(ballsBank1Start+1);
  }

  // copy the textures
  set_sprite_pattern_index(48);
  TransferMemoryToPortDMA(MEMORY_BLOCK_START_ADDRESS + (256*9)*textureFrame, 256 * 9, __IO_SPRITE_PATTERN);
}

