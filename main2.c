#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "main.h"


void afficherHeader(Elf32_Ehdr header){
		if(header.e_ident[0]== 0x7f && header.e_ident[1]=='E' && header.e_ident[2]=='L' && header.e_ident[3]== 'F'){
			printf("En-tête : %c%c%c\n", header.e_ident[1],header.e_ident[2],header.e_ident[3]);
			printf("Magique : ");
			int i;
			for(i =0; i<16;i++){
				printf(" %x", header.e_ident[i]);

			}
			switch(header.e_ident[EI_CLASS]){
				case 1: 
					printf("\n Classe : ELF32 \n"); 
					break;
				case 2 : 
					printf("\n Classe : ELF64 \n"); 
					break;
				default : 
					printf("\n Classe : Invalide \n"); 
					break;
			}
			switch(header.e_ident[EI_DATA]){
				case 1: 
					printf("Donnees : Complement à 2, Little Endian\n"); 
					break;
				case 2 : 
					printf("Donnees : Complement à 2, Big Endian\n"); 
					break;
				default : 
					printf("Donnees: Invalide\n"); 
					break;
			}
			printf("Version : %d\n",header.e_ident[EI_VERSION]);
			printf("OS/ABI : %d\n", header.e_ident[EI_OSABI]);	// switch a faire
			printf("Version ABI: %d\n",header.e_ident[EI_ABIVERSION]);	
			switch(header.e_type){
				case ET_REL:
					printf("Type : RELOCABLE\n");
					break;
				case ET_EXEC:
					printf("Type : EXEC\n");
					break;
				case ET_DYN : 
					printf("Type : SHARED\n");
					break;
				case ET_CORE :
					printf("Type : CORE\n");
					break;
				case ET_LOPROC :
					printf("Type : LOPROC\n");
					break;
				case ET_HIPROC :
					printf("Type : HIPROC\n");
					break;
				case ET_NONE : 
					printf("Type : NOFILE\n");
					break;
				default: 	
					break;

			}
			printf("Machine : ");
			switch(header.e_machine){
				case EM_NONE:         
					printf("An unknown machine");
					break;
                		case EM_M32:         
					printf("AT&T WE 32100");
					break;
               			case  EM_SPARC:      
					printf("Sun Microsystems SPARC");
					break;
              			case   EM_386:          
					printf("Intel 80386");
					break;
            	    		case  EM_68K:        
					printf("Motorola 68000");
					break;
                		case EM_88K:         
					printf("Motorola 88000");
					break;
                 		case EM_860:          
					printf("Intel 80860");
					break;
                		case EM_MIPS:         
					printf("MIPS RS3000 (big-endian only)");
					break;
                		case EM_PARISC:       
					printf("HP/PA");
					break;
				case EM_SPARC32PLUS:  
					printf("SPARC with enhanced instruction set");
					break;
				case  EM_PPC:         
					printf("PowerPC");
					break;
				case EM_PPC64:        
					printf("PowerPC 64-bit");
					break;
				case EM_S390:         
					printf("IBM S/390");
					break;
				case EM_ARM:         
					printf("Advanced RISC Machines");
					break;
				case EM_SH:           
					printf("Renesas SuperH");
					break;
				case EM_SPARCV9:      
					printf("SPARC v9 64-bit");
					break;
				case EM_IA_64:        
					printf("Intel Itanium");
					break;
				case EM_X86_64:       
					printf("AMD x86-64");
					break;
				case EM_VAX:          
					printf("DEC Vax");
					break;

			}
			printf("\n Version: %x\n",header.e_version);
			printf("Adresse du point d'entree 0x%x\n",header.e_entry);
			printf("Début des  en-têtes de programme: %x\n",header.e_phoff);
			printf("Débuts des entêtes de sections: %d\n",header.e_shoff); //
			printf("Flags : %x\n",header.e_flags);
				
			printf("Taille de cet entête : %d\n",header.e_ehsize);
			printf("Taille de l'entête du programme : %d\n",header.e_phentsize);
			printf("Nombre d'entêtes du programme : %d\n",header.e_phnum);
			printf("Taille des entêtes de section : %d\n",header.e_shentsize); //
			printf("Nombre d'entêtes de section : %d\n",header.e_shnum); //
			printf("Table d'indexes des chaînes d'entête de sections : %d\n",header.e_shstrndx);
			
		}
}

void sectionHexDump(Elf32_Shdr sectionHeader, FILE *file){
	char buffer[sectionHeader.sh_size];
	if(fseek(file, sectionHeader.sh_offset, SEEK_SET) != 0){
		fprintf(stderr, "Fseek fail !\n");
	}

	int nbc = fread(buffer, sectionHeader.sh_size, 1, file);
	printf("%s\n", buffer+9);

}


// header de la section
void displaySectionContent(Elf32_Ehdr header, Elf32_Shdr sectionHeader, FILE* file){

	// printf("Affichage hexadécimal de la section « %s »\n", sectionHeader.sh_name);
	printf("Affichage hexadécimal de la section « »\n");
	uint64_t mot;
	int ligne = sectionHeader.sh_offset;
	fseek(file, sectionHeader.sh_offset, SEEK_SET);
	
	int i;
	for(i=0; i<sectionHeader.sh_size; i+=64){	
		fread(&mot, 1, 64, file);
		printf("0x%08x %x \n", ligne, (unsigned int)mot);
		ligne += 64;
	}
}

void displaySectionHeaders(Elf32_Shdr sectionHeaders[], char *sectionNames[], int size){

}

void getSectionContent(FILE *file, Elf32_Shdr sectionHeader, char *buffer){

	if(fseek(file, sectionHeader.sh_offset, SEEK_SET) != 0){
		fprintf(stderr, "Fseek fail !\n");
	}

	int nbc = fread(buffer, sectionHeader.sh_size, 1, file);

}

void getSectionNames(FILE * file, Elf32_Ehdr header, Elf32_Shdr sectionHeaders[], char * sectionNames[]){
	/* On récupère la section des noms */
	int size = sectionHeaders[header.e_shstrndx].sh_size;
	char str[size], *s="\0";
	
	getSectionContent(file, sectionHeaders[header.e_shstrndx], str);

	int i, j=0, numName = 0;
	char tmp[1000]; // TODO set to section size

	for (i=0; i<header.e_shnum; i++){
		j=-1;
		do{
			j++;
			tmp[j] = str[sectionHeaders[i].sh_name + j];
		}while(str[sectionHeaders[i].sh_name + j] == '\0');
		
		sectionNames[numName] = malloc(strlen(tmp));
		strcpy(sectionNames[numName], tmp);
		numName++;
	}
}



void usage(char *name){
	fprintf(stderr, "Usage:\n"
		"%s [ options ] file\n\n"
		"-h Prints file heders\n"
		"-S Prints sections headers\n"
		"-x <num> Hexa dump of section number <num>\n"
		, name);
}

int main(int argc, char* argv[]){
	
	/* Récupération des arguments */
	int opt;
	char *option1, *option2;
	
	char arg_header=0, arg_hexdump=0, arg_sectionsHeaders=0;
	int arg_section=0;
	FILE *file;

	struct option longopts[] = {
		{ "h", required_argument, NULL, 'h' },
		{ "x", required_argument, NULL, 'x' },
		{ "section-headers", no_argument, NULL, 'S' },
		{ "help", no_argument, NULL, '?' },
		{ NULL, 0, NULL, 0 }
	};

	while ((opt = getopt_long(argc, argv, "h:x:S", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			arg_header=1;
			break;
		case 'S':
			arg_sectionsHeaders=1;
			break;
		case 'x':
			arg_hexdump=1;
			arg_section = atoi(optarg);
			break;
		default:
			usage(argv[0]);
			exit(1);
		}
	}
	
	/* Existence et ouverture du fichier */
	if(optind < argc){
		file = fopen(argv[optind++], "rb");
       	if(!file){
			fprintf(stderr, "Erreur d'ouverture du fichier : %s\n", argv[optind]);
			return 0;
		}
	}
	else{
		usage(argv[0]);
		return 0;
	}

	/* Lecture des en-têtes */
	Elf32_Ehdr header;
	fread(&header, 1, sizeof(header), file);

	Elf32_Shdr sectionHeaders[header.e_shnum];
	fseek(file, header.e_shoff, SEEK_SET);
	fread(sectionHeaders, header.e_shentsize, header.e_shnum, file);
	Elf32_Shdr test = sectionHeaders[28];
	// Elf32_Shdr test2;
	// memcpy(&test, sectionHeaders+28, sizeof(Elf32_Shdr));
	// memcpy(&test2, sectionHeaders+28, sizeof(Elf32_Shdr));

	// TODO

	char * sectionNames[header.e_shnum];
	getSectionNames(file, header, sectionHeaders, sectionNames);
	printf("%s\n", sectionNames[sectionHeaders[28].sh_name]);


	/* Execution des fonctions demandées */
	if(arg_header){
		afficherHeader(header);	
	}
	if(arg_sectionsHeaders){
		displaySectionHeaders(sectionHeaders, sectionNames, header.e_shnum);
	}
	if(arg_hexdump){
		if(arg_section > header.e_shnum || arg_section < 0){
			fprintf(stderr, "Numéro de section invalide : %d\n", arg_section);
		}else{
			sectionHexDump(sectionHeaders[arg_section], file);
		}
	}
	
	fclose(file);
	//free(sectionHeaders);
	//free(sectionNames);

	return 0;
}

