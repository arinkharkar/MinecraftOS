#ifndef __MC__OS_H___
#define __MC__OS_H___

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Somehow, C++ doesnt define these by default?? In case some non standard compiler does, do a quick check
#ifndef byte
typedef uint8_t byte;
typedef uint8_t BYTE;
#endif


#define BITS_PER_BYTE     8
#define BITS_PER_SHORT    16
#define BITS_PER_INT      32

// When returning an int from a function, use these
#define ERROR   0
#define SUCCESS 1

typedef struct {
    int x;
    int y;
} position_t;

// Suspend execution for a certian amount of seconds
void sleep_seconds(size_t seconds);

// defined in start.S
extern int GRUB_MAGIC_NUMBER;

// wrapping a few assembly functions into C functions, to not mess up intelisense add the __VSCODE_ check
#ifndef __VSCODE_
inline void disable_interupts() { asm volatile("cli"); }
inline void enable_interupts() { asm volatile("sti"); }
inline void halt_cpu() { asm volatile("hlt"); }
#else
inline void disable_interupts() { asm("cli"); }
inline void enable_interupts() { asm("sti"); }
inline void halt_cpu() { asm("hlt"); }
#endif


#ifndef __cplusplus
    #define nullptr ((void*)0)

    // we are including
    #ifndef true
        #define true 1
    #endif

    #ifndef false
        #define false 0
    #endif

#endif

// This just defines NORETURN so that it is standard between C and C++
#ifdef __cplusplus
    #define NORETURN [[noreturn]]
#else
    #if __STDC_VERSION__ >= 201112L
        #define NORETURN _Noreturn
    #elif defined(__GNUC__) || defined(__clang__)
        #define NORETURN __attribute__((noreturn))
    #elif defined(_MSC_VER)
        #define NORETURN __declspec(noreturn)
    #else
        #define NORETURN
    #endif
#endif

#ifndef __cplusplus
#define EXTERN_C
#else
#define EXTERN_C extern "C"
#endif

#endif