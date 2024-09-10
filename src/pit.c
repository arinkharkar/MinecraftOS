#include "pit.h"


// 0 = Start at Jan 1 1970 00:00 UTC
// THIS WILL STOP WORKING ON Tue Jan 19 2038 03:14:07 GMT+0000
int32_t epoch_timestamp = 0;

void init_pit(int freq) {
    int div = PIT_FREQUENCY / freq;
    // We set the PIT to use a 16 bit counter and generate a square wave
    outb(0x43, 0b00110110);
    // send the lower 8 bits of the divisor
    outb(0x40, freq & 0b11111111);
    // send the higher 8 bits of the divisor
    outb(0x40, freq >> 8);
    
}

int ticks = 0;

void pit_callback() {
    
    ticks++;
    if (ticks % 1000 == 0)
        print_str("called");
    
}