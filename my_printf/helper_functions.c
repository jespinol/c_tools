//
// Created by Jose Espinola-Lopez on 10/3/22.
//
#include "my_printf.h"

// prints contents of buffer to stdout character by character
void output_print(char *buffer) {
    int i = 0;
    while (buffer[i]) {
        i++;
    }
    write(1, buffer, i);
}

// for %s
// first check if NULL, if so reassign str to "(null)" which is the resulting string seen when the same is done with printf()
// if not, copy the contents of the va_arg to the output buffer
// returns the length of the resulting string
int format_str_return_len(char *buffer, char *str) {
    if (str == NULL) {
        str = "(null)";
    }
    my_strcpy(buffer, str);
    return my_strlen(str) - 1;
}

/*
 * for numerical specifiers
 * in general, the input number is divided by a base (i.e. 10 for decimal, 8 for octal, 16 for hexadecimal, etc.)
 * and the remainder is stored in scratch_buffer as a character. This, however, produces a reverse string which is reversed by my_strrev()
 * there are a number of specific actions taken for special types of numbers:
 * ** 0 is handled as a special case
 * ** negative decimals are multiplied by -1 and at the end a '-' symbol is added.
 * ** pointers have a '0x' preceding the string
 * ** negative octal, hex, and unsigned decimals have undefined behaviour (at least by C11 and C17 standards).
 *    However, to exactly replicate what printf does in my environment (and in DoCode) I preprocess them with preprocess_special_num()
 */
int format_num_return_len(long number, char *output_buffer, char base_input) {
    int SCRATCH_BUFFER_SIZE = 512;
    char scratch_buffer[SCRATCH_BUFFER_SIZE * sizeof(char)];
    int base = set_base(base_input);
    int i = 0;

    bool input_zero = number == 0;
    bool negative_decimal = base_input == 'd' && number < 0;
    bool negative_special = base_input != 'd' && number < 0;
    bool void_pointer = base_input == 'p';

    if (input_zero) {
        scratch_buffer[i++] = '0';
    } else if (negative_special) {
        number = preprocess_special_num(number);
    }

    if (negative_decimal) {
        number *= -1;
    }

    while (number != 0) {
        int remainder = number % base;
        if (remainder > 9) {
            scratch_buffer[i++] = (remainder - 10) + 'a';
        } else {
            scratch_buffer[i++] = remainder + '0';
        }
        number = number / base;
    }

    if (void_pointer) {
        scratch_buffer[i++] = 'x';
        scratch_buffer[i++] = '0';
    }

    if (negative_decimal) {
        scratch_buffer[i++] = '-';
    }

    scratch_buffer[i] = '\0';
    my_strrev(scratch_buffer);
    my_strcpy(output_buffer, scratch_buffer);
    return my_strlen(scratch_buffer) - 1;
}

// Certain negative numbers are first converted to binary, then the 2's complement is calculated,
// then the most significant bit is flipped, then the string is reversed
// The binary is converted back to decimal, which then can be processed as a positive number
long preprocess_special_num(long number) {
    int SCRATCH_BUFFER_SIZE = 512;
    char temp[SCRATCH_BUFFER_SIZE * sizeof(char)];
    int j = 0;
    number *= -1;
    while (number != 0) {
        int remainder = number % 2;
        temp[j++] = remainder + '0';
        number = number / 2;
    }
    temp[j++] = '1';
    temp[j] = '\0';
    my_strrev(temp);
    find_twos_complement(temp);
    temp[0] = '1';
    number = convert_binary_to_decimal(&temp[0]);
    return number;
}

// replaces a binary number in buffer with its 32-bit 2's complement
void find_twos_complement(char *buffer) {
    // calculate one's complement
    for (int i = 0; i < 32; i++) {
        if (buffer[i] == '0') {
            buffer[i] = '1';
        } else if (buffer[i] == '1') {
            buffer[i] = '0';
        }
    }
    // calculate two's complement
    int carry = 1;
    for (int i = 31; i >= 0; i--) {
        if (buffer[i] == '1' && carry == 1) {
            buffer[i] = '0';
        } else if (buffer[i] == '0' && carry == 1) {
            buffer[i] = '1';
            carry = 0;
        }
    }
}

// converts an input binary to decimal and returns it
long convert_binary_to_decimal(char *buffer) {
    long output = 0;
    while (*buffer) {
        output *= 2;
        if (*buffer++ == '1') {
            output += 1;
        }
    }
    return output;
}

// returns an integer base related to a numerical specifier
int set_base(char c) {
    switch (c) {
        case 'o':
            return 8;
        case 'd':
        case 'u':
            return 10;
        case 'x':
        case 'p':
            return 16;
        default:
            return 2;
    }
}
