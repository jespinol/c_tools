//
// Created by Jose Espinola-Lopez on 11/16/23.
//

#ifndef MY_MALLOC_MEMORY_ADDRESSES_H
#define MY_MALLOC_MEMORY_ADDRESSES_H

#define HASH_TABLE_SIZE 1024

struct address {
  int key;
  void* value;
};

extern struct address hash_table[HASH_TABLE_SIZE];

void init_address_table();
int hash_function(int key);
struct address* search_address(void* address);
void add_address(void* new_address);
void remove_address(void* address);

#endif  // MY_MALLOC_MEMORY_ADDRESSES_H
