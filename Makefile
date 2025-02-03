# Compiler
CC = gcc

# Directories
SRC_DIR = src
BUILD_DIR = build
DEBUG_DIR = debug

# Output executable
OUTPUT = $(BUILD_DIR)/fb

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Default target
all: clean build debug

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR) $(DEBUG_DIR)

# Compile the source code
build: 
	mkdir -p $(BUILD_DIR)
	$(CC) $(SRCS) -o $(OUTPUT)

# Copy the build to debug folder
debug: build
	mkdir -p $(DEBUG_DIR)
	@if [ -n "$$(ls -A $(BUILD_DIR) 2>/dev/null)" ]; then cp -r $(BUILD_DIR)/* $(DEBUG_DIR)/; fi

.PHONY: all clean build debug
