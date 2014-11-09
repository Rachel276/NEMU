#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "memory.h"

make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
	REG(R_ESP) = REG(R_ESP) - DATA_BYTE;
	MEM_W(REG(R_ESP),cpu.eip);
	if (DATA_BYTE == 2)
		cpu.eip = (cpu.eip + rel) & 0x0000ffff;
	else cpu.eip = cpu.eip + rel;
	print_asm("call" str(SUFFIX) " %x",eip + rel + 2);
	return rel + 2;
}

make_helper(concat(call_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE_S res = REG(m.R_M);
		REG(R_ESP) = REG(R_ESP) - DATA_BYTE;
	    MEM_W(REG(R_ESP),cpu.eip);	
		if (DATA_BYTE == 2)
			cpu.eip = res & 0x0000ffff;
		else cpu.eip = res;
		print_asm("call" str(SUFFIX) " *%%%s",REG_NAME(m.R_M));
		return res + 2;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE_S res = MEM_R(addr);
		REG(R_ESP) = REG(R_ESP) - DATA_BYTE;
		MEM_W(REG(R_ESP),cpu.eip);
		if (DATA_BYTE == 2)
			cpu.eip = res & 0x0000ffff;
		else cpu.eip = res;
		print_asm("call" str(SUFFIX) " *%s",ModR_M_asm);
		return res + len + 1;
	}
}

#include "exec/template-end.h"
