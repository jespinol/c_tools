//
// Created by Jose Espinola-Lopez on 11/16/23.
//

#include "../include/my_realloc.h"

#include <string.h>

#include "../include/memory_addresses.h"
#include "../include/memory_blocks.h"
#include "../include/my_free.h"
#include "../include/my_malloc.h"

/**
 * Resizes the memory block associated with a pointer to the given size.
 * @param ptr Pointer to the contents of the memory block to be resized.
 * @param size New size of the memory block.
 * @return Pointer to the contents of the resized memory block, NULL if reallocation fails.
 * @details Initializes memory management structures, if needed.
 */
void* my_realloc(void* ptr, unsigned long size) {
  init_address_table();
  initialize_blocks();

  if (ptr == NULL) {
    return my_malloc(size);
  }

  if (size < 1) {
    my_free(ptr);
    return NULL;
  }

  if (search_address(ptr)->value == NULL) {
    return NULL;
  }

  void* new_block = my_malloc(size);
  if (new_block == NULL) {
    return NULL;
  }

  memcpy(new_block, ptr, size);

  my_free(ptr);

  return new_block;
}