#ifndef __IDT__H_
#define __IDT__H_

#include <stdint.h>


typedef struct {
    uint16_t offsetFirst;
    uint16_t segmentSelector;
    uint8_t reserved;
    uint8_t gateTypeZeroDPLP;
    uint16_t offsetLast;
} __attribute__((packed)) idt_entry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr;

typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t int_no;
    uint32_t error_code;
} __attribute__((packed)) exception_regs;

typedef struct {
    uint32_t ecx;
    uint32_t eax;
} __attribute__((packed)) exception_reegs;

// The idt, a table of 256 pointers so that the CPU knows where to go during an interupt
static idt_entry idt[256];

// Each exception Name
static const char* exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};
int init_idt();

void irq_remap();

#endif