#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/zxn/esxdos.h>

#define LOWRES_TOP_HALF     (uint8_t*)(0x4000)
#define LOWRES_BOTTOM_HALF  (uint8_t*)(0x6000)

// NOTE : Assume values for xpos, ypos are valid ( 0-127, 0-95)
void loResPlot(uint8_t xpos, uint8_t ypos, uint8_t colourIndex)
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

void loResSetOffsetX(int8_t xOffset)
{
  IO_NEXTREG_REG = REG_LORES_OFFSET_X;
  IO_NEXTREG_DAT = xOffset * 2; 
}

void loResSetOffsetY(int8_t yOffset)
{
  IO_NEXTREG_REG = REG_LORES_OFFSET_Y;
  IO_NEXTREG_DAT = (yOffset * 2) & 0xbf;   // NOTE : restrict to 192 for vertical values 
}

void loResSetClipWindow(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom)
{
  IO_NEXTREG_REG = REG_CLIP_WINDOW_ULA;
  IO_NEXTREG_DAT = left;
  IO_NEXTREG_DAT = right;
  IO_NEXTREG_DAT = top;
  IO_NEXTREG_DAT = bottom;

}

void loResSetInitPallete()
{
  IO_NEXTREG_REG = REG_PALETTE_CONTROL;
  IO_NEXTREG_DAT = RPC_SELECT_ULA_PALETTE_0 | RPC_ENABLE_ULANEXT | RPC_ENABLE_ULA_PALETTE_0;

  IO_NEXTREG_REG = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x00;
  IO_NEXTREG_DAT = 0x20;
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
  IO_NEXTREG_DAT = 0xe0;      // border green
  IO_NEXTREG_DAT = 0x1f;      // border cyan
  IO_NEXTREG_DAT = 0xfc;      // border yellow
  IO_NEXTREG_DAT = 0xff;      // border white
}

bool loResLoadImage(char* filename)
{
  uint8_t filehandle;
  errno = 0;

  filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

  uint8_t* destination = LOWRES_TOP_HALF;

  for ( uint8_t ypos = 0; ypos < 48; ypos++)
  {
    esxdos_f_read(filehandle, (void *) destination, 128);
    if (errno)
    {
      return false;
    }
    destination += 128;
  }

  destination = LOWRES_BOTTOM_HALF;

  for ( uint8_t ypos = 0; ypos < 48; ypos++)
  {
    esxdos_f_read(filehandle, (void *) destination, 128);
    if (errno)
    {
      return false;
    }
    destination += 128;
  }

  esxdos_f_close(filehandle);

  return true;
}
