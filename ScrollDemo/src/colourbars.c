#include <arch/zxn.h>
#include "dma.h"

#define COPPER_WAIT     0x80

#define BAR_MAX_SIZE            14
#define MAX_BARS                10
#define BAR_FIRST_PAUSE         0
#define BAR_MOVE_PAUSE          0
#define BAR_NEXT_PAUSE          (18 * (BAR_MOVE_PAUSE+1))
#define BAR_COLOUR_PAUSE        500
#define BAR_COLOUR_FADE_PAUSE   10
#define BAR_COLOUR_SET_NUM      5

/* ---------------------------------- */

static void ColourBars_Build();
static void ColourBars_DrawBar(unsigned char barNumber, unsigned char sectionLine);

/* ---------------------------------- */

unsigned char sectionLines[96];

unsigned char currentBarColours[] = {0,0,0,0,0,0,0}; // current colours
unsigned char targetBarColours[] = {0,0,0,0,0,0,0};  // target colours

unsigned char colourFadeMask = 7;                   // value 0 - 7
unsigned char nextColourSet = 0;                    // value 0 - 5
unsigned char colourPause = 0;                      // time to wait before picking next colourset

unsigned char barColours[] = 
{
  (1 << 5),     (2 << 5),     (3 << 5),     (4 << 5),     (5 << 5),     (6 << 5),     (7 << 5),       // red
  (1 << 5) + 0, (2 << 5) + 1, (3 << 5) + 1, (4 << 5) + 2, (5 << 5) + 2, (6 << 5) + 3, (7 << 5) + 2,   // purple (red and blue)
  (1 << 2) + 0, (2 << 2) + 1, (3 << 2) + 1, (4 << 2) + 2, (5 << 2) + 2, (6 << 2) + 3, (7 << 2) + 3,   // cyan (green and blue)
  (1 << 2) + 0, (2 << 2) + 0, (3 << 2) + 0, (4 << 2) + 0, (5 << 2) + 0, (6 << 2) + 0, (7 << 2) + 0,   // green
  (1 << 2) + (1 << 5 ), (2 << 2) + (2 << 5 ), (3 << 2) + (3 << 5 ), (4 << 2) + (4 << 5 ), 
  (5 << 2) + (5 << 5 ), (6 << 2) + (6 << 5 ), (7 << 2) + (7 << 5 ),   // green and red
};

unsigned char ColourBarSineData[] = {
  63 , 64 , 65 , 65 , 66 , 67 , 68 , 68,
  69 , 70 , 70 , 71 , 72 , 72 , 73 , 73,
  74 , 74 , 75 , 75 , 76 , 76 , 77 , 77,
  77 , 78 , 78 , 78 , 79 , 79 , 79 , 79,
  79 , 80 , 80 , 80 , 80 , 80 , 80 , 80,
  80 , 80 , 80 , 80 , 80 , 80 , 79 , 79,
  79 , 79 , 79 , 78 , 78 , 78 , 77 , 77,
  77 , 76 , 76 , 75 , 75 , 74 , 74 , 73,
  73 , 72 , 72 , 71 , 70 , 70 , 69 , 68,
  68 , 67 , 66 , 65 , 65 , 64 , 63 , 62,
  61 , 61 , 60 , 59 , 58 , 57 , 56 , 55,
  54 , 53 , 53 , 52 , 51 , 50 , 49 , 48,
  47 , 46 , 45 , 44 , 43 , 42 , 41 , 40,
  39 , 38 , 37 , 36 , 35 , 34 , 33 , 32,
  31 , 30 , 29 , 28 , 27 , 27 , 26 , 25,
  24 , 23 , 22 , 21 , 20 , 19 , 19 , 18,
  17 , 16 , 15 , 15 , 14 , 13 , 12 , 12,
  11 , 10 , 10 , 9 , 8 , 8 , 7 , 7,
  6 , 6 , 5 , 5 , 4 , 4 , 3 , 3,
  3 , 2 , 2 , 2 , 1 , 1 , 1 , 1,
  1 , 0 , 0 , 0 , 0 , 0 , 0 , 0,
  0 , 0 , 0 , 0 , 0 , 0 , 1 , 1,
  1 , 1 , 1 , 2 , 2 , 2 , 3 , 3,
  3 , 4 , 4 , 5 , 5 , 6 , 6 , 7,
  7 , 8 , 8 , 9 , 10 , 10 , 11 , 12,
  12 , 13 , 14 , 15 , 15 , 16 , 17 , 18,
  19 , 19 , 20 , 21 , 22 , 23 , 24 , 25,
  26 , 27 , 27 , 28 , 29 , 30 , 31 , 32,
  33 , 34 , 35 , 36 , 37 , 38 , 39 , 40,
  41 , 42 , 43 , 44 , 45 , 46 , 47 , 48,
  49 , 50 , 51 , 52 , 53 , 53 , 54 , 55,
  56 , 57 , 58 , 59 , 60 , 61 , 61 , 62,
};

unsigned char bars[MAX_BARS];

unsigned char barPause = 0; // BAR_FIRST_PAUSE;
unsigned char barCount = 0;
unsigned char barMoveCount = 0;

void ColourBars_Init()
{

}

void ColourBars_Update()
{
  // is it time to create a new bar
  if ( barPause == 0)
  {
    barPause = BAR_NEXT_PAUSE;
    if ( barCount < MAX_BARS)
    {
      bars[barCount] = 128+32;
      barCount++;
    }
  }
  else
  {
    barPause--;
  }

  // If there are no bars visible then we are done
  if ( barCount == 0)
  {
    return;
  }

  if ( barMoveCount == 0 )
  {
    // Update the positions of all the bars
    for ( unsigned char idx=0; idx < barCount; idx++)
    {
      bars[idx]++;
    }
    barMoveCount = BAR_MOVE_PAUSE;
  }
  else
  {
    barMoveCount--;
  }

  // is it time to switch colour sets / fade
  if ( colourPause > 0)
  {
    colourPause--;
  }
  else
  {
    // if we are in the process of fading
    if ( colourFadeMask > 0)
    {
      for(unsigned char c = 0; c < 7; c++)
      {
        // See if blue needs to change towards target
        unsigned char newcolour = currentBarColours[c];

        // blue component
        unsigned char colour = currentBarColours[c] & 0x03;
        unsigned char targetcolour = barColours[nextColourSet*7 + c] & 0x03;

        if ( colour < targetcolour)
        {
          newcolour += 1;
        }
        if ( colour > targetcolour)
        {
          newcolour -= 1;
        }

        // green component
        colour = currentBarColours[c] & 0x1c;
        targetcolour = barColours[nextColourSet*7 + c] & 0x1c;

        if ( colour < targetcolour)
        {
          newcolour += 4;
        }
        if ( colour > targetcolour)
        {
          newcolour -= 4;
        }

        // red component
        colour = currentBarColours[c] & 0xe0;
        targetcolour = barColours[nextColourSet*7 + c] & 0xe0;

        if ( colour < targetcolour)
        {
          newcolour += 32;
        }
        if ( colour > targetcolour)
        {
          newcolour -= 32;
        }

        currentBarColours[c] = newcolour;
      }
      // set the colour fade mask
      colourFadeMask--; // all 3 bits set
      colourPause = BAR_COLOUR_FADE_PAUSE;
    }
    else
    {
      // reset the fade mask
      colourFadeMask = 7;
      // select what the next colourset will be
      nextColourSet++;
      if ( nextColourSet >= BAR_COLOUR_SET_NUM)
      {
        nextColourSet = 0;
      }
      // wait a while with this colour set
      colourPause = BAR_COLOUR_PAUSE;
    }
  }
  ColourBars_Build();
}

static void ColourBars_Build()
{
  // reset the sectionLines
  TransferValueToMemory(0, sectionLines, 96);

  if ( barCount > 0)
  {
    for( unsigned char barIdx = barCount-1; barIdx < 255; barIdx--)
    {
      ColourBars_DrawBar(barIdx, ColourBarSineData[bars[barIdx]]);
    }
  }
}

void ColourBars_Copper_Section(unsigned char section)
{
  unsigned char screenScanLine    = section * 32 + 32;
  unsigned char *sectionPos       = &sectionLines[0] + section * 32;
  unsigned char colour            = *sectionPos; 
  unsigned char prevColour; 

  colour = *sectionPos++; 
  prevColour = colour;
  if ( colour != 0)
  {
    IO_NEXTREG_DAT = REG_PALETTE_INDEX;
    IO_NEXTREG_DAT = 0x00;
    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = colour;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = colour;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = colour;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = colour;
  }
  else
  {
    IO_NEXTREG_DAT = REG_PALETTE_INDEX;
    IO_NEXTREG_DAT = 0x00;
    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x00;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x20;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x40;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x60;
  }

  for ( unsigned char sectionLine = 1; sectionLine < 32; sectionLine++)
  {
    prevColour = colour;
    colour = *sectionPos++; 

    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = screenScanLine++;

    if ( colour != prevColour)
    {
      if ( colour != 0)
      {
        IO_NEXTREG_DAT = REG_PALETTE_INDEX;
        IO_NEXTREG_DAT = 0x00;
        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = colour;

        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = colour;

        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = colour;

        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = colour;
      }
      else
      {
        IO_NEXTREG_DAT = REG_PALETTE_INDEX;
        IO_NEXTREG_DAT = 0x00;
        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = 0x00;

        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = 0x20;

        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = 0x40;

        IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
        IO_NEXTREG_DAT = 0x60;
      }
    }
  }
}

void ColourBars_Copper_Done()
{
  IO_NEXTREG_DAT = REG_PALETTE_INDEX;
  IO_NEXTREG_DAT = 0x00;
  IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x00;

  IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x20;

  IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x40;

  IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
  IO_NEXTREG_DAT = 0x60;
}

static void ColourBars_DrawBar(unsigned char barNumber, unsigned char sectionLine)
{
  unsigned char barHalfHeight = 7 - (barNumber / 2);
  unsigned char barFullHeight = barHalfHeight * 2;
  unsigned char* barColourPtr = currentBarColours;
  unsigned char barLine = 0;

  unsigned char* sectionLinePtr = &sectionLines[sectionLine]; 
  sectionLinePtr += (barNumber/2);

  for ( barLine = 0; barLine < barHalfHeight; barLine++)
  {
    *sectionLinePtr++ = *barColourPtr++;
  }

  barColourPtr--;
  for ( barLine = 0; barLine < barHalfHeight; barLine++)
  {
    *sectionLinePtr++ = *barColourPtr--;
  }
}
