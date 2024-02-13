CC = gcc
CFLAGS = -Wall -Wextra -Werror
#CFLAGS += -fsanitize=address -g3

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
#    CFLAGS += -fsanitize=leak
endif

SRC_DIR = src
HEADERS_DIR = include
OBJ_DIR = obj
LIB_DIR = lib

# Source and Header files for the library
SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HEADERS_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

STATIC_LIB = $(LIB_DIR)/liballoc.a

# Test files
TESTS_DIR = test
TESTS_SOURCES = $(wildcard $(TESTS_DIR)/*.c)
TESTS_HEADERS = $(wildcard $(TESTS_DIR)/*.h)
TESTS_OBJECTS = $(patsubst $(TESTS_DIR)/%.c, $(OBJ_DIR)/test_%.o, $(TESTS_SOURCES))

INCLUDES = -I$(HEADERS_DIR) -I$(TESTS_DIR)

TEST_EXECUTABLE = my_malloc

.PHONY: all lib clean fclean

all: $(TEST_EXECUTABLE)

lib: $(STATIC_LIB)

$(TEST_EXECUTABLE): $(TESTS_OBJECTS) $(STATIC_LIB)
	$(CC) $(CFLAGS) $(TESTS_OBJECTS) -L$(LIB_DIR) -lalloc -o $@

$(OBJ_DIR)/test_%.o: $(TESTS_DIR)/%.c $(TESTS_HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(STATIC_LIB): $(OBJECTS)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(STATIC_LIB) $(TEST_EXECUTABLE)
	rm -rf $(LIB_DIR)

-include $(DEPS)
