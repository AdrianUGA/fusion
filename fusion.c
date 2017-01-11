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
    content = malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
    f3->sectionNames = (char **)malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
    f3->sectionHeaders = malloc(sizeof(Elf32_Shdr)*(f1.header.e_shnum + f2.header.e_shnum));
    int i,z;
    int size = 0;
    for(i=0; i<f1.header.e_shnum; i++){
        symb[i] = -1;
        size++;
        memcpy(&f3->sectionHeaders[i], &f1.sectionHeaders[i], sizeof(Elf32_Shdr));
        
                            
        f3->sectionNames[i] = (char *) malloc(sizeof(char)*strlen(f1.sectionNames[i]));
        memcpy(f3->sectionNames[i], f1.sectionNames[i],sizeof(char)*strlen(f1.sectionNames[i]));
        
        content[i] = malloc(f3->sectionHeaders[i].sh_size);
        getSectionContent(&f1,i,content[i]);
         
    }
    int k;
    for(i=1; i<f2.header.e_shnum;i++){
        if(f2.sectionHeaders[i].sh_type == SHT_ARM_ATTRIBUTES || f2.sectionHeaders[i].sh_type == SHT_NOBITS);
        else if(f2.sectionHeaders[i].sh_type != SHT_PROGBITS && f2.sectionHeaders[i].sh_type != SHT_STRTAB && f2.sectionHeaders[i].sh_type != SHT_SYMTAB && f1.sectionHeaders[i].sh_type != SHT_REL){
                                
            memcpy(&f3->sectionHeaders[size], &f2.sectionHeaders[i], sizeof(Elf32_Shdr));
            //maj sh_name
            f3->sectionHeaders[size].sh_name += f1.sectionHeaders[f1.header.e_shstrndx].sh_size;
            
            f3->sectionNames[size] = malloc(sizeof(char)*strlen(f2.sectionNames[i]));
            memcpy(f3->sectionNames[size], f2.sectionNames[i],sizeof(char)*strlen(f2.sectionNames[i]));
            printf("%s inséré | %s | %s\n",f3->sectionNames[size],f3->sectionNames[0],f3->sectionNames[1]);
            
             content[size] = (char *)malloc(f3->sectionHeaders[size].sh_size);
             getSectionContent(&f2,i,content[size]);
             
        printf("\n");
            size++;
        } else {
            int j, modif=1;
            for(j=0; j<f1.header.e_shnum && modif;j++){
                if(strcmp(f1.sectionNames[j],f2.sectionNames[i]) == 0 && ( f1.sectionHeaders[j].sh_type == SHT_PROGBITS || f1.sectionHeaders[j].sh_type == SHT_SYMTAB || f1.sectionHeaders[j].sh_type == SHT_STRTAB || f1.sectionHeaders[j].sh_type == SHT_REL)){
                    modif = 0;
                    symb[j] = i;
                                        
                    //maj size
                    f3->sectionHeaders[j].sh_size += f2.sectionHeaders[i].sh_size;
                    printf("%s fusionné | %s | %s\n",f3->sectionNames[j],f3->sectionNames[0],f3->sectionNames[1]);
                    
                    if(f3->sectionHeaders[j].sh_type == SHT_PROGBITS || f3->sectionHeaders[j].sh_type == SHT_STRTAB){
                        int taille = f1.sectionHeaders[j].sh_size;
                        int taille2 = f2.sectionHeaders[i].sh_size;
                        char * tmp = malloc(taille + taille2);
                        char * tmp2 = malloc(taille2);
                        memcpy(tmp, content[j], taille);
//                         printf("%d\n",taille);/*
                        
                        getSectionContent(&f2,i,tmp2);
                        
                        memcpy(tmp+taille, tmp2, taille2);
                        content[j] = malloc(taille+taille2);
                        memcpy(content[j],tmp,taille+taille2);
                        free(tmp);
                        free(tmp2);
                    }
                }
            }
            if(modif==1){
                
                memcpy(&f3->sectionHeaders[size], &f2.sectionHeaders[i], sizeof(Elf32_Shdr));
                //maj sh_name
                f3->sectionHeaders[size].sh_name += f1.sectionHeaders[f1.header.e_shstrndx].sh_size;
                
                f3->sectionNames[size] = malloc(sizeof(char)*strlen(f2.sectionNames[i]));
                memcpy(f3->sectionNames[size], f2.sectionNames[i], sizeof(char)*strlen(f2.sectionNames[i]));
               
                printf("%s inséré | %s | %s\n",f3->sectionNames[size],f3->sectionNames[0],f3->sectionNames[1]);
                                    
                content[size] = malloc(f3->sectionHeaders[size].sh_size);
                printf("Avant bug : %s \n",f3->sectionNames[0]);
                getSectionContent(&f2,i,content[size]);
                printf("Après bug : %s \n",f3->sectionNames[0]);
                size++;
                
            }
        }
    }
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
    char ** content;
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
    
    printf("\n\n\n");
    
    elf_t * fichier3;
    fichier3 = malloc(sizeof(elf_t));
    fichier3->file = f3;
    
    
    getHeader(fichier1,fichier1,fichier3);
    
    
    
    int symb[fichier1.header.e_shnum];
    getSection(fichier1,fichier2,fichier3,content,symb);
            
            
            
    displayHeader(fichier3);
            
    displaySectionHeaders(fichier3);
    
    printf("\n\n\n");
    printf("fini\n");
}
