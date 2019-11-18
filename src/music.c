#include <stdlib.h>
#include <stddef.h>
#include <arch/zxn.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/zxn/esxdos.h>

#include "defines.h"

// assume music fits into 1 bank
bool loadMusic()
{
  uint8_t filehandle;
  errno = 0;

  filehandle = esxdos_f_open("music.pt3", ESXDOS_MODE_R | ESXDOS_MODE_OE);
  if (errno)
  {
    return false;
  }

    unsigned char* destination = 0x4000;
    ZXN_WRITE_MMU2(musicBankStart);

    esxdos_f_read(filehandle, (void *) destination, 1024*8);
    if (errno)
    {
        return false;
    }

    return true;

  esxdos_f_close(filehandle);
  ZXN_WRITE_MMU2(0xff);

  return true;
}