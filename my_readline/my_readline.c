//
// Created by Jose Espinola-Lopez on 5/10/23.
//
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"

size_t READLINE_READ_SIZE = 0;

char *overflow_buffer = NULL;

void init_my_readline(void);

char *my_readline(int fd);

void set_overflow_buffer(char *src, int start);

void empty_overflow_buffer(char **dest);

int find_EOL(char *str);

int my_strlen(char *str);

char *my_strcpy(char *dest, char *src);

char *my_strncpy(char *dest, char *src, size_t n);

char *return_line_set_overflow(char *curr_read, int end);

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s [<file1>]\n", argv[0]);
        return 1;
    }

    init_my_readline();

    int fd = 0;
    if (argc > 1) {
        fd = open(argv[1], O_RDONLY);
    }
    if (fd < 0) {
        printf("File unreadable or not found\n");
        return 0;
    }

    char *line;
    while ((line = my_readline(fd)) != NULL) {
        printf("%s\n", line);
        free(line);
    }

    close(fd);

    return 0;
}

void init_my_readline(void) {
    READLINE_READ_SIZE = 512;
    free(overflow_buffer);
}

char *my_readline(int fd) {
    char *curr_read = NULL; // Contains enough chunks of READLINE_READ_SIZE until a newline character is found. May be prefixed by chars from the previous line
    empty_overflow_buffer(&curr_read);
    size_t read_len = my_strlen(curr_read);
    int eol_pos = 0;

    ssize_t bytes_read;
    while (1) {
        curr_read = realloc(curr_read, READLINE_READ_SIZE + read_len + 1);
        bytes_read = read(fd, &curr_read[read_len], READLINE_READ_SIZE);
        read_len += bytes_read;
        if (bytes_read < 0) {
            // Error
            return NULL;
        } else if (bytes_read == 0) {
            // Reached the end of stream
            if (read_len == 0 && (curr_read != NULL && curr_read[0] != '\0')) {
                // Empty stream or buffer is empty
                return NULL;
            }

            // The rest of the stream is already in curr_read
            break;
        } else {
            // New data from the stream was read
            curr_read[read_len] = '\0';
            eol_pos = find_EOL(curr_read);
            if (eol_pos >= 0) {
                return return_line_set_overflow(curr_read, eol_pos);
            }
            // Needs more chunks of size READLINE_READ_SIZE to complete a line
        }
    }

    // The rest of the stream was read in a previous iteration
    if (my_strlen(curr_read)) {
        eol_pos = find_EOL(curr_read);
        if (eol_pos >= 0) {
            return return_line_set_overflow(curr_read, eol_pos);
        }
        return curr_read;
    }

    return NULL;
}

// Copies the contents of curr_read up to curr_read[end] to the string output (a full line of the input file)
// Any characters after end correspond to the next line(s) and are stored in overflow_buffer for further processing
char *return_line_set_overflow(char *curr_read, int end) {
    char *output = NULL;
    output = realloc(output, end + 1);
    my_strncpy(output, curr_read, end);
    set_overflow_buffer(curr_read, end);
    free(curr_read);
    return output;
}

// Copies the contents of overflow_buffer, if any, to the string dest
void empty_overflow_buffer(char **dest) {
    if (overflow_buffer != NULL) {
        int len = my_strlen(overflow_buffer);
        *dest = malloc(len + 1);
        my_strcpy(*dest, overflow_buffer);
        free(overflow_buffer);
        overflow_buffer = NULL;
    }
}

// Copies the portion of the string src from src[start] to the overflow_buffer
void set_overflow_buffer(char *src, int start) {
    int len = my_strlen(&src[start]);
    overflow_buffer = malloc(len + 1);
    if (len > 1) {
        my_strcpy(overflow_buffer, &src[start + 1]);
    }
    if (len == 1 && src[start] == '\n') {
        my_strcpy(overflow_buffer, "\0");
    }
}

// Returns the index of the first occurrence of '\n' in str, if none return -1;
int find_EOL(char *str) {
    int i = -1;

    while (*str != '\0') {
        i++;
        if (*str == '\n') {
            return i;
        }
        str++;
    }

    return -1;
}

// Returns the length of a string str excluding null char
int my_strlen(char *str) {
    if (str == NULL) {
        return 0;
    }

    int len = 0;
    while (*str) {
        len++;
        str++;
    }

    return len;
}

// Copies a string src to a string dest
char *my_strcpy(char *dest, char *src) {
    char *output = dest;

    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';

    return output;
}

// Copies n bytes from a string src to a string dest
char *my_strncpy(char *dest, char *src, size_t n) {
    char *output = dest;

    while (n > 0 && *src) {
        *dest = *src;
        dest++;
        src++;
        n--;
    }

    *dest = '\0';

    return output;
}
