#include <elf.h>
#include "trap.h"

void loader() {
	/* The ELF file is located at memory address 0 */
	Elf32_Ehdr *elf = (void*)0;

	uint32_t ph_size = elf->e_phentsize*elf->e_phnum;
	Elf32_Phdr *ph;

	for (ph = (void*)elf->e_phoff;ph != (void*)(elf->e_phoff+ph_size);ph++) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {
			uint8_t *p = (void*)ph->p_vaddr;
			uint8_t *q = (void*)elf+ph->p_offset;
			int i;
			for (i = 0;i < ph->p_filesz;i++)
				p[i] = q[i];
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			
			for (;i < ph->p_memsz;i++)
				p[i] = 0;
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
