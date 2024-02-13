CC = gcc
CFLAGS = -Wall -Wextra -Werror
#CFLAGS += -g3 -fsanitize=address
SOURCES = my_printf.c my_printf.h helper_functions.c reimplemented_functions.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = my_printf

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o my_printf $(OBJECTS)

clean:
	rm -f *.o

fclean: clean
	rm -f my_printf

re: fclean all