#include "ps2keyboard.h"
#include <draw.h>

// stores the state of each key
uint8_t keyboard_state[128] = {0};

int total_keydown_callbacks_in_use = 0;
int total_keyup_callbacks_in_use = 0;
void(*keydown_callbacks[128])(key_state_t key);
void(*keyup_callbacks[128])(key_state_t key);





int init_ps2keyboard() {
    // Set the deafult settings of the keyboard
    ps2_controller_waittowrite();
    write_to_keyboard(PS2_DEVICE_SET_DEFAULTS);
    ps2_controller_waittoread();  
    byte c = recv_from_keyboard();
    if (c != PS2_ACK)
        return ERROR;

    // Enable keyboard scanning
    ps2_controller_waittowrite();
    write_to_keyboard(PS2_DEVICE_ENABLE_SCANNING);
    ps2_controller_waittoread();  
    c = recv_from_keyboard();
    if (c != PS2_ACK)
        return ERROR;
    
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