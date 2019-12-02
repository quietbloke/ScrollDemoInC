#include <stdlib.h>
#include <stddef.h>
#include <input.h>
#include <arch/zxn.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <intrinsic.h>

#include "sprite.h"
#include "dma.h"

#define MEMORY_BLOCK_SIZE 256
#define MEMORY_BLOCK_START_ADDRESS 0x2000

unsigned char borderColour = 0;
unsigned char clearColour = 1;
bool useDMA = false;
unsigned numberOfBlocks = 1;

unsigned char ballFrame = 0;
unsigned char ballFramePause = 0;

/* --------------------------------- */

static void Initialise();
static void SetBorder(unsigned char border);
static void Update();
static void Render();
static void CreateBallSprite();

/* --------------------------------- */

int main(void)
{
  zx_border(INK_BLUE);
  ZXN_WRITE_REG(REG_TURBO_MODE, 2);

  Initialise();

//   TransferValueToMemoryCPU(0, 0x4000, 256 * 20);

  while(!in_key_pressed(IN_KEY_SCANCODE_SPACE) )
  {
    SetBorder(INK_BLACK);
    // wait until line 192 is active
    IO_NEXTREG_REG = REG_ACTIVE_VIDEO_LINE_L;

    // REG_ACTIVE_VIDEO_LINE_H
    while (IO_NEXTREG_DAT != 192);

    SetBorder(INK_BLUE);

    Render();

    SetBorder(INK_CYAN);

    Update();
   
    SetBorder(INK_BLACK);
  }

  SetBorder(INK_RED);
  return 0;
}

static void SetBorder(unsigned char border)
{
  zx_border(border);
  borderColour = border;
}

static void Update()
{
    if ( in_key_pressed(IN_KEY_SCANCODE_d))
    {
      useDMA = true;
    }
    if (in_key_pressed(IN_KEY_SCANCODE_c))
    {
      useDMA = false;
    }
    if (in_key_pressed(IN_KEY_SCANCODE_p) && clearColour == 0)
    {
      if ( numberOfBlocks < 10)
      {
        numberOfBlocks++;
      }
    }
    if (in_key_pressed(IN_KEY_SCANCODE_l) && clearColour == 0)
    {
      if ( numberOfBlocks > 1)
      {
        numberOfBlocks--;
      }
    }

    clearColour++;
    if ( clearColour > 3)
    {
      clearColour = 0;
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
}

static void Render()
{
  // decide what frame of animation ( block of 9 textures ) to use
  // for now just assume frame 0

  // copy the textures
  // map the texture block from extended memory into main memory

  // hard coded extended memory page
  unsigned char textureFrame = ballFrame;
  if ( ballFrame > 6)
  {
    ZXN_WRITE_MMU1(54);
    ZXN_WRITE_MMU2(55);
    textureFrame -= 7;
  }
  else
  {
    ZXN_WRITE_MMU1(52);
    ZXN_WRITE_MMU2(53);
  }

  set_sprite_pattern_index(0);
  if (useDMA) 
  {
    TransferMemoryToSpriteIOPortDMA(MEMORY_BLOCK_START_ADDRESS + (256*9)*textureFrame, 256 * 9);
  }
  else
  {
    TransferMemoryToSpriteIOPortCPU(MEMORY_BLOCK_START_ADDRESS + (256*9)*textureFrame, 256 * 9);
  }
}

static void loResSetInitPallete()
{
  IO_NEXTREG_REG = REG_PALETTE_CONTROL;
  IO_NEXTREG_DAT = RPC_SELECT_ULA_PALETTE_0 | RPC_ENABLE_ULANEXT | RPC_ENABLE_ULA_PALETTE_0;

  IO_NEXTREG_REG = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x20;
  IO_NEXTREG_DAT = 0x40;
  IO_NEXTREG_DAT = 0x60;
  IO_NEXTREG_DAT = 0x80;

  // Set some colours for the border (for debugging purposes)
  IO_NEXTREG_REG = REG_PALETTE_INDEX;
  IO_NEXTREG_DAT = 128;
  IO_NEXTREG_REG = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x00;      // border black
  IO_NEXTREG_DAT = 0x03;      // border blue
  IO_NEXTREG_DAT = 0x1c;      // border red
  IO_NEXTREG_DAT = 0xE3;      // border magenta
  IO_NEXTREG_DAT = 0x10;      // border green
  IO_NEXTREG_DAT = 0x1f;      // border cyan
  IO_NEXTREG_DAT = 0xfc;      // border yellow
  IO_NEXTREG_DAT = 0xff;      // border white
}

static void Initialise()
{
//  zx_border(INK_BLUE);

  // Enable the lowres screen, show sprites
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_LSU; 

//  loResSetInitPallete();

  if ( !sprites_load_patterns())
  {
//    zx_border(7);
  }

  CreateBallSprite();
}

static void CreateBallSprite()
{
  set_sprite(128, 64, 4);

  unsigned char spritesize = 16;

  // data is 
  //  sprite size offset x
  //  sprite size offset y
  //  sprite texture
  signed char childsprites[] = {
     -1,-1,0, 0,-1,1, 1,-1,2,
     -1,0,3, 1,0,5,
     -1,1,6, 0,1,7, 1,1,8
  };
  
  for (unsigned char childSprite = 0; childSprite < sizeof(childsprites); childSprite+=3)
  {
    set_sprite_composite(childsprites[childSprite] * spritesize, childsprites[childSprite + 1] * spritesize, childsprites[childSprite + 2]);
  }
}

