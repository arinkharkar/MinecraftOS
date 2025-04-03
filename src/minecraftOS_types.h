#ifndef __MC__OS_TYPES___
#define __MC__OS_TYPES___

#include <stdint.h>
#include <stdbool.h>

#define byte uint8_t
#define BYTE uint8_t
#define BITS_PER_BYTE 8

// When returning an int from a function, use these
#define ERROR 0
#define SUCCESS 1

#ifndef __cplusplus
    #define nullptr ((void*)0)

    #ifndef true
        #define true 1
    #endif

    #ifndef false
        #define false 0
    #endif

#endif

#endif