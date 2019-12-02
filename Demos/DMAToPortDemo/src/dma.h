#ifndef DMA_H
#define DMA_H

void TransferMemoryToSpriteIOPortCPU(unsigned char* source, unsigned int length);
void TransferMemoryToSpriteIOPortDMA(unsigned char* source, unsigned int length);

#endif