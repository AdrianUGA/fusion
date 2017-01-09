main_O=elf.o display.o main.o debug.o
phase2_O=fusion.o elf.o display.o debug.o phase2.o


CC=gcc
LD=gcc
CFLAGS=-Wall -Werror -g
INCLUDES=
LINKS=elf.o debug.o display.o

# BINS=$(shell grep  -Plz  'int\s*main\s*\(.*?\)\s*{' *.c | cut -d '.' -f 1)
BINS=main phase2
sources=$(wildcard *.c)
cources_o = $(firstword $(subst :, ,$1))
DEP=$(patsubst %.c, %.dep, $(wildcard *.c))

-include $(DEP)

#$(foreach B,$(BINS),$(eval $(B):$($(B)_O)))


binarize = $(1): $($(1)_O)


#Not a file to look for
.PHONY: default all clean dep clear hello
default: all 


all: clear dep main

# main: $(main_O)
#phase2: $(phase2_O)

test1=main:elf.o display.o main.o debug.o
test2=phase2:fusion.o elf.o display.o debug.o phase2.o
$(eval $(test1))
$(eval $(test2))

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
	#@echo $(foo)
	#@$(foreach module,$(MODULES),$(foreach cfile,$(SRC_$(module)),echo '[$(cfile)]';))






none:
	# @for B in $(BINS); \
	# 	do \
	# 	echo $(B); \
	# done

# foo = $(call binarize, main)



	# @for B in $(BINS); \
 #        do \
 #            @echo $(LD) -o $B $B.o $($B_O) || exit $$?; \
 #        done
 

	 #$(foreach B, $(BINS), $(LD) -o $B $B.o $($B_O) $(LINKS); )
# Obtains the OS type, either 'Darwin' (OS X) or 'Linux'
	UNAME_S:=$(shell uname -s)