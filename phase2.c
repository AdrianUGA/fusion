#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#include "display.h"
#include "debug.h"
#include "getelf.h"
#include "writeelf.h"
#include "fusionelf.h"

int main(int argc, char * argv[]){
    if(argc < 4){
        printf("To few arguments\n");
        return -1;
    }
    elf_t elf1,elf2, elf3;

    initElf(&elf1, argv[1]);
    initElf(&elf2, argv[2]);



    char *filename3;
    filename3 = malloc(strlen(argv[3]) *  sizeof(char));
    strcpy(filename3, argv[3]);
    elf3.file = fopen(filename3, "wb+");

    /* Récupération des tables de réalocation */
    getRelocTable(&elf1);
    getRelocTable(&elf2);  

    /* Ecriture du fichier de fusion au format ELF */
    writeELF(elf1, elf2, &elf3);

    fclose(elf1.file);
    fclose(elf2.file);
    fclose(elf3.file);

    return 0;
}