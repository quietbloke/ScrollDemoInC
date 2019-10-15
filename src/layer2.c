#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/zxn/esxdos.h>
#include "defines.h"
#include "layer2.h"
#include "scroller.h"

void layer2WriteCharacter( unsigned char* charAddress, unsigned char* layer2Address, char character);
void layer2WriteBigCharacter( unsigned char* charAddress, unsigned char* layer2Address, char character);

bool loadFont(char* filename, unsigned int startBank, unsigned char totalBanks)
{
  uint8_t filehandle;
  errno = 0;

  int saveBank7 = ZXN_READ_MMU0();

  filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

  for (unsigned char bankIndex = 0; bankIndex <= totalBanks; bankIndex++)
  {
    unsigned char* destination = 0x2000;
    ZXN_WRITE_MMU1(startBank + bankIndex);

    esxdos_f_read(filehandle, (void *) destination, 1024*8);
    if (errno)
    {
      return false;
    }
  }

  esxdos_f_close(filehandle);
  ZXN_WRITE_MMU1(0xff);

  return true;
}

void layer2Initialise()
{
  // specify which ram bank the layer2 data exists ( note : this is a 16k page)
  ZXN_NEXTREG(REG_LAYER_2_RAM_BANK, layer216KBankStart);

  // Load fonts
  loadFont("font.nxt", fontBankStart, 8);
  loadFont("font2.nxt", font2BankStart, 8);
  loadFont("font3.nxt", font3BankStart, 8);
  loadFont("font4.nxt", font4BankStart, 2);
}

void layer2Clear(unsigned char colour)
{
  for ( unsigned char bank = layer2BankStart; bank < layer2BankStart+6; bank++)
  {
    ZXN_WRITE_MMU1(bank);

    unsigned char* basepokeaddress = (unsigned char *)0x2000;

    unsigned char col = 0;
    for ( unsigned int p=0; p < 0x2000; p++)
    {
      basepokeaddress[p] = colour; //col++;
    }
  }

  ZXN_WRITE_MMU1(255);
}

void layer2Show()
{
  IO_LAYER_2_CONFIG = IL2C_SHOW_LAYER_2;
}

void layer2WriteText( unsigned char xTile,  unsigned char yTile, char* message)
{
  if ( xTile > 15)
    return;
  if ( yTile > 11)
    return;

  if ( message == "")
    return;

  unsigned char bankOffset = yTile >> 1;

  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU6(font4BankStart+1);
  ZXN_WRITE_MMU1(layer2BankStart + bankOffset );

  unsigned char* fontaddress = (unsigned char *)0xc000;
  unsigned char* layer2address = (unsigned char *)(0x2000 + (16 * xTile));

  unsigned char isOddRow = yTile & 0x01;

  if ( isOddRow == 0x01)
  {
    layer2address += 0x1000;
  }

  unsigned char charPos = 0;
  while(message[charPos] != 0x00)
  {
    unsigned char character = message[charPos];
    if ( character > 63)
    {
      character -= 64;
    }
    if ( character < 32)
    {
      layer2WriteCharacter(fontaddress, layer2address, character);
    }
    charPos++;
    layer2address += 16;
  }

//  for ( unsigned int chary = 0; chary < 16; chary++)
//  {
//    for ( unsigned int charx = 0; charx < 16; charx++)
//    {
//      unsigned char pixel = fontaddress[chary * 16 + charx];
//      layer2address[chary * 256 + charx] = pixel;
//    }
//  }

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);
}

void layer2WriteCharacter( unsigned char* charAddress, unsigned char* layer2Address, char character)
{
  for ( unsigned int chary = 0; chary < 16; chary++)
  {
    for ( unsigned int charx = 0; charx < 16; charx++)
    {
      unsigned char pixel = charAddress[character * 256 + chary * 16 + charx];
      layer2Address[chary * 256 + charx] = pixel;
    }
  }
}

void layer2WriteBigText( unsigned char xTile,  unsigned char yTile, char* message, unsigned int fontBank)
{
  if ( xTile > 7)
    return;
  if ( yTile > 5)
    return;

  if ( message == "")
    return;

  unsigned char bankOffset = yTile;

  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU1(layer2BankStart + bankOffset );

  unsigned char* fontaddress = (unsigned char *)0xc000;
  unsigned char* layer2address = (unsigned char *)(0x2000 + (32 * xTile));

  unsigned char charPos = 0;
  while(message[charPos] != 0x00)
  {
    unsigned char character = message[charPos];

    character -= 32;
    unsigned char fontCharBankOffset = character / 8;

    ZXN_WRITE_MMU6(fontBank+fontCharBankOffset);

    character = character & 0x07;

    layer2WriteBigCharacter(fontaddress, layer2address, character);

    charPos++;
    layer2address += 32;
  }

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);
}

void layer2WriteBigCharacter( unsigned char* charAddress, unsigned char* layer2Address, char character)
{
  for ( unsigned int chary = 0; chary < 32; chary++)
  {
    for ( unsigned int charx = 0; charx < 32; charx++)
    {
      unsigned char pixel = charAddress[character * 1024 + chary * 32 + charx];
      layer2Address[chary * 256 + charx] = pixel;
    }
  }
}

void layer2WriteBigCharacterSlice( unsigned char xTile,  unsigned char yTile, unsigned char fontNumber, char character, unsigned char xOffset, unsigned char width)
{
  unsigned char xOffEnd = xOffset + width;

  if ( xOffset > 31)
  {
    return;
  }

  if ( xOffEnd > 31)
  {
    xOffEnd = 31;
  }

  unsigned char bankOffset = yTile;

  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU1(layer2BankStart + bankOffset );

  unsigned char* fontaddress = (unsigned char *)0xc000;
  unsigned char* layer2address = (unsigned char *)(0x2000 + (32 * xTile));

  character -= 32;
  unsigned char fontCharBankOffset = character / 8;

  character = character & 0x07;

  int fontBank = fontBankStart;

  ZXN_WRITE_MMU6(fontBank+fontCharBankOffset);

  fontaddress += character * 1024;
  for ( unsigned int chary = 0; chary < 32; chary++)
  {
    for ( unsigned int charx = xOffset; charx <= xOffEnd; charx++)
    {
      unsigned char pixel = fontaddress[chary * 32 + charx];
      layer2address[chary * 256 + charx] = pixel;
    }
  }

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);

}

//void layer2WriteBigCharacterSliceFast( unsigned char xTile,  unsigned char yTile, unsigned char fontNumber, char character, unsigned char xOffset, unsigned char width)
void layer2WriteBigCharacterSliceFast( Scroller* scroller)
{
  unsigned char xOffEnd = scroller->charXPos + scroller->scrollSize;

//  if ( xOffset > 31)
//  {
//    return;
//  }
//
//  if ( xOffEnd > 31)
//  {
//    xOffEnd = 31;
//  }

  unsigned char bankOffset = scroller->yTile;

  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU1(layer2BankStart + bankOffset );

  unsigned char* fontaddress = (unsigned char *)0xc000;
  unsigned char* layer2address = (unsigned char *)(0x2000 + (32 * scroller->xTile)) + scroller->charXPos;

  unsigned char character = scroller->message[scroller->charPos] - 32;
  unsigned char fontCharBankOffset = character / 8;

  character = character & 0x07;

  ZXN_WRITE_MMU6(scroller->fontBank + fontCharBankOffset);

  unsigned char fontNextRow = 32 - scroller->scrollSize;
  unsigned char screenNextRow = 256 - scroller->scrollSize; 

  fontaddress += character * 1024 + scroller->charXPos;

  for ( unsigned int chary = 0; chary < 32; chary++)
  {
    for ( unsigned int charx = 0; charx < scroller->scrollSize; charx++)
    {
      *layer2address++ = *fontaddress++;
    }
//    fontaddress += 32 - scroller->scrollSize;
//    layer2address += 256 - scroller->scrollSize;
    fontaddress += fontNextRow;
    layer2address += screenNextRow;
  }

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);

}