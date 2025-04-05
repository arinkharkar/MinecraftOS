#include <minecraftOS_types.h>
#include <video.h>
#include "ps2_helper.h"
#include <video.h>
#include <draw.h>
#include "io.h"


// These will be used by the keyboard and mouse drivers
void(*write_to_keyboard)(byte data);
void(*write_to_mouse)(byte data);
byte(*recv_from_keyboard)();
byte(*recv_from_mouse)();


bool test_ps2controller() {
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_TEST_CONTROLLER_CMD);

    ps2_controller_waittoread();
    byte res = ps2_read_from_dataport();
    
    if (res == PS2_CONTROLLER_TEST_PASSED) {
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
    return SUCCESS;
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
    int resetCounter = 0;
a:
    if (resetCounter == 4)
        return ERROR;
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
        resetCounter++;
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
    const int MAX_TIMES = 1e7;
    int times = 0;
    while (!(inb(PS2_STATUS_PORT) &  0b00000001) && times < MAX_TIMES) { times++; }
    if (times == MAX_TIMES) {
        print_str("MAX READ");
    }
}


int ps2_identify_first_device(ps2_device_types* dvce_type) {
    // in case anything goes wrong, return with device_type being set to error
    *dvce_type = PS2_ERROR_TYPE;
    // first disable scanning so that nothing interferes
    ps2_controller_waittowrite();
    ps2_write_to_firstport(PS2_DEVICE_DISABLE_SCANNING);
    ps2_controller_waittoread();  
    byte c = ps2_read_from_dataport();
    if (c != PS2_ACK)
        return ERROR;


    ps2_write_to_firstport(PS2_DEVICE_IDENTIFY);
    ps2_controller_waittoread();  
    c = ps2_read_from_dataport();
    if (c != PS2_ACK)
        return ERROR;

    byte b1, b2;
    ps2_controller_waittoread(); 
    b1 = ps2_read_from_dataport();
    // A second byte is only given if the ps2 device is a keyboard, shown by b1 being PS2_KEYBD_GENERAL
    if (b1 == PS2_KEYBD_GENERAL) {
        ps2_controller_waittoread(); 
        b2 = ps2_read_from_dataport();
        *dvce_type = (ps2_device_types)b2;
    } else {
        *dvce_type = (ps2_device_types)b1;
    }

    return SUCCESS;
}

int ps2_identify_second_device(ps2_device_types* dvce_type) {
    // in case anything goes wrong, return with device_type being set to error
    *dvce_type = PS2_ERROR_TYPE;
    // first disable scanning so that nothing interferes
    ps2_controller_waittowrite();
    ps2_write_to_secondport(PS2_DEVICE_DISABLE_SCANNING);
    ps2_controller_waittoread();  
    byte c = ps2_read_from_dataport();
    if (c != PS2_ACK)
        return ERROR;


    ps2_write_to_secondport(PS2_DEVICE_IDENTIFY);
    ps2_controller_waittoread();  
    c = ps2_read_from_dataport();
    if (c != PS2_ACK)
        return ERROR;

    byte b1, b2;
    ps2_controller_waittoread(); 
    b1 = ps2_read_from_dataport();
    // A second byte is only given if the ps2 device is a keyboard, shown by b1 being PS2_KEYBD_GENERAL
    if (b1 == PS2_KEYBD_GENERAL) {
        ps2_controller_waittoread(); 
        b2 = ps2_read_from_dataport();
        *dvce_type = (ps2_device_types)b2;
    } else {
        *dvce_type = (ps2_device_types)b1;
    }



    return SUCCESS;
}

int init_ps2controller() {

    // Enable the second ps2 port (technically we should check if there are 2 but every pc since like 1990 supports two so it should be fine)
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_ENABLE_SECOND_PORT);

    // get and modify the ps2 controller byte
    byte c = get_ps2_controllerconfig_byte();
    c |= 0b11;
    set_ps2_controllerconfig_byte(c);

    ps2_controller_waittowrite();
    // Disable scanning of the first ps2 device
    ps2_write_to_firstport(PS2_DEVICE_DISABLE_SCANNING);
    ps2_controller_waittoread();  
    c = ps2_read_from_dataport();
    print_hex(c);
    if (c != PS2_ACK)
        return ERROR;


    ps2_device_types type1, type2;
    if (ps2_identify_first_device(&type1) == ERROR)
        return ERROR;
    if (ps2_identify_second_device(&type2) == ERROR)
        return ERROR;
  
    // if the first device is a mouse, set the proper write and read functions for the mouse
    if (type1 == PS2_MOUSE || type1 == PS2_5BTN_MOUSE || type1 == PS2_MOUSE_SCROLL_WHL) {
        write_to_mouse = ps2_write_to_firstport;
        recv_from_mouse = ps2_read_from_dataport;
      // There are more types of keyboards, shouldnt run into them though
    } else if (type1 == PS2122_KYBD || type1 == PS2_KEYBD || type1 == PS2_97_KYBD || type1 == PS2_SHORT_KYBD) {
        write_to_keyboard = ps2_write_to_firstport;
        recv_from_keyboard = ps2_read_from_dataport;
    }

    if (type2 == PS2_MOUSE || type2 == PS2_5BTN_MOUSE || type2 == PS2_MOUSE_SCROLL_WHL) {
        write_to_mouse = ps2_write_to_secondport;
        recv_from_mouse = ps2_read_from_dataport;
      // There are more types of keyboards, shouldnt run into them though
    } else if (type2 == PS2122_KYBD || type2 == PS2_KEYBD || type2 == PS2_97_KYBD || type2 == PS2_SHORT_KYBD) {
        write_to_keyboard = ps2_write_to_secondport;
        recv_from_keyboard = ps2_read_from_dataport;
    }
    
    return SUCCESS;
}

void ps2_write_to_secondport(byte data) {
    while((inb(PS2_STATUS_PORT) & 2)) {}
    outb(PS2_STATUS_PORT, 0xD4);
    while((inb(PS2_STATUS_PORT) & 2)) {}
    outb(PS2_DATA_PORT, data);
}

void ps2_write_to_firstport(byte data) {
    while((inb(PS2_STATUS_PORT) & 2)) {}
    outb(PS2_DATA_PORT, data);
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


