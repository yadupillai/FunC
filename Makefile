CC = gcc
CFLAGS = -Wall -Wextra -std=c99
DEBUG_CFLAGS = -DDEBUG
LDFLAGS = 

SRC = main.c cli.c compress.c encrypt.c io.c search.c sort.c
OBJ = $(SRC:.c=.o)
HEADERS = cli.h compress.h encrypt.h io.h search.h sort.h

TARGET = file_processor

.PHONY: all debug clean test

all: $(TARGET)

debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test: $(TARGET)
	./$(TARGET) --help 