#include "memory.h"

#include <stdlib.h>
#include <elf.h>
#include <sys/stat.h>

static int main_argc;
static char **main_argv;
static char *exec_file;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

void set_main_args(int argc, char *argv[]) {
	main_argc = argc;
	main_argv = argv;
	exec_file = main_argv[0];
}

/* Load symbol table and string table from ELF file for future use.
 * For detail information, please refer to "man elf". */
void load_table() {
	FILE *fp = fopen(exec_file, "rb");
	test(fp, "file not exist!");

	uint8_t buf[4096];
	/* Read the first 4096 bytes from the exec_file.
	 * They should contain the ELF header and program headers. */
	fread(buf, 4096, 1, fp);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	fread(sh, sh_size, 1, fp);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			fread(symtab, sh[i].sh_size, 1, fp);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
 		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			fread(strtab, sh[i].sh_size, 1, fp);
//			printf("%d %d %d\n",sh[i].sh_size,sh[i].sh_offset,strlen(strtab));
 		}
 	}
//	printf("%d\n",strlen(strtab));
	free(sh);
	free(shstrtab);

	/* Double check */
	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

void load_prog() {
	struct stat st;
	stat(exec_file, &st);
	assert(st.st_size < 0xa0000);

	FILE *fp = fopen(exec_file, "rb");
	assert(fp);

	/* We do not have a virtual hard disk now. Before we have a virutal hard disk,
	 * the beginning of physical memory is used as a "disk". The loader in NEMU
	 * will load the program from this "disk" (at the beginning of the physical
	 * memory).
	 */
	fread(hwa_to_va(0), st.st_size, 1, fp);
	fclose(fp);
}
swaddr_t find_tokens(char virable[])
{
	int i;
	for (i = 0; i < nr_symtab_entry; i ++)
	{
		if (ELF32_ST_TYPE(symtab[i].st_info) != STT_OBJECT && ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC)continue;
		if (strcmp(virable,strtab + symtab[i].st_name) == 0)
			return symtab[i].st_value;
	} 
	return -1;
}

int find_func_name(swaddr_t addr) {
	int i;
	for (i = 0; i < nr_symtab_entry; i++)
		if (ELF32_ST_TYPE(symtab[i].st_info) != STT_FUNC)
			if (symtab[i].st_value <= addr && symtab[i].st_value + symtab[i].st_size > addr) return i;
	return -1;
}

char* print_func(int x) {
	return strtab + symtab[x].st_name;
}
