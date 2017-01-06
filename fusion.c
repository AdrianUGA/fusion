#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "fusion.h"
#include "display.h"
#include <string.h>

void concatHeader(FILE * f1, FILE * f2, FILE * f3,Elf32_Ehdr * newHeader, Elf32_Ehdr * header){ 
    newHeader->e_shnum = newHeader->e_shnum + header->e_shnum;
    newHeader->e_phnum = newHeader->e_phnum + header->e_phnum;
  //  printf("Magique : %x %c%c%c %d et %d\n",newHeader->e_ident[0],newHeader->e_ident[1],newHeader->e_ident[2],newHeader->e_ident[3] , newHeader->e_shnum,header->e_shnum);
    fseek(f3,0,SEEK_SET);
    fwrite(newHeader, sizeof(Elf32_Ehdr),1,f3);
}

/*Demander pour les attributs sh_link, sh_addr, sh_addralign, fonctionnement lors de la fusion*/
/*void concatSectionHeader(Elf32_Ehdr * newHeader, Elf32_Ehdr * header, Elf32_Ehdr * tmp, char * sectionNames1[],char * sectionNames2[],Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[], FILE * f3){
    int passe[header->e_shnum];
    int i,j,res=0;
    fseek(f3,sizeof(newHeader),SEEK_SET);
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
                        if(sectionHeaders1[k].sh_offset >= sectionHeaders1[i].sh_offset)
                            sectionHeaders1[k].sh_offset += sectionHeaders2[j].sh_size;
                    }
                    //Maj des flags
                    sectionHeaders1[i].sh_flags |= sectionHeaders2[j].sh_flags;
                    //Ecriture de l'header de section dans f3
                    fwrite(&sectionHeaders1[i], sizeof(sectionHeaders1[i]),1,f3);
                    passe[j] = -1;
                    res++;
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
                if(sectionHeaders1[j].sh_offset >= sectionHeaders2[i].sh_offset)
                    sectionHeaders1[j].sh_offset += sectionHeaders2[i].sh_size;
            }
            for(j=0; j<header->e_shnum; j++){
                if(sectionHeaders2[j].sh_offset >= sectionHeaders2[i].sh_offset)
                    sectionHeaders2[j].sh_offset += sectionHeaders2[i].sh_size;
            }
            
        }
        fwrite(&sectionHeaders2[i], sizeof(sectionHeaders2[i]),1,f3);
    }
    Elf32_Ehdr * tmp;
    fseek(f3,0,SEEK_SET);
    fread(&tmp,sizeof(Elf32_Ehdr),1,f3);
    tmp->e_shnum -= res;
}*/

Elf32_Shdr * getSectionHeader(Elf32_Ehdr * header1, Elf32_Ehdr * header2, Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[], char * sectionNames1[], char * sectionNames2[],char** resChar ,int * size ){
        Elf32_Shdr * res;
        res = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr)*header1->e_shnum);
        int i,l=1;
        *size = 0;
        for(i=0; i<header1->e_shnum; i++){
            res[i] = sectionHeaders1[i];
            *size=*size+1;
            resChar[i] = (char *) malloc(sizeof(sectionNames1[i]));
            resChar[i] = sectionNames1[i];

        }
        for(i=0; i<header2->e_shnum;i++){
            if(sectionHeaders2[i].sh_type != SHT_PROGBITS){
                res = realloc(res,l);
                res[header1->e_shnum+(l-1)] = sectionHeaders2[i];
                resChar[header1->e_shnum+(l-1)] = (char *) malloc(sizeof(sectionNames1[i]));
                resChar[header1->e_shnum+(l-1)] = sectionNames2[i];
                l++;
                *size=*size+1;
            } else {
                int j, modif=1;
                for(j=0; j<header1->e_shnum;j++){
                    if(sectionNames1[j] == sectionNames2[i] && sectionHeaders1[j].sh_type == SHT_PROGBITS){
                        modif = 0;
                    }
                }
                if(modif){
                    
                    res = realloc(res,l);
                    res[header1->e_shnum+(l-1)] = sectionHeaders2[i];
                    res[header1->e_shnum+(l-1)] = sectionHeaders2[i];
                    resChar[header1->e_shnum+(l-1)] = (char *) malloc(sizeof(sectionNames1[i]));
                     resChar[header1->e_shnum+(l-1)] = sectionNames2[i];
                    *size=*size+1;
                    l++;
                }
            }
        }
        return res;
}
/*void concatSection(FILE * f1, FILE * f2, FILE * f3, char * sectionNames1[],char * sectionNames2[],Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[],Elf32_Ehdr * newHeader, Elf32_Ehdr * header){
    //Concatener les sections
    int i;
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
            int k;
            fseek(f1, newHeader->e_shoff+(newHeader->e_shentsize*i), SEEK_SET);
            for(k=0; k < newHeader->e_shnum; k++)
                fread(&section1,sizeof(section1),1,f1);
            section2.sh_offset += section1.sh_offset;
            section2.sh_size += section1.sh_size;
            char * tmp;
            getSectionContent(f2,section2,tmp);
            fwrite(tmp,sizeof(tmp),1,f1);
        }
    }
    printf("section\n");
    
}*/



int main(int argc, char * argv[]){
    FILE * f1;
    FILE * f2;
    FILE * f3;
    f1 = fopen(argv[1],"rb");
    f2 = fopen(argv[2],"rb");
    f3 = fopen(argv[3],"wb+");
    printf("ouvert\n");
    
    
    
    //header, tab header et tab name pour f1
    Elf32_Ehdr newHeader;
    fread(&newHeader, 1, sizeof(newHeader), f1);
    Elf32_Shdr sectionHeaders1[newHeader.e_shnum];
    fseek(f1, newHeader.e_shoff, SEEK_SET);
    fread(sectionHeaders1, newHeader.e_shentsize, newHeader.e_shnum, f1);
    char * sectionNames1[newHeader.e_shnum];
    getSectionNames(f1, newHeader, sectionHeaders1, sectionNames1);
        
    
    //header, tab header et tab name pour f2
    Elf32_Ehdr header;
    fread(&header, 1, sizeof(header), f2);
    Elf32_Shdr sectionHeaders2[header.e_shnum];
    fseek(f2, header.e_shoff, SEEK_SET);
    fread(sectionHeaders2, header.e_shentsize, header.e_shnum, f2);
    char * sectionNames2[header.e_shnum];
    getSectionNames(f2, header, sectionHeaders2, sectionNames2);
    
    
    //concatHeader(f1,f2,f3,&newHeader, &header);
    
    
    Elf32_Ehdr tmp;
    fseek(f3,0,SEEK_SET);
    fread(&tmp, sizeof(tmp), 1,f3);
    //concatSectionHeader(&newHeader, &header, &tmp, sectionNames1, sectionNames2, sectionHeaders1,sectionHeaders2,f3);
    int t=0, cpt = 0;
    char * resChar[newHeader.e_shnum+header.e_shnum];
    Elf32_Shdr * test = getSectionHeader(&newHeader, &header, sectionHeaders1, sectionHeaders2, sectionNames1, sectionNames2, resChar,&t);
    printf("%d\n",t);
    for(cpt;cpt<t;cpt++){
            printf("%d et %d\n", test[cpt].sh_type,cpt);
    }
    
   /* Elf32_Ehdr header3;
    fseek(f3,0,SEEK_SET);
    fread(&header3, sizeof(header),1, f3);*/
    //displayHeader(header3);
}