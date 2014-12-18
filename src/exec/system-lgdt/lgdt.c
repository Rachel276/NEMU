#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(lgdt_m) {
	swaddr_t addr;
	int len = read_ModR_M(eip + 1, &addr);

	cpu.gdtr.limit = swaddr_read(eip + 4, 2);
	cpu.gdtr.base = swaddr_read(eip, 4);

	print_asm("lgdt  %s", ModR_M_asm);
	return len + 1;
}
