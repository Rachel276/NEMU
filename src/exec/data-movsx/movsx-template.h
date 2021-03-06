#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(movsx_rmb2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		REG(m.reg) = (DATA_TYPE_S)(int8_t)REG(m.R_M);
		print_asm("movsx" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		REG(m.reg) = (DATA_TYPE_S)(int8_t)MEM_R(addr);
		print_asm("movsx" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return 1 + len;
	}
}

/*make_helper(movsx_rmw2r_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		REG(m.reg) = (int32_t)(int16_t)REG(m.R_M);
		print_asm("movsx" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
	    print_asm("movsx" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));	
		return 1 + len;
	}
}*/

#include "exec/template-end.h"
