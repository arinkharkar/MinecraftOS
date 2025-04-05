#ifndef REGS_INFO_H
#define REGS_INFO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    uint32_t get_eax();
    uint32_t get_ebx();
    uint32_t get_ecx();
    uint32_t get_edx();
    uint32_t get_esi();
    uint32_t get_edi();
    uint32_t get_ebp();
    uint32_t get_esp();
    uint32_t get_eip();
    uint32_t get_eflags();
#ifdef __cplusplus
}
#endif

#endif