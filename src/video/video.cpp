#include "video.h" 

#ifndef __STD_LIB_H_
static char* itoa(int value, char* str, int base);
static char* float_to_str(float num, char* str, int precision);
#endif

// height of the screen
uint32_t SCREEN_WIDTH;
// width of the screen
uint32_t SCREEN_HEIGHT;
// bits per pixel (color quality) of the screen
uint32_t SCREEN_BPP;

// pointer to video memory, do not draw to this, instead use plot_pixel, due to the hardcoded 1080p restriction, we cannot support higher resolutions
uint32_t back_buffer[BACK_BUFFER_SIZE];

uint32_t* front_buffer = FRONT_BUFFER_DEFAULT_VAL;

uint32_t vSz;

static position_t current_cursor_pos;

int init_video(multiboot_info_t* multiboot_info) {
    // get all the screen values from GRUB
    uint8_t* vMem = (uint8_t*)(size_t)multiboot_info->framebuffer_addr;
    int w, h, bpp;
    w = multiboot_info->framebuffer_width;
    h = multiboot_info->framebuffer_height;
    bpp = multiboot_info->framebuffer_bpp;
     
    if (verify_video_memory(w, h, bpp, vMem) == ERROR) {
        // already threw error
        return ERROR;
    }
    
    front_buffer = (uint32_t*)multiboot_info->framebuffer_addr;

    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
    SCREEN_BPP = bpp;
    current_cursor_pos.x = 0;
    current_cursor_pos.y = 0;

    vSz = SCREEN_BPP / BITS_PER_BYTE * SCREEN_WIDTH * SCREEN_HEIGHT;

    fill_screen(rgb(87, 0, 61));

    swap();

    return SUCCESS;
}

void fill_screen(color col) {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        for (int j = 0; j < SCREEN_HEIGHT; j++) {
            plot_pixel_f(i, j, col);
        }
    }
}

// go through each byte of video memory and ensure it is zero, otherwise return an error
int verify_video_memory(int width, int height, int bpp, uint8_t* beginVideoMem) {
    for (int i = 0; i < width * height * bpp / BITS_PER_BYTE; i++) {
        if (*beginVideoMem != 0) {
            set_last_error("Error Initializing Video Memory");
            return ERROR;
        }
        beginVideoMem++;
    }
    return SUCCESS;
} 


int plot_pixel(int x, int y, color col) {
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT) {
        set_last_error("Error plotting pixel: x or y out of range");
        return ERROR;
    }
    back_buffer[y * SCREEN_WIDTH + x] = col;
    return SUCCESS;
}

void plot_pixel_f(int x, int y, color col) {
    back_buffer[y * SCREEN_WIDTH + x] = col;
}


int print_str(const char* str) {
    char* _str = (char*)str;
    while (*_str != '\0') {
        if (draw_char(*_str) == ERROR) {
            set_last_error("Error printing");
            return ERROR;
        }
        _str++;
    }
    return SUCCESS;
}

int print_ch(const char c) {
    return draw_char(c);
}

int print_int(int num) {
    char s[15];
    itoa(num, s, 10);
    return print_str(s);
}

int print_hex(int num) {
    char s[15];
    itoa(num, s, 16);
    return print_str(s);
}


int print_float(float num) {
    return print_floatd(num, 4);
}

int print_bool(bool b) {
    if (b)
        return print_str("True");
    return print_str("False");
}

int print_floatd(float num, int digits) {
    char s[25];
    float_to_str(num, s, digits);
    return print_str(s);
}

int print_float_inplace(float num, int x, int y) {
    // save the cursor position so that we can set the cursor back here after we are done printing
    position_t old_cursor_pos = current_cursor_pos;
    current_cursor_pos.x = x;
    current_cursor_pos.y = y;
    int rVal = print_floatd(num, 4);
    current_cursor_pos = old_cursor_pos;
    return rVal;
}

int print_int_inplace(int num, int x, int y) {
    // save the cursor position so that we can set the cursor back here after we are done printing
    position_t old_cursor_pos = current_cursor_pos;
    current_cursor_pos.x = x;
    current_cursor_pos.y = y;
    int rVal = print_int(num);
    current_cursor_pos = old_cursor_pos;
    return rVal;
}

int print_str_inplace(const char* str, int x, int y) {
    // save the cursor position so that we can set the cursor back here after we are done printing
    position_t old_cursor_pos = current_cursor_pos;
    current_cursor_pos.x = x;
    current_cursor_pos.y = y;
    int rVal = print_str(str);
    current_cursor_pos = old_cursor_pos;
    return rVal;
}

int print_bool_inplace(bool b, int x, int y) {
    // save the cursor position so that we can set the cursor back here after we are done printing
    position_t old_cursor_pos = current_cursor_pos;
    current_cursor_pos.x = x;
    current_cursor_pos.y = y;
    int rVal = print_bool(b);
    current_cursor_pos = old_cursor_pos;
    return rVal;
}


int draw_char(char c) {
    // all the valid ascii font values
    if (c >= ' ' && c <= '~') {       
        for (int i = 0; i < CHAR_HEIGHT; i++) {
            // if the character is going to go outside of screen bounds, move it to the next line
            if (current_cursor_pos.x + CHAR_WIDTH > SCREEN_WIDTH) {
                current_cursor_pos.x = 0;
                current_cursor_pos.y += (CHAR_HEIGHT + 1);
            }
            for (int j = CHAR_WIDTH - 1; j >=0; j--) {
                if ((ascii_letter_font[c - ' '][i] >> j) & 0b00000001) {
                    plot_pixel(CHAR_WIDTH - j + current_cursor_pos.x, CHAR_HEIGHT - i + current_cursor_pos.y, 0xFFFFFF);
                }
            }
        }
        // despite the font being 16 wide, no characters are that wide so 10 works fine, 6 is arbitrary and can be changed to add more/less space between chars
        current_cursor_pos.x += CHAR_WIDTH - 6;
    } else if(c == '\n') {
        // Slightly more than the character's height to add a little space in between them
        current_cursor_pos.y += (CHAR_HEIGHT + 1);
        current_cursor_pos.x = 0;
    } else {
        set_last_error("Error, trying to print invalid char (not valid ascii)");
        return ERROR;
    }
    return SUCCESS;
}

void draw_image(const uint32_t* p_img_data, const uint32_t width, const uint32_t height, const uint32_t startX, const uint32_t startY) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            plot_pixel_f(j+startX, i+startY, p_img_data[j+i*width]);
        }
    } 
}

void draw_vertical_line(int yAxis, int len, color col) {
    for (int i = 0; i < len; i++) {
        plot_pixel(i, yAxis, col);
    }
}

void clear_screen() {
    fill_screen(COLOR_BLACK);
}