ACLOCAL_AMFLAGS = -I m4

# no code transformations to work with thumb
AM_CFLAGS= @AM_CFLAGS@ -mno-thumb-interwork

if HAVE_ARM_COMPILER

noinst_PROGRAMS=fusion_result
# il faudra linker avec fusion_result.o
fusion_result_LDADD=fusion_result.o
# mais il faut aussi construire explicitement fusion_result.o
# (cf la règle ci-dessous)
nodist_EXTRA_fusion_result_SOURCES=fusion_result.o

if HAVE_NO_STDLIB
# avec arm-none-eabi-, on a besoin de "puts" pour générer le programme
fusion_result_SOURCES=eabi-none-lib.c
endif

# Appel du linker avec -no-stdlib car on ne fait qu'un link partiel
fusion_result.o: file1.o file2.o
	$(CC) $(AM_CFLAGS) $(AM_LDFLAGS) -nostdlib -Wl,-r -o $@ $^

endif

clean-local:
	rm -f *.o fusion_result

EXTRA_DIST=file1.c file2.c
