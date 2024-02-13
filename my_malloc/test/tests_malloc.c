//
// Created by Jose Espinola-Lopez on 11/29/23.
//

#include "./tests_malloc.h"

#include "./tests_helpers.h"

void test_allocate_single_block() {
  printf("Testing single block allocation and free...\n");

  const char* str = "Malloc";

  char* block = allocate_initialize_strings(str);
  if (block) {
    printf("\tSuccess!\n");
    printf("\t\tBlock contents: '%s'\n", block);

    printf("\tFreeing block(s)...\n");
    my_free(block);
  } else {
    printf("\tFailed!\n");
  }
}

void test_allocate_blocks_sequential() {
  printf("Testing sequential block allocations...\n");

  const char* str1 = "Hello world!";
  const char* str2 = "Hola mundo!";
  const char* str3 = "Hallo Welt!";

  char* block1 = allocate_initialize_strings(str1);
  char* block2 = allocate_initialize_strings(str2);
  char* block3 = allocate_initialize_strings(str3);
  if (block1 && block2 && block3) {
    printf("\tSuccess!\n");
    printf("\t\tBlock1 contents: '%s'\n", block1);
    printf("\t\tBlock2 contents: '%s'\n", block2);
    printf("\t\tBlock3 contents: '%s'\n", block3);

    printf("\tFreeing block(s)...\n");
    my_free(block3);
    my_free(block2);
    my_free(block1);
  } else {
    printf("\tFailed!\n");
  }
}

void test_allocate_blocks_complex() {
  printf("Testing multiple block allocations...\n");

  const char* str1 = "A";
  const char* str2 = "BB";
  const char* str3 = "CCC";

  char* block1 = allocate_initialize_strings(str1);
  char* block2 = allocate_initialize_strings(str2);
  char* block3 = allocate_initialize_strings(str3);
  if (block1 && block2 && block3) {
    printf("\tSuccess!\n");
    printf("\t\tBlock1 contents: '%s'\n", block1);
    printf("\t\tBlock2 contents: '%s'\n", block2);
    printf("\t\tBlock3 contents: '%s'\n", block3);
  } else {
    printf("\tFailed!\n");
    return;
  }

  printf("\tFreeing Block2 and allocating a slightly larger block...\n");
  my_free(block2);

  const char* str4 = "DDDD";
  char* block4 = allocate_initialize_strings(str4);
  if (block4) {
    printf("\tSuccess!\n");
    printf("\t\tBlock4 contents: '%s'\n", block4);
  } else {
    printf("\tFailed!\n");
    return;
  }

  printf("\tAllocating a Block2 again...\n");
  block2 = allocate_initialize_strings(str2);
  if (block2) {
    printf("\tSuccess!\n");
    printf("\t\tBlock2 contents: '%s'\n", block2);
  } else {
    printf("\tFailed!\n");
    return;
  }

  printf("\tExpect blocks A -> BB -> CCC -> DDDD\n");
  print_data_structures();

  printf("\tFreeing block(s)...\n");
  my_free(block4);
  my_free(block3);
  my_free(block2);
  my_free(block1);
}

void test_very_large_allocation() {
  printf("Testing allocation of a block exceeding managed memory size...\n");

  unsigned long large_size = MEMORY_SIZE + 1;
  int* large_block = my_malloc(large_size);
  if (large_block == NULL) {
    printf("\tFailed to allocate block (expected behavior)\n");
  } else {
    printf("\tSuccess? Unexpectedly allocated block\n");
    printf("\tFreeing block(s)...\n");
    my_free(large_block);
  }
}

void test_merge_free_blocks() {
  printf("Testing merging of contiguous free blocks...\n");

  const char* str1 = "Hello world!";
  const char* str2 = "Hola mundo!";
  const char* str3 = "Hallo Welt!";

  char* block1 = allocate_initialize_strings(str1);
  char* block2 = allocate_initialize_strings(str2);
  char* block3 = allocate_initialize_strings(str3);

  if (block1 && block2 && block3) {
    printf("\tSuccess allocating three blocks!\n");

    int alloc_num = 3;
    int free_num = 1;
    printf("\t\tExpect\t%d allocated and %d free block(s)\n", alloc_num, free_num);
    printf("\t\tFound\t%d allocated and %d free block(s)\n", count_allocated_memory_blocks(),
           count_free_memory_blocks());

    printf("\tFreeing block(s)...\n");
    my_free(block1);
    my_free(block2);
    my_free(block3);

    printf("\tAfter freeing three blocks,\n");

    alloc_num = 0;
    free_num = 1;
    printf("\t\tExpect\t%d allocated and %d free block(s)\n", alloc_num, free_num);
    printf("\t\tFound\t%d allocated and %d free block(s)\n", count_allocated_memory_blocks(),
           count_free_memory_blocks());
  } else {
    printf("\tFailed!\n");
  }
}

void test_malloc() {
  printf("******* Starting my_malloc Tests *******\n");
  test_allocate_single_block();
  test_allocate_blocks_sequential();
  test_allocate_blocks_complex();
  test_very_large_allocation();
  test_merge_free_blocks();
}