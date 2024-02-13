//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#include "../include/execute.h"

// Calls a builtin in function, if appropriate, otherwise creates a fork and calls functions to execute the input
// command as a child process
int execute(struct CommandData *command, char ***env) {
  if (command->input_cmd == NULL) {
    return 0;
  }

  BuiltinFunction builtin = is_builtin(command->input_cmd);
  if (builtin != NULL) {
    return builtin(command, env);
  }

  pid_t pid = fork();
  if (pid == 0) {
    build_argv(command, *env);
    execute_binary(command, env);
    exit(1);
  } else {
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      return WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      int term_signal = WTERMSIG(status);
      if (term_signal == SIGSEGV) {
        fprintf(stderr, "my_zsh: execute: segmentation fault\n");
      } else {
        fprintf(stderr, "my_zsh: execute: %s\n", strerror(term_signal));
      }
    }
  }

  return 0;
}

// Creates a string array containing the full path to the executable and pointers to its arguments
void build_argv(struct CommandData *command, char **env) {
  command->argv = (char **)malloc(sizeof(char *) * (command->num_args + 2));
  char *command_path;

  if (my_strchr(command->input_cmd, '/') == NULL) {
    command_path = get_path_from_PATH(command->input_cmd, env);
  } else {
    command_path = command->input_cmd;
  }
  int i = 0;
  command->argv[i++] = command_path;
  for (int j = 0; j < command->num_args; ++j, ++i) {
    command->argv[i] = command->input_args[j];
  }

  command->argv[i] = NULL;
}

// Checks if a command is executable, and if so calls exec passing the command path, arguments, and the current
// environment variables of my_zsh
void execute_binary(struct CommandData *command, char ***env) {
  char *command_path = command->argv[0];

  if (is_executable(command_path)) {
    int result = execve(command_path, command->argv, *env);
    if (result == -1) {
      perror("my_zsh: execve");
    }
  } else {
    fprintf(stderr, "my_zsh: execute: command not found: %s\n", command->input_cmd);
  }
}
