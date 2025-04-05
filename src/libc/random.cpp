#include <minecraftOS.h>

/* Static state variable for the random number generator */
uint32_t rand_state = 1;  /* Seed */

int rand() {
    /* Linear Congruential Generator (LCG) formula */
    rand_state = rand_state * 1103515245 + 12345;
    return (rand_state / 65536) % 32768;  // Return a number between 0 and RAND_MAX (typically 32767)
}

void srand(uint32_t seed) {
    rand_state = seed;  // Set the seed for the generator
}