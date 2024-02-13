# Welcome to My Readline
***

## Task
The task is to recreate a function that reads a line from a stream. The function should be able to run in a loop to read an entire stream.

## Description
I took advantage of the file control options header functions to open a stream and then used the read function from unistd.h to read chunks of the stream defined by the global variable READLINE_READ_SIZE. The chunks are added to a temporary string until the end of the line is reached. At that point a pointer to a string up to the end of the line is returned, and any characters beyond that (corresponding to the next line) are stored for later processing.

## Installation
Simply compile with ```gcc ./my_readline.c -o my_readline```

## Usage
To run the program use
```
./my_readline [file1]
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
