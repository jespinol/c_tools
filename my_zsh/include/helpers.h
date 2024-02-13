//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#ifndef MY_ZSH_HELPERS_H
#define MY_ZSH_HELPERS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int my_strlen(const char *str);

char *my_strcpy(char *dest, const char *src);

char *my_strncpy(char *dest, const char *src, size_t n);

int my_strcmp(char *str1, const char *str2);

int my_strncmp(const char *str1, const char *str2, size_t n);

char *my_strchr(char *str, int chr);

char *my_strcat(char *dest, const char *src);

char *my_strdup(const char *str);

size_t my_strspn(const char *str, const char *charset);

size_t my_strcspn(const char *str, const char *charset);

char *my_strtok(char *str, const char *delim, char **saveptr);

void copy_arr(char **src, char ***dest);

void free_arr(char **arr);

void concat_with_separator(char *dest, const char *str1, const char *separator, const char *str2);

char *concat_dynamic_strings(const char *str1, const char *str2, const char *str3);

bool is_executable(const char *path);

#endif  // MY_ZSH_HELPERS_H
