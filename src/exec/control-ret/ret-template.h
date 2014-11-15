#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(ret_, SUFFIX)) {
	cpu.eip = MEM_R(REG(R_ESP));
	printf("%x\n",eip);
	REG(R_ESP) = REG(R_ESP) + DATA_BYTE;
	printf("%x\n",REG(R_ESP));
	if (DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
	print_asm("ret\n");
	return 1;
}

#include "exec/template-end.h"
