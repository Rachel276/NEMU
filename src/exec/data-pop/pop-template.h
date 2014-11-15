#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(pop_r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	int reg_m = 0x4;
	REG(reg_code) = MEM_R(REG(reg_m));
	REG(reg_m) = REG(reg_m) + DATA_BYTE;

	print_asm("pop" str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(pop_m_, SUFFIX)) {
	swaddr_t addr;
	int reg_m = 0x4;
	int len = read_ModR_M(eip + 1,&addr);
	MEM_W(addr,MEM_R(REG(reg_m)));
	REG(reg_m) = REG(reg_m) + DATA_BYTE;
	
	print_asm("pop" str(SUFFIX) "%s", ModR_M_asm);
	return len;
}

#include "exec/template-end.h"
