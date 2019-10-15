#ifndef LAYER2_H
#define LOWRES_H

#include "scroller.h"

void layer2Initialise();
void layer2Clear(unsigned char colour);
void layer2Show();
void layer2WriteText( unsigned char xTile, unsigned char yTile, char* message);
void layer2WriteBigText( unsigned char xTile,  unsigned char yTile, char* message, unsigned int fontBank);
void layer2WriteBigCharacterSlice( unsigned char xTile,  unsigned char yTile, unsigned char fontNumber, char character, unsigned char xOffset, unsigned char width);
void layer2WriteBigCharacterSliceFast( Scroller* scroller);

#endif
