#ifndef __PS2_KEYBOARD_H_
#define __PS2_KEYBOARD_H_

#include "io.h"
#include <video.h>

#define PS2_ACK 0xFA
#define PS2_RESEND 0xFE
#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_REGISTER 0X64


// Call to initalize communication with the ps2 keyboard
int init_ps2keyboard();

// Callback function whenever the OS recieves an IRQ of a key press
void keycode_recv();


#endif