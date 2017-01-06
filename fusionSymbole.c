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


void fusionTableSymbole(elf* elf1, elf* elf2, int taille1, int taille2){
    int i ;
    for(i=0;i<elf1.taille1;i++){
        

    }
}


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


    /* ELF header */
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

    if(!(isELF(elf1.header))){
        fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", filename1);
        return -1;
    }

    if(!(isELF(elf2.header))){
        fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", filename2);
        return -1;
    }

    /* Section headers */
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

    /* Section names */
    elf1.sectionNames = malloc(elf1.header.e_shnum*sizeof(char*));
    getSectionNames(&elf1);

    elf2.sectionNames = malloc(elf2.header.e_shnum*sizeof(char*));
    getSectionNames(&elf2);

    /* Symbol table */
    int tailleSymb1,tailleSymb2;

    elf1.symTable =  getTableSymbole(&elf1,&tailleSymb1);
    elf2.symTable =  getTableSymbole(&elf2,&tailleSymb2);

    int numSymSec = getSectionNumber(&elf1, ".strtab");
    //printf(".strtab %d de taille %d \n", numSymSec, sectionHeadersFile1[numSymSec].sh_size);

    int size = elf1.sectionHeaders[numSymSec].sh_size;
    char strtab[size];
    getSectionContent(&elf1, numSymSec, strtab);

    int i;
    printf("Affichage de la table des symboles du fichier 1 : \n");
    for(i=0;i<tailleSymb1;i++){
        printf("nom du symbole : %s\n", strtab+elf1.symTable[i].st_name);
        //displaySymbole(elf1.symTable[i], strtab, i);
    }


    numSymSec = getSectionNumber(&elf2, ".strtab");
    //printf(".strtab %d de taille %d \n", numSymSec, sectionHeadersFile1[numSymSec].sh_size);
    size = elf2.sectionHeaders[numSymSec].sh_size;
    getSectionContent(&elf2, numSymSec, strtab);
    printf("Affichage de la table des symboles du fichier 2 : \n");
    for(i=0;i<tailleSymb2;i++){
         printf("nom du symbole : %s\n", strtab+elf2.symTable[i].st_name);
    }
    elf3 = elf1;


    return 0;
}