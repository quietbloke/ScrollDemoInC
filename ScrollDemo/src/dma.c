#include <arch/zxn.h>

void TransferValueToMemorySlow(uint8_t value, uint8_t* destination, uint16_t length)
{
  uint8_t* finalAddress = destination + length;
  while( destination < finalAddress)
  {
    *destination++ = value;
  }
}

void TransferValueToMemory(uint8_t value, uint8_t* destination, uint16_t length)
{
  // Note : D_WR0 is defined as zero, however wiki page doc says the lower 2 bits must not be 00.
  // However WR0_TRANSFER has a value of 1, so you must always add this ??
  //
  // port A is the destination
  // port B is the source single byte

  // WR0 instruction, write A -> B, supply port A start address and block length as the next 4 values sent

  uint16_t sourceValueAddress = (uint16_t) &value;
  uint16_t destinationAddress = (uint16_t) destination;

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

void TransferMemoryToPortDMA(uint8_t* source, uint16_t length, uint8_t port)
{
  // Note : D_WR0 is defined as zero, however wiki page doc says the lower 2 bits must not be 00.
  // However WR0_TRANSFER has a value of 1, so you must always add this ??
  //
  // port A is the source data
  // port B is the destination port

  // WR0 instruction, write A -> B, supply port A start address and block length as the next 4 values sent

  uint16_t sourceAddress = (uint16_t) source;

  IO_DMA = D_DISABLE_DMA;

  IO_DMA = D_WR0_TRANSFER + D_WR0_A_TO_B + D_WR0_X34_A_START + D_WR0_X56_LEN;
  IO_DMA = sourceAddress & 0xff;
  IO_DMA = (sourceAddress >> 8) & 0xff;
  IO_DMA = length & 0xff;
  IO_DMA = (length >> 8) & 0xff;

  IO_DMA = D_WR1 + D_WR1_A_IS_MEM + D_WR1_A_INC + D_WR1_X6_A_TIMING; 
  IO_DMA = D_WR1X6_A_CLEN_2;

  // Now set Port B as non increment
  IO_DMA = D_WR2 + D_WR2_B_IS_IO_FIX + + D_WR2_X6_B_TIMING;
  IO_DMA = D_WR2X6_B_CLEN_2;

  // Set Port B Address ( and set the Continuous transfer mode)
  IO_DMA = D_WR4 + D_WR4_X2_B_START_L + D_WR4_CONT;
  IO_DMA = port; 

  IO_DMA = D_LOAD;

  IO_DMA = D_ENABLE_DMA;
}
