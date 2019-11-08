#ifndef LAYER2_H
#define LOWRES_H

#include "scroller.h"

void layer2Initialise();
void layer2SetClipWindow(unsigned char left, unsigned char right, unsigned char top, unsigned char bottom);
void layer2Clear(unsigned char colour);
void layer2Show();
void layer2WriteText( unsigned char xTile, unsigned char yTile, char* message);
void layer2WriteBigText( unsigned char xTile,  unsigned char yTile, char* message, unsigned int fontBank);
void layer2WriteBigCharacterSliceFast( Scroller* scroller);

#endif
