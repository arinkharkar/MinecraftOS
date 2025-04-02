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

#define PS2_DEVICE_SET_DEFAULTS 0xF6
#define PS2_DEVICE_ENABLE_SCANNING 0xF4

#define PS2_TEST_FIRST_PORT_PASSED 0x00

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

void writetosecondps2port(byte data);

int test_ps2ports(bool secondPortExists);

int enable_ps2ports(bool isSecondChannel);
static const char* PS2_PORT_ERROR_CODES[] = {
    "Success", "Clock Line Stuck Low", "Clock Line Stuck High", "Data Line Stuck Low", "Data Line Stuck High"
};

#endif