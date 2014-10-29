#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(xchg_r2a_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	DATA_TYPE imm = REG(R_EAX);
	REG(R_EAX) = REG(reg_code);
	REG(reg_code) = imm;
	
	print_asm("xchg" str(SUFFIX) " %%%s,%%%s", REG_NAME(R_EAX),REG_NAME(reg_code));
	return 1;
}

make_helper(concat(xchg_rm2r_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = REG(m.R_M);
		REG(m.R_M) = REG(m.reg);
		REG(m.reg) = imm;

		print_asm("xchg" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr);
		MEM_W(addr, REG(m.reg));
		REG(m.reg) = imm;

		print_asm("xchg" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return len + 1;
	}
}

#include "exec/template-end.h"
