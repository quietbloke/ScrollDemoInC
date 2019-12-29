#include <stdlib.h>
#include <stddef.h>
#include <input.h>
#include <arch/zxn.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <intrinsic.h>

#include "copper.h"

#define COPPER_BLOCK_SIZE 16

unsigned char borderColour = 0;
unsigned char copperBlocks = 1;

/* --------------------------------- */

static void Initialise();
static void SetBorder(unsigned char border);
static void TransferValueToMemory(unsigned char value, unsigned char* destination, unsigned int length);
static void TransferValueToMemorySlow(unsigned char value, unsigned char* destination, unsigned int length);
static void Render();
static void Update();
static void loResPlot(uint8_t xpos, uint8_t ypos, uint8_t colourIndex);

/* --------------------------------- */

void SetBorder(unsigned char border)
{
  zx_border(border);
  borderColour = border;
}

unsigned char clearColour = 1;

int main(void)
{
  zx_border(INK_BLUE);
  ZXN_WRITE_REG(REG_TURBO_MODE, 2);

  Initialise();

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

/* --------------------------------- */

static void Update()
{
    if (in_key_pressed(IN_KEY_SCANCODE_p))
    {
      if (copperBlocks < 10)
      {
        copperBlocks++;
      }
    }
    if (in_key_pressed(IN_KEY_SCANCODE_l))
    {
      if (copperBlocks > 1)
      {
        copperBlocks--;
      }
    }

}

static void Render()
{
  copperRun(copperBlocks);
}

static void Initialise()
{
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_LAYER_PRIORITY_LSU; 

  loResSetInitPallete();

  IO_NEXTREG_REG = REG_CLIP_WINDOW_ULA;
  IO_NEXTREG_DAT = 0;
  IO_NEXTREG_DAT = 255;
  IO_NEXTREG_DAT = 0;
  IO_NEXTREG_DAT = 191;

  // clear the lowres screen
  for ( uint8_t y = 0; y < 100; y++)
  {
    for ( uint8_t x = 0; x < 128; x++)
    {
      loResPlot(x,y,0);
    }
  }
}

static void loResSetInitPallete()
{
  IO_NEXTREG_REG = REG_PALETTE_CONTROL;
  IO_NEXTREG_DAT = RPC_SELECT_ULA_PALETTE_0 | RPC_ENABLE_ULANEXT | RPC_ENABLE_ULA_PALETTE_0;

  IO_NEXTREG_REG = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x20;
  IO_NEXTREG_DAT = 0x30;
  IO_NEXTREG_DAT = 0x40;
  IO_NEXTREG_DAT = 0x60;

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

static void loResPlot(uint8_t xpos, uint8_t ypos, uint8_t colourIndex)
{
  uint8_t* basepokeaddress = (uint8_t *)0x4000;
  if (ypos > 47)
  {
    basepokeaddress = (uint8_t *)0x6000;
    ypos -= 48;
  }

  basepokeaddress[ypos * 128 + xpos] = colourIndex;

  return;
}

