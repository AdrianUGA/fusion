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
#include "fusionelf.h"

void writeELF(elf_t elf1, elf_t elf2, elf_t* elf3){
    int i;

    /* Récupération des tables de symboles */
    // int tailleSymb1,tailleSymb2;

    getTableSymbole(&elf1);
    getTableSymbole(&elf2);

    printf("Récupération table symbole OK\n");

    /* Récupération des strtab*/
    int numSymSec = getSectionNumber(&elf1, ".strtab");

    int size = elf1.sectionHeaders[numSymSec].sh_size;
    char strtab[size];
    memcpy(strtab, elf1.sectionContents[numSymSec], elf1.sectionHeaders[numSymSec].sh_size);
    elf1.strtab = strtab;

    // int size2 = elf2.sectionHeaders[numSymSec].sh_size;
    char strtab2[size];
    int numSymSec2 = getSectionNumber(&elf2, ".strtab");
    memcpy(strtab2, elf1.sectionContents[numSymSec2], elf1.sectionHeaders[numSymSec2].sh_size);
    elf2.strtab = strtab2;

    char *content[elf1.header.e_shnum+elf2.header.e_shnum];
    int symb[elf1.header.e_shnum];
    int newSymtabIdx1[elf1.symboleNumber];
    int newSymtabIdx2[elf2.symboleNumber];

    printf("Récupération des strtab OK\n");

    /* Fusion des données */
    getHeader(elf1, elf2, elf3);    
    getSection(elf1, elf2, elf3, content, symb);

    //displaySectionHeaders(elf3);

    fusionTableSymbole(&elf1, &elf2, elf3, symb, newSymtabIdx1, newSymtabIdx2);

    /* Initialisation de l'offset après les section headers */
    int current_offset = sizeof(Elf32_Ehdr)+sizeof(Elf32_Shdr)*elf3->header.e_shnum;

    fusionRelocTable(&elf1, &elf2, elf3, symb, newSymtabIdx1, newSymtabIdx2);

    int k1 = getSectionNumber(elf3, ".symtab");
    int k2 = getSectionNumber(elf3, ".rel.text");
    int k3 = getSectionNumber(elf3, ".strtab");

    elf3->sectionHeaders[k1].sh_size = sizeof(Elf32_Sym)*elf3->symboleNumber;
    elf3->sectionHeaders[k2].sh_size = sizeof(Elf32_Rel)*elf3->tailleRelocTable;

    printf("Fusion des données OK\n");

    /* Ecriture */
    fseek(elf3->file, 0, SEEK_SET);
    /* Header ELF */
    fwrite(&(elf3->header), sizeof(Elf32_Ehdr), 1, elf3->file);

    printf("Ecriture Header ELF OK\n");

    /* Section headers */
    for(i = 0; i < elf3->header.e_shnum; i++){
        elf3->sectionHeaders[i].sh_offset = current_offset;
        current_offset  += elf3->sectionHeaders[i].sh_size;
        fwrite(&(elf3->sectionHeaders[i]), sizeof(Elf32_Shdr), 1, elf3->file);
    }

    printf("Ecriture Section headers OK\n");

    //printf("SN: %d  RN: %d\n", elf3->symboleNumber, elf3->tailleRelocTable);
    //printf("%s\n", content[0]);
    //printf("%s\n", content[1]);


    for(i = 0; i < elf3->header.e_shnum; i++){
        if(i != k1 && i!= k2 && i != k3){
             int k;
            for(k=0; k<elf3->sectionHeaders[i].sh_size;k++){
                //unsigned char c = content[i][k];
                //printf("%x", c);
            }
            //printf("\n");
            fwrite(content[i], elf3->sectionHeaders[i].sh_size, 1, elf3->file);
        }
        else
            if(i == k1)
                fwrite(elf3->symTable, sizeof(Elf32_Sym), elf3->symboleNumber, elf3->file);
            else if(i == k2) 
                fwrite(elf3->relTable, sizeof(Elf32_Rel), elf3->tailleRelocTable, elf3->file);
            else
                fwrite(elf3->strtab, elf3->sectionHeaders[i].sh_size, 1, elf3->file);
    }

    printf("Ecriture Section content OK\n");
}