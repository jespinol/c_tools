//
// Created by Jose Espinola-Lopez on 11/1/23.
//

#include "../include/my_libasm.h"

int main(void) {
  test_strlen();
  test_strchr();
  test_index();
  test_strcmp();
  test_strncmp();
  test_strcasecmp();

  test_memset();
  test_memcpy();
  test_memmove();

  test_write();
  test_read();

  return 0;
}

void test_strlen() {
  printf("\n*** strlen ************************\n");
  printf("Test get length of \"Hello, world\"\n");

  const char* str = "Hello, world";
  size_t test = my_strlen(str);
  size_t target = (str != NULL) ? strlen(str) : 0;
  char* outcome = test == target ? "is" : "IS NOT";

  printf("\tmy_strlen returns '%ld', which %s the same as strlen (%ld)\n", test, outcome, target);
}

void test_strchr() {
  printf("\n*** strchr ************************\n");
  printf("Test find 'w' in \"Hello, world\"\n");

  const char* str = "Hello, world";
  int c = 'w';
  char* test = my_strchr(str, c);
  char* target = (str != NULL) ? strchr(str, c) : NULL;
  char* outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strchr returns '%p', which %s the same as strchr (%p)\n", test, outcome, target);

  printf("Test find 'x' in \"Hello, world\"\n");

  c = 'x';
  test = my_strchr(str, c);
  target = (str != NULL) ? strchr(str, c) : NULL;
  outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strchr returns '%p', which %s the same as strchr (%p)\n", test, outcome, target);
}

void test_index() {
  printf("\n*** index ************************\n");
  printf("Test find 'w' in \"Hello, world\"\n");

  const char* str = "Hello, world";
  int c = 'w';
  char* test = my_index(str, c);
  char* target = index(str, c);
  char* outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_index returns '%s', which %s the same as index (%s)\n", test, outcome, target);

  printf("Test find 'x' in \"Hello, world\"\n");

  c = 'x';
  test = my_strchr(str, c);
  target = (str != NULL) ? strchr(str, c) : NULL;
  outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_index returns '%p', which %s the same as index (%p)\n", test, outcome, target);
}

void test_strcmp() {
  printf("\n*** strcmp ************************\n");
  printf("Test compare \"Hello, world\" and \"Hello, world\"\n");

  const char* str1 = "Hello, world";
  const char* str2 = "Hello, world";
  int test = my_strcmp(str1, str2);
  int target = strcmp(str1, str2);
  char* outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strcmp returns '%d', which %s the same as strcmp (%d)\n", test, outcome, target);

  printf("Test compare \"Hello, world\" and \"Hello, world!\"\n");

  const char* str3 = "Hello, world!";
  test = my_strcmp(str1, str3);
  target = strcmp(str1, str3);
  outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strcmp returns '%d', which %s the same as strcmp (%d)\n", test, outcome, target);
}

void test_strncmp() {
  printf("\n*** strncmp ************************\n");
  printf("Test compare n=12 between \"Hello, world\" and \"Hello, world\"\n");

  const char* str1 = "Hello, world";
  const char* str2 = "Hello, world";
  size_t n = strlen(str1);
  int test = my_strncmp(str1, str2, n);
  int target = strncmp(str1, str2, n);
  char* outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strncmp returns '%d', which %s the same as strncmp (%d)\n", test, outcome, target);

  printf("Test compare n=12 between \"Hello, world\" and \"Hello, world!\"\n");

  const char* str3 = "Hello qwasar";
  test = my_strncmp(str1, str3, n);
  target = strncmp(str1, str3, n);
  outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strncmp returns '%d', which %s the same as strncmp (%d)\n", test, outcome, target);
}

void test_strcasecmp() {
  printf("\n*** strcasecmp ************************\n");
  printf("Test compare \"Hello, world\" and \"hELlO, WoRlD\"\n");

  const char* str1 = "Hello, world";
  const char* str2 = "hELlO, WoRlD";
  int test = my_strcasecmp(str1, str2);
  int target = strcasecmp(str1, str2);
  char* outcome = (test == target) ? "is" : "IS NOT";

  printf("\tmy_strcasecmp returns '%d', which %s the same as strcasecmp (%d)\n", test, outcome, target);
}

void test_memset() {
  printf("\n*** memset ************************\n");
  printf("Test set dest to '@'\n");

  char c = '@';
  size_t n = 12;
  char dest[13] = {'\0'};
  char buff[13] = {'\0'};

  my_memset(dest, c, n);
  memset(buff, c, n);

  int match = 1;
  for (int i = 0; i < (int)n; i++) {
    if (dest[i] != buff[i]) {
      match = 0;
      break;
    }
  }

  if (!match) {
    printf("\tFailed - ");
  } else {
    printf("\tSuccess - ");
  }
  printf("Was '%s' and should be '%s'\n", dest, buff);
}

void test_memcpy() {
  printf("\n*** memcpy ************************\n");
  printf("Test copy \"Hello, world\"\n");

  const char* src = "Hello, world";
  size_t n = strlen(src);
  char dest[n + 1];
  char buff[n + 1];

  my_memcpy(dest, src, n);
  memcpy(buff, src, n);

  int match = 1;
  for (size_t i = 0; i < n; i++) {
    if (dest[i] != buff[i]) {
      match = 0;
      break;
    }
  }

  dest[n] = '\0';
  buff[n] = '\0';

  if (!match) {
    printf("\tFailed - ");
  } else {
    printf("\tSuccess - ");
  }
  printf("Was '%s' and should be '%s'\n", dest, buff);
}

void test_memmove() {
  printf("\n*** memmove ************************\n");
  printf("Test non-overlapping\n\tSource: \"Hello, world\"\n");
  const char* src = "Hello, world";
  size_t n = strlen(src);
  char dest[64] = {'\0'};

  printf("\tDestination:\n");
  printf("\t\tBefore:\t'%s'\n", dest);
  my_memmove(dest, src, n);
  printf("\t\tAfter:\t'%s'\n", dest);

  printf("Test overlapping\n\tSource: \"WORLD!!!!\"\n");

  char* src_2 = "WORLD!!!!";
  size_t mem_overlap = n - 5;

  printf("\tDestination:\n");
  printf("\t\tBefore:\t'%s'\n", dest);
  my_memmove(&dest[mem_overlap], src_2, n);
  printf("\t\tAfter:\t'%s'\n", dest);
}

void test_write() {
  printf("\n*** write ************************\n");
  printf("Test writing \"Hello, world\" to a file\n");

  char *file_test = "test/write_test.txt";
  int fd_test = open(file_test, O_CREAT | O_WRONLY, S_IRWXU);
  char *file_target = "test/write_target.txt";
  int fd_target = open(file_target, O_CREAT | O_WRONLY, S_IRWXU);
  const char* str = "Hello, world";
  size_t n = strlen(str);

  my_write(fd_test, str, n);
  write(fd_target, str, n);

  close(fd_test);
  close(fd_target);

  fd_test = open(file_test, O_RDONLY);
  fd_target = open(file_target, O_RDONLY);

  char buff_test[32] = {'\0'};
  char buff_target[32] = {'\0'};

  read(fd_test, buff_test, n);
  read(fd_target, buff_target, n);

  int match = (strcmp(buff_test, buff_target) == 0);
  if (match) {
    printf("\tSuccess - files are the same\n");
  } else {
    printf("\tFailed - files are NOT the same\n");
  }

  close(fd_target);
  close(fd_test);
  remove("test/write_test.txt");
  remove("test/write_target.txt");

  printf("Test writing \"Hello, world\" to stdin\n");

  printf("\t my_write outputs: '");
  fflush(stdout);
  my_write(1, str, n);
  printf("'\n");
}

void test_read() {
  printf("\n*** read ************************\n");
  printf("Test reading form file\n");

  char buff_a[512] = {'\0'};
  char buff_b[512] = {'\0'};

  int fd;
  char *filename = "test/read.txt";
  fd = open(filename, O_RDONLY);
  ssize_t test = my_read(fd, buff_a, 445);
  close(fd);
  fd = open(filename, O_RDONLY);
  ssize_t target = read(fd, buff_b, 445);
  close(fd);

  char* outcome = (test == target) ? "is" : "IS NOT";
  printf("\tmy_read returns '%zd', which %s the same as read (%zd)\n", test, outcome, target);
  if (strcmp(buff_a, buff_b) != 0) {
    printf("\t\tHowever, the strings are not the same\n");
    printf("'%s'\n", buff_a);
    printf("\n'%s'\n", buff_b);
  }

  printf("Test reading form stdin\n");
  printf("\tEnter a short string: ");
  fflush(stdout);
  char buff_c[128] = {'\0'};
  test = my_read(0, buff_c, 128);
  buff_c[--test] = '\0';  // newline
  printf("\tmy_read read '%zd' characters of the input '%s'\n", test, buff_c);
}
