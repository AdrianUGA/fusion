#ifndef __FUSION_N__
#define __FUSION_H__

#include "display.h"

void getHeader(elf_t f1, elf_t f2, elf_t * f3);
void getSection(elf_t f1, elf_t f2, elf_t * f3, char ** content, int * symb);

#endif
