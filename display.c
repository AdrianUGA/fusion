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

//Affichage de l'header du fichier elf

void displayHeader(elf_t *elf){
	printf("En-tête : %c%c%c\n", elf->header.e_ident[1],elf->header.e_ident[2],elf->header.e_ident[3]);
	printf("\tMagique : ");
	int i;
	for(i =0; i<16;i++){
		printf(" %02x", elf->header.e_ident[i]);

	}
	switch(elf->header.e_ident[EI_CLASS]){
		case 1: 
			printf("\n\tClasse : ELF32 \n"); 
			break;
		case 2 : 
			printf("\n\tClasse : ELF64 \n"); 
			break;
		default : 
			printf("\n\tClasse : Invalide \n"); 
			break;
	}
	switch(elf->header.e_ident[EI_DATA]){
		case 1: 
			printf("\tDonnees : Complement à 2, Little Endian\n"); 
			break;
		case 2 : 
			printf("\tDonnees : Complement à 2, Big Endian\n"); 
			break;
		default : 
			printf("\tDonnees: Invalide\n"); 
			break;
	}
	printf("\tVersion : %d\n",elf->header.e_ident[EI_VERSION]);
	printf("\tOS/ABI : %d\n", elf->header.e_ident[EI_OSABI]);	// switch a faire
	printf("\tVersion ABI: %d\n",elf->header.e_ident[EI_ABIVERSION]);	
	switch(elf->header.e_type){
		case ET_REL:
			printf("\tType : RELOCABLE\n");
			break;
		case ET_EXEC:
			printf("\tType : EXEC\n");
			break;
		case ET_DYN : 
			printf("\tType : SHARED\n");
			break;
		case ET_CORE :
			printf("\tType : CORE\n");
			break;
		case ET_LOPROC :
			printf("\tType : LOPROC\n");
			break;
		case ET_HIPROC :
			printf("\tType : HIPROC\n");
			break;
		case ET_NONE : 
			printf("\tType : NOFILE\n");
			break;
		default: 	
			break;

	}
	printf("\tMachine : ");
	switch(elf->header.e_machine){
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
	printf("\n\tVersion: %x\n",elf->header.e_version);
	printf("\tAdresse du point d'entree 0x%x\n",elf->header.e_entry);
	printf("\tDébut des  en-têtes de programme: %x\n",elf->header.e_phoff);
	printf("\tDébuts des entêtes de sections: %d\n",elf->header.e_shoff); //
	printf("\tFlags : %x\n",elf->header.e_flags);
		
	printf("\tTaille de cet entête : %d\n",elf->header.e_ehsize);
	printf("\tTaille de l'entête du programme : %d\n",elf->header.e_phentsize);
	printf("\tNombre d'entêtes du programme : %d\n",elf->header.e_phnum);
	printf("\tTaille des entêtes de section : %d\n",elf->header.e_shentsize); //
	printf("\tNombre d'entêtes de section : %d\n",elf->header.e_shnum); //
	printf("\tTable d'indexes des chaînes d'entête de sections : %d\n",elf->header.e_shstrndx);
}

//Affichage du contenu de la section avec pour parametre son idice
void displaySectionContent(elf_t *elf, int sectionNumber){
	Elf32_Shdr *sectionHeader = &(elf->sectionHeaders[sectionNumber]);

	printf("Affichage hexadécimal de la section « %s »\n", elf->sectionNames[sectionNumber]);
	int ligne = sectionHeader->sh_addr;

	// char sectionContent[sectionHeader->sh_size];
	// getSectionContent(elf, sectionNumber, sectionContent);
	int cursor = 0, i, k;
	unsigned char mot;
	char *convert;

	for(i=0;i < sectionHeader->sh_size; i+=16){
		printf("  Ox%08x",ligne);
		convert = (char *) malloc(sizeof(char));		
		for(k=0;k<16;k++){
			if(sectionHeader->sh_size > i+k){
				memcpy(&mot, elf->sectionContents[sectionNumber] + cursor, sizeof(unsigned char));
				cursor += sizeof(unsigned char);

				if (isprint(mot))			
					convert[k] = mot;
				else
					convert[k] = '.';							
				if(k % 4 == 0)
					printf(" ");

				printf("%02x",mot);
				convert = realloc(convert, k+1);
			}else{
				if(k % 4 == 0)
					printf(" ");
				printf("  ");
			}
		}

		printf(" %s\n",convert);		
		ligne += 16;	
	}
}


//Affichage du contenu des headers des différentes sections
void displaySectionHeaders(elf_t *elf){
		char flags[4];
        printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x\n",elf->header.e_shnum,elf->header.e_shoff);
        printf("[Nr]\t%-15s\t%-10s\tAdr\t\tDecala.\t\tTaille\tES\tFan\tLN\tInf\tAl\n", "Nom", "Type");

        int i;
		char* type;
        for(i=0; i < elf->header.e_shnum; i++){
		switch(elf->sectionHeaders[i].sh_type){
			case SHT_NULL :
				type = malloc(4*sizeof(char));
				type = "NULL";
				break;
			case SHT_PROGBITS :
				type = malloc(8*sizeof(char));
				type ="PROGBITS";
				break;
			case SHT_SYMTAB :
				type = malloc(6*sizeof(char));
				type = "SYMTAB";
				break;
			case SHT_STRTAB :
				type = malloc(6*sizeof(char));
				type = "STRTAB";
				break;
			case SHT_RELA :
				type = malloc(4*sizeof(char));
				 type ="RELA";
				break;
			case SHT_HASH :
				type = malloc(4*sizeof(char));
				type ="HASH";
				break;
			case SHT_DYNAMIC :
				type = malloc(7*sizeof(char));
				type ="DYNAMIC";
				break;
			case SHT_NOTE :
				type = malloc(4*sizeof(char));
				type ="NOTE";
				break;
			case SHT_NOBITS :
				type = malloc(6*sizeof(char));
				type ="NOBITS";
				break;
			case SHT_REL :
				type = malloc(3*sizeof(char));
				type ="REL";
				break;
			case SHT_SHLIB :
				 type ="SHLIB";
				break;
			case SHT_DYNSYM :
				type = malloc(6*sizeof(char));
				type ="DYNSYM";
				break;
			case SHT_LOPROC :
				type = malloc(6*sizeof(char));
				type ="LOPROC";
				break;
			case SHT_HIPROC :
				type = malloc(6*sizeof(char));
				type ="HIPROC";
				break;
			case SHT_LOUSER :
				type = malloc(6*sizeof(char));
				type ="LOUSER";
				break;
			case SHT_HIUSER :
				type = malloc(6*sizeof(char));
				type ="HIUSER";
				break;
			default : 
				type = malloc(6*sizeof(char));
				type ="NONDEF";
				break;
		}
		flags[0] = '\0';
		if((elf->sectionHeaders[i].sh_flags & SHF_WRITE) == SHF_WRITE ){
			strcat(flags,"W");
		}
		if((elf->sectionHeaders[i].sh_flags & SHF_ALLOC) == SHF_ALLOC ){
			strcat(flags,"A");
		}

		if((elf->sectionHeaders[i].sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR ){
			strcat(flags,"X");
		}
		if((elf->sectionHeaders[i].sh_flags & SHF_MASKPROC) == SHF_MASKPROC ){
			strcat(flags,"M");
		}


		printf("[%d]\t%-15s\t%-10s\t%08x\t%08x\t%d\t%02x\t%s\t%d\t%d\t%d\n",
			i,
			elf->sectionNames[i],
			type,
			elf->sectionHeaders[i].sh_addr,
			elf->sectionHeaders[i].sh_offset,
			elf->sectionHeaders[i].sh_size,
			elf->sectionHeaders[i].sh_entsize,
			flags,
			elf->sectionHeaders[i].sh_link,
			elf->sectionHeaders[i].sh_info,
			elf->sectionHeaders[i].sh_addralign);
    }
}

void displaySymbole(Elf32_Sym symbole, char * symboleName, int symboleNumber){
	
	char * type;		
	switch(ELF32_ST_TYPE(symbole.st_info)){
		case STT_NOTYPE : 
		type = (char *)malloc(6*sizeof(char));
		type = "NOTYPE";
		break;
		case STT_OBJECT :
		type = (char *)malloc(6*sizeof(char));
		type = "OBJECT";
		break;
		case STT_FUNC :
		type = (char *)malloc(8*sizeof(char));
		type = "FONCTION";
		break;
		case STT_SECTION:
			type = (char *)malloc(7*sizeof(char));
			type = "SECTION";
			break;
		case STT_FILE :
		type = (char *)malloc(4*sizeof(char));
		type = "FILE";
		break;
		case STT_COMMON :
		type = (char *)malloc(6*sizeof(char));
		type = "COMMON";
		break;
		case STT_TLS :
		type = (char *)malloc(3*sizeof(char));
		type = "TLS";
		break;
		case STT_LOOS :
		type = (char *)malloc(4*sizeof(char));
		type = "LOOS";
		break;
		case STT_HIOS :
		type = (char *)malloc(4*sizeof(char));
		type = "HIOS";
		break;
		case STT_LOPROC :
		type = (char *)malloc(6*sizeof(char));
		type = "LOPROC";
		break;
		case STT_HIPROC :
		type = (char *)malloc(6*sizeof(char));
		type = "HIPROC";
		break;
		default :
		type=NULL;
		break;
	}
	char * visibilite;
	switch(symbole.st_other){
		case STV_INTERNAL :
		visibilite = (char *)malloc(8*sizeof(char));			
		visibilite = "INTERNAL";
		break;
		case STV_HIDDEN :
		visibilite = (char *)malloc(6*sizeof(char));			
		visibilite = "HIDDEN";
		break;
		case STV_DEFAULT :
		visibilite = (char *)malloc(7*sizeof(char));			
		visibilite = "DEFAULT";
		break;
		case STV_PROTECTED :
		visibilite = (char *)malloc(9*sizeof(char));			
		visibilite = "PROTECTED";
		break;
		default :
		visibilite = NULL;
		break;
	}
	char * lien;
	switch(ELF32_ST_BIND(symbole.st_info)){
		case STB_LOCAL :
			lien = (char *)malloc(5*sizeof(char));
			lien = "LOCAL";			
		break;
		case STB_GLOBAL :
			lien = (char *)malloc(6*sizeof(char));
			lien = "GLOBAL";
		break;
		case STB_LOOS :
			lien = (char *)malloc(4*sizeof(char));
			lien = "LOOS";
		break;
		case STB_HIOS :
			lien = (char *)malloc(4*sizeof(char));
			lien = "HIOS";
		break;
		case STB_WEAK :
			lien = (char *)malloc(4*sizeof(char));
			lien = "WEAK";
		break;
		case STB_LOPROC :
			lien = (char *)malloc(6*sizeof(char));
			lien = "LOPROC";
		break;
		case STB_HIPROC :
			lien = (char *)malloc(6*sizeof(char));
			lien = "HIPROC";
		break;
		default :
			lien=NULL;
		break;
	}

	printf("%3d : %08x %3d %8s %8s %8s %5d %s \n",symboleNumber,symbole.st_value,symbole.st_size,type,lien,visibilite,symbole.st_shndx,symboleName);
}

//Affiche les tables de symboles
void displayTableSymbole(elf_t *elf){
	
    printf("\n\n");
    printf("Table de symboles « .symtab » contient %d entrées:\n",elf->symboleNumber);
    printf("Num : Valeur Tail   Type    Lien      Vis      Ndx    Nom\n");

    int i;
    for(i=0; i < elf->symboleNumber; i++){
        displaySymbole(elf->symTable[i], elf->symbolesNames[i], i);
    }
}


//Affiche les tables de réimplantation 
void displayRelocTable(elf_t *elf){
	printf("********************\n");
	printf("* Relocation table *\n");
	printf("********************\n");
	int i,j,nbc;
	Elf32_Rel relcel;
	for(i=0;i<elf->header.e_shnum;i++){
		Elf32_Shdr sectionHeader = elf->sectionHeaders[i];
		if (sectionHeader.sh_type==SHT_REL){
			int nbEnt = sectionHeader.sh_size / sectionHeader.sh_entsize;
			fseek(elf->file,(int)sectionHeader.sh_offset,SEEK_SET);
			printf("Relocation section %s at offset 0x3e0 contains %d entries:\n", elf->sectionNames[i], nbEnt); 
			printf("---------------------------------------------------------\n"); 
			printf(" Offset     Info         Type            Sym.\n");
			printf("---------------------------------------------------------\n"); 
			for(j = 0; j < nbEnt;j++){
				nbc = fread(&relcel,sizeof(Elf32_Rel),1,elf->file);
				if(nbc != 1){
					if(feof(elf->file)){
						/* End of elf->file */
					}else{
						debug("Erreur de lecture.");
					}
				}
				printf("%08x  %08x  %17s  %6d\n",relcel.r_offset, relcel.r_info,getTypeRealoc(ELF32_R_TYPE(relcel.r_info)),ELF32_R_SYM(relcel.r_info));
 			}			
			printf("---------------------------------------------------------\n"); 
			printf("\n");
		}
		
	}
}