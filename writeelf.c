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

    //printf("Récupération table symbole OK\n");
    
//     char *content[elf1.header.e_shnum+elf2.header.e_shnum];
    int symb[elf2.header.e_shnum];
    int newSymtabIdx1[elf1.symboleNumber];
    int newSymtabIdx2[elf2.symboleNumber];

    //printf("Récupération des strtab OK\n");
    
    

    /* Fusion des données */
    getHeader(elf1, elf2, elf3);
    printf("Récupération des sections : \n\n");
    getSection(elf1, elf2, elf3, symb);
    displaySectionHeaders(elf3);

    //displaySectionHeaders(elf3);
    printf("\nRécupération des symboles : \n\n");
    fusionTableSymbole(&elf1, &elf2, elf3, symb, newSymtabIdx1, newSymtabIdx2);
    displayTableSymbole(elf3);

    /* Initialisation de l'offset après les section headers */
    int current_offset = sizeof(Elf32_Ehdr)+sizeof(Elf32_Shdr)*elf3->header.e_shnum;

    fusionRelocTable(&elf1, &elf2, elf3, symb, newSymtabIdx1, newSymtabIdx2);

    int k1 = getSectionNumber(elf3, ".symtab");
    //int k2 = getSectionNumber(elf3, ".rel.text");
    int k3 = getSectionNumber(elf3, ".strtab");

    //elf3->sectionHeaders[k1].sh_size = sizeof(Elf32_Sym)*elf3->symboleNumber;
    //elf3->sectionHeaders[k2].sh_size = sizeof(Elf32_Rel)*elf3->tailleRelocTable;

    //printf("Fusion des données OK\n");

    /* Ecriture */
    fseek(elf3->file, 0, SEEK_SET);
    /* Header ELF */
    fwrite(&(elf3->header), sizeof(Elf32_Ehdr), 1, elf3->file);

    //printf("Ecriture Header ELF OK\n");

    /* Section headers */
    for(i = 0; i < elf3->header.e_shnum; i++){
        elf3->sectionHeaders[i].sh_offset = current_offset;
        current_offset  += elf3->sectionHeaders[i].sh_size;
        fwrite(&(elf3->sectionHeaders[i]), sizeof(Elf32_Shdr), 1, elf3->file);
    }

    int k;
    for(i = 0; i < elf3->header.e_shnum; i++){
        if(i != k1 && i != k3){
             
            if(elf3->sectionHeaders[i].sh_type != SHT_REL){
                fwrite(elf3->sectionContents[i], elf3->sectionHeaders[i].sh_size, 1, elf3->file);
            }
            else{
                for(k = 0; k<elf3->nbRelTable;k++){
                    if(strcmp(elf3->relocTables[k].name, elf3->sectionNames[i])==0){
                        fwrite(elf3->relocTables[k].relTable, sizeof(Elf32_Rel), elf3->relocTables[k].tailleRelocTable, elf3->file);
                    }                        
                }
            }
        }
        else
            if(i == k1)
                fwrite(elf3->symTable, sizeof(Elf32_Sym), elf3->symboleNumber, elf3->file);
            else
                fwrite(elf3->strtab, elf3->sectionHeaders[i].sh_size, 1, elf3->file);
    }
}
