//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#include "../include/helpers.h"

// Returns the length of a string excluding null char (modified from Bootcamp C)
int my_strlen(const char *str) {
  int len = 0;
  while (*str) {
    len++;
    str++;
  }

  return len;
}

// Copies a string src to a string dest
char *my_strcpy(char *dest, const char *src) {
  char *output = dest;

  while (*src) {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';

  return output;
}

// Copies n bytes from a string src to a string dest
char *my_strncpy(char *dest, const char *src, size_t n) {
  char *output = dest;

  while (n > 0 && *src) {
    *dest = *src;
    dest++;
    src++;
    n--;
  }

  *dest = '\0';

  return output;
}

// Compares str1 and str2, if they are identical returns 0
int my_strcmp(char *str1, const char *str2) {
  while (*str1 && (*str1 == *str2)) {
    str1++;
    str2++;
  }

  return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Compares up to n characters of str1 and str2, if they are identical returns 0
int my_strncmp(const char *str1, const char *str2, size_t n) {
  while (n > 0 && *str1 && (*str1 == *str2)) {
    str1++;
    str2++;
    n--;
  }

  if (n == 0) {
    return 0;
  }

  return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Searches for the first occurrence of chr in str, if not found returns NULL
char *my_strchr(char *str, int chr) {
  while (*str != '\0') {
    if (*str == chr) {
      return str;
    }
    str++;
  }

  return NULL;
}

// Concatenates a string src on a string dest
char *my_strcat(char *dest, const char *src) {
  char *output = dest;

  while (*dest) {
    dest++;
  }

  while (*src) {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';

  return output;
}

// Allocates memory for a copy of str, copies, and returns a pointer to the copy
char *my_strdup(const char *str) {
  if (str == NULL) {
    return NULL;
  }

  size_t len = my_strlen(str) + 1;
  char *copy = (char *)malloc(len);

  if (copy != NULL) {
    my_strncpy(copy, str, len);
  }

  return copy;
}

// Computes the string array index of the first character of str not in charset
size_t my_strspn(const char *str, const char *charset) {
  size_t count = 0;

  while (*str) {
    int found = 0;
    for (const char *a = charset; *a; a++) {
      if (*str == *a) {
        found = 1;
        break;
      }
    }

    if (found) {
      count++;
    } else {
      break;
    }

    str++;
  }

  return count;
}

// Computes the string array index of the first character of str in charset
size_t my_strcspn(const char *str, const char *charset) {
  size_t count = 0;

  while (*str) {
    int found = 0;
    for (const char *r = charset; *r; r++) {
      if (*str == *r) {
        found = 1;
        break;
      }
    }

    if (!found) {
      count++;
    } else {
      break;
    }

    str++;
  }

  return count;
}

// Isolates tokens in str separated by delim. Quoted string are treated as a single token. saveptr maintains the state
// of the string across multiple function calls
char *my_strtok(char *str, const char *delim, char **saveptr) {
  if (str != NULL) {
    *saveptr = str;
  }

  if (*saveptr == NULL || **saveptr == '\0') {
    return NULL;
  }

  *saveptr += my_strspn(*saveptr, delim);
  if (**saveptr == '\0') {
    return NULL;
  }

  char *token = *saveptr;
  if (**saveptr == '\'' || **saveptr == '"') {
    token++;
    char quote_char = **saveptr;
    (*saveptr)++;

    char *closing_quote = my_strchr(*saveptr, quote_char);

    if (closing_quote == NULL) {
      *saveptr = NULL;
      //      fprintf(stderr, "strtok: unbalanced quotes\n");
      return NULL;
    }

    *closing_quote = '\0';
    *saveptr = closing_quote + 1;
  } else {
    *saveptr += my_strcspn(*saveptr, delim);
  }

  if (**saveptr != '\0') {
    *(*saveptr) = '\0';
    (*saveptr)++;
  }

  return token;
}

// Copies a null-terminated array if strings src to dest. Memory is allocated as needed.
void copy_arr(char **src, char ***dest) {
  if (src == NULL) {
    return;
  }
  int num_items = 0;
  while (src[num_items] != NULL) {
    num_items++;
  }
  *dest = (char **)malloc((num_items + 1) * sizeof(char *));
  if (dest == NULL) {
    perror("copy_arr: malloc");
    exit(1);
  }
  for (int i = 0; i < num_items; i++) {
    (*dest)[i] = my_strdup(src[i]);
  }
  (*dest)[num_items] = NULL;
}

// Frees memory allocated for a null-terminated array of strings
void free_arr(char **arr) {
  if (arr == NULL) {
    return;
  }
  int num_items = 0;
  while (arr[num_items] != NULL) {
    num_items++;
  }
  for (int i = 0; i < num_items; i++) {
    if (arr[i] != NULL) {
      free(arr[i]);
    }
  }
  free(arr);
  arr = NULL;
}

// Composes a string with format "str1 separator str2" and stores it in dest which has enough allocated space for the
// new string
void concat_with_separator(char *dest, const char *str1, const char *separator, const char *str2) {
  my_strcpy(dest, str1);
  my_strcat(dest, separator);
  my_strcat(dest, str2);
}

// Dynamically concatenates two strings, allocating needed memory and returning the result as a new string
char *concat_dynamic_strings(const char *str1, const char *str2, const char *str3) {
  if (str1 == NULL) {
    return strdup(str2);
  }

  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  size_t delimiter_len = strlen(str3);

  char *result = (char *)malloc(len1 + delimiter_len + len2 + 1);
  if (result == NULL) {
    return NULL;  // Handle memory allocation failure
  }

  strcpy(result, str1);
  strcat(result, str3);
  strcat(result, str2);

  return result;
}

// Checks if path is executable, if so returns true
bool is_executable(const char *path) {
  struct stat st;
  if (stat(path, &st) == 0) {
    return S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR);
  }
  return false;
}
