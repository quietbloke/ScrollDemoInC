#include <arch/zxn.h>

#define COPPER_WAIT     0x80

#define BAR_MAX_SIZE        16
#define MAX_BARS            10
#define BAR_FIRST_PAUSE     200
#define BAR_NEXT_PAUSE      20
#define BAR_MOVE_DELAY	    2

unsigned char sectionLines[96];

unsigned char barColours[] = 
{
    32,64,96,128,160,192,224,232
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

unsigned char barPause = BAR_FIRST_PAUSE;
unsigned char barCount = 0;
unsigned char barMoveWait = 0;

void ColourBars_Init()
{

}

void ColourBars_Update()
{
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

    for ( unsigned char idx=0; idx < barCount; idx++)
    {
        bars[idx]++;
    }
}

void barsDrawBar(unsigned char barNumber, unsigned char sectionLine)
{
    unsigned char barHalfHeight = 8 - (barNumber / 2);
    unsigned char barFullHeight = barHalfHeight * 2;
    unsigned char barColourIdx = 0;
    unsigned char barLine = 0;

    for ( barLine = 0; barLine < barHalfHeight; barLine++, barColourIdx++, sectionLine++)
    {
        sectionLines[sectionLine] = barColours[barColourIdx];
    }
    barColourIdx--;
    for ( barLine = 0; barLine < barHalfHeight; barLine++, barColourIdx--, sectionLine++)
    {
        sectionLines[sectionLine] = barColours[barColourIdx];
    }
}

void ColourBars_Build()
{
    // reset the sectionLines
    for ( unsigned char i = 0; i < 96; i++)
    {
        sectionLines[i] = 0;
    }

//    if ( barMoveWait == 0)
//    {
        barMoveWait = BAR_MOVE_DELAY;

//        if ( barCount < MAX_BARS)
//        {
//            barPause--;
//            if ( barPause == 0)
//            {
//                barPause = BAR_NEXT_PAUSE;
//                barCount++;
//            }
//        }
//    }
    if ( barCount > 0)
    {
        for( unsigned char barIdx = barCount-1; barIdx < 255; barIdx--)
        {
            barsDrawBar(barIdx, ColourBarSineData[bars[barIdx]]);
        }
    }
//    barsDrawBar(0, 10);
//    barsDrawBar(0, 30);
}


void BarsRenderSection(unsigned char section)
{
    unsigned char screenScanLine = section * 32 + 32;
    unsigned char sectionPos = section * 32;
 
    for ( unsigned char sectionLine = 0; sectionLine < 32; sectionLine++, sectionPos++, screenScanLine++)
    {
        unsigned char colour = sectionLines[sectionPos]; 

        if ( sectionLine != 0)
        {
            IO_NEXTREG_DAT = COPPER_WAIT;
            IO_NEXTREG_DAT = screenScanLine;
        }

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


void ColourBars_Update_Done()
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
