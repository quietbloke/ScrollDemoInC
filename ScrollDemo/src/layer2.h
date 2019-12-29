#ifndef LAYER2_H
#define LAYER2_H

#include "scroller.h"

void layer2Initialise();
void layer2SetClipWindow(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);
void layer2Clear(uint8_t colour);
void layer2Show();
void layer2WriteText( uint8_t xTile, uint8_t yTile, char* message);
void layer2WriteBigText( uint8_t xTile,  uint8_t yTile, char* message, uint16_t fontBank);
void layer2WriteBigCharacterSliceFast( Scroller* scroller);

#endif
