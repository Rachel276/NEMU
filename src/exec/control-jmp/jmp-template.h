#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(jmp_rel_, SUFFIX)) {
	DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jmp" str(SUFFIX) " %x",eip + rel + 1 + DATA_BYTE);
	cpu.eip += rel;
	if (DATA_BYTE == 2)cpu.eip &= 0x0000ffff;
	return 1 + DATA_BYTE;
}

make_helper(concat(jmp_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE_S res = REG(m.R_M);
		print_asm("jmp" str(SUFFIX) " *%%%s",REG_NAME(m.R_M));
		return res + 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE_S res = MEM_R(addr);
		print_asm("jmp" str(SUFFIX) " *%s",ModR_M_asm);
		return res + len + 1;
	}
}

#include "exec/template-end.h"
