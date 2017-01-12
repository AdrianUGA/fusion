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

    if(!initElf(&elf1, argv[1], MODE_R)){
        return -1;
    }
    if(!initElf(&elf2, argv[2], MODE_R)){
        return -1;
    }
    if(!initElf(&elf3, argv[3], MODE_W)){
        return -1;
    }
    if(elf1.header.e_type != ET_REL){
        fprintf(stderr,"Le fichier %s n'est pas translatable\n", argv[1]);
        exit(1);
    }
    if(elf2.header.e_type != ET_REL){
        fprintf(stderr,"Le fichier %s n'est pas translatable\n", argv[2]);
        exit(1);
    }

    //fusionElf(elf1, elf2, &elf3);
    /* Ecriture du fichier de fusion au format ELF */
    writeELF(elf1, elf2, &elf3);

    fclose(elf1.file);
    fclose(elf2.file);
    fclose(elf3.file);

    return 0;
}