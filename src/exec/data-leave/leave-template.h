#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "memory.h"

make_helper(concat(leave_, SUFFIX)) {
	REG(R_ESP) = REG(R_EBP);
	REG(R_EBP) = MEM_R(cpu.esp);
    cpu.esp +=  DATA_BYTE;
	printf("%x %x\n",REG(R_ESP),REG(R_EBP));
	print_asm("leave\n");
	return 1;
}	

#include "exec/template-end.h"

