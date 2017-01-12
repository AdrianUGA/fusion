#ifndef __GETELF_H__
#define __GETELF_H__

typedef struct{
    char *name;
    Elf32_Rel *relTable;
    int tailleRelocTable;
} reloc_t;


typedef struct{
	char *filename;
	char *fileContent;
	int fileSize;
	FILE *file;
	
	Elf32_Ehdr header;

	Elf32_Shdr *sectionHeaders;
	char **sectionNames;
	char **sectionContents;

	Elf32_Sym *symTable;
	int symboleNumber;
	char **symbolesNames;
	char* strtab;

	reloc_t *relocTables;
        int nbRelTable;
}elf_t;

#define MODE_R 1
#define MODE_W 2

int isElf(elf_t *elf);
int initElf(elf_t *elf, char *filename, int mode);
void freeelf(elf_t *elf);
void readElf(elf_t *elf, int offset, int size, void *buffer);

void getElfHeader(elf_t *elf);
void getSectionsHeaders(elf_t *elf);
void getSectionsContent(elf_t *elf);
void getSectionNames(elf_t *elf);
int getSectionNumber(elf_t *elf, char *name);
char* getTypeRealoc(int type);
void getTableSymbole(elf_t *elf);
void getSymbolesNames(elf_t *elf);
void getRelocTable(elf_t *elf);
char isNumber(char *str);
void writeELF(elf_t elf1, elf_t elf2, elf_t *elf3);
void getStrtab(elf_t *elf);


#endif //__GETELF_H__
