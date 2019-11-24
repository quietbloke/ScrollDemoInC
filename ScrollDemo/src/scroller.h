#ifndef SCROLLER_H
#define SCROLLER_H

typedef struct Scroller
{
  unsigned char   fontBank;
  unsigned char   xTile;
  unsigned char   yTile;
  unsigned char   scrXPos;
  unsigned char   charPos;
  unsigned char   charXPos;
  unsigned char   scrollSize;
  char*           message;
  unsigned int    pauseTime;
} Scroller;

void Scroller_Init();
void Scroller_Update();
void Scroller_Render();
Scroller* Scroller_Get(unsigned char scrollerNum);

#endif