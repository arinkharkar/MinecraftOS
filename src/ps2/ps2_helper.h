#ifndef PS2_HELPER_H
#define PS2_HELPER_H

#define PS2_ACK 0xFA
#define PS2_RESEND 0xFE
#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_REGISTER 0X64

#define PS2_TEST_CONTROLLER_CMD 0xAA
#define PS2_TEST_FIRST_PORT     0xAB
#define PS2_TEST_SECOND_PORT     0xA9
#define PS2_ENABLE_FIRST_PORT   0xAE
#define PS2_DISABLE_FIRST_PORT  0xAD
#define PS2_RESET_DEVICES       0xFF

#define PS2_DISABLE_SECOND_PORT 0xA7
#define PS2_ENABLE_SECOND_PORT 0xA8

#define PS2_GET_CONTROLERCONFIG_BYTE 0x20
#define PS2_SET_CONTROLERCONFIG_BYTE 0x60

#define PS2_RESET_RESPONSE_SUCCESS1 0xFA
#define PS2_RESET_RESPONSE_SUCCESS2 0xAA

#define PS2_ENABLE_SECOND_CHANNEL 0xA8

#define PS2_RESET_RESPONSE_FAIL 0xFC

#define PS2_CONTROLLER_TEST_PASSED 0x55
#define PS2_CONTROLLER_TEST_FAILED 0xFC

#define PS2_WRITE_TO_PORT2 0xD4

#define PS2_DEVICE_DISABLE_SCANNING 0xF5
#define PS2_DEVICE_SET_DEFAULTS 0xF6
#define PS2_DEVICE_ENABLE_SCANNING 0xF4
#define PS2_DEVICE_IDENTIFY 0xF2

#define PS2_MOUSE_SET_SAMPLE_RATE 0xF3

#define PS2_TEST_FIRST_PORT_PASSED 0x00

// All Known PS/2 Device Types included for completness, 99% of the time will only need PS2_MOUSE and PS2_KYBD
typedef enum {
    PS2_ERROR_TYPE = -1,
    PS2_MOUSE = 0x0,
    PS2_MOUSE_SCROLL_WHL = 0x3,
    PS2_5BTN_MOUSE = 0x4,
    PS2_KEYBD_GENERAL = 0xAB,
    PS2_KEYBD = 0x41,
    PS2_SHORT_KYBD = 0x54,
    PS2_97_KYBD = 0x85,
    PS2122_KYBD = 0x86,
    PS2_JP_G_KYBD = 0x90,
    PS2_JP_P_KYBD = 0x91,
    PS2_JP_A_KYBD = 0x92,
    PS2_NCD_SUN_KYBD = 0xA1
} ps2_device_types;

// After Intializing the PS/2 Controller, these will be used by the keyboard and mouse driver to control each device respectively
extern void(*write_to_keyboard)(byte data);
extern void(*write_to_mouse)(byte data);

extern byte(*recv_from_keyboard)();
extern byte(*recv_from_mouse)();

// Intializes ps2 controller, must be done before initalizing the ps/2 keyboard and mouse
int init_ps2controller();

// Wait to make sure its okay to write to the PS2 Status Port
void ps2_controller_waittowrite();

// Wait to make sure its okay to read from the PS2 Data Port
void ps2_controller_waittoread();

// Write to the PS2 command port
void ps2_write_to_commandport(byte data);

// Write to the PS/2 Data Port
void ps2_write_to_dataport(byte data);

// Read from the PS2 data port
byte ps2_read_from_dataport();

byte ps2_read_from_statusport();

// Test if the ps/2 controller is working, returns true if working, false if not
bool test_ps2controller();

// Enable first PS/2 port
int enable_first_ps2port();

// Get the PS/2 Controller Config Byte from the PS/2 Controller, byte info found at: https://wiki.osdev.org/%228042%22_PS/2_Controller
byte get_ps2_controllerconfig_byte();

int set_ps2_controllerconfig_byte(byte c);

int ps2_reset_devices();

// Disables First and Second PS/2 Ports
void disable_ps2_devices();

bool ps2_enable_second_channel();

void ps2_write_to_firstport(byte data);

void ps2_write_to_secondport(byte data);

int test_ps2ports(bool secondPortExists);

int enable_ps2ports(bool isSecondChannel);

int ps2_identify_first_device(ps2_device_types* device_type);

int ps2_identify_second_device(ps2_device_types* dvce_type);

static const char* PS2_PORT_ERROR_CODES[] = {
    "Success", "Clock Line Stuck Low", "Clock Line Stuck High", "Data Line Stuck Low", "Data Line Stuck High"
};

#endif