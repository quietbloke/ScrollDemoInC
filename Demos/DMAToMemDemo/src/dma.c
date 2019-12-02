#include <arch/zxn.h>

void TransferValueToMemoryCPU(unsigned char value, unsigned char* destination, unsigned int length)
{
  unsigned char* finalAddress = destination + length;
  while( destination < finalAddress)
  {
    *destination++ = value;
  }
}

void TransferValueToMemoryDMA(unsigned char value, unsigned char* destination, unsigned int length)
{
  // Note : D_WR0 is defined as zero, however wiki page doc says the lower 2 bits must not be 00.
  // However WR0_TRANSFER has a value of 1, so you must always add this ??
  //
  // port A is the destination
  // port B is the source single byte

  // WR0 instruction, write A -> B, supply port A start address and block length as the next 4 values sent

  unsigned int sourceValueAddress = (unsigned int) &value;
  unsigned int destinationAddress = (unsigned int) destination;

  IO_DMA = D_DISABLE_DMA;

  IO_DMA = D_WR0_TRANSFER + D_WR0_A_TO_B + D_WR0_X34_A_START + D_WR0_X56_LEN;
  IO_DMA = sourceValueAddress & 0xff;
  IO_DMA = (sourceValueAddress >> 8) & 0xff;
  IO_DMA = length & 0xff;
  IO_DMA = (length >> 8) & 0xff;

  IO_DMA = D_WR1 + D_WR1_A_FIX + D_WR1_X6_A_TIMING; 
  IO_DMA = D_WR1X6_A_CLEN_2;

  // Now set Port B as non increment
  IO_DMA = D_WR2 + D_WR2_B_IS_MEM_INC + D_WR2_X6_B_TIMING;
  IO_DMA = D_WR2X6_B_CLEN_2;

  // Set Port B Address ( and set the Continuous transfer mode)
  IO_DMA = D_WR4 + D_WR4_X23_B_START + D_WR4_CONT;
  IO_DMA = destinationAddress & 0xff;
  IO_DMA = (destinationAddress >> 8) & 0xff;

  IO_DMA = D_LOAD;

  IO_DMA = D_ENABLE_DMA;

}

void TransferValueToMemoryDMABtoA(unsigned char value, unsigned char* destination, unsigned int length)
{
  // Note : D_WR0 is defined as zero, however wiki page doc says the lower 2 bits must not be 00.
  // However WR0_TRANSFER has a value of 1, so you must always add this ??
  //
  // port A is the destination
  // port B is the source single byte

  // WR0 instruction, write A -> B, supply port A start address and block length as the next 4 values sent

  unsigned int sourceValueAddress = (unsigned int) &value;
  unsigned int destinationAddress = (unsigned int) destination;

  IO_DMA = D_DISABLE_DMA;

  IO_DMA = D_WR0_TRANSFER + D_WR0_B_TO_A + D_WR0_X34_A_START + D_WR0_X56_LEN;
  IO_DMA = destinationAddress & 0xff;
  IO_DMA = (destinationAddress >> 8) & 0xff;
  IO_DMA = length & 0xff;
  IO_DMA = (length >> 8) & 0xff;

  IO_DMA = D_WR1 + D_WR1_A_INC + D_WR1_X6_A_TIMING; 
  IO_DMA = D_WR1X6_A_CLEN_2;

  // Now set Port B as non increment
  IO_DMA = D_WR2 + D_WR2_B_IS_MEM_FIX + D_WR2_X6_B_TIMING;
  IO_DMA = D_WR2X6_B_CLEN_2;

  // Set Port B Address ( and set the Continuous transfer mode)
  IO_DMA = D_WR4 + D_WR4_X23_B_START + D_WR4_CONT;
  IO_DMA = sourceValueAddress & 0xff;
  IO_DMA = (sourceValueAddress >> 8) & 0xff;

  IO_DMA = D_LOAD;

  IO_DMA = D_ENABLE_DMA;

}

