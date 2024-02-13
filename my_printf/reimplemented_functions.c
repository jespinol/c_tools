//
// Created by Jose Espinola-Lopez on 10/3/22.
//
#include "my_printf.h"

// copies string from source to a destination
void my_strcpy(char *destination, char *source) {
    while (*source) {
        *destination = *source;
        destination++;
        source++;
    }
}

// returns the length of a string including null char
int my_strlen(char *str) {
    int len = 0;
    while (*str) {
        len++;
        str++;
    }
    return len;
}

// reverses a string and returns it
char *my_strrev(char *str) {
    int len = 0;
    while (str[len]) {
        len++;
    }
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}
