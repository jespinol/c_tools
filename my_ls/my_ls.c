////
//// Created by Jose Espinola-Lopez on 4/14/23.
////
#include "my_ls.h"

int main(int argc, char **argv) {
    int show_hidden, sort_by_modified, first_path_index, num_paths;

    if (!parse_args(argc, argv, &show_hidden, &sort_by_modified, &first_path_index)) {
        printf("Usage: ./my_ls [-at] [/path/to/directory]...\n");
        return 1;
    }
    num_paths = count_paths(argc, first_path_index);

    struct ElementInfo *paths;
    paths = malloc((num_paths + 1) * sizeof(struct ElementInfo));
    populate_paths(argv, first_path_index, &num_paths, &paths, sort_by_modified);

    int found_nondir = 0;
    for (int i = 0; i < num_paths; i++) {
        struct ElementInfo curr_path = paths[i];

        if (!found_nondir) {
            found_nondir = !curr_path.exists || !curr_path.is_dir;
        }

        // Print empty line after non dir path(s) if there are 2+ paths to dirs or for 2+ paths to dir
        if ((num_paths > 2 && ((found_nondir && curr_path.is_dir) || (curr_path.is_dir && i > 0)))) {
            printf("\n");
        }
        if (i > 0 && (num_paths == 2 && !found_nondir)) {
            printf("\n");
        }

        my_ls(curr_path, show_hidden, sort_by_modified, num_paths > 1);
    }

    free(paths);

    return 0;
}

// Prints the contents of a path or the path, if valid, otherwise print message.
void my_ls(struct ElementInfo path, int show_hidden, int sort_by_modified, int include_header) {
    DIR *dir_opened = opendir(path.name);

    if (dir_opened && path.is_dir) {
        struct ElementInfo *entries = NULL;
        int num_entries = 0;

        populate_entries(dir_opened, &num_entries, show_hidden, &entries, path.name);
        closedir(dir_opened);

        if (sort_by_modified) {
            my_quicksort(entries, 0, num_entries - 1, sizeof(struct ElementInfo), compare_modified_times);
        } else {
            my_quicksort(entries, 0, num_entries - 1, sizeof(struct ElementInfo), compare_entry_names);
        }

        if (include_header) {
            printf("%s:\n", path.name);
        }

        for (int i = 0; i < num_entries; i++) {
            printf("%s\n", entries[i].name);
        }

        free(entries);
    } else {
        if (!path.exists) {
            printf("ls: cannot access '%s': No such file or directory\n", path.name);
        } else {
            printf("%s\n", path.name); // Path to a file was provided
        }
    }
}

// Counts the number of paths in argv after flags, if any
int count_paths(int argc, int path_start) {
    int output = 0;
    for (int i = path_start; i < argc; i++) {
        output++;
    }

    return output;
}

// Sets a and/or t flag variables to 1 if used, unimplemented flags will return 0 causing program to end
// Stores the index of the first path (flags present after a non-flag arg are treated as paths, as in the real ls)
int parse_args(int argc, char **argv, int *a_flag, int *t_flag, int *index_first_path) {
    *a_flag = 0;
    *t_flag = 0;
    *index_first_path = 1;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            if (has_bad_flag(argv[i])) {
                return 0;
            } else {
                int a_found = has_flag(argv[i], 'a');
                int t_found = has_flag(argv[i], 't');
                if (a_found) {
                    *a_flag = 1;
                }
                if (t_found) {
                    *t_flag = 1;
                }
                (*index_first_path)++;
            }
        } else {
            (*index_first_path) = i;
            break;
        }
    }
    return 1;
}

// Returns 1 if flags are valid, i.e. -a -t or any combination of the two, otherwise 0
// A flag is any string of the format -[something] when it appears before another flag, non-flag string, or no string ("./")
int has_bad_flag(const char *arg) {
    arg++;
    while (*arg) {
        if (*arg != 'a' && *arg != 't') {
            return 1;
        }
        arg++;
    }

    return 0;
}

// Looks for a flag character in an argv argument
int has_flag(char *arg, char arg_char) {
    while (*arg) {
        if (*arg == arg_char) {
            return 1;
        }
        arg++;
    }

    return 0;
}

// Attempts to read the paths found in argv and store relevant information in a ElementInfo struct array
// If no path is given in argv, uses a default "./" and as a side effect increases num_paths. If 1+ path given, sorts them
void populate_paths(char **argv, int path_start, int *num_paths, struct ElementInfo **paths_ptr, int sort_by_modified) {
    int j = 0;
    for (int i = path_start; i < (path_start + *num_paths); i++, j++) {
        (*paths_ptr)[j] = create_element_struct(argv[i]);
    }

    if (*num_paths == 0) {
        (*paths_ptr)[j] = create_element_struct("./");
        (*num_paths)++;
    }

    if (j > 1) {
        if (sort_by_modified) {
            my_quicksort(*paths_ptr, 0, j - 1, sizeof(struct ElementInfo), compare_path_modified_times);
        } else {
            my_quicksort(*paths_ptr, 0, j - 1, sizeof(struct ElementInfo), compare_path_names);
        }
    }
}

// Returns an ElementInfo struct with populated members
struct ElementInfo create_element_struct(char *element_name) {
    struct ElementInfo path;
    my_strcpy(path.name, element_name);
    path.exists = 0, path.is_dir = 0, path.modified_time_sec = 0, path.modified_time_nsec = 0;

    struct stat attr;
    int path_attr = get_element_stat(path.name, &attr);
    path.exists = path_attr == 0;
    if (path.exists) {
        set_time_since_modified(&attr, &path.modified_time_sec, &path.modified_time_nsec);
        path.is_dir = S_ISDIR(attr.st_mode) != 0;
    }

    return path;
}

// Stores sec and nsec since last modification time to members of a struct
void set_time_since_modified(struct stat *attr, long *entry_sec_ptr, long *entry_nsec_ptr) {
// This might be considered a multiline macro/macro with logic, but I'm using it for convenience (no st_mtim in macOS 13)
// If that's an issue don't fail PR, instead use the fully functional alternative code below
#ifdef __APPLE__ // macOS
    *entry_sec_ptr = attr->st_mtimespec.tv_sec;
    printf("%ld \n",attr->st_mtimespec.tv_sec);
    *entry_nsec_ptr = attr->st_mtimespec.tv_nsec;
    printf("%ld \n",attr->st_mtimespec.tv_nsec);
#else // Linux
    *entry_sec_ptr = attr->st_mtim.tv_sec;
    *entry_nsec_ptr = attr->st_mtim.tv_nsec;
#endif

// Alternative to macro above. Uncomment the two lines that correspond to your environment
    // for macOS
//    *entry_sec_ptr = attr->st_mtimespec.tv_sec;
//    *entry_nsec_ptr = attr->st_mtimespec.tv_nsec;;
    // for Linux
//    *entry_sec_ptr = attr->st_mtim.tv_sec;
//    *entry_nsec_ptr = attr->st_mtim.tv_nsec;
}

// Calls stat on a directory or file and outputs the results to attr pointer
// Returns 1 upon success or 0 if the element does not exist
int get_element_stat(char *path, struct stat *attr) {
    int path_attr = 0;
    int path_attr_symlink = lstat(path, attr);
    int is_symlink = S_ISLNK(attr->st_mode);

    if (!path_attr_symlink && !is_symlink) {
        path_attr = stat(path, attr);
    }

    return path_attr || path_attr_symlink;
}

// Attempts to read the contents of a directory and store relevant information in a EntryInfo struct array
void populate_entries(DIR *dir, int *num_entries, int show_hidden, struct ElementInfo **entries, char *path) {
    struct dirent *dir_entry;
    while ((dir_entry = readdir(dir)) != NULL) {
        char *entry_name = dir_entry->d_name;
        if (entry_name[0]) {
            int entry_is_hidden = entry_name[0] == '.';
            if (!entry_is_hidden || (entry_is_hidden && show_hidden)) {
                struct ElementInfo entry;
                my_strcpy(entry.name, entry_name);

                struct stat attr;
                char *path_to_entry = get_full_path(path, entry.name);
                get_element_stat(path_to_entry, &attr);
                set_time_since_modified(&attr, &entry.modified_time_sec, &entry.modified_time_nsec);

                free(path_to_entry);

                (*entries) = realloc((*entries), ((*num_entries) + 1) * sizeof(struct ElementInfo));
                (*entries)[(*num_entries)++] = entry;
            }
        }
    }
}

// Returns a string containing the full path of a file
char *get_full_path(char *path, char *file) {
    int path_len = my_strlen(path);
    int file_len = my_strlen(file);

    // +1 for '/' + 1 for '\0'
    char *full_path = (char *) malloc((path_len + file_len + 2) * sizeof(char));

    my_strcpy(full_path, path);
    my_strcat(full_path, "/");
    my_strcat(full_path, file);

    return full_path;
}

// Quicksort implementation
void my_quicksort(void *entries, int start, int end, int size, int (*comparison)(void *, void *)) {
    if (start < end) {
        int pivot = partition(entries, start, end, size, comparison);
        my_quicksort(entries, start, pivot - 1, size, comparison);
        my_quicksort(entries, pivot + 1, end, size, comparison);
    }
}

// Compares two paths from argv by last modified time. Returns 1 if path2 comes before path1, otherwise -1
// Sort them by category (1st: does not exist; 2nd: regular file, 3rd: directory), if same sort by last modified time
int compare_path_modified_times(void *a, void *b) {
    struct ElementInfo *path1 = (struct ElementInfo *) a;
    struct ElementInfo *path2 = (struct ElementInfo *) b;

    int exists_compare = path1->exists - path2->exists;
    int is_dir_compare = path1->is_dir - path2->is_dir;
    if (exists_compare) {
        return exists_compare;
    } else if (is_dir_compare) {
        return is_dir_compare;
    } else {
        return compare_modified_times(a, b);
    }
}

// Compares two paths from argv by name. Returns 1 if path2 comes before path1, otherwise -1
// Sort them by category (1st: does not exist; 2nd: regular file, 3rd: directory), if same sort lexicographically
int compare_path_names(void *a, void *b) {
    struct ElementInfo *path1 = (struct ElementInfo *) a;
    struct ElementInfo *path2 = (struct ElementInfo *) b;

    int exists_compare = path1->exists - path2->exists;
    int is_dir_compare = path1->is_dir - path2->is_dir;
    if (exists_compare) {
        return exists_compare;
    } else if (is_dir_compare) {
        return is_dir_compare;
    } else {
        return compare_strings(path1->name, path2->name);
    }
}

// Compares two entries or paths by last modified time. Returns 1 if element2 comes before element1, otherwise -1
// If they have the exact same modification time, sort them lexicographically
int compare_modified_times(void *a, void *b) {
    struct ElementInfo *element1 = (struct ElementInfo *) a;
    struct ElementInfo *element2 = (struct ElementInfo *) b;

    long diff_sec = element2->modified_time_sec - element1->modified_time_sec;
    long diff_nsec = element2->modified_time_nsec - element1->modified_time_nsec;

    if (diff_sec < 0) {
        return -1;
    } else if (diff_sec == 0) {
        return diff_nsec > 0 ? 1 : diff_nsec < 0 ? -1 : compare_entry_names(a, b);
    }

    return 1;
}

// Compares two directory entries by name. Returns output from compare_strings, e.g. 1 if element2 comes before element1
int compare_entry_names(void *a, void *b) {
    struct ElementInfo *element1 = (struct ElementInfo *) a;
    struct ElementInfo *element2 = (struct ElementInfo *) b;

    return compare_strings(element1->name, element2->name);
}

// Compares two strings lexicographically. Returns 1 if str2 comes before str1, otherwise -1
int compare_strings(void *a, void *b) {
    char *name1 = (char *) a;
    char *name2 = (char *) b;

    int i = 0;

    while (name1[i] != '\0' && name2[i] != '\0') {
        int diff = name2[i] - name1[i];
        if (diff != 0) {
            return diff < 0 ? 1 : -1;
        }
        i++;
    }

    if (name1[i] == '\0' && name2[i] != '\0') {
        return -1;
    }
    return 1;
}

// Partition entries from entries[start] to entries[end] using the last element as the pivot
// i tracks the position of the last smallest element found
// Loop over the partition and swap elements smaller than the pivot to the left, increment i after each swap
// After the last iteration, swap the position of the pivot with the element at index i (right of last smallest found)
// Return the index of the pivot for further partitioning
int partition(void *entries, int start, int end, int size, int (*comparison)(void *, void *)) {
    char *pivot = NULL;
    char *current = NULL;
    int i = start;

    // To compare directory names
    if (size == sizeof(struct ElementInfo)) {
        struct ElementInfo *entry_arr = (struct ElementInfo *) entries;
        pivot = (char *) &entry_arr[end];

        for (int j = start; j < end; j++) {
            current = (char *) &entry_arr[j];
            if (comparison(current, pivot) < 0) {
                swap(&entry_arr[i], &entry_arr[j], sizeof(struct ElementInfo));
                i++;
            }
        }

        swap(&entry_arr[i], &entry_arr[end], sizeof(struct ElementInfo));
    } else {
        // To compare strings, i.e. from argv
        char **entry_arr = (char **) entries;
        char **pivotPtr = (char **) (entry_arr + end);
        pivot = (char *) *pivotPtr;

        for (int j = start; j < end; j++) {
            current = (char *) *(entry_arr + j);
            if (comparison(current, pivot) < 0) {
                swap((entry_arr + i), (entry_arr + j), sizeof(char *));
                i++;
            }
        }

        swap((entry_arr + i), (entry_arr + end), sizeof(char *));
    }

    return i;
}

// Copy n bytes from src to a memory block pointed by dest
void *my_memcpy(void *dest, void *src, int n) {
    char *dest_temp = (char *) dest;
    char *src_temp = (char *) src;

    for (int i = 0; i < n; i++) {
        dest_temp[i] = src_temp[i];
    }

    return dest;
}

// Swap two elements in memory
void swap(void *a, void *b, int size) {
    char *temp = malloc(size);
    my_memcpy(temp, a, size);
    my_memcpy(a, b, size);
    my_memcpy(b, temp, size);
    free(temp);
}

// Copies a string src to a string destination (modified from Bootcamp C)
char *my_strcpy(char *dest, char *src) {
    char *output = dest;
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';

    return output;
}

// Returns the length of a string excluding null char (modified from Bootcamp C)
int my_strlen(char *str) {
    int len = 0;
    while (*str) {
        len++;
        str++;
    }

    return len;
}

// Concatenate a string src on a string dest
char *my_strcat(char *dest, char *src) {
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