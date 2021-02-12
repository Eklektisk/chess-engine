# POSIX-ish Makefile with extensions common to *BSD and GNU such as:
# - Usage of backticks for shell evaluation
# - Usage of ?= for defining variables when not already defined
# - Usage of += for appending to a variable

PACKAGE = chess-engine
VERSION = 1.0.2

CC        ?= cc
CFLAGS    ?= -Wall -Wno-deprecated-declarations
CFLAGS    += -std=c99 -pedantic -O2

SRC = chessengine.c lookups.c
OBJECTS = chessengine.o lookups.o

all: shared static

generate:
	-@python generator/gen.py

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

shared: $(OBJECTS)
	$(CC) $(CFLAGS) -shared $^ -o libchessengine.so

static: $(OBJECTS)
	ar rcs libchessengine.a $^

.PHONY: all clean generate debug shared static

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -vf *.o
	-@rm -vf libchessengine.so libchessengine.a

