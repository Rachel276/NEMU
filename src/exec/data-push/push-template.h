#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(push_r_,SUFFIX)){
	int reg_code = instr_fetch(eip, 1) & 0x7;
	int reg_m = 0x4;
	REG(reg_m) = REG(reg_m) - DATA_BYTE;
	MEM_W(REG(reg_m),REG(reg_code));
	
	print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));
	return 1;
}

make_helper(concat(push_m_,SUFFIX)){
	swaddr_t addr;
	int reg_m  = 0x4;
	int len = read_ModR_M(eip + 1,&addr);
	REG(reg_m) = REG(reg_m) - DATA_BYTE;
	MEM_W(REG(reg_m),MEM_R(addr));
	
	print_asm("push" str(SUFFIX) "%s", ModR_M_asm);
	return len;
}

make_helper(concat(push_i_,SUFFIX)){
	DATA_TYPE imm = instr_fetch(eip, DATA_BYTE);
	int reg_m = 0x4;
	REG(reg_m) = REG(reg_m) - DATA_BYTE;
	MEM_W(REG(reg_m),imm);

	print_asm("push" str(SUFFIX) "$0x%x", imm);
	return DATA_BYTE;
}

#include "exec/template-end.h"
