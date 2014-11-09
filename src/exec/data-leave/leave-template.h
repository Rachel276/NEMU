#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "memory.h"

make_helper(concat(leave_, SUFFIX)) {
	REG(R_ESP) = REG(R_EBP);
	REG(R_EBP) = MEM_R(REG(R_ESP));
    REG(R_ESP) = REG(R_ESP) + DATA_BYTE;
	print_asm("leave\n");
	return 1;
}	

#include "exec/template-end.h"

