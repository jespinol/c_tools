//
// Created by Jose Espinola-Lopez on 10/2/22.
//

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void output_print(char *buffer);

int format_str_return_len(char *buffer, char *str);

int format_num_return_len(long number, char *output_buffer, char base_input);

long preprocess_special_num(long number);

long convert_binary_to_decimal(char *buffer);

void find_twos_complement(char *buffer);

int set_base(char c);

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
  strcpy(buffer, str);
  return strlen(str) - 1;
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
  strcpy(output_buffer, scratch_buffer);
  return strlen(scratch_buffer) - 1;
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

int my_printf(char *input, ...) {
    va_list vl;
    va_start(vl, input);

    bool error = false;
    int MAX_BUFFER_SIZE = 1024;
    char output[MAX_BUFFER_SIZE * sizeof(char)];
    int in_index = 0;
    int out_index = 0;

    // iterates through the input string until null char or an error is found
    while (input[in_index] && !error) {
        // if a given character is not % then print it as is
        if (input[in_index] != '%') {
            output[out_index] = input[in_index];
        } else {
            // if a % is followed by % treat it as an escape character and print the following chars as is
            if (input[in_index + 1] == '%') {
                output[out_index] = input[in_index + 1];
                in_index++;
                // if % is followed by a char other than % consider it a specifier
                // however, it the char is space or null char set error to true and end the loop
            } else if (input[in_index + 1] == ' ' || input[in_index + 1] == '\0' || input[in_index + 1] == '\n') {
                error = true;
                my_printf("Specifier is not valid. %% must be followed by s, c, p, d, o, u, or x\n");
                break;
            } else {
                char specifier = input[in_index + 1];
                switch (specifier) {
                    // numbers
                    case 'd':
                    case 'o':
                    case 'u':
                    case 'x':
                        out_index += format_num_return_len(va_arg(vl, int), &output[out_index], specifier);
                        break;
                        // characters
                    case 'c':
                        output[out_index] = va_arg(vl, int);
                        break;
                        // strings
                    case 's':
                        out_index += format_str_return_len(&output[out_index], va_arg(vl, char*));
                        break;
                        // pointer
                    case 'p':
                        out_index += format_num_return_len(va_arg(vl, long int), &output[out_index], specifier);
                        break;
                        // other specifiers result in an error message
                    default:
                        error = true;
                        my_printf("Specifier '%%%c' has not been implemented\n", specifier);
                        break;
                }
                in_index++;
            }
        }
        in_index++, out_index++;
    }

    output[out_index] = '\0';

    // if no error was found print the output buffer, otherwise print an error message
    !error ? output_print(output) : output_print("Exited with an error\n");

    va_end(vl);
    return strlen(output);
}

int main(void) {
    my_printf("----------examples of positive numbers handling----------\n");
    int number = 1234567890;
    my_printf("my_printf   %%d %d   %%o %o   %%x %x   %%u %u\n", number, number, number, number);
    printf("   printf   %%d %d   %%o %o   %%x %x   %%u %u\n", number, number, number, number);

    my_printf("\n");

    my_printf("----------examples of negative numbers handling----------\n");
    number *= -1;
    my_printf("my_printf   %%d %d   %%o %o   %%x %x   %%u %u\n", number, number, number, number);
    printf("   printf   %%d %d   %%o %o   %%x %x   %%u %u\n", number, number, number, number);

    my_printf("\n");

    my_printf("--------------examples of pointer handling--------------\n");
    int pointer = 1;
    my_printf("my_printf    %%p %p\n", (void *) &pointer);
    printf("   printf    %%p %p\n", (void *) &pointer);

    my_printf("\n");

    my_printf("-------examples of strings and characters handling-------\n");
    char c = 'a';
    char str[] = "abc";
    my_printf("my_printf   %%c %c   %%s %s\n", c, str);
    printf("   printf   %%c %c   %%s %s\n", c, str);

    my_printf("\n");

    my_printf("---example of challenging arguments and special cases---\n");
    my_printf("my_printf 0   %d\n", 0);
    printf("   printf 0   %d\n", 0);
    my_printf("\n");
    my_printf("my_printf   %d - %d - %d!\n", 2048, 0, -1337);
    printf("   printf   %d - %d - %d!\n", 2048, 0, -1337);
    my_printf("\n");
    my_printf("my_printf multiple args   %d %c %s\n", number, c, str);
    printf("   printf multiple args   %d %c %s\n", number, c, str);
    my_printf("\n");
    my_printf("Expect the following output \"NULL STRING (null)! 31\"\n");
    my_printf(" %d\n", my_printf("my_printf   NULL STRING %s!", (char *) NULL));
//    printf(" %d\n",printf("   printf NULL STRING %s!", (char*)NULL)); // will lead to a compilation error with -Werror, remove flag to see behavior
    my_printf("\n");
    my_printf("Using an invalid specifier, 'my_printf(\"%%i %%d\\n\", 123, 123);' produces message:\n");
    my_printf("%d %i\n", 123, 123);
    my_printf("\n");
    my_printf("Using an invalid specifier, 'my_printf(\"%% %%\\n\", 123, 123);' produces message:\n");
    my_printf("% %\n", 123, 123);
    return 0;
}
