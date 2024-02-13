//
// Created by Jose Espinola-Lopez on 11/16/23.
//

#include "../include/memory_addresses.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../include/memory_blocks.h"

/**
 * array of address structures containing key value pairs corresponding to addresses of allocated blocks of memory. The
 * key is a hashed representation of the block's address.
 */
struct address hash_table[HASH_TABLE_SIZE];

/**
 * Global variable to track if the hash table has been initialized.
 */
static bool hash_table_initialized = false;

/**
 * Initializes the hash table by setting all elements to default values.
 */
void init_address_table() {
  if (!hash_table_initialized) {
    for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
      hash_table[i].key = -1;
      hash_table[i].value = NULL;
    }

    hash_table_initialized = true;
  }
}

/**
 * Generates a hash value for the given key.
 *
 * @param key Key to be hashed.
 * @return Hash value.
 */
int hash_function(int key) { return key % HASH_TABLE_SIZE; }

/**
 * Searches for the address of block of memory in the hash table using the hashed key derived form the block's contents
 * pointer.
 *
 * @param address A pointer to a block address to look up in the hash table.
 * @return An address structure corresponding to the memory block address. If this address is not populated in the hash
 * table, i.e. the block was not previously added to the table, its value member will be NULL.
 */
struct address* search_address(void* address) {
  int key = (int)((uintptr_t)address - METADATA_SIZE);
  return &hash_table[hash_function(key)];
}

/**
 * Inserts an address corresponding to a memory block into the hash table.
 *
 * @param new_address the address of the memory block to be added to the hash table.
 */
void add_address(void* new_address) {
  int key = (int)((uintptr_t)new_address - (uintptr_t)managed_memory);
  int index = hash_function(key);

  if (hash_table[index].key >= 0) {
    return;
  }

  while (hash_table[index].key != -1) {
    index = (index + 1) % HASH_TABLE_SIZE;
  }

  hash_table[index].key = key;
  hash_table[index].value = new_address;
}

/**
 * Removes an address from the hash table based on the address of a memory block.
 *
 * @param address A pointer to the address of the contents of the memory block to be removed from the hash table.
 */
void remove_address(void* address) {
  struct address* element = search_address(address);
  if (element != NULL) {
    element->key = -1;
    element->value = NULL;
  }
}
