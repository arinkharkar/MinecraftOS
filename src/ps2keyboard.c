#include "ps2keyboard.h"

int get_echo();
uint8_t sendandrecvb(uint8_t val);

int init_ps2keyboard() {
    uint8_t cur_byte;

    // Disable PS/2 devices
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_COMMAND_REGISTER, 0xAD);
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_COMMAND_REGISTER, 0xA7);
    // Flush output buffer
    inb (PS2_DATA_PORT);
    // Set the controller config byte
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_COMMAND_REGISTER, 0x20);
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {}
    cur_byte = inb(PS2_DATA_PORT);
    cur_byte &= ~(1 << 0);
	cur_byte &= ~(1 << 1);
	//cur_byte &= ~(1 << 6); 
    outb(PS2_COMMAND_REGISTER, 0x60);
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_DATA_PORT, cur_byte);
    // Send self test
    outb(PS2_COMMAND_REGISTER, 0xAA);
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {}
    cur_byte = inb(PS2_DATA_PORT);
    if (cur_byte == 0x55)
        print_str("Test Passed!");
    else 
        print_str("Test Failed!");
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_COMMAND_REGISTER, 0x20);
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {}
    cur_byte = inb(PS2_DATA_PORT);
    cur_byte = (cur_byte | 0b11); 
    outb(PS2_COMMAND_REGISTER, 0x60);
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_DATA_PORT, cur_byte);

    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_COMMAND_REGISTER, 0xAE);
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_COMMAND_REGISTER, 0xA8);

    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_DATA_PORT, 0xFF);
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {}
    cur_byte = inb(PS2_DATA_PORT);
    if (cur_byte == 0xFA || cur_byte == 0xAA)
        print_str("Successfuly Initialized!\n");
    
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_DATA_PORT, 0xF4);
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_DATA_PORT, 0xFF);
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    outb(PS2_DATA_PORT, 0xEE);
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {}
    cur_byte = inb(PS2_DATA_PORT);
    if (get_echo() == ERROR)
        print_str("Error with ps2");
    else
        print_str("echo!");

    return SUCCESS;
}

int get_echo() {
    int retry_count = 0;
retry:
    if (retry_count > 10)
        return ERROR;
    uint8_t b = sendandrecvb(0xEE);
    if (b == 0xEE)
        return SUCCESS;
    else {
        retry_count++;
        goto retry;
    }
    return ERROR;
}

// Send a byte to the keyboard and wait for response
uint8_t sendandrecvb(uint8_t val) {
    while (inb(PS2_STATUS_PORT) & 0x2) {}
    // Send to the data port
    outb(PS2_DATA_PORT, val);
    // wait until keyboard processes data
    while (!(inb(PS2_STATUS_PORT) & 0x1)) {}
    return inb(PS2_DATA_PORT);
}


void keycode_recv() {
    print_str("Recieved Key ");
    inb(0x60);
}