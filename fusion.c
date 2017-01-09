#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "fusion.h"
#include "display.h"
#include <string.h>

void getHeader(elf_t f1, elf_t f2, elf_t * f3){ 
       		Elf32_Ehdr newHeader;
		newHeader = f1.header;
       		newHeader.e_phnum = newHeader.e_phnum + f2.header.e_phnum;
        	f3->header = newHeader;
        }

        

void getSection(elf_t f1, elf_t f2, elf_t * f3, char ** content, int * symb){
		char ** resChar; resChar = malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
                f3->sectionHeaders = malloc(sizeof(Elf32_Shdr)*f3->header.e_shnum);
                int i,l=1;
                int size = 0;
                for(i=0; i<f1.header.e_shnum; i++){
                    symb[i] = -1;
                    f3->sectionHeaders[i] = f1.sectionHeaders[i];
                    size++;
                    
                    resChar[i] = (char *) malloc(sizeof(f1.sectionNames[i]));
                    resChar[i] = f1.sectionNames[i];
                    
                     content[i] = malloc(sizeof(char)*f3->sectionHeaders[i].sh_size);
                     getSectionContent(&f1,i,content[i]);

                }
                for(i=1; i<f2.header.e_shnum;i++){
                    if(f2.sectionHeaders[i].sh_type != SHT_PROGBITS && f2.sectionHeaders[i].sh_type != SHT_STRTAB && f2.sectionHeaders[i].sh_type != SHT_SYMTAB){
                        
                        f3->sectionHeaders[f1.header.e_shnum+(l-1)] = f2.sectionHeaders[i];
                        
                        resChar[f1.header.e_shnum+(l-1)] = f2.sectionNames[i];
                        
                         content[f1.header.e_shnum+(l-1)] = malloc(sizeof(char)*f3->sectionHeaders[f1.header.e_shnum+(l-1)].sh_size);
                         getSectionContent(&f2,i,content[f1.header.e_shnum+(l-1)]);
                        
                        l++;
                        size++;
                    } else {
                        int j, modif=1;
                        for(j=0; j<f1.header.e_shnum && modif;j++){
                            if(strcmp(f1.sectionNames[j],f2.sectionNames[i]) == 0 && ( f1.sectionHeaders[j].sh_type == SHT_PROGBITS || f1.sectionHeaders[j].sh_type == SHT_SYMTAB || f1.sectionHeaders[j].sh_type == SHT_STRTAB )){
                                modif = 0;
                                symb[j] = i;
                                char * tmp = malloc(sizeof(f2.sectionNames[i]));
                                getSectionContent(&f2,i,tmp);
                                resChar[f1.header.e_shnum+(l-1)] = realloc (resChar[f1.header.e_shnum+(l-1)],sizeof(tmp));
                                strcat(resChar[f1.header.e_shnum+(l-1)],tmp);
                            }
                        }
                        if(modif==1){
                            f3->sectionHeaders[f1.header.e_shnum+(l-1)] = f2.sectionHeaders[i];
                            
                            resChar[f1.header.e_shnum+(l-1)] = f2.sectionNames[i];
                            
                             content[f1.header.e_shnum+(l-1)] = malloc(sizeof(char)*f3->sectionHeaders[f1.header.e_shnum+(l-1)].sh_size);
                             getSectionContent(&f2,i,content[f1.header.e_shnum+(l-1)]);
                            
                            size++;
                            l++;
                        }
                    }
                }
		f3->sectionNames = resChar;
		f3->header.e_shnum = size;
        }




int main(int argc, char * argv[]){
    FILE * f1;
    FILE * f2;
    FILE * f3;
    f1 = fopen(argv[1],"rb");
    f2 = fopen(argv[2],"rb");
    f3 = fopen(argv[3],"wb+");
    printf("ouvert\n");
    
	elf_t fichier1;
	elf_t fichier2;
    
    //header, tab header et tab name pour f1
    Elf32_Ehdr newHeader;
    fread(&newHeader, 1, sizeof(newHeader), f1);
    Elf32_Shdr sectionHeaders1[newHeader.e_shnum];
    fseek(f1, newHeader.e_shoff, SEEK_SET);
    fread(sectionHeaders1, newHeader.e_shentsize, newHeader.e_shnum, f1);
    fichier1.header = newHeader;
    fichier1.file = f1;
    fichier1.sectionHeaders=sectionHeaders1;
    fichier1.sectionNames = malloc(sizeof(char *)*fichier1.header.e_shnum);
    getSectionNames(&fichier1);
    printf("file 1\n");    
    
    //header, tab header et tab name pour f2
    Elf32_Ehdr header;
    fread(&header, 1, sizeof(header), f2);
    Elf32_Shdr sectionHeaders2[header.e_shnum];
    fseek(f2, header.e_shoff, SEEK_SET);
    fread(sectionHeaders2, header.e_shentsize, header.e_shnum, f2);
    fichier2.header = header;
    fichier2.file = f2;
    fichier2.sectionHeaders=sectionHeaders2;
    fichier2.sectionNames = malloc(sizeof(char *)*fichier2.header.e_shnum);
    getSectionNames(&fichier2);
    printf("file 2\n");
    
    elf_t * fichier3;
    fichier3 = malloc(sizeof(elf_t));
    fichier3->file = f3;
    getHeader(fichier1,fichier1,fichier3);
    char ** content = malloc(sizeof(char *)*fichier3->header.e_shnum);
    int symb[fichier1.header.e_shnum];
    getSection(fichier1,fichier2,fichier3,content,symb);
    
    
    
    displayHeader(fichier3);
    
    displaySectionHeaders(fichier3);

}
