//
// Created by Jose Espinola-Lopez on 11/16/23.
//

#include "./tests_calloc.h"
#include "./tests_helpers.h"
#include "./tests_malloc.h"
#include "./tests_realloc.h"

int main(void) {
  test_malloc();
  printf("\n");
  test_realloc();
  printf("\n");
  test_calloc();

  destroy_all_blocks();
}
