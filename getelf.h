#ifndef __GETELF_H__
#define __GETELF_H__

typedef struct{
	char *filename;
	Elf32_Ehdr header;
	Elf32_Shdr *sectionHeaders;
	FILE *file;
	char **sectionNames;
	char **sectionContents;
	Elf32_Sym* symTable;
	int symboleNumber;
}elf_t;

int isElf(elf_t *elf);
void getSectionsContent(elf_t *elf);
void initElf(elf_t *elf, char *filename);
void getSectionNames(elf_t *elf);
void getSectionsHeaders(elf_t *elf);
void getElfHeader(elf_t *elf);
int getSectionNumber(elf_t *elf, char *name);
char* getTypeRealoc(int type);
void getTableSymbole(elf_t *elf);

char isNumber(char *str);


#endif //__GETELF_H__