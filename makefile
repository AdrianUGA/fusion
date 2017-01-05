CC=gcc
LD=gcc
CFLAGS=-Wall -Werror -g
INCLUDES=
BIN=main
LINKS=elf.o debug.o display.o


SOURCES=$(wildcard *.c)
DEP=$(patsubst %.c, %.dep, $(wildcard *.c))


-include $(DEPS)

.PHONY: default #Not a file to look for
default: all

.PHONY: all
all: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCLUDES)

$(BIN): $(BIN).o $(LINKS)
	$(LD) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(BIN) *.dep
%.dep: %.c
	gcc -MM $< > $@
dep: $(DEP)

