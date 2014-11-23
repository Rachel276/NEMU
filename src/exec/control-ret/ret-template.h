#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(ret_, SUFFIX)) {
	cpu.eip = MEM_R(REG(R_ESP));
	REG(R_ESP) = REG(R_ESP) + DATA_BYTE;
	if (DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
	print_asm("ret");
	cpu.eip -= 1;
	return 1;
}

make_helper(concat(ret_i_, SUFFIX)) {
	uint16_t imm = instr_fetch(eip + 1, 1 + 1);
	if (DATA_BYTE == 4) {
		cpu.eip = swaddr_read(reg_l(R_ESP), 4) - 1;
		reg_l(R_ESP) += 4;
	}
	else { 
		cpu.eip = swaddr_read(reg_l(R_ESP), 2)&0xffff;
		cpu.eip -= 1;
		reg_l(R_ESP) += 2;
	}
	reg_l(R_ESP) += imm;

	print_asm("ret" str(SUFFIX) " $0x%x", imm);
	return 1;
}

#include "exec/template-end.h"
