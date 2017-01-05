void afficherHeader(Elf32_Ehdr header);
void displaySectionContentI(Elf32_Ehdr header, int i, FILE* file,  char * sectionNames[]);
void displaySectionContentC(Elf32_Ehdr header, char * section, FILE* file,  char * sectionNames[]);
void displaySectionHeader(Elf32_Shdr* sectionH, Elf32_Ehdr header, char * sectionNames[]);
void getSectionContent(FILE *file, Elf32_Shdr sectionHeader, char *buffer);
void getSectionNames(FILE * file, Elf32_Ehdr header, Elf32_Shdr sectionHeaders[], char * sectionNames[]);
void usage(char *name);
void displaySymbole(Elf32_Sym symbole, char * strtab, int i);
void displayTableSymbole(FILE * file,char * sectionNames[]);
void displayRelocTable(FILE* file, Elf32_Ehdr *header);
