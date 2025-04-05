#include "pit.h"
#include <ps2keyboard.h>


volatile uint32_t ticks_passed = 0;
volatile uint32_t seconds_passed = 0;
// 0 = Start at Jan 1 1970 00:00 UTC
// THIS WILL STOP WORKING ON Tue Jan 19 2038 03:14:07 GMT+0000
// Getting pretty close to it cant lie :DD
int32_t epoch_timestamp = 0;

void init_pit(int freq) {
    int div = PIT_FREQUENCY / freq;
    // We set the PIT to use a 16 bit counter and generate a square wave
    outb(0x43, 0b00110110);
    // send the lower 8 bits of the divisor
    outb(0x40, div & 0b11111111);
    // send the higher 8 bits of the divisor
    outb(0x40, (div >> 8) & 0b11111111);    
}

__attribute__((optimize("O0")))
void pit_callback() {
    ticks_passed++;
    if (ticks_passed % 1000 == 0) {

        seconds_passed++;
    }
}

uint32_t get_seconds_passed() { return seconds_passed; }
__attribute__((optimize("O0"))) void sleep_seconds(size_t seconds) {
    // get the seconds since startup from pit.h
    size_t start_seconds = get_seconds_passed();
    while (get_seconds_passed() < start_seconds + seconds) {
    }
}