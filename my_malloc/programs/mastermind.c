//
// Created by Jose Espinola-Lopez
// Modified from Qwasar submission dated  10/02/2022
//

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../include/my_free.h"
#include "../include/my_malloc.h"

char* checkArgCode(int argNum, char** args, int codeLength);
void generateCode(char** code, int codeLength);
int checkArgAttempts(int argNum, char** args);
void displayWelcomeMessage(int round);
bool validateCode(char* code);
bool checkGuess(char* code, const char* guess, int codeLength);
int findIndex(char* str, char searchChar);

// searches searchChar in str. If searchChar is found returns its index, if not
// returns -1
int findIndex(char* str, char searchChar) {
  int index = 0;
  while (index < 4) {
    if (*str == searchChar) {
      return index;
    }
    str++;
    index++;
  }
  return -1;
}

// checks if -c <code> was passed as argument and checks if the code is valid.
// If so return it. if no argument was passed (or if it was not formatted
// properly) or if the code is not valid, generate a code randomly and return
// it.
char* checkArgCode(int argNum, char** args, int codeLength) {
  char* code = my_malloc((codeLength + 1) * sizeof(char));
  for (int i = 1; i < argNum - 1; i++) {
    if (strcmp(args[i], "-c") == 0) {
      char* inputCode = args[i + 1];
      if (validateCode(inputCode)) {
        strcpy(code, inputCode);
        return code;
      }
    }
  }
  printf(
      "Invalid or missing code. Set a 4 digit code with '-c' A random code "
      "will be generated.\n");
  printf("---\n");
  generateCode(&code, codeLength);
  return code;
}

// generates a 4-digit code with values between 0 and 8 randomly
void generateCode(char** code, int codeLength) {
  srand(time(NULL));
  for (int i = 0; i < codeLength; i++) {
    (*code)[i] = "012345678"[rand() % 8];
  }
  (*code)[codeLength] = '\0';
}

// checks if -t <number> was passed as argument and checks if it is greater than
// 0 (can't play 0 rounds). If so return it. if no argument is present, or if
// it's not in the right format, or if the value was invalid, return 10.
int checkArgAttempts(int argNum, char** args) {
  for (int i = 1; i < argNum - 1; i++) {
    if (strcmp(args[i], "-t") == 0) {
      int inputAttempt = atoi(args[i + 1]);
      if (inputAttempt > 0) {
        return inputAttempt;
      }
    }
  }
  printf(
      "Invalid or missing number of attempts. Set a number of attempts with "
      "'-t' Game will run for 10 rounds.\n");
  printf("---\n");
  return 10;
}

// displays lines intended for the beginning of the game
void displayWelcomeMessage(int round) {
  printf("Will you find the secret code?\n");
  printf("Please enter a valid guess\n");
  printf("---\n");
  printf("Round %d\n", round);
}

// checks that a code passed as argument or a guess is composed of digits that
// are between 0 and 8, both including
bool validateCode(char* code) {
  for (int i = 0; code[i] != '\0'; i++) {
    if (!isdigit(code[i]) || code[i] == '9') {
      return false;
    }
  }
  return true;
}

// checks if the guess matches the code. Also reports the number of well-placed
// and mismatches to the user returns false if the user has not guessed the
// code, or true if they have
bool checkGuess(char* code, const char* guess, int codeLength) {
  int placed = 0;
  int misplaced = 0;
  char* mismatches = my_malloc((codeLength + 1) * sizeof(char));
  strcpy(mismatches, code);
  mismatches[codeLength] = '\0';

  for (int i = 0; i < codeLength; i++) {
    if (code[i] == guess[i]) {
      mismatches[i] = 'x';
      placed++;
    }
  }

  for (int i = 0; i < codeLength; i++) {
    int indexOfMismatch = findIndex(mismatches, guess[i]);
    if (code[i] != guess[i] && indexOfMismatch > -1) {
      mismatches[indexOfMismatch] = 'x';
      misplaced++;
    }
  }
  if (placed == codeLength) {
    my_free(mismatches);
    return true;
  }

  printf("Well placed pieces: %d\n", placed);
  printf("Misplaced pieces: %d\n", misplaced);

  my_free(mismatches);
  return false;
}

// the program runs as follows:
// check or set a code, check or set the number of attempts at guessing the code
// read the user's guess from the stdin. If the guess is invalid ask again. If
// the user inputs ctd+d end the game for a valid guess, check how well it
// matches the code return a congratulation message if the user won, if not
// inform the user how many pieces matched/mismatched check if all the attempts
// have been consumed. If not give the user another try, if so end the game
int main(int ac, char** av) {
  int CODE_LENGTH = 4;
  char* code = checkArgCode(ac, av, CODE_LENGTH);
  int attempts = checkArgAttempts(ac, av);
  int round = 1;
  char* guess;
  guess = my_malloc((CODE_LENGTH + 1) * sizeof(char));

  displayWelcomeMessage(round);
  while (read(0, guess, CODE_LENGTH + 1)) {
    if (guess[CODE_LENGTH] == EOF) {
      break;
    }

    guess[CODE_LENGTH] = '\0';

    if (validateCode(guess)) {
      if (checkGuess(code, guess, CODE_LENGTH)) {
        printf("Congratz! You did it!\n");
        break;
      }
    } else {
      printf("Wrong input!\n");
      continue;
    }

    if (round == attempts) {
      printf("---\n");
      printf("Out of attempts\n");
      break;
    }

    round++;
    printf("---\n");
    printf("Round %d\n", round);
  }

  my_free(guess);
  my_free(code);
  return 0;
}
