#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "memory.h"

make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
	REG(R_ESP) = REG(R_ESP) - DATA_BYTE;
	MEM_W(REG(R_ESP),eip);
//	printf("%x %x %x\n",eip,cpu.eip,rel);
	print_asm("call" str(SUFFIX) " %x",eip + rel + DATA_BYTE + 1);
 	if (DATA_BYTE == 2){
		eip = (eip + rel + DATA_BYTE + 1) & 0x0000ffff;
		return 0;
	}
	else return rel + 1 + DATA_BYTE;
}

make_helper(concat(call_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE_S res = REG(m.R_M);
		REG(R_ESP) = REG(R_ESP) - DATA_BYTE;
	    MEM_W(REG(R_ESP),cpu.eip);	
		print_asm("call" str(SUFFIX) " *%%%s",REG_NAME(m.R_M));
		if (DATA_BYTE == 2){
			eip = (eip + res + 2) & 0x0000ffff;
			return 0;
		}
		else return res + 2;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE_S res = MEM_R(addr);
		REG(R_ESP) = REG(R_ESP) - DATA_BYTE;
		MEM_W(REG(R_ESP),cpu.eip);
		print_asm("call" str(SUFFIX) " *%s",ModR_M_asm);
		if (DATA_BYTE == 2){
			eip = (eip + res + len + 1) & 0x0000ffff;
			return 0;
		}
		else return res + len + 1;
	}
}

#include "exec/template-end.h"
