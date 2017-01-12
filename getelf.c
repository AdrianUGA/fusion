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

int initElf(elf_t *elf, char *filename){

	/* Existence et ouverture du fichier */
	elf->filename = malloc(strlen(filename) * sizeof(char));
	elf->filename = strcpy(elf->filename, filename);
	
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

/* Renvoie la table des symboles et modifie la taille (nombre de symbole) par effet de bord */
void getTableSymbole(elf_t *elf){	
	int numSymtab = getSectionNumber(elf, ".symtab");
	elf->symTable = malloc(sizeof(Elf32_Sym) * elf->sectionHeaders[numSymtab].sh_size);
	readElf(elf, elf->sectionHeaders[numSymtab].sh_offset, elf->sectionHeaders[numSymtab].sh_size, elf->symTable);
	elf->symboleNumber = elf->sectionHeaders[numSymtab].sh_size / sizeof(Elf32_Sym);
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
