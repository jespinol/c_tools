//
// Created by Jose Espinola-Lopez on 11/18/23.
//

#include "../include/memory_blocks.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/**
 * Contains the head of a linked list of MemoryBlock structure. Each node contains metadata regarding the memory block
 * followed by the contents of the memory, if any.
 */
struct MemoryBlock* managed_memory = NULL;

/**
 * Global variable to track if the managed memory has been initialized.
 */
static bool managed_memory_initialized = false;

/**
 * Initializes memory blocks using mmap() if not already initialized.
 * It sets up an initial memory block metadata.
 */
void initialize_blocks() {
  if (!managed_memory_initialized) {
    managed_memory_initialized = true;

    void* mapped = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapped == MAP_FAILED) {
      perror("Memory initialization failed\n");
      exit(EXIT_FAILURE);
    }

    managed_memory = (struct MemoryBlock*)mapped;
    managed_memory->contents = (mapped + METADATA_SIZE);
    managed_memory->content_size = MEMORY_SIZE - METADATA_SIZE;
    managed_memory->is_alloc = false;
    managed_memory->next = NULL;
    managed_memory->prev = NULL;
  }
}

/**
 * Finds an unallocated memory block of requested size.
 * @param requested_size Requested size for the memory block.
 * @return Pointer to the unallocated memory block, NULL if not found.
 */
struct MemoryBlock* find_available_block(unsigned long requested_size) {
  struct MemoryBlock* current = managed_memory;
  while (current) {
    if (!current->is_alloc && current->content_size >= requested_size) {
      return current;
    }

    current = current->next;
  }

  return NULL;
}

/**
 * Allocates a memory block of the requested size.
 * @param block Pointer to the memory block to be allocated.
 * @param requested_size Requested size for the memory block.
 * @details If enough space exists between the new block and the next block,
 * creates an unallocated block to utilize available memory efficiently.
 */
void allocate_block(struct MemoryBlock* block, unsigned long requested_size) {
  unsigned long remaining_size = block->content_size - requested_size;
  block->content_size = requested_size;
  block->is_alloc = true;

  if (remaining_size > METADATA_SIZE) {
    split_allocated_block(block, remaining_size);
  }
}

/**
 * Creates a new deallocated memory block from the remaining size after allocation.
 * @param allocated_block Pointer to the allocated memory block.
 * @param remaining_size Remaining size after allocation.
 */
void split_allocated_block(struct MemoryBlock* allocated_block, unsigned long remaining_size) {
  struct MemoryBlock* free_block = allocated_block->contents + allocated_block->content_size;
  free_block->contents = (void*)free_block + METADATA_SIZE;
  free_block->content_size = remaining_size - METADATA_SIZE;
  free_block->is_alloc = false;
  free_block->next = allocated_block->next;
  free_block->prev = allocated_block;

  allocated_block->next = free_block;

  merge_adjacent_free_blocks(free_block);
}

/**
 * Fuses adjacent free memory blocks, if any, with the given unallocated block.
 * @param block Pointer to an unallocated memory block.
 */
void merge_adjacent_free_blocks(struct MemoryBlock* block) {
  if (block == NULL) {
    return;
  }

  struct MemoryBlock* next = block->next;
  while (next != NULL && !next->is_alloc) {
    block->content_size += next->content_size + METADATA_SIZE;
    block->next = next->next;
    next = next->next;
  }

  struct MemoryBlock* prev = block->prev;
  while (prev != NULL && !prev->is_alloc) {
    prev->content_size += block->content_size + METADATA_SIZE;
    prev->next = block->next;
    prev = prev->prev;
  }
}

/**
 * Deallocates a memory block.
 * @param block Pointer to the memory block to be deallocated.
 */
void deallocate_block(struct MemoryBlock* block) {
  block->is_alloc = 0;

  merge_adjacent_free_blocks(block);
}

/**
 * Destroys all allocated memory blocks and deallocates the mapped memory.
 */
void destroy_all_blocks() {
  if (managed_memory != NULL) {
    munmap(managed_memory, MEMORY_SIZE);
    managed_memory = NULL;
    managed_memory_initialized = false;
  }
}