#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#include "display.h"
#include "getelf.h"
#include "debug.h"

char isNumber(char *str){
	int i;
	for(i=0; i<strlen(str); i++){
		if(!isdigit(str[i]))
			return 0;
	}
	return 1;
}

void readElf(elf_t *elf, int offset, int size, void *buffer){
	buffer = memcpy(buffer, elf->fileContent + offset, size);
}

int getSectionNumber(elf_t *elf, char *name){
	int i;
	for(i=0; i<elf->header.e_shnum; i++){
		if(strcmp(elf->sectionNames[i], name) == 0)
			return i;
	}
	return -1;
}

int isElf(elf_t *elf){
	return (elf->header.e_ident[0]== 0x7f && elf->header.e_ident[1]=='E' && elf->header.e_ident[2]=='L' && elf->header.e_ident[3]== 'F');
}

int initElf(elf_t *elf, char *filename, int mode){
	/* Existence et ouverture du fichier */
	elf->filename = malloc(strlen(filename) * sizeof(char));
	elf->filename = strcpy(elf->filename, filename);
    elf->relocTables = NULL;
    elf->fileContent = NULL;
    elf->symbolesNames = NULL;
	
	/* ouverture en écriture */
	if(mode == MODE_W){
		elf->file = fopen(elf->filename, "wb+");
		return 1;
	}

	elf->file = fopen(filename, "rb");
   	if(!elf->file){
		fprintf(stderr, "Erreur d'ouverture du fichier : %s\n", elf->filename);
		return -1;
	}
	
	fseek(elf->file, 0L, SEEK_END);
	elf->fileSize = ftell(elf->file);
	rewind(elf->file);

	elf->fileContent = malloc(sizeof(char) * elf->fileSize);
	int nbc = fread(elf->fileContent, elf->fileSize, 1, elf->file);

	if(nbc != 1){
		fprintf(stderr, "Erreur de lecture : %s\n", elf->filename);
		freeelf(elf);
		return -1;
	}

	getElfHeader(elf);

	if(!(isElf(elf))){
		fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", elf->filename);
		return -1;
	}
	getSectionsHeaders(elf);
	getSectionsContent(elf);
	getSectionNames(elf);

	getTableSymbole(elf);
	getSymbolesNames(elf);

    getStrtab(elf);
    
    /* Récupération des tables de réalocation */
    getRelocTable(elf);
	return 1;
}

//Récupere le contenu d'une section
void getSectionsContent(elf_t *elf){
	int i;
	elf->sectionContents = malloc(sizeof(char *)*elf->header.e_shnum);
	for(i=0; i<elf->header.e_shnum; i++){
		elf->sectionContents[i] = malloc(sizeof(char) * elf->sectionHeaders[i].sh_size);
		readElf(elf, elf->sectionHeaders[i].sh_offset, elf->sectionHeaders[i].sh_size, elf->sectionContents[i]);
	}
}

void freeelf(elf_t *elf){
	int i;
	free(elf->filename);
	fclose(elf->file);

	if(elf->fileContent != NULL)
		free(elf->fileContent);
	free(elf->sectionHeaders);

	for(i=0; i<elf->header.e_shnum; i++){
		free(elf->sectionContents[i]);
	}
	free(elf->sectionContents);

	for(i=0; i<elf->header.e_shnum; i++){
		free(elf->sectionNames[i]);
	}
	free(elf->sectionNames);

	free(elf->symTable);

	if(elf->symbolesNames != NULL){
		for(i=0; i<elf->symboleNumber; i++){
			free(elf->symbolesNames[i]);
		}
		free(elf->symbolesNames);
	}

	printf("Coucou\n");
	for(i=0; i<elf->nbRelTable; i++){
		free(elf->relocTables[i].relTable);
	}
	if(elf->relocTables != NULL)
		free(elf->relocTables);

	free(elf->strtab);
}

void getElfHeader(elf_t *elf){
	readElf(elf, 0, sizeof(Elf32_Ehdr), (&(elf->header)));
}

void getSectionsHeaders(elf_t *elf){
	elf->sectionHeaders = malloc(elf->header.e_shnum * sizeof(Elf32_Shdr));
	readElf(elf, elf->header.e_shoff, elf->header.e_shentsize * elf->header.e_shnum, elf->sectionHeaders);
}

//Récupère les noms des sections
void getSectionNames(elf_t *elf){
	elf->sectionNames = malloc(elf->header.e_shnum*sizeof(char*));

	int i, j;
	for (i=0; i<elf->header.e_shnum; i++){
		elf->sectionNames[i] = malloc(sizeof(char));
		j=-1;
		do{
			j++;
			elf->sectionNames[i] = realloc(elf->sectionNames[i], (j+2)*sizeof(char));
			elf->sectionNames[i][j] = elf->sectionContents[elf->header.e_shstrndx][elf->sectionHeaders[i].sh_name + j];
		}while(elf->sectionNames[i][j] != '\0');
	}
}

void getTableSymbole(elf_t *elf){	
	int numSymtab = getSectionNumber(elf, ".symtab");
	Elf32_Shdr *symbTableHeader = &(elf->sectionHeaders[numSymtab]);

	elf->symTable = malloc(sizeof(Elf32_Sym) * symbTableHeader->sh_size);
	readElf(elf, symbTableHeader->sh_offset, symbTableHeader->sh_size, elf->symTable);
	elf->symboleNumber = symbTableHeader->sh_size / sizeof(Elf32_Sym);
}



void getSymbolesNames(elf_t *elf){
	elf->symbolesNames = malloc(elf->symboleNumber*sizeof(char*));
	int i, j, strtab = getSectionNumber(elf, ".strtab");

	for (i=0; i<elf->symboleNumber; i++){
		elf->symbolesNames[i] = malloc(sizeof(char));
		j=-1;
		do{
			j++;
			elf->symbolesNames[i] = realloc(elf->symbolesNames[i], (j+2)*sizeof(char));
			elf->symbolesNames[i][j] = elf->sectionContents[strtab][elf->symTable[i].st_name + j];
		}while(elf->symbolesNames[i][j] != '\0');
	}
}

char* getTypeRealoc(int type){
	switch(type){
		case R_ARM_ABS32:
			return "R_ARM_ABS32";
		case R_ARM_PLT32:
			return "R_ARM_PLT32";
		case R_ARM_CALL:
			return "R_ARM_CALL";
		case R_ARM_JUMP24:
			return "R_ARM_JUMP24";
		default:
			return "Autre instruction";
	}
}

//Affiche les tables de réimplantation 
void getRelocTable(elf_t *elf){
	int i,j,nbc;
	int nbEnt = 0;
        int nbRel = 0;
        int count = 0;
        Elf32_Shdr sectionHeader;
        
        for(i=0; i<elf->header.e_shnum; i++){
            Elf32_Shdr sectionHeader = elf->sectionHeaders[i];
            if (sectionHeader.sh_type==SHT_REL){
                nbRel++;
            }
        }
        
	elf->relocTables = (reloc_t * ) malloc(sizeof(struct reloc_t *)*nbRel);
        elf->nbRelTable = nbRel;
        
	for(i=0; i<elf->header.e_shnum; i++){
		sectionHeader = elf->sectionHeaders[i];
		if (sectionHeader.sh_type==SHT_REL){
			count=0;
			nbEnt = sectionHeader.sh_size / sizeof(Elf32_Rel);
			elf->relocTables[count].relTable = malloc(nbEnt*sizeof(Elf32_Rel));
			fseek(elf->file,(int)sectionHeader.sh_offset,SEEK_SET);
			for(j = 0; j < nbEnt;j++){
				nbc = fread(&elf->relocTables[count].relTable[j],sizeof(Elf32_Rel),1,elf->file);
				if(nbc != 1){
					if(feof(elf->file)){
						/* End of elf->file */
					}else{
						debug("Erreur de lecture.");
					}
				}
 			}
                        elf->relocTables[count].tailleRelocTable = nbEnt;
                        elf->relocTables[count].name = (char *) malloc((strlen(elf->sectionNames[i])+1)*sizeof(char));
                        elf->relocTables[count].name = elf->sectionNames[i];
                        count++;
		} 
	}
	
}

void getStrtab(elf_t *elf){
    int numSymSec = getSectionNumber(elf, ".strtab");

    int size = elf->sectionHeaders[numSymSec].sh_size;
    elf->strtab = (char *) malloc(size*sizeof(char));
    memcpy(elf->strtab, elf->sectionContents[numSymSec], elf->sectionHeaders[numSymSec].sh_size);
}