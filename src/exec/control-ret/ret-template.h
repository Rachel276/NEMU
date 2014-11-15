#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(ret_, SUFFIX)) {
	cpu.eip = MEM_R(REG(R_ESP));
	REG(R_ESP) = REG(R_ESP) + DATA_BYTE;
	if (DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
	print_asm("ret\n");
	return 0;
}

#include "exec/template-end.h"
