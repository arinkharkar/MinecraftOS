#ifndef __ERROR_HANDLER__
#define __ERROR_HANDLER__
#include <minecraftOS_types.h>

// In order to help debug, when returning ERROR from a function also call this to provide details as to what failed
int set_last_error(const char* e);
// When a function returns ERROR, use this to find why
const char* get_last_error();

#endif