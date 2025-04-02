# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lrdkafka -ljansson

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
INSTALL_DIR = /usr/local

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Header files
INCLUDES = -I$(INC_DIR)

# Target executable
TARGET = $(BIN_DIR)/ndpi_kafka

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(SRC_DIR)
	@mkdir -p $(INC_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Install the program
install: all
	install -d $(INSTALL_DIR)/bin
	install -m 755 $(TARGET) $(INSTALL_DIR)/bin
	@echo "Installing configuration files..."
	install -d $(INSTALL_DIR)/etc/ndpi_kafka
	install -m 644 config.json $(INSTALL_DIR)/etc/ndpi_kafka/


# Clean build files
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)


# Check dependencies
check-deps:
	@echo "Checking for required libraries..."
	@ldconfig -p | grep librdkafka >/dev/null || echo "Error: librdkafka not found"
	@ldconfig -p | grep libjansson >/dev/null || echo "Error: libjansson not found"


# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  install    - Install the program and configuration files"
	@echo "  clean      - Remove build files"
	@echo "  check-deps - Check for required dependencies"
	@echo "  help       - Show this help message"

.PHONY: all directories install  check-deps  help