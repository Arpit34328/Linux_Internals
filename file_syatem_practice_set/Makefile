CC = gcc
CFLAGS = -Wall -g -I./src/include

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Output Binary
TARGET = $(BIN_DIR)/main

# Find all .c files in src directory and its subdirectories
SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Default target to build everything
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(OBJ_FILES) -o $(TARGET) -lm

# Rule to compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	@./$(TARGET)

# Clean up object files and binaries
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Rebuild everything from scartch 
rebuild: clean all

# Declare phony targets (they arent't files)
.PHONY: all clean rebuild run
