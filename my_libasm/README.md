# Welcome to My Libasm
***

## Task
Assembly is a low-level language that uses human-readable syntax representing machine instructions that a computer can execute directly. This allows the programmer to control the execution of a program at a very basic level, potentially making the program more efficient. However, it lacks many of the features and simplicity that a high-level language has, making it a challenge to learn and use.

NASM (Netwide Assembler) is an assembly language compiler for the x86/x86-64 architecture. It is a widely used, and it is known for its portability and support for various output formats. 

The goal of this project is to read and write a collection of C functions in assembly using the NASM language.

## Description
I wrote simple implementations of the functions: `strlen`, `strchr`, `index`, `strcmp`, `strncmp`, `strcasecmp`, `memset`, `memcpy`, `memmove`, `read`, and `write` in the NASM language. These can be compiled into a dynamic library which can be linked to any project needing those functions.

Additionally, to validate the functionality of my code I wrote tests to compare the outputs and/or changes caused by my code to those of their real counterparts. A source file called `my_libasm.c` is provided in this project to showcase this work. 

The implementations have minimal input validation and attempt to replicate the behavior of their counterpars in macOS.

## Installation
To create the assembly library follow these steps:
1. Clone the repository or manually copy the project files if needed
2. Navigate to the project directory
3. Run the `make` command

This will generate an `so` file called `libasm.so` which can be used with other projects.

## Usage
To use the dynamic library, compile a source file with a command such as:
```
gcc sources.c -L. -lasm -o executable
```
Where `sources.c` is a c file that makes use of one of the function implemented in this project.

In order to run `executable` the path to the dynamic library must be added to the `LD_LIBRARY_PATH` environment variable first. To do so for the current shell session, simply run:
```
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
```
This assumes that the libasm.so file is in the current directory. With that, executing `./executable` will succeed.

Alternatively, one could run `make tests` this will compile the source file `my_libasm.c`. There is no need to add the library path to the environment with this make rule. Executing `./my_libasm` will print the outputs of different tests to the console. 

<br>

***
#### [Jose M. Espinola Lopez](https://github.com/jespinol)


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
