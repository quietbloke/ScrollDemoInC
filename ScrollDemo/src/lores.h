#ifndef LOWRES_H
#define LOWRES_H

void loResSetInitPallete();
void loResPlot(uint8_t xpos, uint8_t ypos, uint8_t colourIndex);
bool loResLoadImage(char* filename);
void loResSetOffsetX(int8_t xOffset);
void loResSetOffsetY(int8_t yOffset);
void loResSetClipWindow(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);

#endif