//
// Created by Jose Espinola-Lopez on 11/30/23.
//

#include "./tests_realloc.h"

#include "./tests_helpers.h"

void test_resize_block(const char* str1, const char* str2) {
  printf("Testing realloc with modified size...\n");

  char* block = allocate_initialize_strings(str1);
  if (block) {
    printf("\tAllocated successfully\n");
    printf("\t\tBlock contents(old): '%s'\n", block);
    printf("\tReallocating to accommodate new string...\n");

    block = my_realloc(block, strlen(str2) + 1);
    if (block == NULL) {
      printf("\tFailed!\n");
      return;
    }

    strcpy(block, str2);
    printf("\t\tBlock contents (new): '%s'\n", block);

    printf("\tFreeing block(s)...\n");
    my_free(block);
  } else {
    printf("\tFailed!\n");
  }
}

void test_realloc_NULL() {
  printf("Testing realloc NULL pointer (behave like malloc)...\n");

  const char* str = "Hello";

  char* block = my_realloc(NULL, strlen(str) + 1);
  if (block) {
    printf("\tSuccess!\n");

    strcpy(block, str);
    printf("\t\tBlock contents: '%s'\n", block);

    printf("\tFreeing block(s)...\n");
    my_free(block);
  } else {
    printf("\tFailed!\n");
  }
}

void test_realloc() {
  printf("******* Starting my_realloc Tests *******\n");

  const char* str1 = "This is str1";
  const char* str2 = "This is str2, it is longer than str1";
  test_resize_block(str1, str2);
  test_resize_block(str2, str1);

  test_realloc_NULL();
}
