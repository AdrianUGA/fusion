#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "fusion.h"
#include "display.h"
#include <string.h>

/*De mander pour les attributs sh_link, sh_addr, sh_addralign, fonctionnement lors de la fusion*/
int fusionale(Elf32_Ehdr * newHeader, Elf32_Ehdr * header, FILE * f2,char * sectionNames1[],char * sectionNames2[],Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[], FILE * f3){
    int passe[header->e_shnum];
    int i,j,res=0;
    int offset=0;
    fseek(f6,sizeof(newHeader),SEEK_SET);
    for(i=0; i < newHeader->e_shnum;i++){
        if(sectionHeaders1[i].sh_type == SHT_PROGBITS){
            for(j=0; j<header->e_shnum; j++){
                if(strcmp(sectionNames1[i],sectionNames2[j]) == 0 && sectionHeaders2[j].sh_type == SHT_PROGBITS){
                    sectionHeaders1[i].sh_size += sectionHeaders2[j].sh_size;
                    //Compare entsize
                    if(sectionHeaders1[i].sh_entsize < sectionHeaders2[j].sh_entsize){
                        sectionHeaders1[i].sh_entsize = sectionHeaders2[j].sh_entsize;
                    }
                    int k;
                    //Maj des offset des sections suivant la section courante du fichier 1
                    for(k=0; k<newHeader->e_phnum; k++){
                        if(sectionHeaders1[k].sh_offset >= sectionHeaders1[i])
                            sectionHeaders1[k].sh_offset += sectionHeaders2[j].sh_size;
                    }
                    //Maj des flags
                    sectionHeaders1[i] |= sectionHeaders2[j];
                    //Ecriture de l'header de section dans f3
                    fwrite(&sectionHeaders1[i], sizeof(sectionHeaders1[i]),1,f3);
                    passe[j] = -1;
                } else if(passe[j] != -1){
                    passe[j] = 0;
                }
            }
        } else {
            //Ecriture de l'header de section dans f3
            fwrite(&sectionHeaders1[i], sizeof(sectionHeaders1[i]),1,f3);
        }
    }
    for(i=0; i< header->e_shnum; i++){
        if(passe[i] != -1){
            //Maj de tous les offset
            for(j=0; j<newHeader->e_shnum; j++){
                if(sectionHeaders1[j].sh_offset >= sectionHeaders2[i])
                    sectionHeaders1[j].sh_offset += sectionHeaders2[i].sh_size;
            }
            for(j=0; j<header->e_shnum; j++){
                if(sectionHeaders2[j].sh_offset >= sectionHeaders2[i])
                    sectionHeaders2[j].sh_offset += sectionHeaders2[i].sh_size;
            }
            
        }
        fwrite(&sectionHeaders2[i], sizeof(sectionHeaders[i]),1,f3);
    }
}

void concatSection(FILE * f1, FILE * f2, FILE * res, char * sectionNames1[],char * sectionNames2[],Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[]){
    printf("entre \n");
    //Nouvel header
    Elf32_Ehdr * newHeader;
    Elf32_Ehdr * header;
    fseek(f1, 0, SEEK_SET);
    fread(newHeader, sizeof(newHeader), 1, f1);
    fseek(f2, 0, SEEK_SET);
    fread(header, sizeof(header), 1, f2);
    int enCommun = 0,i,j;
    newHeader->e_shnum += header->e_shnum;
    newHeader->e_phnum += header->e_phnum;
    fseek(f1,0,SEEK_SET);
    fwrite(&newHeader, 1,sizeof(newHeader),f1);
    printf("Header\n");
    
    
    
    
    //Concatener les sections
 /*   int i;
    for(i=0; i < header->e_shnum; i++){
        Elf32_Shdr section1, section2;
        fread(&section2, sizeof(section2), 1, f2);
        int present=0,j;
        for(j=0; j<newHeader->e_shnum;j++){
            if(strcmp(sectionNames1[j],sectionNames2[i]) == 0) 
                present=1;
        }
        if(present){
            int k;
            fseek(f1, newHeader->e_ehsize, SEEK_SET);
            for(k=0; k <= j; k++)
                fread(&section1,sizeof(section1),1,f1);
            for(k=newHeader->e_ehsize-1; k>=j;){
                fseek(f1,newHeader->e_shoff+(newHeader->e_shentsize*(k-1)),SEEK_SET);
                fread(&section1,sizeof(section1),1,f1);
                section1.sh_offset += section2.sh_size;
                fseek(f1,newHeader->e_shoff+(newHeader->e_shentsize*i),SEEK_SET);
                char * tmp;
                getSectionContent(f1,section1,tmp);
                fwrite(tmp,sizeof(tmp),1,f1);
            }
            
        }else {
            int k;void displayRelocTable(FILE* file, Elf32_Ehdr *header);
            fseek(f1, newHeader->e_shoff+(newHeader->e_shentsize*i), SEEK_SET);
            for(k=0; k < newHeader->e_shnum; k++)->
                fread(&section1,sizeof(section1),1,f1);
            section2.sh_offset += section1.sh_offset;
            section2.sh_size += section1.sh_size;
            char * tmp;
            getSectionContent(f2,section2,tmp);void displayRelocTable(FILE* file, Elf32_Ehdr *header);
            fwrite(tmp,sizeof(tmp),1,f1);
        }
    }
    printf("section\n");*/
    
}

int main(int argc, char * argv[]){
    FILE * f1;
    FILE * f2;
    f1 = fopen(argv[1],"rb+");
    f2 = fopen(argv[2],"rb");
    printf("ouvert\n");
    Elf32_Ehdr header;
    fread(&header, 1, sizeof(header), f1);
    /* Section headers */
  
    concatSection(f1,f2/*,sectionNames,sectionNames2*/);
    
    fseek(f1,0,SEEK_SET);
    fread(&header, sizeof(header),1, f1);
    afficherHeader(header);
}