//
// Created by Jose Espinola-Lopez on 10/14/23.
//

#include "../include/builtin.h"

#include "../include/execute.h"
#include "../include/parse.h"

// Returns a function pointer if the input command is a builtin function, or NULL if not builtin
BuiltinFunction is_builtin(char *command) {
  static struct BuiltinCommand builtins[] = {{"cd", my_cd},       {"echo", my_echo},     {"env", my_env},
                                             {"exit", my_exit},   {"quit", my_exit},     {"pwd", my_pwd},
                                             {"which", my_which}, {"setenv", my_setenv}, {"unsetenv", my_unsetenv}};

  for (int i = 0; i < BUILTIN_NUM; ++i) {
    if (my_strcmp(command, builtins[i].name) == 0) {
      return builtins[i].BuiltinFunction;
    }
  }

  return NULL;
}

// Changes the current working directory of the shell to a path argument. If more than one argument is provided prints
// an error. If no argument is provided, changes the directory to the HOME environment variable
int my_cd(struct CommandData *cmd, char ***env) {
  if (cmd->num_args > 1) {
    fprintf(stderr, "my_cd: usage: too many arguments\n");
    return 1;
  }

  char *curr_pwd = my_getenv(PWD, *env);
  char *path_to_use;

  if (cmd->num_args == 0 || my_strcmp(cmd->input_args[0], "~") == 0) {
    path_to_use = my_getenv(HOME, *env);
  } else {
    path_to_use = cmd->input_args[0];
  }

  if (my_strcmp(path_to_use, "-") == 0) {
    path_to_use = my_getenv(OLDPWD, *env);
    printf("%s\n", path_to_use);
  }

  char *new_pwd = NULL;
  if (chdir(path_to_use) == 0) {
    new_pwd = getcwd(NULL, 0);
  }

  if (new_pwd != NULL) {
    env_setter(OLDPWD, curr_pwd, env);
    env_setter(PWD, new_pwd, env);
    free(new_pwd);
    return 0;
  }

  perror("my_cd");
  return 1;
}

// Echoes a string argument to the standard output
int my_echo(struct CommandData *cmd, char ***env) {
  for (int i = 0; i < cmd->num_args; ++i) {
    if (i > 0) {
      printf(" ");
    }

    char *arg = cmd->input_args[i];
    if (arg[0] == '$') {
      char *var_value = my_getenv(arg + 1, *env);
      if (var_value != NULL) {
        printf("%s", var_value);
      }
    } else {
      printf("%s", arg);
    }
  }
  printf("\n");
  return 0;
}

// Prints the full working directory to the standard output
int my_pwd(struct CommandData *cmd, char ***env) {
  (void)env;
  if (cmd->num_args > 1) {
    fprintf(stderr, "my_pwd: usage: too many arguments\n");
    return 1;
  }
  char current_directory[PATH_MAX];
  if (getcwd(current_directory, sizeof(current_directory)) != NULL) {
    printf("%s\n", current_directory);
    return 0;
  } else {
    perror("my_pwd");
    return 1;
  }
}

// Prints the full path to the executable for each argument provided, if valid
int my_which(struct CommandData *cmd, char ***env) {
  for (int i = 0; i < cmd->num_args; ++i) {
    char *curr_command = cmd->input_args[i];
    if (is_builtin(curr_command) != NULL) {
      printf("%s: shell built-in input_cmd\n", curr_command);
    } else {
      if (my_strchr(curr_command, '/') != NULL) {
        if (is_executable(curr_command)) {
          printf("%s\n", curr_command);
        } else {
          printf("%s not found\n", curr_command);
        }
      } else {
        char *command_path = get_path_from_PATH(curr_command, *env);
        if (command_path != NULL) {
          printf("%s\n", command_path);
        } else {
          printf("%s not found\n", curr_command);
        }
        free(command_path);
      }
    }
  }
  return 0;
}

// For a command, checks if it exists in the paths found in the PATH environment variable. If so, returns the full path
// to the executable. Otherwise, returns NULL
char *get_path_from_PATH(char *command, char **env) {
  char *paths = my_strdup(my_getenv(PATH, env));

  if (paths != NULL) {
    char *path;
    char *delimiter = ":";
    char *saveptr;
    path = my_strtok(paths, delimiter, &saveptr);

    while (path != NULL) {
      size_t command_path_len = my_strlen(path) + 1 + my_strlen(command) + 1;
      char *command_path = (char *)malloc(command_path_len);
      concat_with_separator(command_path, path, "/", command);

      if (is_executable(command_path)) {
        free(paths);
        return command_path;
      }

      free(command_path);
      path = my_strtok(NULL, delimiter, &saveptr);
    }
  }
  free(paths);
  return NULL;
}

// Calls the system exit function to exit the shell
int my_exit(struct CommandData *cmd, char ***env) {
  (void)cmd;
  (void)env;
  printf("my_zsh: good bye!\n");
  return -1;
}

// Sets environment variables and runs a command, after which the environment is restored to its previous state
int my_env(struct CommandData *cmd, char ***env) {
  if (cmd == NULL || cmd->num_args == 0) {
    for (int i = 0; (*env)[i] != NULL; ++i) {
      printf("%s\n", (*env)[i]);
    }
    return 0;
  }

  char **old_env = NULL;
  copy_arr(*env, &old_env);

  char *name_value;
  int i;
  for (i = 0; i < cmd->num_args; ++i) {
    char *is_env_var = my_strchr(cmd->input_args[i], '=');
    if (is_env_var != NULL) {
      name_value = cmd->input_args[i];
      char *saveptr;
      char *name = my_strtok(name_value, "=", &saveptr);
      char *value = saveptr;
      env_setter(name, value, env);
    } else {
      break;
    }
  }

  if (cmd->input_args[i] == NULL) {
    my_env(NULL, env);
  } else {
    struct CommandData cmd_env;
    init_command_struct(&cmd_env);
    cmd_env.input_cmd = cmd->input_args[i++];
    while (cmd->input_args[i] != NULL) {
      append_argument(&cmd_env, cmd->input_args[i++]);
      cmd_env.num_args++;
    }
    execute(&cmd_env, env);
  }

  free_arr(*env);
  copy_arr(old_env, env);
  free_arr(old_env);
  return 0;
}

// Returns a string containing the value of an environment variable name, if it exists, otherwise NULL
char *my_getenv(const char *name, char **env) {
  if (name == NULL || (*env) == NULL) {
    return NULL;
  }
  size_t varname_len = strlen(name);
  for (int i = 0; (env)[i] != NULL; ++i) {
    if (my_strncmp(name, (env)[i], varname_len) == 0 && (env)[i][varname_len] == '=') {
      return &(env)[i][varname_len + 1];
    }
  }

  return NULL;
}

// Takes an input command and, if valid, calls env_setter() with appropriate arguments parsed from the input command
int my_setenv(struct CommandData *cmd, char ***env) {
  if (cmd == NULL || cmd->num_args < 1) {
    fprintf(stderr, "setenv: usage: setenv <name=value>\n");
    return 1;
  }

  char *name_value;
  int i = 0;
  while (i < cmd->num_args) {
    name_value = cmd->input_args[i];
    char *saveptr;
    char *name = my_strtok(name_value, "=", &saveptr);
    char *value = saveptr;

    if (name != NULL && value != NULL) {
      if (my_strchr(value, '$') != NULL) {
        char *value_copy = my_strdup(value);
        char *delimiter = ":";
        char *saveptr_;
        char *varname_in_value = my_strtok(value_copy, delimiter, &saveptr_);
        char *populated_value = "";
        while (varname_in_value != NULL) {
          if (varname_in_value[0] == '$') {
            char *var_name = varname_in_value + 1;
            char *env_value = my_getenv(var_name, *env);
            if (env_value != NULL) {
              populated_value = concat_dynamic_strings(populated_value, env_value, delimiter);
            }
          } else {
            populated_value = concat_dynamic_strings(populated_value, varname_in_value, delimiter);
          }
          varname_in_value = my_strtok(NULL, ":", &saveptr_);
        }

        value = (populated_value[0] == ':') ? populated_value + 1 : populated_value;
        free(value_copy);
      }

      env_setter(name, value, env);
      i++;
    } else {
      fprintf(stderr, "my_setenv: usage: invalid format\n");
      return 1;
    }
  }

  return 0;
}

// Sets an environment variable name=value to the shell's environment. If name already exists, it should be replaced but
// only if overwrite == true. If it does not exist, it will be appended to environment
void env_setter(const char *name, const char *value, char ***env) {
  char *env_var = (char *)malloc(my_strlen(name) + my_strlen(value) + 2);
  concat_with_separator(env_var, name, "=", value);

  int i;
  for (i = 0; (*env)[i] != NULL; ++i) {
    if (my_strncmp((*env)[i], name, my_strlen(name)) == 0 && (*env)[i][my_strlen(name)] == '=') {
      free((*env)[i]);
      (*env)[i] = env_var;
      return;
    }
  }

  (*env) = (char **)realloc((*env), sizeof(char *) * (i + 2));
  (*env)[i] = env_var;
  (*env)[i + 1] = NULL;
}

// Removes an environment variable from the environment, if it exists
int my_unsetenv(struct CommandData *cmd, char ***env) {
  if (cmd->num_args == 0) {
    return 1;
  }

  char *name = cmd->input_args[0];
  int found_index = -1;
  int last_index = -1;

  for (int i = 0; (*env)[i] != NULL; ++i) {
    if (my_strncmp((*env)[i], name, my_strlen(name)) == 0 && (*env)[i][my_strlen(name)] == '=') {
      found_index = i;
    }
    last_index = i;
  }

  if (found_index == -1) {
    return 0;
  }

  free((*env)[found_index]);

  (*env)[found_index] = (*env)[last_index];
  (*env)[last_index] = NULL;

  return 0;
}
