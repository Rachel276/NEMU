#include <elf.h>
#include "trap.h"

void loader() {
	/* The ELF file is located at memory address 0 */
	Elf32_Ehdr *elf = (void *)0;
	Elf32_Phdr *ph = (void *)elf->e_phoff; 
	int i, j;
	for (i = 0; i < elf->e_phnum; i ++,ph++){
			/* Scan the program header table, load each segment into memory */
			if(ph->p_type == PT_LOAD) {
				    int8_t *u = (int8_t*)ph->p_vaddr;
					int8_t *v = (int8_t*)ph->p_offset;
					int8_t *w = (int8_t*)(ph->p_vaddr + ph[i].p_filesz);
					for (j = 0;j < ph->p_filesz;j ++, u ++, v ++)
						*u = *v;
					for (j = ph[i].p_filesz; j < ph[i].p_memsz; j ++, w ++)
						*w = 0;
			}
	}

	((void(*)(void)) elf->e_entry) ();
				 
	/* When returning from the program, it is executed successfully */
	HIT_GOOD_TRAP;
}
