#ifndef __PS2_KEYBOARD_H_
#define __PS2_KEYBOARD_H_

#include <stdbool.h>
#include <stddef.h>
#include <video.h>
#include "ps2_helper.h"
#include "io.h"





constexpr int BREAK_BIT = 0b10000000;

// credit to jdh https://www.github.com/jdah for these
constexpr int KEY_NULL = 0;
constexpr int KEY_ESC = 27;
constexpr char KEY_BACKSPACE = '\b';
constexpr char KEY_TAB = '\t';
constexpr char KEY_ENTER = '\n';
constexpr char KEY_RETURN = '\r';

constexpr int KEY_INSERT = 0x90;
constexpr int KEY_DELETE = 0x91;
constexpr int KEY_HOME = 0x92;
constexpr int KEY_END = 0x93;
constexpr int KEY_PAGE_UP = 0x94;
constexpr int KEY_PAGE_DOWN = 0x95;
constexpr int KEY_LEFT = 0x4B;
constexpr int KEY_UP = 0x48;
constexpr int KEY_RIGHT = 0x4D;
constexpr int KEY_DOWN = 0x50;

constexpr int KEY_F1 = 0x80;
constexpr int KEY_F2 = KEY_F1 + 1;
constexpr int KEY_F3 = KEY_F1 + 2;
constexpr int KEY_F4 = KEY_F1 + 3;
constexpr int KEY_F5 = KEY_F1 + 4;
constexpr int KEY_F6 = KEY_F1 + 5;
constexpr int KEY_F7 = KEY_F1 + 6;
constexpr int KEY_F8 = KEY_F1 + 7;
constexpr int KEY_F9 = KEY_F1 + 8;
constexpr int KEY_F10 = KEY_F1 + 9;
constexpr int KEY_F11 = KEY_F1 + 10;
constexpr int KEY_F12 = KEY_F1 + 11;

constexpr int KEY_LCTRL = 0x1D;
constexpr int KEY_RCTRL = 0x1D;

constexpr int KEY_LALT = 0x38;
constexpr int KEY_RALT = 0x38;

constexpr int KEY_LSHIFT = 0x2A;
constexpr int KEY_RSHIFT = 0x36;

constexpr int KEY_CAPS_LOCK = 0x3A;
constexpr int KEY_SCROLL_LOCK = 0x46;
constexpr int KEY_NUM_LOCK = 0x45;

constexpr int KEY_MOD_ALT = 0x0200;
constexpr int KEY_MOD_CTRL = 0x0400;
constexpr int KEY_MOD_SHIFT = 0x0800;
constexpr int KEY_MOD_CAPS_LOCK = 0x1000;
constexpr int KEY_MOD_NUM_LOCK = 0x2000;
constexpr int KEY_MOD_SCROLL_LOCK = 0x4000;

constexpr int KEY_IS_DOWN = 1 << 1;
constexpr int LSHIFT_PRESSED = 1 << 2;
constexpr int RSHIFT_PRESSED = 1 << 3;
constexpr int LCTRL_PRESSED = 1 << 4;
constexpr int RCTRL_PRESSED = 1 << 5;
constexpr int KEY_IS_UP = 0;


// 60 min
constexpr int INACTIVITY_TIME_SECONDS = 60*60;

typedef uint8_t key_state_t;

static uint8_t keyboard_layout_us[2][128] = {
    {
        KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        ',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }, {
        KEY_NULL, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
        '_', '+', KEY_BACKSPACE, KEY_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U',
        'I', 'O', 'P',   '{', '}', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H',
        'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M', '<', '>', '?', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
        KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }
};

constexpr char KEY_SPACE = ' ';
//typedef void(*callback)(key_state_t key) key_callback_f;


int init_ps2keyboard();
// Callback function whenever the OS recieves an IRQ of a key press
void keycode_recv();

// Checks if the key is currently pressed down
bool is_key_down(key_state_t key);

// set a callback function for when the key is pressed or lifted
int get_key_down_evnt(void(*callback)(key_state_t));
int get_key_up_evnt(void(*callback)(key_state_t));

void check_for_kybd_inactivity();


#endif