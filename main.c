#include <elf.h>
#include <stdio.h>



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


int main(int argc, char* argv[]){
	if(argc==2){
		Elf32_Ehdr header;
		FILE* file = fopen(argv[1],"rb");
		fread(&header,1,sizeof(header),file);
		afficherHeader(header);
		fseek(file,header.e_shoff ,SEEK_SET);
		int i;
		Elf32_Shdr section;
		for(i=0;i<header.e_shnum;i++){
			fread(&section,1,sizeof(Elf32_Shdr));
			afficherSection(section);
		}
	}
	else{
		printf("Ne donnez qu'un seul argument");
	}

}