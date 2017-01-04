#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
void afficherSectionContenu(Elf32_Ehdr header, int j, FILE* file){
	Elf32_Shdr sectionHeader,ITERheader;
	fseek( file, 0, SEEK_SET );
	fread( &header , sizeof(Elf32_Ehdr), 1, file);
	char * STR_buffer=NULL;
	int i;
	for ( i=0; i < header.e_shnum; i++){
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
	for ( i=0; i <= j; i++ )
	{
		fseek( file, header.e_shoff+(header.e_shentsize*i), SEEK_SET);
		fread( &ITERheader, header.e_shentsize, 1, file );
	}


	printf("Affichage hexadécimal de la section « %s »\n", STR_buffer+ITERheader.sh_name);
	int ligne = ITERheader.sh_addr;	
	fseek(file, ITERheader.sh_offset, SEEK_SET);
	for(i=0;i < ITERheader.sh_size; i+=16){
		printf("Ox%08x : ",ligne);
		int k;
		char * convert;
		convert = (char *) malloc(1);		
		for(k=0;k<16;k++){
			if(ITERheader.sh_size > i+k){
				unsigned char mot;
				fread(&mot, sizeof(mot),1, file);
				if (isprint(mot))			
					convert[k] = mot;
				else
					convert[k] = '.';							
				if(k % 4 == 0)
					printf(" %02x",mot);
				else
					printf("%02x",mot);
				convert = realloc(convert, k+1);
			}
		}
		printf(" | %s\n",convert);		
		ligne += 16;	
	}
	free(STR_buffer);
	fclose( file );
}

void afficherSectionHeader(Elf32_Shdr* sectionH, int i){
	char* type;
	switch(sectionH->sh_type){
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
	char flags;
	switch(sectionH->sh_flags){
		case SHF_WRITE :
			flags = 'W';
			break;
		case SHF_ALLOC :
			flags = 'A';
			break;
		case SHF_EXECINSTR :
			flags = 'E';
			break;
		case SHF_MASKPROC :
			flags = 'M';
			break;
		default : 
			flags = ' ';
			break;
	}

	printf("[%d]\t%s\t%s\t%08x\t%08x\t%d\t%02x\t%c\t%d\t%d\t%d\n",i,"NAME",type, sectionH->sh_addr, sectionH->sh_offset, sectionH->sh_size, sectionH->sh_entsize, flags, sectionH->sh_link, sectionH->sh_info, sectionH->sh_addralign);
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

void afficherRelocTable(FILE* file, Elf32_Ehdr *header){
	printf("\nRelocation table:\n");
	int i,j;
	Elf32_Shdr sectionHeader;
	Elf32_Rel relcel;
	Elf32_Rela relacel;
	for(i=0;i<header->e_shnum;i++){
		fseek(file,(int)header->e_shoff+(i*sizeof(Elf32_Shdr)),SEEK_SET);	
		fread(&sectionHeader,1,sizeof(Elf32_Shdr),file);
		if (sectionHeader.sh_type==SHT_REL){
			printf("OK\n");
			int nbEnt = sectionHeader.sh_size / sectionHeader.sh_entsize;
			fseek(file,(int)sectionHeader.sh_offset,SEEK_SET);
			printf("Relocation section %s at offset 0x3e0 contains %d entries:\n", "NAME", nbEnt); 
			printf("  Offset    Info      Type        Sym.\n");
			for(j = 0; j < nbEnt;j++){
				fread(&relcel,sizeof(Elf32_Rel),1,file);
				printf("%08x  %08x  %10d  %x\n",relcel.r_offset, relcel.r_info,ELF32_R_TYPE(relcel.r_info),ELF32_R_SYM(relcel.r_info));
 			}			
			printf("\n");
		} else if (sectionHeader.sh_type==SHT_RELA){
			printf("OK\n");
			int nbEnt = sectionHeader.sh_size / sectionHeader.sh_entsize;
			fseek(file,(int)sectionHeader.sh_offset,SEEK_SET);
			printf("Relocation section %s at offset 0x3e0 contains %d entries:\n", "NAME", nbEnt); 
			printf("  Offset    Info      Type        Sym. + Addend\n");
			for(j = 0; j < nbEnt;j++){
				fread(&relacel,sizeof(Elf32_Rela),1,file);
				printf("%08x  %08x  %10d  %x + %d\n",relacel.r_offset, relacel.r_info,ELF32_R_TYPE(relacel.r_info),ELF32_R_SYM(relacel.r_info),relacel.r_addend);
 			}			
			printf("\n");
		}
		
	}
	
}

int main(int argc, char* argv[]){
	if(argc==2){
		Elf32_Ehdr header;
		FILE* file = fopen(argv[1],"rb");
		fread(&header,1,sizeof(header),file);

		afficherHeader(header);
		fseek(file,(int)header.e_shoff,SEEK_SET);
		int i;
		Elf32_Shdr sectionHeader;
		printf("%d\n",header.e_shoff);
		printf("[Nr]\tNom\tType\tAdr\t\tDecala.\t\tTaille\tES\tFan\tLN\tInf\tAl\n");
		for(i=0;i<header.e_shnum;i++){
			fread(&sectionHeader,1,sizeof(Elf32_Shdr),file);
			afficherSectionHeader(&sectionHeader,i);
		}

		//afficherSectionContenu(header, 3, file);
		//afficherSectionName(file, header);
		afficherRelocTable(file,&header);
	}
	else{
		printf("Ne donnez qu'un seul argument");
	}

}
