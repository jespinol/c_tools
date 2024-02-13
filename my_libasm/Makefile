CC = gcc
CFLAGS = -Wall -Wextra -Werror
# CFLAGS += -fsanitize=address -g3
# CFLAGS += -arch x86_64  # macOS
ASM_COMP = nasm
ASM_FLAGS = -fmacho64

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    ASM_FLAGS = -felf64
    # CFLAGS += -fsanitize=leak
endif

SRC_DIR = src
ASM_DIR = asm
HEADERS_DIR = include
OBJ_DIR = obj

ASM_SOURCES = $(wildcard $(ASM_DIR)/*.S)
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HEADERS_DIR)/*.h)
ASM_OBJECTS = $(patsubst $(ASM_DIR)/%.S, $(OBJ_DIR)/%.o, $(ASM_SOURCES))
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

TARGET = libasm.so
EXECUTABLE = my_libasm

DEP_FILES = $(OBJECTS:.o=.d)

.PHONY: clean fclean all tests

all: $(TARGET)

tests: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

$(TARGET): $(ASM_OBJECTS) | $(OBJ_DIR)
	$(CC) -shared $(ASM_OBJECTS) $(CFLAGS) -o $@
	@echo "\n************\nRun the following command in your shell to add libasm.so to LD_LIBRARY_PATH:"
	@echo "\texport LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH\n************\n"

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.S | $(OBJ_DIR)
	$(ASM_COMP) $(ASM_FLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -c $< -I$(HEADERS_DIR) -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) -f $(TARGET) $(EXECUTABLE)

re: fclean all