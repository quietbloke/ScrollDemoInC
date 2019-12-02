#ifndef DMA_H
#define DMA_H

void TransferValueToMemoryCPU(unsigned char value, unsigned char* destination, unsigned int length);
void TransferValueToMemoryDMA(unsigned char value, unsigned char* destination, unsigned int length);
void TransferValueToMemoryDMABtoA(unsigned char value, unsigned char* destination, unsigned int length);

#endif