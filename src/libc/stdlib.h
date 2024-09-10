#ifndef __STD_LIB_H_
#define __STD_LIB_H_

static void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

static char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) {  // Base should be between 2 and 36
        str[0] = '\0';  // Invalid base case
        return str;
    }

    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, as it is a special case
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers only for base 10
    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    // Process individual digits
    while (value != 0) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value = value / base;
    }

    // If the number was negative, append '-'
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';  // Null-terminate the string

    // Reverse the string as digits are in reverse order
    reverse(str, i);

    return str;
}


#endif