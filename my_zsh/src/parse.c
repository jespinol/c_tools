//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#include "../include/parse.h"

// Parses the input string from stdin and populates a CommandData command appropriately
void parse_input(ssize_t read, char *line, struct CommandData *command) {
  line[read - 1] = '\0';

  init_command_struct(command);

  char *token;
  char *delimiter = " \t";
  char *saveptr;

  token = my_strtok(line, delimiter, &saveptr);
  command->input_cmd = my_strdup(token);

  while ((token = my_strtok(NULL, delimiter, &saveptr)) != NULL) {
    append_argument(command, token);
  }
}

// Initializes CommandData to starting values
void init_command_struct(struct CommandData *command) {
  command->input_cmd = NULL;
  command->num_args = 0;
  command->input_args = NULL;
  command->argv = NULL;
}

// Allocates memory and appends an string arg to a char* input_args member in CommandData
void append_argument(struct CommandData *command, const char *arg) {
  if (command == NULL) {
    return;
  }

  char *new_arg = my_strdup(arg);
  if (new_arg == NULL) {
    perror("my_zsh: parse: malloc error");
    return;
  }

  char **new_arguments = (char **)realloc(command->input_args, (command->num_args + 2) * sizeof(char *));
  if (new_arguments == NULL) {
    perror("my_zsh: parse: malloc error");
    free(new_arg);
    return;
  }

  command->input_args = new_arguments;
  command->input_args[command->num_args] = new_arg;
  command->num_args++;
  command->input_args[command->num_args] = NULL;
}

// Frees resources allocated for a command "object"
void free_command(struct CommandData *command) {
  if (command == NULL) {
    return;
  }

  free(command->input_cmd);

  if (command->input_args != NULL) {
    for (size_t i = 0; command->input_args[i] != NULL; i++) {
      free(command->input_args[i]);
      command->input_args[i] = NULL;
    }
    free(command->input_args);
    command->input_args = NULL;
  }

  if (command->argv != NULL) {
    for (size_t i = 0; command->argv[i] != NULL; i++) {
      free(command->argv[i]);
      command->argv[i] = NULL;
    }
    free(command->argv);
    command->argv = NULL;
  }
}
