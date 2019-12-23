#ifndef DMA_H
#define DMA_H

void TransferValueToMemorySlow(unsigned char value, unsigned char* destination, unsigned int length);
void TransferValueToMemory(unsigned char value, unsigned char* destination, unsigned int length);
void TransferMemoryToPortDMA(unsigned char* source, unsigned int length, unsigned char port);

#endif