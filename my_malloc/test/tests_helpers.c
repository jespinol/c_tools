//
// Created by Jose Espinola-Lopez on 11/29/23.
//

#include "tests_helpers.h"

char* allocate_initialize_strings(const char* str) {
  size_t len = strlen(str) + 1;
  char* block = my_malloc(len * sizeof(char));
  if (block && str) {
    strcpy(block, str);
  }
  return block;
}

void print_readable_size(unsigned long size) {
  const char* units[] = {"B", "KB", "MB", "GB", "TB"};
  int unit_index = 0;
  double sz = (double)size;

  while (sz >= 1024 && unit_index < 5) {
    sz /= 1024;
    unit_index++;
  }

  printf("%8.2f %2s", sz, units[unit_index]);
}

void print_data_structures() {
  const char* labels[] = {"Block", "Size", "Alloc", "Content Add", "Contents"};
  printf("\t\t%11s\t%11s\t%7s\t\t%11s\t\t%s\n", labels[0], labels[1], labels[2], labels[3], labels[4]);

  struct MemoryBlock* current = managed_memory;

  while (current != NULL) {
    bool is_alloc = current->is_alloc;
    printf("\t\t%p\t", (void*)current);
    print_readable_size(current->content_size);
    printf("\t%7s", is_alloc ? "T" : "F");
    printf("\t\t%p\t", current->contents);
    printf("\t'");
    if (is_alloc) {
      printf("%s", (char*)current->contents);
    }
    printf("'\n");
    current = current->next;
  }
}

int count_allocated_memory_blocks() {
  struct MemoryBlock* current = managed_memory;
  int count = 0;
  while (current != NULL) {
    if (current->is_alloc) {
      count++;
    }
    current = current->next;
  }
  return count;
}

int count_free_memory_blocks() {
  struct MemoryBlock* current = managed_memory;
  int count = 0;
  while (current != NULL) {
    if (!current->is_alloc) {
      count++;
    }
    current = current->next;
  }
  return count;
}
