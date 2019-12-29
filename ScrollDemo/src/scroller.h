#ifndef SCROLLER_H
#define SCROLLER_H

#include <arch/zxn.h>

typedef struct Scroller
{
  uint8_t   fontBank;
  uint8_t   xTile;
  uint8_t   yTile;
  uint8_t   scrXPos;
  uint8_t   charPos;
  uint8_t   charXPos;
  uint8_t   scrollSize;
  char*           message;
  uint16_t    pauseTime;
} Scroller;

void Scroller_Init();
void Scroller_Update();
void Scroller_Render();
Scroller* Scroller_Get(uint8_t scrollerNum);

#endif