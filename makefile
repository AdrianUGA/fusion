BINS=main
main_O=elf.o display.o main.o debug.o
phase2_O=fusion.o elf.o display.o debug.o


CC=gcc
LD=gcc
CFLAGS=-Wall -Werror -g
INCLUDES=
LINKS=elf.o debug.o display.o

SOURCES=$(wildcard *.c)
DEP=$(patsubst %.c, %.dep, $(wildcard *.c))

-include $(DEPS)

#Not a file to look for
.PHONY: default all clean dep clear
default: all
all: clear dep $(BINS)

main: $(main_O)
phase2: $(phase2_O)

main:
	$(LD) -o main 



	# @for B in $(BINS); \
 #        do \
 #            @echo $(LD) -o $B $B.o $($B_O) || exit $$?; \
 #        done
 

	 #$(foreach B, $(BINS), $(LD) -o $B $B.o $($B_O) $(LINKS); )

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# $(BINS): $(BINS).o $(LINKS)
# 	$(LD) -o $@ $^

clean:
	rm -f *.o $(BINS) *.dep
%.dep: %.c
	gcc -MM $< > $@
dep: $(DEP)
clear:
	clear

