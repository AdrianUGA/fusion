void getHeader(elf_t f1, elf_t f2, elf_t * f3);
void getSection(elf_t f1, elf_t f2, elf_t * f3, int * symb);
int getIndice(int* tab, int val, int taille);
void fusionTableSymbole(elf_t* elf1, elf_t* elf2, elf_t* elf3, int* secFusion, int* newSymtabIdx1, int* newSymtabIdx2);
void fusionRelocTable(elf_t* elf1, elf_t* elf2, elf_t* elf3, int* secFusion, int* newSymtabIdx1, int* newSymtabIdx2);