//
// Created by Jose Espinola-Lopez on 11/16/23.
//

#include "../include/my_calloc.h"

#include <string.h>

#include "../include/memory_addresses.h"
#include "../include/memory_blocks.h"

/**
 * Allocates memory for an array of elements and initializes them to zero.
 * @param nmemb Number of elements in the array.
 * @param size Size of each element in the array.
 * @return Pointer to the contents of the allocated memory block, NULL if allocation fails.
 * @details Initializes memory management structures, if needed.
 */
void* my_calloc(size_t nmemb, unsigned long size) {
  if (nmemb == 0 || size == 0) {
    return NULL;
  }

  init_address_table();
  initialize_blocks();

  unsigned long mem_needed = nmemb * size;
  struct MemoryBlock* block = find_available_block(mem_needed);
  if (block == NULL) {
    return NULL;
  }

  allocate_block(block, mem_needed);
  add_address(block);

  memset(block->contents, 0, block->content_size);

  return block->contents;
}