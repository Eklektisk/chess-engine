CC = gcc
CFLAGS = -std=c99
TARGET = libchessengine
SRC = chessengine.c lookups.c
OBJECTS = $(SRC:%.c=%.o)

all: shared static

generate:
	-@python generator/gen.py

%.o: %.c
	$(CC) $(CFLAGS) -Wall -fPIC -c $< -o $@

shared: $(OBJECTS)
	$(CC) $(CFLAGS) -Wall -shared $^ -o $(TARGET).so

static: $(OBJECTS)
	ar rcs $(TARGET).a $^

.PHONY: all clean generate debug shared static

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -vf *.o
	-@rm -vf $(TARGET).so $(TARGET).a
