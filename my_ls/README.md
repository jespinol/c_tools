# Welcome to My Ls

***

## Task
The task is to create a program that provides a basic functionality similar to the `ls` command found in Unix-like
operating systems. It lists the contents in directories (optionally showing hidden files or sorting the output by modified
time).

## Description
The project has three main parts:
1. A 'main' function: it serves as an entry point to the program, it calls my_ls with the paths and flags specified, if
   any.
2. The 'my_ls' function: it's the heart of the project. It lists files found in a directory, optionally showing hidden
   files and/or sorting entries by modified time.
3. Helper functions: includes reimplementation of functions not explicitly allowed by the assignment
   and an implementation of a quicksort algorithm to sort directory entries by different criteria.

## Installation
To install the program, navigate with a terminal to the directory containing the code and run:
```
make
make clean
```
This will generate a file named 'my_ls'
There are no external dependencies.

## Usage
To run the program, specify any of the two optional flags available and one or more directories:

```
./my_ls [-at] [/path/to/directory]...
```

### The Core Team

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo'
src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
