#ifndef __PIT_H_
#define __PIT_H_

#include "video.h"
#include <io.h>

#define PIT_FREQUENCY 1193180


void pit_callback();
void init_pit(int freq);

#endif