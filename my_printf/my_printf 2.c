//
// Created by Jose Espinola-Lopez on 10/2/22.
//

#include "my_printf.h"

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
    return my_strlen(output);
}

int main(void) {
  my_printf("%d\n", 123, 1234);
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
