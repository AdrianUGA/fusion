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
#include "getelf.h"

void getHeader(elf_t f1, elf_t f2, elf_t * f3){ 
    Elf32_Ehdr newHeader;
    newHeader = f1.header;
    newHeader.e_phnum = newHeader.e_phnum + f2.header.e_phnum;
    newHeader.e_shoff = sizeof(Elf32_Ehdr);
    f3->header = newHeader;
}
    

void getSection(elf_t f1, elf_t f2, elf_t * f3, char ** content, int * symb){
    //content = malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
    f3->sectionNames = (char **)malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
    f3->sectionHeaders = malloc(sizeof(Elf32_Shdr)*(f1.header.e_shnum + f2.header.e_shnum));
    int i;
    int size = 0;
    for(i=0; i<f1.header.e_shnum; i++){
        symb[i] = -1;
        size++;
        memcpy(&f3->sectionHeaders[i], &f1.sectionHeaders[i], sizeof(Elf32_Shdr));
        
                            
        f3->sectionNames[i] = (char *) malloc(sizeof(char)*strlen(f1.sectionNames[i]));
        memcpy(f3->sectionNames[i], f1.sectionNames[i],sizeof(char)*strlen(f1.sectionNames[i]));
        
        content[i] = malloc(f3->sectionHeaders[i].sh_size);
        memcpy(content[i], f1.sectionContents[i], f1.sectionHeaders[i].sh_size);
       
         
    }
    int k;
    for(i=1; i<f2.header.e_shnum;i++){
        if(f2.sectionHeaders[i].sh_type == SHT_ARM_ATTRIBUTES);
        else if(f2.sectionHeaders[i].sh_type == SHT_NOBITS && strcmp(".bss", f2.sectionNames[i]) == 0) {
            k = getSectionNumber(&f1, ".bss");
            f3->sectionHeaders[k].sh_size += f2.sectionHeaders[i].sh_size;
            symb[k]=i;
        }
        else if(f2.sectionHeaders[i].sh_type != SHT_PROGBITS && f2.sectionHeaders[i].sh_type != SHT_STRTAB && f2.sectionHeaders[i].sh_type != SHT_SYMTAB && f1.sectionHeaders[i].sh_type != SHT_REL){
                                
            memcpy(&f3->sectionHeaders[size], &f2.sectionHeaders[i], sizeof(Elf32_Shdr));
            //maj sh_name
            f3->sectionHeaders[size].sh_name += f1.sectionHeaders[f1.header.e_shstrndx].sh_size;
            
            f3->sectionNames[size] = malloc(sizeof(char)*strlen(f2.sectionNames[i]));
            memcpy(f3->sectionNames[size], f2.sectionNames[i],sizeof(char)*strlen(f2.sectionNames[i]));
            //printf("%s inséré | %s | %s\n",f3->sectionNames[size],f3->sectionNames[0],f3->sectionNames[1]);
            
             content[size] = (char *)malloc(f3->sectionHeaders[size].sh_size);
             memcpy(content[size], f2.sectionContents[i], f2.sectionHeaders[i].sh_size);
             int k;
            for(k=0; k<f3->sectionHeaders[size].sh_size;k++){
                // unsigned char c = content[size][k];
                //printf("%x", c);
            }
            //printf("\n");
        //printf("\n");
            size++;
        } else {
            int j, modif=1;
            for(j=0; j<f1.header.e_shnum && modif;j++){
                if(strcmp(f1.sectionNames[j],f2.sectionNames[i]) == 0 && ( f1.sectionHeaders[j].sh_type == SHT_PROGBITS || f1.sectionHeaders[j].sh_type == SHT_SYMTAB || f1.sectionHeaders[j].sh_type == SHT_STRTAB || f1.sectionHeaders[j].sh_type == SHT_REL)){
                    modif = 0;
                    symb[j] = i;
                                        
                    //maj size
                    f3->sectionHeaders[j].sh_size += f2.sectionHeaders[i].sh_size;
                    //printf("%s fusionné | %s | %s\n",f3->sectionNames[j],f3->sectionNames[0],f3->sectionNames[1]);
                    
                    if(f3->sectionHeaders[j].sh_type == SHT_PROGBITS || f3->sectionHeaders[j].sh_type == SHT_STRTAB){
                        int taille = f1.sectionHeaders[j].sh_size;
                        int taille2 = f2.sectionHeaders[i].sh_size;
                        char * tmp = malloc(taille + taille2);
                        char * tmp2 = malloc(taille2);
                        memcpy(tmp, content[j], taille);
//                         printf("%d\n",taille);/*
                        
                        memcpy(tmp2, f2.sectionContents[i], f2.sectionHeaders[i].sh_size);
                        
                        memcpy(tmp+taille, tmp2, taille2);
                        content[j] = malloc(taille+taille2);
                        memcpy(content[j],tmp,taille+taille2);
                        free(tmp);
                        free(tmp2);
                       
                        //printf("\n");
                                }
                }
            }
            if(modif==1){
                
                memcpy(&f3->sectionHeaders[size], &f2.sectionHeaders[i], sizeof(Elf32_Shdr));
                //maj sh_name
                f3->sectionHeaders[size].sh_name += f1.sectionHeaders[f1.header.e_shstrndx].sh_size;
                
                f3->sectionNames[size] = malloc(sizeof(char)*strlen(f2.sectionNames[i]));
                memcpy(f3->sectionNames[size], f2.sectionNames[i], sizeof(char)*strlen(f2.sectionNames[i]));
               
                //printf("%s inséré | %s | %s\n",f3->sectionNames[size],f3->sectionNames[0],f3->sectionNames[1]);
                                    
                content[size] = malloc(f3->sectionHeaders[size].sh_size);
                //printf("Avant bug : %s \n",f3->sectionNames[0]);
                memcpy(content[size], f2.sectionContents[i], f2.sectionHeaders[i].sh_size);
                //printf("Après bug : %s \n",f3->sectionNames[0]);
                // int k;
           
                size++;
                
            }
        }
    }
    f3->header.e_shnum = size;
}

int getIndice(int* tab, int val, int taille){
    int res = -1;
    int i;
    for(i = 0; i < taille && res == -1; i++){
        if(tab[i] == val){
            res = i;
        }
    }
    return res;
}

void fusionTableSymbole(elf_t* elf1, elf_t* elf2, elf_t* elf3, int* secFusion, int* newSymtabIdx1, int* newSymtabIdx2){
    int i,j, trouve, idxGlob, tmp, idxFin, name;
    int idx = 0;
    int taille1 = elf1->symboleNumber;
    int taille2 = elf2->symboleNumber;


    printf("%d %d \n", taille1, taille2);

    /* Nouvelle table d'index de la table des symboles. Le symbole d'indice i du fichier 2 a l'indice newSymtabIdx[i] dans le fichier 3 */
    //newSymtabIdx = malloc((taille2)*sizeof(int));

    /* Calcul des tailles des strtab */
    int strtabTaille1 = elf1->sectionHeaders[getSectionNumber(elf1, ".strtab")].sh_size;
    int strtabTaille2 = elf2->sectionHeaders[getSectionNumber(elf2, ".strtab")].sh_size;
    int strtabTaille3 = strtabTaille1+strtabTaille2;
    
    /* Allocation de la nouvelle table de symbole et de la nouvelle strtab */
    Elf32_Sym* newSymtab;
    char* newStrtab;
    newSymtab = (Elf32_Sym*) malloc(sizeof(Elf32_Sym)*taille1+sizeof(Elf32_Sym)*taille2);
    newStrtab = (char*) malloc(strtabTaille1+strtabTaille2);

    /* Concaténation des strtab et ajout dans le nouvel elf */
    memcpy(newStrtab, elf1->strtab, strtabTaille1);
    memcpy(newStrtab+strtabTaille1, elf2->strtab, strtabTaille2);
    //memcpy(newSymtab, elf1->symTable, taille1*sizeof(Elf32_Sym));

    /* Ajout des symboles locaux du fichier 1 dans la table des symboles du nouvel elf */
    for(i = 0; i<taille1 && ELF32_ST_BIND(elf1->symTable[i].st_info) != STB_GLOBAL; i++){
        newSymtab[idx] = elf1->symTable[i];
        //printf("%s\n", elf1->strtab+elf1->symTable[i].st_name);
        idx++;
        newSymtabIdx1[i]=idx;
    }
    //printf("%d\n", i);

    /* Ajout des symboles locaux du fichier 2 dans la table des symboles du nouvel elf */
    for(j = 0; j<taille2 && ELF32_ST_BIND(elf2->symTable[j].st_info) != STB_GLOBAL; j++){
        newSymtab[idx] = elf2->symTable[j];
        newSymtab[idx].st_name += strtabTaille1;
        if(ELF32_ST_TYPE(newSymtab[idx].st_info) == STT_SECTION){
            newSymtab[idx].st_shndx = getSectionNumber(elf3, elf2->sectionNames[newSymtab[idx].st_shndx]);
        }
        tmp = getIndice(secFusion, elf2->symTable[j].st_shndx, elf1->header.e_shnum); /* Numéro de la section fusionnée */
        if (tmp != -1){
            printf("maj: %d  old: %d   new: %d\n", idx, newSymtab[idx].st_value, newSymtab[idx].st_value+elf1->sectionHeaders[tmp].sh_size);
            newSymtab[idx].st_value += elf1->sectionHeaders[tmp].sh_size;
        }
        newSymtabIdx2[j]=idx;
        idx++;
    }

    idxGlob = idx;
    //printf("On démarre : %d %d\n", idxGlob, j);

    /* Ajout des symboles globaux du fichier 1 dans la table des symboles du nouvel elf */
    while(i<taille1){
        newSymtab[idx] = elf1->symTable[i];
        newSymtabIdx1[i]=idx;
        idx++;
        i++;
    }

    idxFin = idx;
    /* Ajout et correction des symboles du fichier 2 dans la table des symboles du nouvel elf */
    /* Parcours des symboles globaux du fichier 2 */
    while (j < taille2){
        trouve = 0;
        /* Parcours des symboles globaux du fichier 1 (déjà ajoutés dans le nouvel elf) */
        for(i = idxGlob; i < idxFin; i++){
            /* Si les 2 symboles ont le même nom */
            printf("%d: %s    %d: %s\n", i, newStrtab+newSymtab[i].st_name, j, elf2->strtab+elf2->symTable[j].st_name);
            if(strcmp(newStrtab+newSymtab[i].st_name, elf2->strtab+elf2->symTable[j].st_name) == 0){
                /* S'ils sont tous les 2 définis, echec de la fusion */
                if(newSymtab[i].st_shndx != SHN_UNDEF && elf2->symTable[j].st_shndx != SHN_UNDEF){
                    fprintf(stderr, "Erreur survenue, fin du programme \n");
                    exit(1);
                }
                /* Si les 2 symboles ont le même nom, si celui du fichier 2 est défini et pas celui du fichier 1, on corrige avec les valeurs du fichier 2 */
                else if(newSymtab[i].st_shndx == SHN_UNDEF && elf2->symTable[j].st_shndx != SHN_UNDEF){
                    printf("blabla\n");
                    name = newSymtab[i].st_name;
                    newSymtab[i] = elf2->symTable[j];
                    newSymtab[i].st_name = name;
                    trouve = 1;
                }
                /* Si les 2 symboles ont le même nom, si celui du fichier 1 est défini et pas celui du fichier 2, on garde les valeurs du fichier 1 */
                else if(newSymtab[j].st_shndx != SHN_UNDEF && elf2->symTable[i].st_shndx == SHN_UNDEF){
                    printf("bloblo\n");
                    trouve = 1;
                }
                newSymtabIdx2[j]=i;
            }
        }
        /* Si aucun symbole du fichier 1 n'a le même nom, on ajoute le symbole du fichier 2 à la suite */
        if(trouve == 0){
            printf("Pas trouvé\n");
            newSymtab[idx] = elf2->symTable[j];
            newSymtab[idx].st_name = strtabTaille1 + elf2->symTable[j].st_name;
            if(ELF32_ST_TYPE(newSymtab[idx].st_info) == STT_SECTION){
                newSymtab[idx].st_shndx = getSectionNumber(elf3, elf2->sectionNames[newSymtab[idx].st_shndx]);
            }
            tmp = getIndice(secFusion, elf2->symTable[i].st_shndx, elf1->header.e_shnum); /* Numéro de la section fusionnée */
            printf("name: %s    value: %08x\n", newStrtab+newSymtab[idx].st_name, newSymtab[i].st_value);
            if (tmp != -1){
                newSymtab[idx].st_value += elf1->sectionHeaders[tmp].sh_size;
                printf("maj2: %d\n", idx);
            }
            printf("name: %s    value: %08x\n", newStrtab+newSymtab[idx].st_name, newSymtab[i].st_value);
            newSymtabIdx2[j]=idx;
            idx++;
        }

        j++;
    }

    elf3->sectionHeaders[getSectionNumber(elf3, ".symtab")].sh_size = (idx)*sizeof(Elf32_Sym);
    elf3->sectionHeaders[getSectionNumber(elf3, ".strtab")].sh_size = strtabTaille3;
    elf3->symTable = malloc((idx)*sizeof(Elf32_Sym));
    memcpy(elf3->symTable, newSymtab, (idx)*sizeof(Elf32_Sym));
    //printf("Fin\n");
    elf3->strtab = newStrtab;
    printf("IDX %d\n", idx);
    elf3->symboleNumber = idx;
    elf3->sectionHeaders[getSectionNumber(elf3, ".symtab")].sh_info = idxGlob;

    /*
    for(i=0; i<taille2; i++){
        printf("[%d]%d\n", i, newSymtabIdx2[i]);
    }
    */
}

void fusionRelocTable(elf_t* elf1, elf_t* elf2, elf_t* elf3, int* secFusion, int* newSymtabIdx1, int* newSymtabIdx2){
    
    //displayRelocTable(elf1);
    //displayRelocTable(elf2);

    /* Calcul des taille des table de réalocations en octets */
    int taille1 = elf1->tailleRelocTable*sizeof(Elf32_Rel);
    int taille2 = elf2->tailleRelocTable*sizeof(Elf32_Rel);

    /* Concaténation de la table de réalocation du fichier 1 et du fichier 2 */
    Elf32_Rel* newRelocTab = malloc(taille1 + taille2);
    /*
    memcpy(newRelocTab, elf1->relTable, taille1);
    memcpy(newRelocTab+taille1, elf2->relTable, taille2);
    */

    int i, j;
    for(i = 0; i < elf1->tailleRelocTable; i++){
        newRelocTab[i] = elf1->relTable[i];
        newRelocTab[i].r_info = ELF32_R_INFO(newSymtabIdx1[ELF32_R_SYM(newRelocTab[i].r_info)], ELF32_R_TYPE(newRelocTab[i].r_info));
    } 

    for(j = 0; j < elf2->tailleRelocTable; j++) {
        newRelocTab[i] = elf2->relTable[j];
        //printf("%d --- %d \n",ELF32_R_SYM(newRelocTab[i].r_info), newSymtabIdx[ELF32_R_SYM(newRelocTab[i].r_info)]);
        //printf("\n\n\nIDX SYMBOLE: %d    SYMBOLE: %s\n", newSymtabIdx[ELF32_R_SYM(newRelocTab[i].r_info)], elf3->strtab+elf3->symTable[newSymtabIdx[ELF32_R_SYM(newRelocTab[i].r_info)]].st_name);
        newRelocTab[i].r_info = ELF32_R_INFO(newSymtabIdx2[ELF32_R_SYM(newRelocTab[i].r_info)], ELF32_R_TYPE(newRelocTab[i].r_info));


        int tmp = getIndice(secFusion, elf2->symTable[j].st_shndx, elf1->header.e_shnum); /* Numéro de la section fusionnée */
        //printf("section: %d %d\n", elf2->symTable[j].st_shndx, tmp);
        if (tmp != -1){
            newRelocTab[i].r_offset += elf1->sectionHeaders[tmp].sh_size;
        }
        i++;
    }

    /* Affectation dans le nouveau elf */
    elf3->relTable = malloc((elf1->tailleRelocTable+elf2->tailleRelocTable)*sizeof(Elf32_Rel));
    memcpy(elf3->relTable, newRelocTab, (elf1->tailleRelocTable+elf2->tailleRelocTable)*sizeof(Elf32_Rel));
    elf3->tailleRelocTable = elf1->tailleRelocTable+elf2->tailleRelocTable;

    //displayRelocTable(elf3);
}

