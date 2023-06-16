# Makefile for change_timestamp.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -O3

# Source files
SRCS = change_timestamp.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXE = change_timestamp.x

# Default target
all: $(EXE)

# Link object files to create the executable
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to create object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(EXE)

# Phony targets
.PHONY: all clean
