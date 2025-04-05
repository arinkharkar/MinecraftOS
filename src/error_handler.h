#ifndef __ERROR_HANDLER__
#define __ERROR_HANDLER__
#include <minecraftOS.h>

// In order to help debug, when returning ERROR from a function also call this to provide details as to what failed
int set_last_error(const char* e);
// When a function returns ERROR, use this to find why
const char* get_last_error();
// If the OS is in an unrecoverable state, use this 
#ifdef __cplusplus
NORETURN void PANIC(const char* error_message = "No error message available.");
#else
NORETURN void PANIC(const char* error_message);
#endif

char* build_register_string();

#endif