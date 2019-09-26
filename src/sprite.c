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

void enable_sprites()
{
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = SPRITES_VISIBLE; 
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
    do {
        IO_SPRITE_PATTERN = sprite_pattern[index];
    } while(index++ != 255);
}

bool sprites_load_patterns(char* filename)
{
  uint8_t filehandle;
  errno = 0;
  unsigned char col;

  set_sprite_pattern_index(0);

  filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

  for ( unsigned int l = 0; l < 32 * 32 * 8 * 2; l++  )
  {
    esxdos_f_read(filehandle, (void *) &col, 1);
    if (errno)
    {
      return false;
    }

    IO_SPRITE_PATTERN = col;
  }

  esxdos_f_close(filehandle);

  return true;
}
