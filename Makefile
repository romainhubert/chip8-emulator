CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -Wvla -g

LDFLAGS = -fsanitize=address -g -lSDL2

SRCS = src/main.c src/chip8.c src/display.c src/utils/rom_loader.c

OBJS = $(SRCS:.c=.o)

TARGET=chip8

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

test: $(TARGET)
	./$(TARGET) roms/BC_test.ch8

clean:
	$(RM) $(OBJS) $(TARGET)
