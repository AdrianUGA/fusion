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


void getSection(elf_t f1, elf_t f2, elf_t * f3, char ** content){ //FILE * f1, FILE * f2,Elf32_Ehdr * header1, Elf32_Ehdr * header2, Elf32_Shdr sectionHeaders1[], Elf32_Shdr sectionHeaders2[], char * sectionNames1[], char * sectionNames2[],char** resChar , char ** content, int * size ){
                Elf32_Shdr * res;
		char ** resChar; resChar = malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
                res = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr)*f1.header.e_shnum);
                int i,l=1;
                int size = 0;
                for(i=0; i<f1.header.e_shnum; i++){
                    res[i] = f1.sectionHeaders[i];
                    size++;
                    resChar[i] = (char *) malloc(sizeof(f1.sectionNames[i]));
                    resChar[i] = f1.sectionNames[i];
                    content[i] = malloc(sizeof(char)*res[i].sh_size);
                    getSectionContent(&f1,i,content[i]);
                    //Pas de MaJ

                }
                for(i=1; i<f2.header.e_shnum;i++){
                    if(f2.sectionHeaders[i].sh_type != SHT_PROGBITS && f2.sectionHeaders[i].sh_type != SHT_STRTAB && f2.sectionHeaders[i].sh_type != SHT_SYMTAB){
                        
                        res = realloc(res,l);
                        printf("test1\n");
                        res[f1.header.e_shnum+(l-1)] = f2.sectionHeaders[i];
                        //MaJ sur les sh_name
                        res[f1.header.e_shnum+(l-1)].sh_name += f1.header.e_shnum;
                        //MaJ sur les offset
                        int k;
                        for(k=0; k<(size+1);k++){
                            if(res[k].sh_offset > res[f1.header.e_shnum+(l-1)].sh_offset){
                                res[k].sh_offset += res[f1.header.e_shnum+(l-1)].sh_size;
                            }
                        }
                        
                        printf("On insère %s a l'indice %d\n", f2.sectionNames[i], f1.header.e_shnum+(l-1));
                        resChar[f1.header.e_shnum+(l-1)] = f2.sectionNames[i];
                        content[f1.header.e_shnum+(l-1)] = malloc(sizeof(char)*res[f1.header.e_shnum+(l-1)].sh_size);
                        getSectionContent(&f2,i,content[f1.header.e_shnum+(l-1)]);
                        l++;
                        size++;
                    } else {
                        int j, modif=1;
                        for(j=0; j<f1.header.e_shnum && modif;j++){
                            if(strcmp(f1.sectionNames[j],f1.sectionNames[i]) == 0 && ( f1.sectionHeaders[j].sh_type == SHT_PROGBITS || f1.sectionHeaders[j].sh_type == SHT_SYMTAB || f1.sectionHeaders[j].sh_type == SHT_STRTAB )){
                                modif = 0;
                                //MaJ sur les sh_size
                                res[j].sh_size += f2.sectionHeaders[i].sh_size;
                                //MaJ sur les offset
                                int k;
                                for(k=0; k<size;k++){
                                    if(res[k].sh_offset > f2.sectionHeaders[i].sh_offset){
                                        res[k].sh_offset += f2.sectionHeaders[i].sh_size;
                                    }
                                }
                                printf("fusion : %s\n",f2.sectionNames[i]);
                            }
                        }
                        if(modif){
                printf("test 2\n");
                            res = realloc(res,l);
                            res[f1.header.e_shnum+(l-1)] = f2.sectionHeaders[i];
                            
                            //MaJ sur les sh_name
                            res[f1.header.e_shnum+(l-1)].sh_name += f1.header.e_shnum;
                            //MaJ sur les offset
                            int k;
                            for(k=0; k<(size+1);k++){
                                if(res[k].sh_offset > res[f1.header.e_shnum+(l-1)].sh_offset){
                                    res[k].sh_offset += res[f1.header.e_shnum+(l-1)].sh_size;
                                }
                            }
                            
                            printf("On insère2 %s a l'indice %d\n",f2.sectionNames[i], f1.header.e_shnum+(l-1));
                            resChar[f1.header.e_shnum+(l-1)] = f2.sectionNames[i];
                            content[f1.header.e_shnum+(l-1)] = malloc(sizeof(char)*res[f1.header.e_shnum+(l-1)].sh_size);
                            getSectionContent(&f2,i,content[f1.header.e_shnum+(l-1)]);                       
                            size++;
                            l++;
                        }
                    }
                }
                printf("test 3\n");
		f3->sectionHeaders = res;
		f3->sectionNames = resChar;
		f3->header.e_shnum = size;
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
    getSection(fichier1,fichier1,fichier3,content);
    printf("est passé\n");
    
    displayHeader(fichier3);
    displaySectionHeaders(fichier3);
    
    //concatHeader(f1,f2,f3,&newHeader, &header);
    
    
    /*Elf32_Ehdr tmp;
    fseek(f3,0,SEEK_SET);
    fread(&tmp, sizeof(tmp), 1,f3);
    //concatSectionHeader(&newHeader, &header, &tmp, sectionNames1, sectionNames2, sectionHeaders1,sectionHeaders2,f3);
    int t=0, cpt;
    char * resChar[newHeader.e_shnum+header.e_shnum];
    Elf32_Shdr * test = getSectionHeader(&newHeader, &header, sectionHeaders1, sectionHeaders2, sectionNames1, sectionNames2, resChar,&t);
    printf("%d\n",t);
    for(cpt = 0;cpt<t;cpt++){
            printf("%d et %d\n", test[cpt].sh_type,cpt);
    }*/
    
    
   /* Elf32_Ehdr header3;
    fseek(f3,0,SEEK_SET);
    fread(&header3, sizeof(header),1, f3);*/
    //displayHeader(header3);
}
