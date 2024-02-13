# Welcome to My Zsh

***

## Task

A shell interpreter is a fundamental component of Unix-like operating systems and is responsible for parsing and executing user commands. To gain a deeper understanding of how a shell interpreter works under the hood, we created a basic clone of Zsh and learned about concepts such as command parsing and process execution.

## Description

We implemented a basic shell interpreter, my_zsh, in C. The project has the following components:
- CLI interface: the shell provides a simple prompt and enters a loop where it continuously accepts user input, parses commands, and executes them.
- Parsing input: parses input commands from the standard input. This module tokenizes user input and populates a CommandData structure with the command name and its arguments.
- Built-in commands: we implemented a number of shell built-in commands. If the user enters a built-in command, our shell executes the corresponding function directly.
- Process execution: for non-built-in commands, we fork a new process and use the execve system call to execute external commands. We handle the child process's environment, arguments, and execution of the command.

## Installation

To build and compile my_zsh, follow these steps:
1. Clone the repository or manually copy the project files if needed
2. Navigate to the project directory
3. Compile with the `make` command

This will generate the `my_zsh` executable in the current directory.


## Usage

The shell can be stated with the command
```
./my_zsh
```
This will print a prompt where the user can type commands
```
[/path/to/working/dir]> my_command
```
After execution of the command, the prompt will print once more ready fo the next command.

To exit the shell run the command `exit` or `quit`

### Built-in commands
The following commands are custom implementations:
- `cd`: changes the working directory of the current shell to a specified path, if valid, and updates the previous working directory if necessary. The `-` and `~` operands have been implemented.
- `pwd`: prints the name of the current working directory.
- `echo`: prints its arguments to the standard output. A string preceded by `$` is interpreted as a request for the value of an environment variable, which will be printed if it exists.
- `which`: prints the full path to the executable of each of its arguments on the standard output if it exists and is executable.
- `env`: for the command `env [NAME=VALUE]... [COMMAND] [ARGUMENT]...`, it sets each `NAME` to `VALUE` in the environment and runs `COMMAND`. After execution, the environment is restored to its previous state. If no command is supplied, the environment is printed to the standard output.
- `setenv`: for the command `setenv [NAME=VALUE]...`, it adds each variable `NAME` with value `VALUE` to the environment if it does not exist or overwrites the previous value if it does. The variable persists throughout the session. The `$` operand will attempt to append or prepend the value of an existing environment variable to the value being added to `NAME`.
- `unsetenv`: deletes the variable `NAME` from the environment.

<br>

This is a simplified shell for educational purposes, and it lacks some advanced features such as piping, redirection, and other advanced features.

<br>

***

#### [Jose M. Espinola Lopez](https://github.com/jespinol) and [Alan Chang]()

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo'
src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
