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

//Affichage de l'header du fichier elf
void displayHeader(Elf32_Ehdr header){
	printf("En-tête : %c%c%c\n", header.e_ident[1],header.e_ident[2],header.e_ident[3]);
	printf("\tMagique : ");
	int i;
	for(i =0; i<16;i++){
		printf(" %02x", header.e_ident[i]);

	}
	switch(header.e_ident[EI_CLASS]){
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
	switch(header.e_ident[EI_DATA]){
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
	printf("\tVersion : %d\n",header.e_ident[EI_VERSION]);
	printf("\tOS/ABI : %d\n", header.e_ident[EI_OSABI]);	// switch a faire
	printf("\tVersion ABI: %d\n",header.e_ident[EI_ABIVERSION]);	
	switch(header.e_type){
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
	printf("\n\tVersion: %x\n",header.e_version);
	printf("\tAdresse du point d'entree 0x%x\n",header.e_entry);
	printf("\tDébut des  en-têtes de programme: %x\n",header.e_phoff);
	printf("\tDébuts des entêtes de sections: %d\n",header.e_shoff); //
	printf("\tFlags : %x\n",header.e_flags);
		
	printf("\tTaille de cet entête : %d\n",header.e_ehsize);
	printf("\tTaille de l'entête du programme : %d\n",header.e_phentsize);
	printf("\tNombre d'entêtes du programme : %d\n",header.e_phnum);
	printf("\tTaille des entêtes de section : %d\n",header.e_shentsize); //
	printf("\tNombre d'entêtes de section : %d\n",header.e_shnum); //
	printf("\tTable d'indexes des chaînes d'entête de sections : %d\n",header.e_shstrndx);
			
		
}

//Affichage du contenu de la section avec pour parametre son idice
void displaySectionContentI(Elf32_Ehdr header, int j, FILE* file,  char * sectionNames[]){
	Elf32_Shdr ITERheader;
	int nbc;

    int i;
	fseek(file, 0, SEEK_SET);
	for ( i=0; i <= j; i++ ){
		fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
		nbc = fread( &ITERheader, header.e_shentsize, 1, file );
		if(nbc != 1){
			if(feof(file)){
				/* End of file */
			}else{
				debug("Erreur de lecture.");
			}
		}
	}


	printf("Affichage hexadécimal de la section « %s »\n", sectionNames[j]);
	int ligne = ITERheader.sh_addr;	
	fseek(file, ITERheader.sh_offset, SEEK_SET);
	for(i=0;i < ITERheader.sh_size; i+=16){
		printf("  Ox%08x",ligne);
		int k, nbc;
		char * convert;
		convert = (char *) malloc(1);		
		for(k=0;k<16;k++){
			if(ITERheader.sh_size > i+k){
				unsigned char mot;
				nbc = fread(&mot, sizeof(mot),1, file);
				if(nbc != 1){
					if(feof(file)){
						/* End of file */
					}else{
						debug("Erreur de lecture.");
					}
				}

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
void displaySectionHeader(Elf32_Shdr* sectionH, Elf32_Ehdr header, char * sectionNames[]){
		char flags[4];
        printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x\n",header.e_shnum,header.e_shoff);
        printf("[Nr]\t%-15s\t%-10s\tAdr\t\tDecala.\t\tTaille\tES\tFan\tLN\tInf\tAl\n", "Nom", "Type");

        int i;
		char* type;
        for(i=0; i < header.e_shnum; i++){
	switch(sectionH[i].sh_type){
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
	if((sectionH[i].sh_flags & SHF_WRITE) == SHF_WRITE ){
		strcat(flags,"W");
	}
	if((sectionH[i].sh_flags & SHF_ALLOC) == SHF_ALLOC ){
		strcat(flags,"A");
	}

	if((sectionH[i].sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR ){
		strcat(flags,"X");
	}
	if((sectionH[i].sh_flags & SHF_MASKPROC) == SHF_MASKPROC ){
		strcat(flags,"M");
	}


	printf("[%d]\t%-15s\t%-10s\t%08x\t%08x\t%d\t%02x\t%s\t%d\t%d\t%d\n",i,sectionNames[i],type, sectionH[i].sh_addr, sectionH[i].sh_offset, sectionH[i].sh_size, sectionH[i].sh_entsize, flags, sectionH[i].sh_link, sectionH[i].sh_info, sectionH[i].sh_addralign);
        }
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

//Affiche un symbole
void displaySymbole(Elf32_Sym symbole, char * strtab, int i){
	
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
	printf("%3ld: %08x %3d %8s %8s %8s %5d %s \n",i/sizeof(symbole),symbole.st_value,symbole.st_size,type,lien,   visibilite,symbole.st_shndx,strtab+symbole.st_name);
}

//Affiche les tables de symboles
void displayTableSymbole(FILE * file, char * sectionNames[]){
	
	Elf32_Ehdr header;
	Elf32_Shdr dyn, sym,tmp;
	Elf32_Sym symbole;
	char * strtab;
	int i,itab,ok=0;
        
    fseek( file, 0, SEEK_SET );

    int nbc;
	nbc = fread( &header , sizeof(Elf32_Ehdr), 1, file);
	if(nbc != 1){
		if(feof(file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}


	//Affichage des symboles de .dynsym
	fseek( file, 0, SEEK_SET );
        for(itab = 0; itab < header.e_shnum; itab++){
		fseek( file, header.e_shoff+(header.e_shentsize*itab), SEEK_SET);
		nbc = fread( &strtab, header.e_shentsize, 1, file );
		if(nbc != 1){
			if(feof(file)){
				/* End of file */
			}else{
				debug("Erreur de lecture.");
			}
		}

		if(strcmp(".dynstr", sectionNames[itab]) == 0)
			break;
	}
	fseek( file, 0, SEEK_SET );
	nbc = fread( &header , sizeof(Elf32_Ehdr), 1, file);
	if(nbc != 1){
		if(feof(file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

        fseek( file, 0, SEEK_SET );
	for(i = 0; i < header.e_shnum; i++){
		fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
		nbc = fread( &dyn, header.e_shentsize, 1, file );
		if(nbc != 1){
			if(feof(file)){
				/* End of file */
			}else{
				debug("Erreur de lecture.");
			}
		}

		if(strcmp(".dynsym", sectionNames[i]) == 0){
                        ok=1;
			break;
                }
	}
	if(ok){
            fseek( file, 0, SEEK_SET );
            for ( i=0; i <header.e_shnum; i++ )
            {
                    fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
                    nbc = fread( &tmp, header.e_shentsize, 1, file );
                    if(nbc != 1){
						if(feof(file)){
							/* End of file */
						}else{
							debug("Erreur de lecture.");
						}
					}
                    if (i == itab){
                            strtab = (char *)malloc(tmp.sh_size);
                            fseek( file, tmp.sh_offset, SEEK_SET);
                            nbc = fread( strtab, tmp.sh_size, 1, file);
                            if(nbc != 1){
								if(feof(file)){
									/* End of file */
								}else{
									debug("Erreur de lecture.");
								}
							}
                            i=header.e_shnum+1;
                    }
            }
            fseek( file, 0, SEEK_SET );
            fseek(file, dyn.sh_offset,SEEK_SET);
            printf("Table de symboles « .dynsym » contient %ld entrées:\n",dyn.sh_size/sizeof(symbole));
            printf("Num : Valeur Tail   Type    Lien      Vis      Ndx    Nom\n");
            for(i=0; i < dyn.sh_size; i+=sizeof(symbole)){
                    nbc = fread(&symbole, sizeof(symbole), 1, file);
                    if(nbc != 1){
						if(feof(file)){
							/* End of file */
						}else{
							debug("Erreur de lecture.");
						}
					}
                    displaySymbole(symbole,strtab,i);
            }
            
        }
        ok=0;
	// Affichage des symboles de .symtab
       fseek( file, 0, SEEK_SET );
	for(i = 0; i < header.e_shnum; i++){
		fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
		nbc = fread( &sym, header.e_shentsize, 1, file );
		if(nbc != 1){
			if(feof(file)){
				/* End of file */
			}else{
				debug("Erreur de lecture.");
			}
		}
		if(strcmp(".symtab", sectionNames[i]) == 0){
                        ok=1;
			break;
                }
	}
        if(ok){
            fseek( file, 0, SEEK_SET );
            for(itab = 0; itab < header.e_shnum; itab++){
                    fseek( file, header.e_shoff+(header.e_shentsize*itab), SEEK_SET);
                    nbc = fread( &strtab, header.e_shentsize, 1, file );
                    if(nbc != 1){
						if(feof(file)){
							/* End of file */
						}else{
							debug("Erreur de lecture.");
						}
					}
                    if(strcmp(".strtab", sectionNames[itab]) == 0)
                            break;
            }
            fseek( file, 0, SEEK_SET );
            for ( i=0; i <header.e_shnum; i++ )
            {
                    fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
                    nbc = fread( &tmp, header.e_shentsize, 1, file );
                    if(nbc != 1){
						if(feof(file)){
							/* End of file */
						}else{
							debug("Erreur de lecture.");
						}
					}
                    if (i == itab){
                            strtab = (char *)malloc(tmp.sh_size);
                            fseek( file, tmp.sh_offset, SEEK_SET);
                            nbc = fread( strtab, tmp.sh_size, 1, file);
                            if(nbc != 1){
								if(feof(file)){
									/* End of file */
								}else{
									debug("Erreur de lecture.");
								}
							}
                            i=header.e_shnum+1;
                    }
            }	
            printf("\n\n");
            printf("Table de symboles « .symtab » contient %ld entrées:\n",sym.sh_size/sizeof(symbole));
            printf("Num : Valeur Tail   Type    Lien      Vis      Ndx    Nom\n");
            fseek(file, sym.sh_offset,SEEK_SET);	
            for(i=0; i < sym.sh_size; i+=sizeof(symbole)){
                    nbc = fread(&symbole, sizeof(symbole), 1, file);
                    if(nbc != 1){
						if(feof(file)){
							/* End of file */
						}else{
							debug("Erreur de lecture.");
						}
					}
                    displaySymbole(symbole,strtab,i);
            }
        }
	
	free(strtab);
	
}



//Affiche les tables de réimplantation 
void displayRelocTable(FILE* file, Elf32_Ehdr *header, Elf32_Shdr* sections, char * sectionNames[]){
	printf("********************\n");
	printf("* Relocation table *\n");
	printf("********************\n");
	int i,j,nbc;
	Elf32_Rel relcel;
	Elf32_Rela relacel;
	for(i=0;i<header->e_shnum;i++){
		Elf32_Shdr sectionHeader = sections[i];
		if (sectionHeader.sh_type==SHT_REL){
			int nbEnt = sectionHeader.sh_size / sectionHeader.sh_entsize;
			fseek(file,(int)sectionHeader.sh_offset,SEEK_SET);
			printf("Relocation section %s at offset 0x3e0 contains %d entries:\n", sectionNames[i], nbEnt); 
			printf("---------------------------------------------------------\n"); 
			printf(" Offset     Info         Type            Sym.\n");
			printf("---------------------------------------------------------\n"); 
			for(j = 0; j < nbEnt;j++){
				nbc = fread(&relcel,sizeof(Elf32_Rel),1,file);
				if(nbc != 1){
					if(feof(file)){
						/* End of file */
					}else{
						debug("Erreur de lecture.");
					}
				}
				printf("%08x  %08x  %17s  %6d\n",relcel.r_offset, relcel.r_info,getTypeRealoc(ELF32_R_TYPE(relcel.r_info)),ELF32_R_SYM(relcel.r_info));
 			}			
			printf("---------------------------------------------------------\n"); 
			printf("\n");
		} else if (sectionHeader.sh_type==SHT_RELA){
			int nbEnt = sectionHeader.sh_size / sectionHeader.sh_entsize;
			fseek(file,(int)sectionHeader.sh_offset,SEEK_SET);
			printf("Relocation section %s at offset 0x3e0 contains %d entries:\n", sectionNames[i], nbEnt); 
			printf("---------------------------------------------------------\n"); 
			printf(" Offset     Info         Type             Sym. + Addend\n");
			printf("---------------------------------------------------------\n"); 
			for(j = 0; j < nbEnt;j++){
				nbc = fread(&relacel,sizeof(Elf32_Rela),1,file);
				if(nbc != 1){
					if(feof(file)){
						/* End of file */
					}else{
						debug("Erreur de lecture.");
					}
				}
				printf("%08x  %08x  %17s  %6d + %d\n",relacel.r_offset, relacel.r_info,getTypeRealoc(ELF32_R_TYPE(relacel.r_info)),ELF32_R_SYM(relacel.r_info),relacel.r_addend);
 			}			
			printf("---------------------------------------------------------\n"); 
			printf("\n");
		}
		
	}
}

int main(int argc, char* argv[]){
	
	/* Récupération des arguments */
	int opt, nbc;
	
	char arg_elf_header=0, arg_program_headers=0, arg_section_headers=0, arg_symbols=0, arg_dyn_syms=0, arg_notes=0, arg_relocs=0, /*arg_use_dynamics=0,*/ arg_hexdump=0, arg_string_dump=0;
	char * arg_section;
	int num_section = -1;
	FILE *file;

	struct option longopts[] = {
		{ "all", no_argument, NULL, 'a' },
		{ "file-header", no_argument, NULL, 'h' },
		{ "segments", no_argument, NULL, 'l' },
		{ "section-headers", no_argument, NULL, 'S' },
		{ "sections", no_argument, NULL, 'S' },
		{ "headers", no_argument, NULL, 'e' },
		{ "syms", no_argument, NULL, 's' },
		{ "symbols", no_argument, NULL, 's' },
		{ "dyn-syms", no_argument, NULL, 'y' },
		{ "relocs", no_argument, NULL, 'r' },
		{ "use-dynamic", no_argument, NULL, 'D' },
		{ "hex-dump", required_argument, NULL, 'x' },
		{ "string-dump", required_argument, NULL, 'p' },

		{ "debug-file", required_argument, NULL, 'f' },
		{ "help", no_argument, NULL, '?' },
		{ NULL, 0, NULL, 0 }
	};

	while ((opt = getopt_long(argc, argv, "ahlSesyrDx:p:fx?", longopts, NULL)) != -1) {
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
		case 'y':
			arg_dyn_syms=1;
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
			arg_section = malloc(strlen(optarg));
			strcpy(arg_section, optarg);
			break;
		case 'p':
			arg_string_dump=1;
			arg_section = malloc(strlen(optarg));
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
		filename = malloc(strlen(argv[optind]));
		strcpy(filename, argv[optind]);

		file = fopen(filename, "rb");
       	if(!file){
			fprintf(stderr, "Erreur d'ouverture du fichier : %s\n", filename);
			return 0;
		}
	}
	else{
		usage(argv[0]);
		return 0;
	}

	/* ELF header */
	Elf32_Ehdr header;
	nbc = fread(&header, 1, sizeof(header), file);
	if(nbc != 1){
		if(feof(file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

	if(!(isELF(header))){
		fprintf(stderr, "Le fichier %s n'est pas au format ELF.\n", filename);
		return -1;
	}

	/* Section headers */
	Elf32_Shdr sectionHeaders[header.e_shnum];
	fseek(file, header.e_shoff, SEEK_SET);
	nbc = fread(sectionHeaders, header.e_shentsize, header.e_shnum, file);
	if(nbc != 1){
		if(feof(file)){
			/* End of file */
		}else{
			debug("Erreur de lecture.");
		}
	}

	/* Section names */
	char * sectionNames[header.e_shnum];
	getSectionNames(file, header, sectionHeaders, sectionNames);


	/* Execution des fonctions demandées */
	if(arg_elf_header){
		afficherHeader(header);	
	}
	if(arg_program_headers){
		printf("Option not available yet.\n");
	}
	if(arg_section_headers){
		displaySectionHeader(sectionHeaders, header, sectionNames);
	}
	if(arg_symbols){
		displayTableSymbole(file, sectionNames);
	}
	if(arg_dyn_syms){
		printf("Option not available yet.\n");
	}
	if(arg_notes){
		printf("Option not available yet.\n");
	}
	if(arg_relocs){
		displayRelocTable(file, &header, sectionHeaders, sectionNames);
	}
	if(arg_hexdump){
		if(isNumber(arg_section)){
			num_section = atoi(arg_section);
		}else{
			num_section = getSectionNumber(arg_section, sectionNames, header);
		}

		if(num_section > header.e_shnum || num_section < 0){
			fprintf(stderr, "Identifiant de section invalide : %s\n", arg_section);
		}else{
            displaySectionContentI(header, num_section, file, sectionNames);
		}
	}
	if(arg_string_dump){
		if(num_section > header.e_shnum || num_section < 0){
			fprintf(stderr, "Identifiant de section invalide : %s\n", arg_section);
		}else{
            printf("Option not available yet.\n");
		}
	}

	/* Memory free */
	fclose(file);


	return 0;
}
