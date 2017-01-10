
#ifndef __DISPLAY_H__
#define __DISPLAY_H__
	#include <elf.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <getopt.h>
	#include <string.h>
	#include <assert.h>
	#include <unistd.h>
	#include <ctype.h>

	#include "display.h"
	#include "getelf.h"
	#include "debug.h"



	void displayHeader(elf_t *elf);
	void displaySectionContent(elf_t *elf, int sectionNumber);
	void displaySectionHeaders(elf_t *elf);
	void displaySymbole(elf_t *elf, Elf32_Sym symbole, int symtabNum, int i);
	void displayTableSymbole(elf_t *elf);
	void displayRelocTable(elf_t *elf);

	

#endif // __DISPLAY_H__
