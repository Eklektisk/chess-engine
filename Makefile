# POSIX-ish Makefile with extensions common to *BSD and GNU such as:
# - Usage of backticks for shell evaluation
# - Usage of ?= for defining variables when not already defined
# - Usage of += for appending to a variable

PACKAGE = chess-engine
VERSION = 2-1.0.0

CC        ?= cc
CFLAGS    ?= -Wall -Wno-deprecated-declarations
CFLAGS    += -std=c99 \
			 -pedantic \
			 -Ofast \
			 -march=native \
			 -fuse-linker-plugin \
			 -flto

GENDIR = generators
SRCDIR = src
OBJDIR = build
INCDIR = include
LIBDIR = lib
XMPDIR = examples
BINDIR = bin

INSTALLDIR ?= /usr/local

.PHONY: all install uninstall debug callgrind clean mostlyclean

all: $(BINDIR)/perft

install: \
	$(LIBDIR)/libchessengine.a $(INCDIR)/ChessEngine.h \
	$(INCDIR)/ChessHistory.h
	-@mkdir -pv $(INSTALLDIR)/$(LIBDIR)
	-@mkdir -pv $(INSTALLDIR)/$(INCDIR)
	cp $(LIBDIR)/libchessengine.a $(INSTALLDIR)/$(LIBDIR)/
	cp $(INCDIR)/ChessEngine.h $(INSTALLDIR)/$(INCDIR)/
	cp $(INCDIR)/ChessHistory.h $(INSTALLDIR)/$(INCDIR)/

uninstall:
	-rm $(INSTALLDIR)/$(LIBDIR)/libchessengine.a \
		$(INSTALLDIR)/$(INCDIR)/ChessEngine.h \
		$(INSTALLDIR)/$(INCDIR)/ChessHistory.h

$(BINDIR)/perft: $(OBJDIR)/perft.o $(LIBDIR)/libchessengine.a
	@mkdir -pv $(BINDIR)
	$(CC) $(CFLAGS) -static $< -L$(LIBDIR) -lchessengine -o $@

$(OBJDIR)/perft.o: $(XMPDIR)/perft.c $(INCDIR)/ChessEngine.h \
	$(INCDIR)/ChessHistory.h
	@mkdir -pv $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(LIBDIR)/libchessengine.a: \
	$(OBJDIR)/chessengine.o $(OBJDIR)/chesshistory.o \
	$(OBJDIR)/moveinfo.o $(OBJDIR)/utility.o
	@mkdir -pv $(LIBDIR)
	ar rcs $@ $^

$(OBJDIR)/chessengine.o: \
	$(SRCDIR)/chessengine.c $(SRCDIR)/MoveInfo.h $(SRCDIR)/Utility.h \
	$(INCDIR)/ChessEngine.h
	@mkdir -pv $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR)/chesshistory.o: \
	$(SRCDIR)/chesshistory.c $(SRCDIR)/MoveInfo.h $(SRCDIR)/Utility.h \
	$(INCDIR)/ChessEngine.h $(INCDIR)/ChessHistory.h 
	@mkdir -pv $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR)/moveinfo.o: $(SRCDIR)/moveinfo.c $(SRCDIR)/MoveInfo.h
	@mkdir -pv $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR)/utility.o: $(SRCDIR)/utility.c $(SRCDIR)/MoveInfo.h \
	$(SRCDIR)/Utility.h $(INCDIR)/ChessEngine.h
	@mkdir -pv $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(SRCDIR)/moveinfo.c: $(GENDIR)/moves.py
	python $< > $@

debug: CFLAGS += -g -DDEBUG
debug: all

callgrind: $(BINDIR)/perft
	valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes \
		--cache-sim=yes --branch-sim=yes $<

mostlyclean:
	-@rm -vf $(OBJDIR)/*
	-@rm -vf $(LIBDIR)/*
	-@rm -vf $(BINDIR)/*

clean: mostlyclean
clean:
	-@rm -vf $(SRCDIR)/moveinfo.c

