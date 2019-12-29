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

void layer2WriteCharacter( uint8_t* charAddress, uint8_t* layer2Address, char character);
void layer2WriteBigCharacter( uint8_t* charAddress, uint8_t* layer2Address, char character);

void layer2SetClipWindow(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom)
{
  IO_NEXTREG_REG = REG_CLIP_WINDOW_LAYER_2;
  IO_NEXTREG_DAT = left;
  IO_NEXTREG_DAT = right;
  IO_NEXTREG_DAT = top;
  IO_NEXTREG_DAT = bottom;

}

bool loadFont(char* filename, uint16_t startBank, uint8_t totalBanks)
{
  uint8_t filehandle;
  errno = 0;

  int saveBank7 = ZXN_READ_MMU0();

  filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

  for (uint8_t bankIndex = 0; bankIndex <= totalBanks; bankIndex++)
  {
    uint8_t* destination = 0x2000;
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

void layer2Clear(uint8_t colour)
{
  for ( uint8_t bank = layer2BankStart; bank < layer2BankStart+6; bank++)
  {
    ZXN_WRITE_MMU1(bank);

    uint8_t* basepokeaddress = (uint8_t *)0x2000;

    uint8_t col = 0;
    for ( uint16_t p=0; p < 0x2000; p++)
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

void layer2WriteText( uint8_t xTile,  uint8_t yTile, char* message)
{
  if ( xTile > 15)
    return;
  if ( yTile > 11)
    return;

  if ( message == "")
    return;

  uint8_t bankOffset = yTile >> 1;

  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU6(font4BankStart+1);
  ZXN_WRITE_MMU1(layer2BankStart + bankOffset );

  uint8_t* fontaddress = (uint8_t *)0xc000;
  uint8_t* layer2address = (uint8_t *)(0x2000 + (16 * xTile));

  uint8_t isOddRow = yTile & 0x01;

  if ( isOddRow == 0x01)
  {
    layer2address += 0x1000;
  }

  uint8_t charPos = 0;
  while(message[charPos] != 0x00)
  {
    uint8_t character = message[charPos];
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

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);
}

void layer2WriteCharacter( uint8_t* charAddress, uint8_t* layer2Address, char character)
{
  for ( uint16_t chary = 0; chary < 16; chary++)
  {
    for ( uint16_t charx = 0; charx < 16; charx++)
    {
      uint8_t pixel = charAddress[character * 256 + chary * 16 + charx];
      layer2Address[chary * 256 + charx] = pixel;
    }
  }
}

void layer2WriteBigText( uint8_t xTile,  uint8_t yTile, char* message, uint16_t fontBank)
{
  if ( xTile > 7)
    return;
  if ( yTile > 5)
    return;

  if ( message == "")
    return;

  uint8_t bankOffset = yTile;

  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU1(layer2BankStart + bankOffset );

  uint8_t* fontaddress = (uint8_t *)0xc000;
  uint8_t* layer2address = (uint8_t *)(0x2000 + (32 * xTile));

  uint8_t charPos = 0;
  while(message[charPos] != 0x00)
  {
    uint8_t character = message[charPos];

    character -= 32;
    uint8_t fontCharBankOffset = character / 8;

    ZXN_WRITE_MMU6(fontBank+fontCharBankOffset);

    character = character & 0x07;

    layer2WriteBigCharacter(fontaddress, layer2address, character);

    charPos++;
    layer2address += 32;
  }

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);
}

void layer2WriteBigCharacter( uint8_t* charAddress, uint8_t* layer2Address, char character)
{
  for ( uint16_t chary = 0; chary < 32; chary++)
  {
    for ( uint16_t charx = 0; charx < 32; charx++)
    {
      uint8_t pixel = charAddress[character * 1024 + chary * 32 + charx];
      layer2Address[chary * 256 + charx] = pixel;
    }
  }
}

void layer2WriteBigCharacterSliceFast( Scroller* scroller)
{
  int saveMmuValue = ZXN_READ_MMU6();

  ZXN_WRITE_MMU1(layer2BankStart + scroller->yTile );

  uint8_t* fontaddress = (uint8_t *)0xc000;
  uint8_t* layer2address = (uint8_t *)(0x2000 + (32 * scroller->xTile)) + scroller->charXPos;

  uint8_t character = scroller->message[scroller->charPos] - 32;
  uint8_t fontCharBankOffset = character / 8;

  character = character & 0x07;

  ZXN_WRITE_MMU6(scroller->fontBank + fontCharBankOffset);

  uint8_t fontNextRow = 32 - scroller->scrollSize;
  uint8_t screenNextRow = 256 - scroller->scrollSize; 

  fontaddress += character * 1024 + scroller->charXPos;

  if (scroller->scrollSize == 1)
  {
    for ( uint16_t chary = 0; chary < 32; chary++)
    {
      *layer2address++ = *fontaddress++;
      fontaddress += fontNextRow;
      layer2address += screenNextRow;
    }
  }
  if (scroller->scrollSize == 2)
  {
    for ( uint16_t chary = 0; chary < 32; chary++)
    {
      *layer2address++ = *fontaddress++;
      *layer2address++ = *fontaddress++;

      fontaddress += fontNextRow;
      layer2address += screenNextRow;
    }
  }

  if (scroller->scrollSize == 4)
  {
    for ( uint16_t chary = 0; chary < 32; chary++)
    {
      *layer2address++ = *fontaddress++;
      *layer2address++ = *fontaddress++;
      *layer2address++ = *fontaddress++;
      *layer2address++ = *fontaddress++;

      fontaddress += fontNextRow;
      layer2address += screenNextRow;
    }
  }

  ZXN_WRITE_MMU6(saveMmuValue);
  ZXN_WRITE_MMU1(255);

}