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

int getSectionNumber(char *name, char *sectionNames[], Elf32_Ehdr header){
	int i;
	for(i=0; i<header.e_shnum; i++){
		if(strcmp(sectionNames[i], name) == 0)
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
void getSectionContent(FILE *file, Elf32_Shdr sectionHeader, char *buffer){

	if(fseek(file, sectionHeader.sh_offset, SEEK_SET) != 0){
		fprintf(stderr, "Fseek fail !\n");
	}

	int nbc;
	nbc = fread(buffer, sectionHeader.sh_size, 1, file);
	if(nbc != 1){
		if(feof(file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

}

//Récupère les noms des sections
void getSectionNames(FILE * file, Elf32_Ehdr header, Elf32_Shdr sectionHeaders[], char * sectionNames[]){
	/* On récupère la section des noms */
	int size = sectionHeaders[header.e_shstrndx].sh_size;
	char str[size];
	
	getSectionContent(file, sectionHeaders[header.e_shstrndx], str);

	int i, j=0;
	char tmp[1000]; // TODO set to section size

	for (i=0; i<header.e_shnum; i++){
		j=-1;
		do{
			j++;
			tmp[j] = str[sectionHeaders[i].sh_name + j];
		}while(str[sectionHeaders[i].sh_name + j] != '\0');
		
		sectionNames[i] = malloc(strlen(tmp));
		strcpy(sectionNames[i], tmp);
	}
}