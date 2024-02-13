CC = gcc
CFLAGS = -Wall -Wextra -Werror
# CFLAGS += -g3 -fsanitize=address # for macOS
# CFLAGS += -g3 -fsanitize=address -fsanitize=leak

SRC_DIR = src
HEADERS_DIR = include
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HEADERS_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
DEPS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.d, $(SOURCES))
TARGET = my_zsh

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) $(OBJ_DIR)/%.d | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MM -MT '$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $<)' $< -MF $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean $(TARGET)

.PHONY: clean fclean
