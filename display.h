void afficherHeader(Elf32_Ehdr header);
void displaySectionContent(Elf32_Ehdr header, int i, FILE* file);
void displaySectionContent(Elf32_Ehdr header, char * section, FILE* file);
void displaySectionHeader(Elf32_Shdr* sectionH, Elf32_Ehdr header, char * sectionNames[]);
void getSectionContent(F *file, Elf32_Shdr sectionHeader, char *buffer);ILE
void getSectionNames(FILE * file, Elf32_Ehdr header, Elf32_Shdr sectionHeaders[], char * sectionNames[]);
void usage(char *name);
void displaySymbole(Elf32_Sym symbole, char * strtab, int i);
void displayTableSymbole(FILE * file);
void displayRelocTable(FILE* file, Elf32_Ehdr *header);
