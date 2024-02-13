//
// Created by Jose Espinola-Lopez on 4/14/23.
//

#ifndef MY_LS_MY_LS_H
#define MY_LS_MY_LS_H

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

// Structure to hold information for a path given in argv, or entries in a path
struct ElementInfo {
    char name[256];
    int is_dir;
    int exists;
    long modified_time_sec;
    long modified_time_nsec;
};

// ls
void my_ls(struct ElementInfo path, int show_hidden, int sort_by_modified, int include_header);

// Custom helper functions
int parse_args(int argc, char **argv, int *a_flag, int *t_flag, int *index_first_path);

int has_flag(char *arg, char arg_char);

int has_bad_flag(const char *arg);

int count_paths(int argc, int path_start);

void populate_paths(char **argv, int path_start, int *num_paths, struct ElementInfo **paths_ptr, int sort_by_modified);

void populate_entries(DIR *dir, int *num_entries, int show_hidden, struct ElementInfo **entries, char *path);

struct ElementInfo create_element_struct(char* element_name);

char *get_full_path(char *path, char *file);

int get_element_stat(char *path, struct stat *attr);

void set_time_since_modified(struct stat *attr, long *entry_sec_ptr, long *entry_nsec_ptr);

// Quicksort and its helper functions
void my_quicksort(void *entries, int start, int end, int size, int (*comparison)(void *, void *));

int compare_path_modified_times(void *a, void *b);

int compare_path_names(void *a, void *b);

int compare_modified_times(void *a, void *b);

int compare_entry_names(void *a, void *b);

int compare_strings(void *a, void *b);

int partition(void *entries, int start, int end, int size, int (*comparison)(void *, void *));

void *my_memcpy(void *dest, void *src, int n);

void swap(void *a, void *b, int size);

// reimplemented functions
char *my_strcpy(char *dest, char *src);

int my_strlen(char *str);

char *my_strcat(char *dest, char *src);

#endif //MY_LS_MY_LS_H
