#include "gdt_manager.h"

#define GDT_ENTRY_COUNT 3

// The GDT containing NULL, DATA, and CODE
gdt_segment gdt[GDT_ENTRY_COUNT];
// the info about where our gdt is and how big it is
gdt_ptr pgdt;

// assembly function in gdt_enable.S to run the lgdt instruction
void activate_gdt();

/*
 The GDT we will be using is extremely simple, our structure is:
    1 NULL segment, as required by x86
    1 DATA segment, spanning from 0x1000 to 0xFFFFFFFF (all of memory except first KB)
    1 CODE segment, spanning from 0x1000 to 0xFFFFFFFF (all of memory except first KB)

    This makes it so that there is no memory protection whatsoever, giving us the most freedom but also is super dangerous and any good OS wouldnt do this!
    The only exception is that reads or writes to the first KB will trigger a general protection fault, this is so that null pointers arent valid
*/
int init_gdt() {
    // setting the NULL segment to all zeros
    memset(&gdt[0], 0, sizeof(gdt[0]));
    // the data segment
    if (fill_gdt_segment(&gdt[1], 0x000, 0x000FFFFF, true, true, true) == ERROR)
        return ERROR;
    // the code segment
    if(fill_gdt_segment(&gdt[2], 0x000, 0x000FFFFF, true, false, true) == ERROR)
        return ERROR;
    
    pgdt.base = &gdt[0];
    pgdt.limit = sizeof(gdt[0]) * GDT_ENTRY_COUNT - 1;
    activate_gdt();
    return SUCCESS;
}

/*
 base - the lowest value this segment will pertain to,
 limit - how far this segment is valid
 onlyKernelMode - whether this segment can be accessed from user mode
 isDataSegment - whether this segment is data or code
 isReadWritable - if this is data, whether this is writable, if this is code, whether this is readable

*/
int fill_gdt_segment(gdt_segment* segment, uint32_t base, uint32_t limit, bool onlyKernelMode, bool isDataSegment, bool isReadWriteable) {
    if (!segment) {
        set_last_error("Error Filling GDT: Invalid Pointer to Segment");
        return ERROR;
    }
    segment->limit16bits = (uint16_t) (limit & 0b00000000000000001111111111111111);
    segment->base16bits =  (uint16_t) (base & 0b00000000000000001111111111111111);
    segment->base8bits =   (uint16_t)((base & 0b00000000111111110000000000000000) >> 16);


    /*
        Byte that controls the permissions of this GDT segment is the access byte
        first bit is always 1
        second is RW bit (explained in comment above)
        third is always 0 in our case, https://wiki.osdev.org/Global_Descriptor_Table for a detailed explanation
        fourth is 0 for data and 1 for code
        fifth is always 1 in our case
        sixth and seventh are represent a number between 0 and 3 for each ring level, always 0 in our case
        eighth is always 1 for a valid segment
    */ 

    // For the bits that are always one
    uint8_t alwaysOne =    0b10010001;
    uint8_t rw        =    0b00000000;
    uint8_t dataCode  =    0b00000000;
    if (isReadWriteable)
        rw = 0b00000010;
    if (!isDataSegment)
        dataCode = 0b00001000;

    uint8_t accessByte = alwaysOne | rw | dataCode;

    segment->access8bits = accessByte;
    // first 4 bits are the last part of the limit, the next 4 are flags
    uint8_t limitandflags = (uint8_t) ((limit & 0b00000000000011110000000000000000) >> 16);
    uint8_t flags = 0b00001100;

    // this adds the 4 bits of flags to the limitandflags byte
    limitandflags = (flags << 4) | limitandflags;
    
    segment->limitandflags = limitandflags;

    // adds the last 16 bits of the base
    segment->baselastbits = (base & 0b11111111000000000000000000000000) >> 24;
    return SUCCESS;
}