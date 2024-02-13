//
// Created by Jose Espinola-Lopez on 11/16/23.
//
#include "../include/my_malloc.h"

#include <stdio.h>

#include "../include/memory_addresses.h"
#include "../include/memory_blocks.h"

/**
 * Allocates space for a block memory of specified size.
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the contents of the allocated memory block, NULL if allocation fails.
 * @details Initializes memory management structures, if needed.
 */
void* my_malloc(unsigned long size) {
  if (size < 1) {
    return NULL;
  }

  init_address_table();
  initialize_blocks();

  struct MemoryBlock* block = find_available_block(size);
  if (block == NULL) {
    return NULL;
  }

  allocate_block(block, size);
  add_address(block);

  return block->contents;
}