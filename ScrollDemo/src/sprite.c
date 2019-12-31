#include <intrinsic.h>
#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>

#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/zxn/esxdos.h>

#include "defines.h"
#include "sprite.h"

#define SPRITES_VISIBLE            1
#define SPRITES_OVER_BORDER        2
#define SPRITES_ENABLE_CLIPPING    16

void enable_sprites()
{
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = SPRITES_VISIBLE; 
}

void SpriteSetClipWindow(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom)
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

void set_sprite(uint16_t xpos, uint8_t ypos, uint8_t spritePattern, bool visible)
{
  uint8_t visibleFlag = 0x00;
  if ( visible == true)
  {
    visibleFlag = 0x80;
  }

  // attribute 0
  IO_SPRITE_ATTRIBUTE = xpos; // xpos 

  // attribute 1
  IO_SPRITE_ATTRIBUTE = ypos; // ypos
  
  // attribute 2
  IO_SPRITE_ATTRIBUTE = (xpos >> 8) & 1;
  
  // attribute 3
  IO_SPRITE_ATTRIBUTE = visibleFlag + spritePattern;
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
  IO_SPRITE_ATTRIBUTE = 0x80 + 0x040 + spritePattern;

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

void create_sprite_patterns()
{
  set_sprite_pattern_index(0);

  for ( uint8_t bank = spriteBankStart; bank <= spriteBankStart+1; bank++)
  {
    ZXN_WRITE_MMU1(bank);

    uint8_t* basepokeaddress = (uint8_t *)0x2000;

    uint8_t col = 0;
    for ( uint16_t p=0; p < 0x2000; p++)
    {
      IO_SPRITE_PATTERN = basepokeaddress[p];
    }
  }

  ZXN_WRITE_MMU1(255);
}
