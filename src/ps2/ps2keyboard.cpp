#include "ps2keyboard.h"
#include <draw.h>

// stores the state of each key
uint8_t keyboard_state[128] = {0};

int total_keydown_callbacks_in_use = 0;
int total_keyup_callbacks_in_use = 0;
void(*keydown_callbacks[128])(key_state_t key);
void(*keyup_callbacks[128])(key_state_t key);





int init_ps2keyboard() {
/*
    // Disable the ps/2 devices while we set up everything
    disable_ps2_devices();
    
    // Flush any extra data in the data buffer
    inb(PS2_DATA_PORT);
    

    // Set the Controller Config Byte
    byte c = get_ps2_controllerconfig_byte();
    c = 0b00000100;
    swap();
    // Set the PS/2 Controller Config Byte
    if (set_ps2_controllerconfig_byte(c) == ERROR)
        return ERROR;
        
    // Making sure the PS/2 Controller works
    if (test_ps2controller() == ERROR)
        return ERROR;

        
    bool isSecondChannel = false;
    if (ps2_enable_second_channel()) {
        print_str("Second Channel Exists!");
        isSecondChannel = true;
    } else
        print_str("Second Channel Not Found!");

    test_ps2ports(isSecondChannel);

        // Enable the first PS/2 Port
    if (enable_ps2ports(isSecondChannel) == ERROR)
        return ERROR;
        
    if (ps2_reset_devices() == ERROR)
        return ERROR;
    
    c = get_ps2_controllerconfig_byte();
    c = 0b00110100;
    if (isSecondChannel)
        c |= 0b00000011;
    else
        c |= 0b00000001;
    // Set the PS/2 Controller Config Byte
    if (set_ps2_controllerconfig_byte(c) == ERROR)
        return ERROR;

    print_str("HEREHEHRR\n");

    ps2_controller_waittowrite();;
    ps2_write_to_dataport(0xF4);
    ps2_controller_waittoread();
    print_str("\nScancode1: ");
    print_hex(ps2_read_from_dataport());
    print_ch('\n');

    //ps2_controller_waittoread();
    if (/*isSecondChanneltrue) {
        writetosecondps2port(0xF4);

        print_str("\nScancode2: ");
        ps2_controller_waittoread();
        print_hex(ps2_read_from_dataport());
        print_ch('\n');
    }
    

    
    return SUCCESS;
*/
    // Enable the second ps2 port (technically we should check if there are 2 but every pc since like 1990 supports two soo)
    ps2_controller_waittowrite();
    ps2_write_to_commandport(PS2_ENABLE_SECOND_PORT);

    // get and modify the ps2 controller byte
    byte c = get_ps2_controllerconfig_byte();
    c |= 0b11;
    set_ps2_controllerconfig_byte(c);

    writetosecondps2port(PS2_DEVICE_SET_DEFAULTS);

    ps2_controller_waittoread();  
    c = ps2_read_from_dataport();
    print_str("HEX: ");
    print_hex(c);

    writetosecondps2port(PS2_DEVICE_ENABLE_SCANNING);
    
    ps2_controller_waittoread();  
    c = ps2_read_from_dataport();
print_str("HEX: ");
    print_hex(c);
    
    
    // Not needed anymore
    return SUCCESS;
}


 void keycode_recv() {
    uint8_t in = inb(0x60);
    bool breakCode = in & BREAK_BIT;
    in &= ~BREAK_BIT;
    key_state_t k = keyboard_layout_us[0][in];

    // the key is a break code if the last bit is set
    if (breakCode) {
        keyboard_state[k] = KEY_IS_UP;
        // call all of the keyup callbacks
        for (int i = 0; i < total_keyup_callbacks_in_use; i++) {
            if ((void*)keyup_callbacks != NULL)
                keyup_callbacks[i](k);
        }
    } else {
        keyboard_state[k] |= (KEY_IS_DOWN);
        for (int i = 0; i < total_keydown_callbacks_in_use; i++) {
            if ((void*)keydown_callbacks != NULL)
                keydown_callbacks[i](k);
        }
    }
}


bool is_key_down(key_state_t key) {
    if (keyboard_state[key] & KEY_IS_DOWN)
        return true;
    return false;
}

int get_key_down_evnt(void(*callback)(key_state_t k)) {
    if (total_keydown_callbacks_in_use == 127) {
        set_last_error("ERROR: too many keydown callbacks in place\n");
        return ERROR;
    }

    keydown_callbacks[total_keydown_callbacks_in_use] = callback;
    total_keydown_callbacks_in_use++;
    return SUCCESS;
}

int get_key_up_evnt(void(*callback)(key_state_t k)) {
    if (total_keyup_callbacks_in_use == 127) {
        set_last_error("ERROR: too many keyup callbacks in place\n");
        return ERROR;
    }

    keyup_callbacks[total_keyup_callbacks_in_use] = callback;
    total_keyup_callbacks_in_use++;
    return SUCCESS;
}