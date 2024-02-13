//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#include "../include/my_zsh.h"

#include "../include/builtin.h"
#include "../include/execute.h"
#include "../include/parse.h"

void my_zsh(char **envp) {
  char **env;
  copy_arr(envp, &env);
  char *line = NULL;
  size_t len = 0;
  ssize_t num_chars_read;

  int exit_status = 0;  // exit/quit will return an exit_status of -1

  while (exit_status != -1) {
    printf("[%s]>", my_getenv(PWD, env));

    num_chars_read = getline(&line, &len, stdin);

    if (num_chars_read == -1) {
      if (feof(stdin)) {
        break;
      }
    }

    struct CommandData cmd;
    parse_input(num_chars_read, line, &cmd);
    exit_status = execute(&cmd, &env);
    free_command(&cmd);
  }

  free(line);
  free_arr(env);
}
