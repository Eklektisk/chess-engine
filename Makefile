CC = gcc
CFLAGS = -std=c99
TARGET = libchessengine
SRC = chessengine.c lookups.c
OBJECTS = $(SRC:%.c=%.o)

all: $(TARGET).so

generate:
	-@python generator/gen.py

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(TARGET).so: $(OBJECTS)
	$(CC) $(CFLAGS) -shared $^ -o $(TARGET).so

.PHONY: all clean generate debug

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -vf *.o
	-@rm -vf $(TARGET)
