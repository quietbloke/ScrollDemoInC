#include <arch/zxn.h>

#define COPPER_WAIT     0x80

#define BAR_SIZE        16
#define BAR_DELAY		5

unsigned char barPos = 10;
unsigned char barPosIndex = 0;
unsigned char barDelay = BAR_DELAY;

unsigned char barColours[] = 
{
    32,64,96,128,160,192,224,232, 232,224,192,160,128,96,64,32
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

void ColourBars_Init()
{

}

void ColourBars_Update()
{
    // wait till scanline 0;
    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 130;

    //    IO_NEXTREG_DAT = REG_PALETTE_CONTROL;
    //    IO_NEXTREG_DAT = RPC_SELECT_ULA_PALETTE_0;

    IO_NEXTREG_DAT = REG_PALETTE_INDEX;
    IO_NEXTREG_DAT = 0x00;
    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x70;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x70;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x70;

    IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
    IO_NEXTREG_DAT = 0x70;

    //
    //

    IO_NEXTREG_DAT = COPPER_WAIT;
    IO_NEXTREG_DAT = 140;

    //    IO_NEXTREG_DAT = REG_PALETTE_CONTROL;
    //    IO_NEXTREG_DAT = RPC_SELECT_ULA_PALETTE_0;

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

void ColourBars_Update_Test(unsigned char section)
{
    // section 0 = screen position 0  - 31
    // section 1 = screen position 32 - 63
    // section 2 = screen position 64 - 95
    // section 3 = screen position 96 - 127

    barDelay--;
    if ( barDelay == 0)
    {
        barDelay = BAR_DELAY;
        barPosIndex++;
    }

    barPos = ColourBarSineData[barPosIndex]+32;

    unsigned char sectionTop = section * 32;
    unsigned char sectionBottom = sectionTop + 31;

    unsigned char scanLine = sectionTop;
    unsigned char barPosEnd = barPos + BAR_SIZE;

    unsigned char prevBarColour = 255;
    unsigned char barColour = 0;

    for ( unsigned sectionLine = 0; sectionLine < 32; sectionLine++)
    {
        if (scanLine >= barPos && scanLine < barPosEnd)
        {
            unsigned char colourPos = scanLine - barPos;
            barColour = barColours[colourPos];
        }
        else
        {
            barColour = 0;
        }
        
        if ( barColour != prevBarColour)
        {
            if ( sectionLine != 0)
            {
                // wait till scanline 0;
                IO_NEXTREG_DAT = COPPER_WAIT;
                IO_NEXTREG_DAT = scanLine;
            }

            if (barColour != 0)
            {
                IO_NEXTREG_DAT = REG_PALETTE_INDEX;
                IO_NEXTREG_DAT = 0x00;
                IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
                IO_NEXTREG_DAT = barColour;

                IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
                IO_NEXTREG_DAT = barColour;

                IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
                IO_NEXTREG_DAT = barColour;

                IO_NEXTREG_DAT = REG_PALETTE_VALUE_8;
                IO_NEXTREG_DAT = barColour;
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
            prevBarColour = barColour;
        }
        scanLine++;
    }
}
