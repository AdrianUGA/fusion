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


void getSection(elf_t f1, elf_t f2, elf_t * f3, int * symb){
    f3->sectionContents = malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
    f3->sectionNames = (char **)malloc(sizeof(char *)*(f1.header.e_shnum + f2.header.e_shnum));
    f3->sectionHeaders = malloc(sizeof(Elf32_Shdr)*(f1.header.e_shnum + f2.header.e_shnum));
    int i;
    int size = 0;
    for(i=0; i<f1.header.e_shnum; i++){
        size++;
        memcpy(&f3->sectionHeaders[i], &f1.sectionHeaders[i], sizeof(Elf32_Shdr));
        
        
        f3->sectionNames[i] = (char *) malloc(sizeof(char)*strlen(f1.sectionNames[i]));
        memcpy(f3->sectionNames[i], f1.sectionNames[i],sizeof(char)*strlen(f1.sectionNames[i]));
        
        f3->sectionContents[i] = malloc(f3->sectionHeaders[i].sh_size);
        memcpy(f3->sectionContents[i], f1.sectionContents[i], f1.sectionHeaders[i].sh_size);
        printf("La section présente dans le fichier 1 de nom %s à été ajoutée à la table des sections du fichier résultat\n",f3->sectionNames[i]); 
        
    }
    int k;
    symb[0] = 0;
    for(i=1; i<f2.header.e_shnum;i++){
        if(f2.sectionHeaders[i].sh_type == SHT_ARM_ATTRIBUTES){
            symb[i] = -1;
            printf("On ne fusionne pas la section ARM_ATTRIBUTES du fichier 2\n");
        }
        else if(f2.sectionHeaders[i].sh_type == SHT_NOBITS && strcmp(".bss", f2.sectionNames[i]) == 0) {
            k = getSectionNumber(&f1, ".bss");
            f3->sectionHeaders[k].sh_size += f2.sectionHeaders[i].sh_size;
            symb[i]=k;
            printf("On ne fusionne pas la section .bss du fichier 2\n" );
        }
        else if(f2.sectionHeaders[i].sh_type != SHT_PROGBITS && f2.sectionHeaders[i].sh_type != SHT_STRTAB && f2.sectionHeaders[i].sh_type != SHT_SYMTAB && f1.sectionHeaders[i].sh_type != SHT_REL){
            
            memcpy(&f3->sectionHeaders[size], &f2.sectionHeaders[i], sizeof(Elf32_Shdr));
            //maj sh_name
            f3->sectionHeaders[size].sh_name += f1.sectionHeaders[f1.header.e_shstrndx].sh_size;
            
            f3->sectionNames[size] = malloc(sizeof(char)*strlen(f2.sectionNames[i]));
            memcpy(f3->sectionNames[size], f2.sectionNames[i],sizeof(char)*strlen(f2.sectionNames[i]));

            
            f3->sectionContents[size] = (char *)malloc(f3->sectionHeaders[size].sh_size);
            memcpy(f3->sectionContents[size], f2.sectionContents[i], f2.sectionHeaders[i].sh_size);
            symb[i] = size;
            size++;
        } else {
            int j, modif=1;
            for(j=0; j<f1.header.e_shnum && modif;j++){
                if(strcmp(f1.sectionNames[j],f2.sectionNames[i]) == 0 && ( f1.sectionHeaders[j].sh_type == SHT_PROGBITS || f1.sectionHeaders[j].sh_type == SHT_SYMTAB || f1.sectionHeaders[j].sh_type == SHT_STRTAB || f1.sectionHeaders[j].sh_type == SHT_REL)){
                    modif = 0;
                    symb[i] = j;
                    
                    //maj size
                    f3->sectionHeaders[j].sh_size += f2.sectionHeaders[i].sh_size;
                    printf("On fusionne le contenu de la section %s car elle est présente dans le fichier résultat\n",f3->sectionNames[j]);
                    if(f3->sectionHeaders[j].sh_type == SHT_PROGBITS || f3->sectionHeaders[j].sh_type == SHT_STRTAB){
                        int taille = f1.sectionHeaders[j].sh_size;
                        int taille2 = f2.sectionHeaders[i].sh_size;
                        char * tmp = malloc(taille + taille2);
                        char * tmp2 = malloc(taille2);
                        memcpy(tmp, f3->sectionContents[j], taille);                 
                        memcpy(tmp2, f2.sectionContents[i], f2.sectionHeaders[i].sh_size);
                        memcpy(tmp+taille, tmp2, taille2);
                        f3->sectionContents[j] = malloc(taille+taille2);
                        memcpy(f3->sectionContents[j],tmp,taille+taille2);
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
                printf("La section de nom %s n'était pas présente dans le fichier 1, donc on l'ajoute au fichier résultat\n",f3->sectionNames[size]);
                f3->sectionContents[size] = malloc(f3->sectionHeaders[size].sh_size);
                memcpy(f3->sectionContents[size], f2.sectionContents[i], f2.sectionHeaders[i].sh_size);
                symb[i] = size;
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
    
    /* Nouvelle table d'index de la table des symboles. Le symbole d'indice i du fichier 2 a l'indice newSymtabIdx[i] dans le fichier 3 */
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

    /* Ajout des symboles locaux du fichier 1 dans la table des symboles du nouvel elf */
    for(i = 0; i<taille1 && ELF32_ST_BIND(elf1->symTable[i].st_info) != STB_GLOBAL; i++){
        newSymtab[idx] = elf1->symTable[i];
        newSymtabIdx1[i]=idx;
        printf("Le symbole local numéro %d du fichier numéro 1 est ajouté à la table des symboles\n",idx);
        idx++;        
    }

    /* Ajout des symboles locaux du fichier 2 dans la table des symboles du nouvel elf */
    for(j = 0; j<taille2 && ELF32_ST_BIND(elf2->symTable[j].st_info) != STB_GLOBAL; j++){
        if(elf2->symTable[j].st_shndx == SHN_ABS || secFusion[elf2->symTable[j].st_shndx] != -1){
            newSymtab[idx] = elf2->symTable[j];
            newSymtab[idx].st_name += strtabTaille1;
            if(newSymtab[idx].st_shndx != SHN_ABS){
                newSymtab[idx].st_shndx = secFusion[newSymtab[idx].st_shndx];
            }
            tmp = secFusion[j];
            if (tmp < elf1->header.e_shnum){
                newSymtab[idx].st_value += elf1->sectionHeaders[tmp].sh_size;
            }
            newSymtabIdx2[j]=idx;
            printf("Le symbole local numéro %d du fichier numéro 2 est ajouté à la table des symboles à l'indice %d \n",j,idx);
	    //printf("[%d] %s\n", idx, newStrtab + newSymtab[idx].st_name);

            idx++;
            
        }
    }

    idxGlob = idx;

    /* Ajout des symboles globaux du fichier 1 dans la table des symboles du nouvel elf */
    while(i<taille1){
        newSymtab[idx] = elf1->symTable[i];
        newSymtabIdx1[i]=idx;
        printf("Le symbole global numéro %d du fichier numéro 1 est ajouté à la table des symboles à l'indice %d\n",i,idx);
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
            if(strcmp(newStrtab+newSymtab[i].st_name, elf2->strtab+elf2->symTable[j].st_name) == 0){
                /* S'ils sont tous les 2 définis, echec de la fusion */
                if(newSymtab[i].st_shndx != SHN_UNDEF && elf2->symTable[j].st_shndx != SHN_UNDEF){

                    fprintf(stderr, "Erreur survenue, 2 symboles globaux définis ont le même nom.\nFin du programme \n");
                    free(newSymtab);
                    free(newStrtab);

                    exit(1);
                }
                /* Si les 2 symboles ont le même nom, si celui du fichier 2 est défini et pas celui du fichier 1, on corrige avec les valeurs du fichier 2 */
                else if(newSymtab[i].st_shndx == SHN_UNDEF && elf2->symTable[j].st_shndx != SHN_UNDEF){
                    name = newSymtab[i].st_name;
                    newSymtab[i] = elf2->symTable[j];
                    newSymtab[i].st_name = name;
                    printf("Le symbole global numéro %d du fichier numéro 2 est fusionné avec le symbole  %d du fichier 1 et est mis à l'indice %d\n",j,i,idx);
                    trouve = 1;
                }
                /* Si les 2 symboles ont le même nom, si celui du fichier 1 est défini et pas celui du fichier 2, on garde les valeurs du fichier 1 */
                else if(newSymtab[j].st_shndx != SHN_UNDEF && elf2->symTable[i].st_shndx == SHN_UNDEF){
                    trouve = 1;
                }
                newSymtabIdx2[j]=i;
            }
        }
        /* Si aucun symbole du fichier 1 n'a le même nom, on ajoute le symbole du fichier 2 à la suite */
        if(trouve == 0){
            newSymtab[idx] = elf2->symTable[j];
            newSymtab[idx].st_name = strtabTaille1 + elf2->symTable[j].st_name;
            if(newSymtab[idx].st_shndx != SHN_ABS){
                newSymtab[idx].st_shndx = secFusion[newSymtab[idx].st_shndx];
            }            
            tmp = secFusion[j];
            if (tmp < elf1->header.e_shnum){
                newSymtab[idx].st_value += elf1->sectionHeaders[tmp].sh_size;
            }
            newSymtabIdx2[j]=idx;
            printf("Le symbole global numéro %d du fichier numéro 2 n'était pas présent dans le fichier 1, on l'ajoute à l'indice %d\n",j,idx);
            idx++;
        }

        j++;
    }

    elf3->sectionHeaders[getSectionNumber(elf3, ".symtab")].sh_size = (idx)*sizeof(Elf32_Sym);
    elf3->sectionHeaders[getSectionNumber(elf3, ".strtab")].sh_size = strtabTaille3;
    elf3->symTable = malloc((idx)*sizeof(Elf32_Sym));
    memcpy(elf3->symTable, newSymtab, (idx)*sizeof(Elf32_Sym));
    elf3->strtab = (char*) malloc(strtabTaille3*sizeof(char));
    memcpy(elf3->strtab, newStrtab, strtabTaille3);
    elf3->strtab = newStrtab;
    
    elf3->symboleNumber = idx;
    elf3->sectionHeaders[getSectionNumber(elf3, ".symtab")].sh_info = idxGlob;
    getSymbolesNames(elf3);
}

void fusionRelocTable(elf_t* elf1, elf_t* elf2, elf_t* elf3, int* secFusion, int* newSymtabIdx1, int* newSymtabIdx2){
    /*displayRelocTable(elf1);
    displayRelocTable(elf2);*/
    
    int taille1, taille2, k, l, i, j, taille;
    Elf32_Rel* newRelocTab;
    int trouve = 0;
    int idx = 0;
    
    elf3->relocTables = (reloc_t*) malloc(sizeof(reloc_t)*(elf1->nbRelTable+elf2->nbRelTable));
    elf3->nbRelTable = 0;	
    /* Concaténation des tables de réalocation du fichier 1 et du fichier 2 */
    for(k=0; k<elf1->nbRelTable; k++){
        for(l=0; l<elf2->nbRelTable; l++){
            if(strcmp(elf1->relocTables[k].name, elf2->relocTables[l].name)==0){
                trouve = 1;
                taille1 = elf1->relocTables[k].tailleRelocTable;
                taille2 = elf2->relocTables[l].tailleRelocTable;
                newRelocTab = malloc((taille1+taille2)*sizeof(Elf32_Rel));
                for(i = 0; i < elf1->relocTables[k].tailleRelocTable; i++){
                    newRelocTab[i] = elf1->relocTables[k].relTable[i];
                    newRelocTab[i].r_info = ELF32_R_INFO(newSymtabIdx1[ELF32_R_SYM(newRelocTab[i].r_info)], ELF32_R_TYPE(newRelocTab[i].r_info));
                }
                for(j = 0; j < elf2->relocTables[l].tailleRelocTable; j++) {
                    newRelocTab[i] = elf2->relocTables[l].relTable[j];
                    newRelocTab[i].r_info = ELF32_R_INFO(newSymtabIdx2[ELF32_R_SYM(newRelocTab[i].r_info)], ELF32_R_TYPE(newRelocTab[i].r_info));

                    int tmp = secFusion[elf3->symTable[ELF32_R_SYM(newRelocTab[i].r_info)].st_shndx]; /* Numéro de la section fusionnée */
                    if (tmp!=-1){
                        newRelocTab[i].r_offset += elf1->sectionHeaders[tmp].sh_size;
                    } else {
                        tmp = getSectionNumber(elf3, ".ARM.attributes");
                        newRelocTab[i].r_offset += elf1->sectionHeaders[tmp].sh_size;
                    }
                    
                    i++;
                }
                /* Affectation dans le nouvel elf */
                taille = elf1->relocTables[k].tailleRelocTable + elf2->relocTables[l].tailleRelocTable;

                elf3->relocTables[idx].relTable = (Elf32_Rel *) malloc(taille*sizeof(Elf32_Rel));
                memcpy(elf3->relocTables[idx].relTable, newRelocTab, taille*sizeof(Elf32_Rel));
                elf3->relocTables[idx].tailleRelocTable = taille;
                elf3->relocTables[idx].name = elf1->relocTables[k].name;
                idx++;

                elf3->nbRelTable++;
                free(newRelocTab);
            }
            
      }
      if(trouve==0){
       memcpy(&elf3->relocTables[idx], &elf1->relocTables[k], sizeof(reloc_t));
       idx++;
       elf3->nbRelTable++;
   }
  }
  
  trouve = 0;
  for(i=0; i<elf2->nbRelTable; i++){
    for(j=0; j<elf3->nbRelTable; j++){
        if(strcmp(elf2->relocTables[i].name, elf3->relocTables[j].name)==0){	
            trouve = 1;
        }
        
    }
    if(trouve==0){
        memcpy(&elf3->relocTables[idx], &elf2->relocTables[i], sizeof(reloc_t));
        idx++;
        elf3->nbRelTable++;
    }
} 
}

