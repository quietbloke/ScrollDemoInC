#include <intrinsic.h>
#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>

#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/zxn/esxdos.h>

#include "sprite.h"

#define SPRITES_VISIBLE            1
#define SPRITES_OVER_BORDER        2
#define SPRITES_ENABLE_CLIPPING    16

#define ballsBank1Start 52 //( requires 2 banks for frames 0 - 6)
#define ballsBank2Start 54 //( requires 2 banks for frames 7 - 10)

bool load_ball_textures_to_memory(char* filename, unsigned int startBank, unsigned char totalBanks);

void enable_sprites()
{
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = SPRITES_VISIBLE; 
}

void SpriteSetClipWindow(unsigned char left, unsigned char right, unsigned char top, unsigned char bottom)
{
  IO_NEXTREG_REG = REG_CLIP_WINDOW_SPRITES;
  IO_NEXTREG_DAT = left;
  IO_NEXTREG_DAT = right;
  IO_NEXTREG_DAT = top;
  IO_NEXTREG_DAT = bottom;
}

void set_sprite_pattern_index(uint8_t slot)
{
  IO_SPRITE_SLOT = slot;
}

void set_sprite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern)
{
  IO_SPRITE_ATTRIBUTE = xpos; // xpos 
  IO_SPRITE_ATTRIBUTE = ypos; // ypos
  
  IO_SPRITE_ATTRIBUTE = (xpos >> 8) & 1;
  IO_SPRITE_ATTRIBUTE = 0x80 + spritePattern;
}

void set_sprite_composite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern)
{
  // attribute 0
  IO_SPRITE_ATTRIBUTE = xpos; // xpos 
  // attribute 1
  IO_SPRITE_ATTRIBUTE = ypos; // ypos
  
  // attribute 2
  IO_SPRITE_ATTRIBUTE = (xpos >> 8) & 1;

  // attribute 3
  IO_SPRITE_ATTRIBUTE = 0xc0 + spritePattern;

  // attribute 4
  IO_SPRITE_ATTRIBUTE = 0x40;
}

void set_pattern(uint8_t *sprite_pattern)
{
  uint8_t index = 0;
  do 
  {
    IO_SPRITE_PATTERN = sprite_pattern[index];
  } while(index++ != 255);
}

bool sprites_load_patterns()
{
  load_ball_textures_to_memory("balls01.spr", ballsBank1Start, 1);
  load_ball_textures_to_memory("balls02.spr", ballsBank2Start, 1);

  return true;
}

bool load_ball_textures_to_memory(char* filename, unsigned int startBank, unsigned char totalBanks)
{
  uint8_t filehandle;
  errno = 0;

//  int saveBank7 = ZXN_READ_MMU0();

  filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

  for (unsigned char bankIndex = 0; bankIndex <= totalBanks; bankIndex++)
  {
    unsigned char* destination = 0x2000;
    ZXN_WRITE_MMU1(startBank + bankIndex);

    esxdos_f_read(filehandle, (void *) destination, 1024*8);
    if (errno)
    {
      return false;
    }
  }

  esxdos_f_close(filehandle);

  // put back the main memory
  ZXN_WRITE_MMU1(0xff);

  return true;
}
