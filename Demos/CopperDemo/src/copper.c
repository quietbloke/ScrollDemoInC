#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>

#define COPPER_WAIT 0x80;    // is this RCCH_COPPER_RUN_LOOP ??
#define LINES_PER_BLOCK 16
unsigned char topColour = 0;

void copperRun(unsigned char numberOfBlocks)
{
  // Start Send
  ZXN_WRITE_REG(REG_COPPER_CONTROL_H, 0);
  ZXN_WRITE_REG(REG_COPPER_CONTROL_L, 0);

  // Select copper data port
  IO_NEXTREG_REG = REG_COPPER_DATA;

  unsigned currentColour = topColour;
  for ( unsigned char yLine = 0; yLine < numberOfBlocks * LINES_PER_BLOCK; yLine++)
  {
    // wait till scanline we want
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = yLine;

    // change colour of index 0
    IO_NEXTREG_DAT = REG_PALETTE_INDEX;
    IO_NEXTREG_DAT = 0x00;
    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = currentColour++;
  }

  // The next line after we must set the colour index zero back to what it is supposed to be
  IO_NEXTREG_DAT = COPPER_WAIT;
  IO_NEXTREG_DAT = numberOfBlocks * LINES_PER_BLOCK;

  // change colour of index 0
  IO_NEXTREG_DAT = REG_PALETTE_INDEX;
  IO_NEXTREG_DAT = 0x00;
  IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0;

  topColour--;

  // done
  IO_NEXTREG_DAT = 0xff;
  IO_NEXTREG_DAT = 0xff;

    // Start
  ZXN_WRITE_REG(REG_COPPER_CONTROL_H, 0xc0); // or is it RCCH_COPPER_RUN_VBI
  ZXN_WRITE_REG(REG_COPPER_CONTROL_L, 0);
}
