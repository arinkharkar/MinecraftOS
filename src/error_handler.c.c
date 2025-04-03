#include "error_handler.h"

const char* last_error = "null";

const char* get_last_error() {
    return last_error;
}

int set_last_error(const char* e) {
    if (e != 0)
        last_error = e;
    else {
        last_error = "Error Setting the Last Error!";
        return ERROR;
    }
    return SUCCESS;
}
