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


         Elf32_Shdr * getSection(FILE * f1, FILE * f2,Elf32_Ehdr * header1, Elf32_Ehdr * header2, Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[], char * sectionNames1[], char * sectionNames2[],char** resChar , char ** content, int * size ){
                Elf32_Shdr * res;
                res = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr)*header1->e_shnum);
                int i,l=1;
                *size = 0;
                for(i=0; i<header1->e_shnum; i++){
                    res[i] = sectionHeaders1[i];
                    *size=*size+1;
                    resChar[i] = (char *) malloc(sizeof(sectionNames1[i]));
                    resChar[i] = sectionNames1[i];
                    content[i] = (char *)malloc(res[i].sh_size);
                    getSectionContent(f1, res[i], content[i]);
                    //Pas de MaJ

                }
                for(i=1; i<header2->e_shnum;i++){
                    if(sectionHeaders2[i].sh_type != SHT_PROGBITS && sectionHeaders2[i].sh_type != SHT_STRTAB && sectionHeaders2[i].sh_type != SHT_SYMTAB){
                        res = realloc(res,l);
                        res[header1->e_shnum+(l-1)] = sectionHeaders2[i];
                        //MaJ sur les sh_name
                        res[header1->e_shnum+(l-1)].sh_name += header1->e_shnum;
                        //MaJ sur les offset
                        int k;
                        for(k=0; k<(*size+1);k++){
                            if(res[k].sh_offset > res[header1->e_shnum+(l-1)].sh_offset){
                                res[k].sh_offset += res[header1->e_shnum+(l-1)].sh_size;
                            }
                        }
                        
                        printf("On insère %s a l'indice %d\n", sectionNames2[i], header1->e_shnum+(l-1));
                        resChar[header1->e_shnum+(l-1)] = sectionNames2[i];
                        
                        content[header1->e_shnum+(l-1)] = (char *)malloc(res[header1->e_shnum+(l-1)].sh_size);
                        getSectionContent(f2, res[header1->e_shnum+(l-1)], content[header1->e_shnum+(l-1)]);
                        
                        l++;
                        *size=*size+1;
                    }
                }
                for(i=1; i<header2->e_shnum;i++){
                    if(!(sectionHeaders2[i].sh_type != SHT_PROGBITS && sectionHeaders2[i].sh_type != SHT_STRTAB && sectionHeaders2[i].sh_type != SHT_SYMTAB)) {
                        int j, modif=1;
                        for(j=0; j<header1->e_shnum && modif;j++){
                            if(strcmp(sectionNames1[j],sectionNames2[i]) == 0 && ( sectionHeaders1[j].sh_type == SHT_PROGBITS || sectionHeaders1[j].sh_type == SHT_SYMTAB || sectionHeaders1[j].sh_type == SHT_STRTAB )){
                                modif = 0;
                                //MaJ sur les sh_size
                                res[j].sh_size += sectionHeaders2[i].sh_size;
                                //MaJ sur les offset
                                int k;
                                for(k=0; k<(*size);k++){
                                    if(res[k].sh_offset > sectionHeaders2[i].sh_offset){
                                        res[k].sh_offset += sectionHeaders2[i].sh_size;
                                    }
                                }
                                printf("fusion : %s\n",sectionNames2[i]);
                                
                            }
                        }
                        if(modif){
                            res = realloc(res,l);
                            res[header1->e_shnum+(l-1)] = sectionHeaders2[i];
                            
                            //MaJ sur les sh_name
                            res[header1->e_shnum+(l-1)].sh_name += header1->e_shnum;
                            //MaJ sur les offset
                            int k;
                            for(k=0; k<(*size+1);k++){
                                if(res[k].sh_offset > res[header1->e_shnum+(l-1)].sh_offset){
                                    res[k].sh_offset += res[header1->e_shnum+(l-1)].sh_size;
                                }
                            }
                            
                            printf("On insère2 %s a l'indice %d\n",sectionNames2[i], header1->e_shnum+(l-1));
                            resChar[header1->e_shnum+(l-1)] = sectionNames2[i];
                            
                            content[header1->e_shnum+(l-1)] = (char *)malloc(res[header1->e_shnum+(l-1)].sh_size);
                            getSectionContent(f2, res[header1->e_shnum+(l-1)], content[header1->e_shnum+(l-1)]);
                            
                            *size=*size+1;
                            l++;
                        }
                    }
                }
                for(i=1; i<header2->e_shnum;i++){
                    if(!(sectionHeaders2[i].sh_type != SHT_PROGBITS && sectionHeaders2[i].sh_type != SHT_STRTAB && sectionHeaders2[i].sh_type != SHT_SYMTAB)) {
                        int j;
                        for(j=0; j<header1->e_shnum;j++){
                            if(strcmp(sectionNames1[j],sectionNames2[i]) == 0 && ( sectionHeaders1[j].sh_type == SHT_PROGBITS || sectionHeaders1[j].sh_type == SHT_SYMTAB || sectionHeaders1[j].sh_type == SHT_STRTAB )){
                                char * tmp = (char *)malloc(sectionHeaders2[i].sh_size+sizeof(content[j]));
                                tmp = content[j];
                                char * tmp2 = (char *)malloc(sectionHeaders2[i].sh_size);
                                getSectionContent(f2, sectionHeaders2[i], tmp2);
                                tmp = strcat(tmp,tmp2);
                                content[j] = (char *)malloc(sizeof(tmp));
                                content[j] = tmp;
                             //   printf("%x %x\n",tmp,tmp2);
                                //free(tmp); free(tmp2);
                            }
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
