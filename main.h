#include <elf.h>

void afficherHeader(Elf32_Ehdr header);
void displaySectionContent(Elf32_Ehdr header, Elf32_Shdr sectionHeader, FILE* file);
void displaySectionHeaders(Elf32_Shdr sectionHeaders[], char *sectionNames[], int size);
void getSectionContent(FILE *file, Elf32_Shdr sectionHeader, char *buffer);
void getSectionNames(FILE * file, Elf32_Ehdr header, Elf32_Shdr sectionHeaders[], char * sectionNames[]);
void usage(char *name);
void sectionHexDump(Elf32_Shdr sectionHeader, FILE *file);