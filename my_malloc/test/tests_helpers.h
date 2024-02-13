//
// Created by Jose Espinola-Lopez on 11/29/23.
//

#ifndef MY_MALLOC_TESTS_HELPERS_H
#define MY_MALLOC_TESTS_HELPERS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../include/memory_addresses.h"
#include "../include/memory_blocks.h"
#include "../include/my_calloc.h"
#include "../include/my_free.h"
#include "../include/my_malloc.h"
#include "../include/my_realloc.h"

char* allocate_initialize_strings(const char* str);
int count_allocated_memory_blocks();
int count_free_memory_blocks();
void print_data_structures();

#endif  // MY_MALLOC_TESTS_HELPERS_H
