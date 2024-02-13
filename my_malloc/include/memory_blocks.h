//
// Created by Jose Espinola-Lopez on 11/18/23.
//

#ifndef MY_MALLOC_MEMORY_BLOCKS_H
#define MY_MALLOC_MEMORY_BLOCKS_H

#include <stdbool.h>

#define MEMORY_SIZE 1024 * 1000 * 100
struct MemoryBlock {
  bool is_alloc;
  struct MemoryBlock* next;
  struct MemoryBlock* prev;
  unsigned long content_size;
  void* contents;
};

#define METADATA_SIZE sizeof(struct MemoryBlock)

extern struct MemoryBlock* managed_memory;

void initialize_blocks();
struct MemoryBlock* find_available_block(unsigned long requested_size);
void allocate_block(struct MemoryBlock* block, unsigned long requested_size);
void split_allocated_block(struct MemoryBlock* allocated_block, unsigned long remaining_size);
void merge_adjacent_free_blocks(struct MemoryBlock* block);
void deallocate_block(struct MemoryBlock* block);
void destroy_all_blocks();

#endif  // MY_MALLOC_MEMORY_BLOCKS_H
