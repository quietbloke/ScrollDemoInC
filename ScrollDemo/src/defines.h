#ifndef DEFINES_H
#define DEFINES_H

#define layer216KBankStart  9
#define layer2BankStart     (layer216KBankStart*2)

#define fontBankStart  24 // ( requires 8 banks)
#define font2BankStart  32 // ( requires 8 banks)
#define font3BankStart  40 // ( requires 8 banks)
#define font4BankStart 48 //( requires 2 banks)

#define musicBankStart  50  // 2 banks required for the music

#define ballsBank1Start 52 //( requires 2 banks for frames 0 - 6)
#define ballsBank2Start 54 //( requires 2 banks for frames 7 - 10)

#define spriteBankStart 56 //( requires 2 banks )

#define loresBGBankStart 58 //( requires 2 banks )

#endif