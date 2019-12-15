#ifndef DMA_H
#define DMA_H

void TransferMemoryToSpriteIOPortCPU(unsigned char* source, unsigned int length);
void TransferMemoryToPortCPU(unsigned char* source, unsigned int length, unsigned char port);

void TransferMemoryToPortDMA(unsigned char* source, unsigned int length, unsigned char port);

#endif