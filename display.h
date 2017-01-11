#ifndef __DISPLAY_H__
#define __DISPLAY_H__

	typedef struct{
		Elf32_Ehdr header;
		Elf32_Shdr *sectionHeaders;
		FILE *file;
		char **sectionNames;
		Elf32_Sym* symTable;
		int symboleNumber;
		char* strtab;
		Elf32_Rel* relTable;
		int tailleRelocTable;
	}elf_t;

	void displayHeader(elf_t *elf);
	void displaySectionContent(elf_t *elf, int sectionNumber);
	void displaySectionHeaders(elf_t *elf);
	void getSectionContent(elf_t *elf, int sectionNumber, char *buffer);
	void getSectionNames(elf_t *elf);
	void displaySymbole(Elf32_Sym symbole, char * strtab, int i);
	void displayTableSymbole(elf_t *elf);
	void displayRelocTable(elf_t *elf);
	int getSectionNumber(elf_t *elf, char *name);
	int isELF(Elf32_Ehdr header);
	char* getTypeRealoc(int type);
	char isNumber(char *str);
	void getTableSymbole(elf_t *elf);
	void getRelocTable(elf_t *elf);

#endif // __DISPLAY_H__
