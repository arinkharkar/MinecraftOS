#include <minecraftOS_types.h>
#include <video.h>
#include "ps2_helper.h"
#include <video.h>
#include <draw.h>
#include "io.h"



bool test_ps2controller() {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_TEST_CONTROLLER_CMD);

    ps2_controller_waittoread();
    byte res = ps2_read_from_dataport();
    
    if (res == PS2_CONTROLLER_TEST_PASSED) {
        print_str("PS/2 Keyboard Test Passed!");
        return true;
    } else if (res == PS2_CONTROLLER_TEST_FAILED) {
        print_str("PS/2 Keyboard Test Failed!");
        return false;
    } else {
        print_str("PS/2 Keyboard Test Unkown!");
        return false;
    }
}

int test_ps2ports(bool secondPortExists) {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_TEST_FIRST_PORT);
    ps2_controller_waittoread();
    byte res = ps2_read_from_dataport();
    if (res != 0) {
        print_str("Error Testing PS/2 Port 1: ");
        print_str(PS2_PORT_ERROR_CODES[res]);
        return ERROR;
    }
    if (secondPortExists) {
        ps2_controller_waittowrite();
        ps2_write_to_commandport(PS2_TEST_SECOND_PORT);
        ps2_controller_waittoread();
        byte res = ps2_read_from_dataport();
        if (res != 0) {
            print_str("Error Testing PS/2 Port 1: ");
            print_str(PS2_PORT_ERROR_CODES[res]);
            return ERROR;
        }
    }
}

int enable_ps2ports(bool isSecondChannel) {
   
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_ENABLE_FIRST_PORT);
    
    if (isSecondChannel) {
        ps2_controller_waittowrite();
        ps2_write_to_commandport(PS2_ENABLE_SECOND_PORT);
    }
    return SUCCESS;
}

byte get_ps2_controllerconfig_byte() {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_GET_CONTROLERCONFIG_BYTE);

    ps2_controller_waittoread();
    byte rVal = ps2_read_from_dataport();

    return rVal;
}

int set_ps2_controllerconfig_byte(byte c) {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_SET_CONTROLERCONFIG_BYTE);

    ps2_controller_waittowrite();
    ps2_write_to_dataport(c);

    return SUCCESS;
}

void disable_ps2_devices() {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_DISABLE_FIRST_PORT);

    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_DISABLE_SECOND_PORT);
}

bool ps2_enable_second_channel() {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_ENABLE_SECOND_CHANNEL);

    if (get_ps2_controllerconfig_byte() & 0b00100000) {
        return false;
    }

    // Disable the 2nd port
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_DISABLE_SECOND_PORT);

    byte b = get_ps2_controllerconfig_byte() & 0b11011101;
    set_ps2_controllerconfig_byte(b);
    return true;
}


int ps2_reset_devices() {
a:
    ps2_controller_waittowrite();
    ps2_write_to_dataport(PS2_RESET_DEVICES);


    ps2_controller_waittoread();
    print_ch('\n');
    byte res = ps2_read_from_dataport();
    print_hex(res);
    print_ch('\n');
    if (res == PS2_RESET_RESPONSE_SUCCESS1 || res == PS2_RESET_RESPONSE_SUCCESS2) {}
    else if (res == PS2_RESET_RESPONSE_FAIL) {
        print_str("PS2_RESET_RESPONSE_FAIL from the PS/2 Controller!");
        return ERROR;
    } else if (res == PS2_RESEND) {
        print_str("Invalid Response after resetting PS/2 Controller, Resending...");
        swap();
        goto a;
    } else {
        print_str("Invalid Response after resetting PS/2 Controller!");
        return ERROR;
    }
    res = ps2_read_from_dataport();
    print_hex(res);
    print_ch('\n');
    res = ps2_read_from_dataport();
    print_hex(res);
    print_str(" ");
    // Redoing this for port 2
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_WRITE_TO_PORT2);
    ps2_controller_waittowrite();
    ps2_write_to_dataport(PS2_RESET_DEVICES);
    ps2_controller_waittoread();
    res = ps2_read_from_dataport();
    print_hex(res);
    print_ch('\n');
    if (res == PS2_RESET_RESPONSE_SUCCESS1 || res == PS2_RESET_RESPONSE_SUCCESS2) {}
    else if (res == PS2_RESET_RESPONSE_FAIL) {
        print_str("PS2_RESET_RESPONSE_FAIL from the PS/2 Controller!");
        return ERROR;
    } else {
        print_str("Invalid Response after resetting PS/2 Controller!");
        return ERROR;
    }
    res = ps2_read_from_dataport();
    print_hex(res);
    print_ch('\n');
    res = ps2_read_from_dataport();
    print_hex(res);
    print_str("\nSUCCESS");
    return SUCCESS;
}

// Wait to make sure its okay to write to the PS2 Data Port
void ps2_controller_waittowrite() {
    const int MAX_TIMES = 1e6;
    int times = 0;
    while ((inb(PS2_STATUS_PORT) &  0b00000010) && times < MAX_TIMES) { times++; }
    if (times == MAX_TIMES) {
        print_str("MAX WRITE");
    }
}

// Wait to make sure its okay to read from the PS2 Data Port
void ps2_controller_waittoread() {
    const int MAX_TIMES = 1e6;
    int times = 0;
    while (!(inb(PS2_STATUS_PORT) &  0b00000001) && times < MAX_TIMES) { times++; }
    if (times == MAX_TIMES) {
        print_str("MAX READ");
    }
}

void ps2_write_to_commandport(byte data) {
    outb(PS2_COMMAND_REGISTER, data);
}

void ps2_write_to_dataport(byte data) {
    outb(PS2_DATA_PORT, data);
}

byte ps2_read_from_dataport() {
    return inb(PS2_DATA_PORT);
}

byte ps2_read_from_statusport() {
    return inb(PS2_STATUS_PORT);
}


