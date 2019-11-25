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
#include "vt_sound.h"
#include "music.h"

extern uint8_t music_module[];

unsigned char borderColour;
bool musicInInterrupt = false;

/* --------------------------------- */

static void initialise();
static void SetBorder(unsigned char border);

/* --------------------------------- */

IM2_DEFINE_ISR_8080(isr)
{
  unsigned char save;
  unsigned char borderSave;
   
   // save nextreg register
  save = IO_NEXTREG_REG;
  borderSave = borderColour;
  SetBorder(INK_WHITE);

  if (musicInInterrupt == true)
  {
    // swap in the music bank
    ZXN_WRITE_MMU2(musicBankStart);

    vt_play();

    // swap it back out again
    ZXN_WRITE_MMU2(0xff);
  }

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
    // wait until line 192 is active

    // REG_ACTIVE_VIDEO_LINE_H
    IO_NEXTREG_REG = REG_ACTIVE_VIDEO_LINE_L;

    while (IO_NEXTREG_DAT != 192);

    SetBorder(INK_GREEN);

    if (musicInInterrupt == false)
    {
      // swap in the music bank
      ZXN_WRITE_MMU2(musicBankStart);

      vt_play();

      // swap it back out again
      ZXN_WRITE_MMU2(0xff);
    }

    SetBorder(INK_BLUE);
    Update();

    SetBorder(INK_CYAN);
    Render();

    SetBorder(INK_BLACK);
  }

  vt_mute();

  return 0;
}

/* --------------------------------- */

static void Update()
{
  if( in_key_pressed(IN_KEY_SCANCODE_i))
  {
    musicInInterrupt = true;
  }
  if( in_key_pressed(IN_KEY_SCANCODE_c))
  {
    musicInInterrupt = false;
  }
}

static void Render()
{

}

static void initialise()
{
  // Enable the lowres screen, show sprites
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_SPRITES_VISIBLE | RSLS_LAYER_PRIORITY_LSU; 
}
