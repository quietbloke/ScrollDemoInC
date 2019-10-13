#ifndef LAYER2_H
#define LOWRES_H

#define fontBankStart  24 // ( requires 8 banks)
#define font2BankStart  32 // ( requires 8 banks)
#define font3BankStart  40 // ( requires 8 banks)
#define font4BankStart 48 //( requires 2 banks)

void layer2Initialise();
void layer2Clear(unsigned char colour);
void layer2Show();
void layer2WriteText( unsigned char xTile, unsigned char yTile, char* message);
void layer2WriteBigText( unsigned char xTile,  unsigned char yTile, char* message, unsigned int fontBank);


#endif
