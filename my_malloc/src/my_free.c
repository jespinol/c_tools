//
// Created by Jose Espinola-Lopez on 11/16/23.
//

#include "../include/my_free.h"

#include <string.h>

#include "../include/memory_addresses.h"
#include "../include/memory_blocks.h"

/**
 * Frees the memory block associated with the given pointer.
 * Removes its address entry from the hash table and deallocates the block.
 * @param ptr Pointer to the contents of the memory block to be freed.
 * @details Adjusts for METADATA_SIZE offset to locate the block's actual start.
 * @details Initializes the address table if not initialized.
 */
void my_free(void* ptr) {
  if (ptr == NULL) {
    return;
  }

  init_address_table();

  struct address* address_entry = search_address(ptr);

  if (address_entry->value == NULL) {
    return;
  }

  struct MemoryBlock* block = address_entry->value;
  remove_address(ptr);

  deallocate_block(block);
}