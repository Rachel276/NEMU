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
					for (j = 0;j < ph->p_filesz;j ++)
						*((int8_t*)ph->p_vaddr + j) = *((int8_t*)ph->p_offset + j);	
					for (j = ph[i].p_filesz; j < ph[i].p_memsz; j ++)
						*((int8_t*)ph->p_vaddr + j) = 0;
//					memset((void *)(ph[i].p_vaddr + ph[i].p_filesz), 0, sizeof(ph[i].p_memsz - ph[i].p_filesz);			
			}
	}

	((void(*)(void)) elf->e_entry) ();
				 
	/* When returning from the program, it is executed successfully */
	HIT_GOOD_TRAP;
}
