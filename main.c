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

void usage(char *name);

int main(int argc, char* argv[]){
	
	/* Récupération des arguments */
	int opt, nbc;
	
	char arg_elf_header=0, arg_program_headers=0, arg_section_headers=0, arg_symbols=0, arg_dyn_syms=0, arg_notes=0, arg_relocs=0, arg_hexdump=0, arg_string_dump=0;
	char * arg_section;
	int num_section = -1;
	elf_t elf;

	struct option longopts[] = {
		{ "all", no_argument, NULL, 'a' },
		{ "file-header", no_argument, NULL, 'h' },
		{ "segments", no_argument, NULL, 'l' },
		{ "section-headers", no_argument, NULL, 'S' },
		{ "sections", no_argument, NULL, 'S' },
		{ "headers", no_argument, NULL, 'e' },
		{ "syms", no_argument, NULL, 's' },
		{ "symbols", no_argument, NULL, 's' },
		{ "relocs", no_argument, NULL, 'r' },
		{ "use-dynamic", no_argument, NULL, 'D' },
		{ "hex-dump", required_argument, NULL, 'x' },
		{ "string-dump", required_argument, NULL, 'p' },

		{ "debug-file", required_argument, NULL, 'f' },
		{ "help", no_argument, NULL, '?' },
		{ NULL, 0, NULL, 0 }
	};

	while ((opt = getopt_long(argc, argv, "ahlSesrDx:p:fx?", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			arg_elf_header=1;
			break;
		case 'l':
			arg_program_headers=1;
			break;
		case 'S':
			arg_section_headers=1;
			break;
		case 'a':
		case 'e':
			arg_elf_header=1;
			arg_program_headers=1;
			arg_section_headers=1;
			break;
		case 's':
			arg_symbols=1;
			break;
		case 'n':
			arg_notes=1;
			break;
		case 'r':
			arg_relocs=1;
			break;
		case 'D':
			//arg_use_dynamics=1;
			break;
		case 'x':
			arg_hexdump=1;
			arg_section = malloc(strlen(optarg) * sizeof(char));
			strcpy(arg_section, optarg);
			break;
		case 'p':
			arg_string_dump=1;
			arg_section = malloc(strlen(optarg) * sizeof(char));
			strcpy(arg_section, optarg);
			break;
		case 'f':			
			add_debug_to(optarg);
			break;
		case '?':
		default:
			usage(argv[0]);
			exit(1);
		}
	}
	
	/* Existence et ouverture du fichier */
	char *filename;
	if(optind < argc){
		filename = malloc(strlen(argv[optind]) *  sizeof(char));
		strcpy(filename, argv[optind]);
		optind++;
		elf.file = fopen(filename, "rb");
       	
       	if(!elf.file){
			fprintf(stderr, "Erreur d'ouverture du fichier : %s\n", filename);
			return 0;
		}
	}
	else{
		usage(argv[0]);
		return 0;
	}


	/* ELF header */
	nbc = fread(&(elf.header), 1, sizeof(elf.header), elf.file);
	if(nbc != 1){
		if(feof(elf.file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

	if(!(isELF(elf.header))){
		fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", filename);
		return -1;
	}

	/* Section headers */
	elf.sectionHeaders = malloc(elf.header.e_shnum * sizeof(Elf32_Shdr));
	fseek(elf.file, elf.header.e_shoff, SEEK_SET);
	nbc = fread(elf.sectionHeaders, elf.header.e_shentsize, elf.header.e_shnum, elf.file);
	if(nbc != 1){
		if(feof(elf.file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

	/* Section names */
	elf.sectionNames = malloc(elf.header.e_shnum*sizeof(char*));
	getSectionNames(&elf);

	/* Récupération de la strtab*/
    int numSymSec = getSectionNumber(&elf, ".strtab");

    int size = elf.sectionHeaders[numSymSec].sh_size;
    char strtab[size];
    getSectionContent(&elf, numSymSec, strtab);
    elf.strtab = strtab;

	/* Table de réalcation */

	getRelocTable(&elf);

	getTableSymbole(&elf);

	/* Execution des fonctions demandées */
	if(arg_elf_header){
		displayHeader(&elf);	
	}
	if(arg_program_headers){
		printf("Option not available yet.\n");
	}
	if(arg_section_headers){
		displaySectionHeaders(&elf);
	}
	if(arg_symbols){
		displayTableSymbole(&elf);
	}
	if(arg_dyn_syms){
		printf("Option not available yet.\n");
	}
	if(arg_notes){
		printf("Option not available yet.\n");
	}
	if(arg_relocs){
		displayRelocTable(&elf);
	}
	if(arg_hexdump){
		if(isNumber(arg_section)){
			num_section = atoi(arg_section);
		}else{
			num_section = getSectionNumber(&elf, arg_section);
		}

		if(num_section > elf.header.e_shnum || num_section < 0){
			fprintf(stderr, "Identifiant de section invalide : %s\n", arg_section);
		}else{
            displaySectionContent(&elf, num_section);
		}
	}
	if(arg_string_dump){
		if(num_section > elf.header.e_shnum || num_section < 0){
			fprintf(stderr, "Identifiant de section invalide : %s\n", arg_section);
		}else{
            printf("Option not available yet.\n");
		}
	}

	/* Memory free */
	fclose(elf.file);
	// free(elf.sectionHeaders);
	// free(elf.sectionNames);


	return 0;
}


//Aide pour les options 
void usage(char *name){
	fprintf(stderr, "Usage:\n"
		"%s [ options ] file\n\n"
		"-h Prints file headers\n"
		"-S Prints sections headers\n"
		"-s Prints symbols table\n"
		"-r Prints relocation table\n"
		"-x <num> Hexa dump of section number <num>\n"
		, name);
}