#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "memory.h"

make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE rel = instr_fetch(eip + 1, DATA_BYTE);
	reg_l(R_ESP) -= 4;
	MEM_W(reg_l(R_ESP),eip + 1 + DATA_BYTE);
//	printf("%x %x %x\n",eip,cpu.eip,rel);
	print_asm("call" str(SUFFIX) " %x",eip + rel + DATA_BYTE + 1);
	cpu.eip += rel;
// 	if (DATA_BYTE == 2)cpu.eip &= 0x0000ffff;
	return 1 + DATA_BYTE;
}

make_helper(concat(call_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE res = REG(m.R_M);
		reg_l(R_ESP) -= 4;
	    MEM_W(reg_l(R_ESP),eip + 1 + 1);	
		print_asm("call" str(SUFFIX) " *%%%s",REG_NAME(m.R_M));
		cpu.eip = res - 2;
//		if (DATA_BYTE == 2)cpu.eip &= 0x0000ffff;
		return 2;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = MEM_R(addr);
		reg_l(R_ESP) -= 4;
		MEM_W(reg_l(R_ESP),eip + 1 + len);
		print_asm("call" str(SUFFIX) " *%s",ModR_M_asm);
		cpu.eip = res - (len + 1);
//		if (DATA_BYTE == 2)cpu.eip &= 0x0000ffff;
		return 1 + len;
 	}
}

#include "exec/template-end.h"
