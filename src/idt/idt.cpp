#include "idt.h"
#include <video.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <ps2keyboard.h>
#include <ps2mouse.h>
#include "pit.h"

void register_interupt(int intNo, void(*function)());
EXTERN_C void idt_enable();

#ifndef __STD_LIB_H_
static char* itoa(int value, char* str, int base);
static char* float_to_str(float num, char* str, int precision);
#endif

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
    enable_interupts();
    return SUCCESS;
}

// in the idt, set the first 32 values to point to each exception handler
void register_exceptions() {
    for (int i = 0; i < 48; i++) {
        register_interupt(i, (void(*)())isr_table[i]);
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

EXTERN_C void interrupt_handler(exception_regs r) {
    // If the interupt is an exception
    if (r.int_no < 32) {
        char error_message[100] = {0};
        // Combine the error message and pass it to PANIC
        const char* first_msg_part = "CRITICAL EXCEPTION OCCURED, MinecraftOS has terminated: ";
        int len = strlen(first_msg_part);
        // copy the first part of the error message to error_message
        strcpy(error_message, first_msg_part);
        // offset by the first_msg_part length
        strcpy(&error_message[len], exception_messages[r.int_no]);
        len += strlen(error_message);

        
        const char* second_msg_part = "\nException Number: ";
        strcpy(&error_message[len], second_msg_part);
        len += strlen(second_msg_part);

        char s_int_no[15];
        itoa(r.int_no, s_int_no, 10);
        strcpy(&error_message[len], s_int_no);

        PANIC(error_message);
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