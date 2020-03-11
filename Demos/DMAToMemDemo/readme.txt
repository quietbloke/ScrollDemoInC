DMA to Memory Demo
==================

This demo copies a value to a block of memory using either the CPU or the DMA.

When using the CPU the code sets value in a block of memory one byte at a time 
in a loop.

When using the DMA the code writes the the DMA IO port the instructions to fill the 
memory block with a value.

The blue border at the bottom shows how much time is being taken 
to fill the block of memory with the value.

Keys
    p = increase size of the block of memory to fill
    l = decrease size of the block of memory to fill

    c = use the CPU to transfer the value
    d = use the DMA to transfer the value
