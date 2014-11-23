#include <elf.h>
#include "trap.h"

void loader() {
	/* The ELF file is located at memory address 0 */
	Elf32_Ehdr *elf = (void*)0;
	Elf32_Phdr *ph = (void*)elf->e_phoff;
	int i, j;
	for (i = 0;i < elf->e_phnum; i ++,ph ++) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD)  { 
			uint8_t *u = (void*)ph->p_vaddr;
			uint8_t *v = (void*)elf+ph->p_offset;
			for (j = 0;j < ph->p_filesz; j ++)
				u[j] = v[j];
			/* TODO: rea d the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			
			for (;j < ph->p_memsz; j ++)
				u[j] = 0;
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
 			 */ 
 
		}
	} 

	/* Here we go! */
	((void(*)(void)) elf->e_entry) ();

	/* When returning from the program, it is executed successfully */
	HIT_GOOD_TRAP;
}
