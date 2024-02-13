//
// Created by Jose Espinola-Lopez on 12/12/23.
//

#include "./tests_calloc.h"

#include <unistd.h>

#include "./tests_helpers.h"

size_t num_elements = 30;

char* allocate_with_malloc() {
  char* block = my_malloc(num_elements);
  for (int i = 0; i < (int)num_elements; ++i) {
    block[i] = 97;
  }
  return block;
}

char* allocate_with_calloc() { return my_calloc(num_elements, sizeof(char)); }

void test_calloc() {
  printf("******* Starting my_calloc Tests *******\n");

  char* original = allocate_with_malloc();
  printf("\t%p after malloc:\n", original);
  write(0, "\t\t'", 3);
  write(0, original, num_elements);
  write(0, "'", 1);

  printf("\n\tFreeing block(s)...\n");
  my_free(original);

  printf("\t%p after free (memory not overwritten, but available):\n", original);
  write(0, "\t\t'", 3);
  write(0, original, num_elements);
  write(0, "'", 1);

  char* callocd = allocate_with_calloc();
  printf("\n\t%p after calloc (should overwrite):\n", callocd);
  write(0, "\t\t'", 3);
  write(0, callocd, num_elements);
  write(0, "'", 1);

  printf("\n\tFreeing block(s)...\n");
  my_free(callocd);
}