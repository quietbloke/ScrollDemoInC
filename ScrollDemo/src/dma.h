#ifndef DMA_H
#define DMA_H

void TransferValueToMemorySlow(uint8_t value, uint8_t* destination, uint16_t length);
void TransferValueToMemory(uint8_t value, uint8_t* destination, uint16_t length);
void TransferMemoryToPortDMA(uint8_t* source, uint16_t length, uint8_t port);

#endif