#include <stdlib.h>
#include <video.h>
#include "error_handler.h"
#include "regsinfo.h"

// itoa isnt in the real stdlib.h (gcc, include this wtf??) so to make vscode happy, add the function declaration here
#ifdef __VSCODE_
static char* itoa(int value, char* str, int base);
#endif

const char* last_error = "null";

const char* get_last_error() {
    return last_error;
}

int set_last_error(const char* e) {
    if (e != nullptr)
        last_error = e;
    else {
        last_error = "Error Setting the Last Error!";
        return ERROR;
    }
    return SUCCESS;
}

/*
 * if something has gone super wrong that we need to shut down the whole operating system, equivilent to a BSOD in windows
*/

NORETURN void PANIC(const char* error_message) {
    
    color panic_color = rgb(110, 22, 22);
    // set the screen to the panic color, overwriting anything previously drawn
    fill_screen(panic_color);
    print_str_inplace(error_message, 0, 0);

    print_str_inplace(build_register_string(), 0, 40);

    // we have to make sure the front_buffer has been initialized
    if (front_buffer != FRONT_BUFFER_DEFAULT_VAL)
        swap();
    else {}

    // cli;hlt
    disable_interupts();
    halt_cpu();
    // shouldnt reach here
    while(1) {}
} 

char* build_register_string() {
    static char buffer[256]; // Enough for all output
    memset(buffer, 0, sizeof(buffer));
    const char* names[] = {
        "eax: ", "ebx: ", "ecx: ", "edx: ",
        "esi: ", "edi: ", "ebp: ", "esp: ",
        "eip: ", "eflags: "
    };

    uint32_t values[] = {
        get_eax(), get_ebx(), get_ecx(), get_edx(),
        get_esi(), get_edi(), get_ebp(), get_esp(),
        get_eip(), get_eflags()
    };

    char temp[16]; // Temporary buffer for each integer
    size_t offset = 0;

    for (int i = 0; i < 10; ++i) {
        // Copy "reg: "
        strcpy(buffer + offset, names[i]);
        offset += strlen(names[i]);

        // Convert value to string
        itoa(values[i], temp, 16);

        // Copy value string
        size_t len = strlen(temp);
        memcpy(buffer + offset, temp, len);
        offset += len;

        // Add newline
        buffer[offset++] = '\n';
    }

    buffer[offset] = '\0'; // Null-terminate
    return buffer;
}