#ifndef DMA_H
#define DMA_H

void TransferValueToMemorySlow(unsigned char value, unsigned char* destination, unsigned int length);
void TransferValueToMemory(unsigned char value, unsigned char* destination, unsigned int length);

#endif