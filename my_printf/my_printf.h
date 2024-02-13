//
// Created by Jose Espinola-Lopez on 10/3/22.
//

#ifndef MY_PRINTF_MY_PRINTF_H
#define MY_PRINTF_MY_PRINTF_H

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

// own functions
void output_print(char *buffer);

int format_str_return_len(char *buffer, char *str);

int format_num_return_len(long number, char *output_buffer, char base_input);

long preprocess_special_num(long number);

long convert_binary_to_decimal(char *buffer);

void find_twos_complement(char *buffer);

int set_base(char c);

// reimplemented functions
void my_strcpy(char *destination, char *source);

int my_strlen(char *str);

char *my_strrev(char *str);

#endif //MY_PRINTF_MY_PRINTF_H
