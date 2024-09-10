#ifndef __GDT__MANAGER__H_
#define __GDT__MANAGER__H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <error_handler.h>
/*
 The GDP is set up super weirdly to maintain backwards compatibility, sadly this makes our lives a bit harder
 Even more weirdly, the 'limit' value (how far from the base this gdt segment is valid) is only 20 bits long, so you must specify you want to have 4kb granularity 
 with the granularity so that the max limit * 4kb = 4.2 billion  
*/
typedef struct {
    // first 16 bits of the limit
    uint16_t limit16bits;
    // first 16 bits of the base
    uint16_t base16bits;
    // next 8 bits of the base
    uint8_t  base8bits;
    // 8 bits for the access
    uint8_t access8bits;
    //first 4 bits are the last part of the limit, last 4 bits are the flags
    uint8_t limitandflags;
    // last 16 bits of the base
    uint8_t baselastbits;

} __attribute__((packed)) gdt_segment;

// when calling lgdt, we pass base as the start of our gdt and limit as the size of it
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr;

int init_gdt();

int fill_gdt_segment(gdt_segment* segment, uint32_t base, uint32_t limit, bool onlyKernelMode, bool isDataSegment, bool isReadWriteable);

#endif