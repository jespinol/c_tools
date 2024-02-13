//
// Created by Jose Espinola-Lopez on 11/1/23.
//

#ifndef MY_LIBASM_MY_LIBASM_H
#define MY_LIBASM_MY_LIBASM_H

#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

size_t my_strlen(const char *s);

char *my_strchr(const char *s, int c);

int my_strcmp(const char *s1, const char *s2);

int my_strncmp(const char *s1, const char *s2, size_t n);

int my_strcasecmp(const char *s1, const char *s2);

char *my_index(const char *s, int c);

void *my_memset(void *s, int c, size_t n);

void *my_memcpy(void *dest, const void *src, size_t n);

void *my_memmove(void *dest, const void *src, size_t n);

ssize_t my_write(int fd, const void *buf, size_t count);

ssize_t my_read(int fd, void *buf, size_t count);

// tests
void test_strlen();
void test_strchr();
void test_index();
void test_strcmp();
void test_strncmp();
void test_strcasecmp();
void test_memset();
void test_memcpy();
void test_memmove();
void test_read();
void test_write();

#endif  // MY_LIBASM_MY_LIBASM_H
