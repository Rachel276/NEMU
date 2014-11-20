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
				    int8_t *u = (void*)ph->p_vaddr;
					int8_t *v = (void*)elf + ph->p_offset;
					for (j = 0;j < ph->p_filesz;j ++, u ++, v ++)
						*u = *v;
					for (j = ph->p_filesz; j < ph->p_memsz; j ++, u ++)
						*u = 0;
			}
	}

	((void(*)(void)) elf->e_entry) ();
				 
	/* When returning from the program, it is executed successfully */
	HIT_GOOD_TRAP;
}
