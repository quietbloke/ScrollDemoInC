#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/zxn/esxdos.h>

#define layer216KBankStart 9
#define layer2BankStart (layer216KBankStart*2)

#define font4BankStart 24

void layer2WriteCharacter( unsigned char* charAddress, unsigned char* layer2Address, char character);

bool loadFont4(char* filename)
{
  uint8_t filehandle;
  errno = 0;

  int saveBank7 = ZXN_READ_MMU0();

  filehandle = esxdos_f_open(filename, ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

  for (unsigned char bankIndex = 0; bankIndex < 2; bankIndex++)
  {
    unsigned char* destination = 0x2000;
    ZXN_WRITE_MMU1(font4BankStart + bankIndex);

    for ( unsigned char ypos = 0; ypos < 8; ypos++)
    {
      esxdos_f_read(filehandle, (void *) destination, 1024);
      if (errno)
      {
        return false;
      }
      destination += 1024;
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
  loadFont4("font4.nxt");

//  IO_LAYER_2_CONFIG = IL2C_SHOW_LAYER_2;

//   tshr_cls_pix(0xe3);

//  ZXN_WRITE_REG(REG_GLOBAL_TRANSPARENCY_COLOR, 0x00);

//  ZXN_WRITE_MMU1(255);
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

  ZXN_WRITE_MMU6(1);
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