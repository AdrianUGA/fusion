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

char isNumber(char *str){
	int i;
	for(i=0; i<strlen(str); i++){
		if(!isdigit(str[i]))
			return 0;
	}
	return 1;
}

int getSectionNumber(elf_t *elf, char *name){
	int i;
	for(i=0; i<elf->header.e_shnum; i++){
		if(strcmp(elf->sectionNames[i], name) == 0)
			return i;
	}
	return -1;
}

int isELF(Elf32_Ehdr header){
	return (header.e_ident[0]== 0x7f && header.e_ident[1]=='E' && header.e_ident[2]=='L' && header.e_ident[3]== 'F');
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

//Récupere le contenu d'une section
void getSectionContent(elf_t *elf, int sectionNumber, char *buffer){

	if(fseek(elf->file, elf->sectionHeaders[sectionNumber].sh_offset, SEEK_SET) != 0){
		fprintf(stderr, "Fseek fail !\n");
	}

	int nbc;
	nbc = fread(buffer, elf->sectionHeaders[sectionNumber].sh_size, 1, elf->file);
	if(nbc != 1){
		if(feof(elf->file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

}

//Récupère les noms des sections
void getSectionNames(elf_t *elf){
	/* On récupère la section des noms */
	int size = elf->sectionHeaders[elf->header.e_shstrndx].sh_size;
	char str[size];

	getSectionContent(elf, elf->header.e_shstrndx, str);

	int i, j;

	for (i=0; i<elf->header.e_shnum; i++){
		elf->sectionNames[i] = malloc(sizeof(char));
		j=-1;
		do{
			j++;
			elf->sectionNames[i] = realloc(elf->sectionNames[i], (j+2)*sizeof(char));
			elf->sectionNames[i][j] = str[elf->sectionHeaders[i].sh_name + j];
		}while(elf->sectionNames[i][j] != '\0');
		
	}

}