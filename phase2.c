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
    elf_t elf1,elf2, elf3;

    /* Existence et ouverture du fichier */
    char *filename1;
    char *filename2;
    char *filename3;

    filename1 = malloc(strlen(argv[1]) *  sizeof(char));
    strcpy(filename1, argv[1]);
    filename2 = malloc(strlen(argv[2]) *  sizeof(char));
    strcpy(filename2, argv[2]);
    filename3 = malloc(strlen(argv[3]) *  sizeof(char));
    strcpy(filename3, argv[3]);


    elf1.file = fopen(filename1, "rb");
    elf2.file = fopen(filename2, "rb");
    elf3.file = fopen(filename3, "wb+");


    /* Lecture des ELF header */
    int nbc = fread(&(elf1.header), 1, sizeof(elf1.header), elf1.file);
    if(nbc != 1){
        if(feof(elf1.file)){
            /* End of file */
        }else{
            debug("Erreur de lecture.");
        }
    }

    nbc = fread(&(elf2.header), 1, sizeof(elf2.header), elf2.file);
    if(nbc != 1){
        if(feof(elf2.file)){
            /* End of file */
        }else{
            debug("Erreur de lecture.");
        }
    }

    /* Vérification format ELF */
    if(!(isELF(elf1.header))){
        fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", filename1);
        return -1;
    }

    if(!(isELF(elf2.header))){
        fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", filename2);
        return -1;
    }

    /* Récupération des Section headers */
    elf1.sectionHeaders = malloc(elf1.header.e_shnum * sizeof(Elf32_Shdr));
    fseek(elf1.file, elf1.header.e_shoff, SEEK_SET);
    nbc = fread(elf1.sectionHeaders, elf1.header.e_shentsize, elf1.header.e_shnum, elf1.file);
    if(nbc != 1){
        if(feof(elf1.file)){
            /* End of file */
        }else{
            debug("Erreur de lecture.");
        }
    }

    elf2.sectionHeaders = malloc(elf2.header.e_shnum * sizeof(Elf32_Shdr));
    fseek(elf2.file, elf2.header.e_shoff, SEEK_SET);
    nbc = fread(elf2.sectionHeaders, elf2.header.e_shentsize, elf2.header.e_shnum, elf2.file);
    if(nbc != 1){
        if(feof(elf2.file)){
            /* End of file */
        }else{
            debug("Erreur de lecture.");
        }
    }

    /* Récupératuion des Section names */
    elf1.sectionNames = malloc(elf1.header.e_shnum*sizeof(char*));
    getSectionNames(&elf1);

    elf2.sectionNames = malloc(elf2.header.e_shnum*sizeof(char*));
    getSectionNames(&elf2);

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