#ifndef __PIT_H_
#define __PIT_H_

#include "video.h"
#include <io.h>

const uint32_t PIT_FREQUENCY = 1193180;

extern volatile uint32_t ticks_passed;
extern volatile uint32_t seconds_passed;

void pit_callback();
void init_pit(int freq);

#endif