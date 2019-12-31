#include <stdlib.h>
#include <stddef.h>
#include <input.h>
#include <arch/zxn.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <intrinsic.h>

#include "lores.h"

unsigned char borderColour = 0;
unsigned char clearColour = 1;

/* --------------------------------- */

static void Initialise();
static void SetBorder(unsigned char border);
static void Update();
static void Render();

/* --------------------------------- */

static int16_t sinTable[] = {
0,6,12,18,24,31,37,43,49,55,61,68,74,79,85,91,
97,103,109,114,120,125,131,136,141,146,151,156,161,
166,171,175,180,184,188,193,197,201,204,208,212,215,
218,221,224,227,230,233,235,237,240,242,244,245,247,248,250,
251,252, 253, 254,255, 254, 253,252,251,250,248,247,245,244,242,240,237,
235,233,230,227,224,221,218,215,212,208,204,201,197,193,188,184,180,175,
171,166,161,156,151,146,141,136,131,125,120,114,109,103,97,91,85,79,
74,68,61,55,49,43,37,31,24,18,12,6,0,-6,-12,-18,-24,-31,-37,-43,-49,
-55,-61,-68,-74,-79,-85,-91,-97,-103,-109,-114,-120,-125,-131,-136,-141,-146,-151,
-156,-161,-166,-171,-175,-180,-184,-188,-193,-197,-201,-204,-208,-212,-215,-218,-221,
-224,-227,-230,-233,-235,-237,-240,-242,-244,-245,-247,-248,-250,-251,-252, -253, -254,
-255, -254, -253,-252,-251,-250,-248,-247,-245,-244,-242,-240,-237,-235,-233,-230,-227,
-224,-221,-218,-215,-212,-208,-204,-201,-197,-193,-188,-184,-180,-175,-171,-166,-161,-156,
-151,-146,-141,-136,-131,-125,-120,-114,-109,-103,-97,-91,-85,-79,-74,-68,-61,-55,-49,
-43,-37,-31,-24,-18,12,-6
};

static int16_t cosTable[] = {
255, 254, 253,252,251,250,248,247,245,244,242,240,237,235,233,230,227,
224,221,218,215,212,208,204,201,197,193,188,184,180,175,171,166,161,
156,151,146,141,136,131,125,120,114,109,103,97,91,85,79,74,68,
61,55,49,43,37,31,24,18,12,6,0,-6,-12,-18,-24,-31,-37,
-43,-49,-55,-61,-68,-74,-79,-85,-91,-97,-103,-109,-114,-120,-125,-131,-136,
-141,-146,-151,-156,-161,-166,-171,-175,-180,-184,-188,-193,-197,-201,-204,-208,-212,
-215,-218,-221,-224,-227,-230,-233,-235,-237,-240,-242,-244,-245,-247,-248,-250,-251,
-252, -253, -254,-255, -254, -253,-252,-251,-250,-248,-247,-245,-244,-242,-240,-237,-235,
-233,-230,-227,-224,-221,-218,-215,-212,-208,-204,-201,-197,-193,-188,-184,-180,-175,-171,
-166,-161,-156,-151,-146,-141,-136,-131,-125,-120,-114,-109,-103,-97,-91,-85,-79,
-74,-68,-61,-55,-49,-43,-37,-31,-24,-18,-12,-6,0,6,12,18,24,
31,37,43,49,55,61,68,74,79,85,91,97,103,109,114,120,125,
131,136,141,146,151,156,161,166,171,175,180,184,188,193,197,201,204,
208,212,215,218,221,224,227,230,233,235,237,240,242,244,245,247,248,
250,251,252, 253, 254
};

#define MAX_SPARKS  100
#define FLOAT_SHIFT 6

uint8_t randValues [256];

uint16_t explosionPause = 0;

typedef struct Spark
{
  int16_t   xpos;
  int16_t   ypos;
  int16_t   prevxpos;
  int16_t   prevypos;
  int16_t   xvel;
  int16_t   yvel;
  uint8_t   colour;
  int16_t   life;
} Spark;

Spark sparks[MAX_SPARKS];

uint8_t nextSpark = 0;

void InitSparks()
{
  for (uint8_t s = 0; s < MAX_SPARKS; s++ )
  {
    Spark* spark = &sparks[s];
    spark->xpos = 0;
    spark->ypos = 0;
    spark->life = 0;
  }
}

void explosion(uint8_t numberOfSparks)
{
  // get position in screen
  int16_t xCenter = (RandomGet()%100 + 14) << FLOAT_SHIFT;
  int16_t yCenter = (RandomGet()%50 + 14) << FLOAT_SHIFT;
  uint8_t colour = RandomGet()%16;

  for ( uint8_t l = 0; l < numberOfSparks; l++)
  {
    uint8_t angle = RandomGet();
    int16_t velocity = RandomGet()%2 + 3;
    Spark* spark = &sparks[nextSpark];
    spark->xpos = xCenter;
    spark->ypos = yCenter;
    spark->xvel = (sinTable[angle] * velocity) / 16;
    spark->yvel = (cosTable[angle] * velocity) / 16;
    spark->colour = colour;
    spark->life = RandomGet()%20 + 80;

    nextSpark++;
    if ( nextSpark >= MAX_SPARKS)
    {
      nextSpark = 0;
    }
  }
}

bool doExplosion = false;
int main(void)
{
  zx_border(INK_BLACK);
  ZXN_WRITE_REG(REG_TURBO_MODE, 3);

  Initialise();
  InitSparks();

  while(!in_key_pressed(IN_KEY_SCANCODE_SPACE) )
  {
    SetBorder(INK_BLACK);
    // wait until line 192 is active
    IO_NEXTREG_REG = REG_ACTIVE_VIDEO_LINE_L;

    // REG_ACTIVE_VIDEO_LINE_H
    while (IO_NEXTREG_DAT != 192);

    SetBorder(INK_BLUE);

    Render();
    SetBorder(INK_RED);

    Update();
   
    SetBorder(INK_BLACK);
  }

  SetBorder(INK_RED);
  return 0;
}

static void SetBorder(unsigned char border)
{
//  zx_border(border);
  borderColour = border;
}

bool e_pressed = false;

static void Update()
{
    if (in_key_pressed(IN_KEY_SCANCODE_e))
    {
      if (e_pressed == false)
      {
        doExplosion = true;
        e_pressed = true;
      }
    }
    else
    {
      e_pressed = false;
    }

    if ( explosionPause == 0)
    {
      doExplosion = true;
    }
    else
    {
      explosionPause--;
    }
    

    if (doExplosion == true)
    {
      SetBorder(INK_GREEN);
      explosion(40);
      explosionPause = 20 + RandomGet()%20;
      doExplosion = false;
    }
}

uint8_t randomPos = 0;

static uint8_t RandomGet()
{
  return randValues[randomPos++];
}

static void Render()
{
  Spark* spark;
  for ( uint8_t s = 0; s < MAX_SPARKS; s++)
  {
    spark = &sparks[s];
    // delete the old sprite pos
    loResPlot(spark->prevxpos >> FLOAT_SHIFT, spark->prevypos >> FLOAT_SHIFT, 0);
    if (spark->life > 0 )
    {
      spark->xpos += spark->xvel;
      spark->ypos += spark->yvel;
      loResPlot(spark->xpos >> FLOAT_SHIFT, spark->ypos >> FLOAT_SHIFT, spark->colour);
      spark->prevxpos = spark->xpos;
      spark->prevypos = spark->ypos;
      if ( spark->xvel > 0 )
      {
        spark->xvel--;
      }
      else if ( spark->xvel < 0 )
      {
        spark->xvel++;
      }
//      if ( spark->yvel > 0 )
//      {
//        spark->yvel--;
//      }
      else if ( spark->yvel < 0 )
      {
        spark->yvel++;
      }
      spark->yvel += 1;
      if ( spark->yvel > 128)
      {
        spark->yvel = 128;
      }
      spark->life--;
    }
  }

}

static void Initialise()
{
  IO_NEXTREG_REG = REG_SPRITE_LAYER_SYSTEM;
  IO_NEXTREG_DAT = RSLS_ENABLE_LORES | RSLS_LAYER_PRIORITY_LSU; 

  loResSetInitPallete();

  // build a random(ish) number table
  uint8_t r = 0;
  while ( r < 255)
  {
    uint8_t v = rand()%256;
    randValues[r] = v;
    r++;
  }
}

