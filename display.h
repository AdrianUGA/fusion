#ifndef __DISPLAY_H__
#define __DISPLAY_H__

	typedef struct{
		Elf32_Ehdr header;
		Elf32_Shdr *sectionHeaders;
		FILE *file;
		char **sectionNames;
		char **sectionContents;
		Elf32_Sym* symTable;
		int symboleNumber;
	}elf_t;

	void displayHeader(elf_t *elf);
	void displaySectionContent(elf_t *elf, int sectionNumber);
	void displaySectionHeaders(elf_t *elf);
	void getSectionsContent(elf_t *elf);
	void getSectionNames(elf_t *elf);
	void displaySymbole(Elf32_Sym symbole, char * strtab, int i);
	void displayTableSymbole(elf_t *elf);
	void displayRelocTable(elf_t *elf);
	int getSectionNumber(elf_t *elf, char *name);
	int isELF(Elf32_Ehdr header);
	char* getTypeRealoc(int type);
	char isNumber(char *str);
	Elf32_Sym* getTableSymbole(elf_t *elf);

#endif // __DISPLAY_H__
