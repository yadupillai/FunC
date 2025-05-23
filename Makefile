CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
DEBUG_CFLAGS = -DDEBUG
LDFLAGS = 

SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=%.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

TARGET = $(BIN_DIR)/file_processor

.PHONY: all debug clean test

all: $(TARGET)

debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test: $(TARGET)
	$(TARGET) --help 