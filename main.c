#include <elf.h>
#include <stdio.h>
#include <stdlib.h>



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

void afficherSection(Elf32_Shdr header){

	

}

// header de la section
void afficherSectionContenu(Elf32_Ehdr header, Elf32_Shdr sectionHeader, FILE* file){

	// printf("Affichage hexadécimal de la section « %s »\n", sectionHeader.sh_name);
	printf("Affichage hexadécimal de la section « »\n");
	uint64_t mot;
	int ligne = sectionHeader.sh_offset;
	fseek(file, sectionHeader.sh_offset, SEEK_SET);
	
	int i;
	for(i=0; i<sectionHeader.sh_size; i+=64){	
		fread(&mot, 1, 64, file);
		printf("0x%08x %x \n", ligne, mot);
		ligne += 64;
	}
}
void afficherSectionName(FILE * file, Elf32_Ehdr header){
	Elf32_Shdr sectionHeader,ITERheader;
	fseek( file, 0, SEEK_SET );
	fread( &header , sizeof(Elf32_Ehdr), 1, file);
	char * STR_buffer=NULL;
	int i;
	for ( i=0; i < header.e_shnum; i++  ){
		fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
		fread( &sectionHeader, header.e_shentsize, 1, file );
		if ((sectionHeader.sh_type == SHT_STRTAB) && (sectionHeader.sh_addr == 0x00000000)){
			STR_buffer = (char *)malloc( sectionHeader.sh_size);
			fseek( file, sectionHeader.sh_offset, SEEK_SET);
			fread( STR_buffer, sectionHeader.sh_size, 1, file);
			i=header.e_shnum+1;
		}
	}
	
	fseek(file, 0, SEEK_SET);
	for ( i=0; i < header.e_shnum; i++ )
	{
		fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
		fread( &ITERheader, header.e_shentsize, 1, file );
		printf("%d %d %s\n", i, ITERheader.sh_name, STR_buffer+ITERheader.sh_name);
	}

	free(STR_buffer);
	fclose( file );
}


int main(int argc, char* argv[]){
	if(argc==2){
		Elf32_Ehdr header;
		FILE* file = fopen(argv[1],"rb");
		fread(&header,1,sizeof(header),file);
/*		Elf32_Shdr sectionHeaders[header.e_shnum];

		fseek(file,header.e_shoff ,SEEK_SET);
		int i;
		for(i=0;i<header.e_shnum;i++){
			fread(&sectionHeaders[i], 1, sizeof(Elf32_Shdr), file);
		}

		afficherHeader(header);
			afficherSection(section);
			afficherSectionContenu(header, section, file);*/
		afficherSectionName(file, header);
	}
	else{
		printf("Ne donnez qu'un seul argument");
	}

}
