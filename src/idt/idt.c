#include "idt.h"
#include <video.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <ps2keyboard.h>
#include <ps2mouse.h>
#include "pit.h"

void register_interupt(int intNo, void(*function)());
void interrupt_handler();
void idt_enable();

extern void* isr_table[]; 

idt_ptr pidt;

/*
 Initializes the Interupt Descriptor Table, a table of pointers so that the CPU knows where to go when an interupt is called
*/
int init_idt() {
    // zero out the idt
    memset(&idt[0], 0, sizeof(idt[0]) * 256);
    
    pidt.base = (uint32_t)&idt[0];
    pidt.limit = sizeof(idt[0]) * 256 - 1;
    idt_enable();
    irq_remap();
    register_exceptions();
    asm("sti");
    return SUCCESS;
}

// in the idt, set the first 32 values to point to each exception handler
void register_exceptions() {
    for (int i = 0; i < 48; i++) {
        register_interupt(i, isr_table[i]);
    }
}

void register_interupt(int intNo, void(*function)()) {
    idt[intNo].offsetFirst = (uint32_t)function & 0b00000000000000001111111111111111;
    idt[intNo].offsetLast = (uint32_t)function >> 16;
    idt[intNo].reserved = 0;
    idt[intNo].gateTypeZeroDPLP = 0b10001111;
    idt[intNo].segmentSelector = 0b00010000;
}

void irq_remap()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

}


void interrupt_handler(exception_regs r) {
    // If the interupt is an exception
    if (r.int_no < 32) {
        print_str("CRITICAL EXCEPTION OCCURED, DoomOS has terminated: ");
        print_str(exception_messages[r.int_no]);
        print_str("\nException Number: ");
        char s_int_no[15];
        itoa(r.int_no, s_int_no, 10);
        print_str(s_int_no);
        asm("cli;hlt");
    } else if (r.int_no < 48) {
        if (r.int_no == 32) {
            pit_callback();
        }
        if (r.int_no == 33) {
            keycode_recv();
        }
        if (r.int_no == 44) {
            mouse_recv();
        }
    }

    if (r.int_no >= 40)
        outb(0xA0, 0x20);
    outb(0x20, 0x20);
}