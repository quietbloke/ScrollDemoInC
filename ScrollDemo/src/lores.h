#ifndef LOWRES_H
#define LOWRES_H

void loResSetInitPallete();
void loResPlot(unsigned char xpos, unsigned char ypos, unsigned char colourIndex);
bool loResLoadImage(char* filename);
void loResSetOffsetX(signed char xOffset);
void loResSetOffsetY(signed char yOffset);
void loResSetClipWindow(unsigned char left, unsigned char right, unsigned char top, unsigned char bottom);

#endif