#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(idiv_rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE quo,rem;
	DATA_TYPE imm1,imm2,imm11,imm22;
	m.val = instr_fetch(eip + 1, 1);
	int i;
	imm11 = 0x1 << ((DATA_BYTE - 1) * 8);imm1 = 0x0;
	imm22 = 0x1;imm2 = 0x1;
	for (i = 0;i < (DATA_BYTE - 1) * 8; i++) {
		imm1 = imm1 | imm11; imm2 = imm2 | imm22;
		imm11 <<= 1; imm22 <<= 1;
	}
	if (m.mod == 3){
		quo = REG(R_EAX) / REG(m.R_M);
		rem = REG(R_EAX) % REG(m.R_M);
		REG(R_EAX) = (rem & imm1) | (quo & imm2);

		print_asm("idiv" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = REG(R_EAX) / MEM_R(addr);
		rem = REG(R_EAX) % MEM_R(addr);
		REG(R_EAX) = (rem & imm1) | (quo & imm2);

		print_asm("idiv" str(SUFFIX) " %s", ModR_M_asm);
		return len + DATA_BYTE;
	}
}

#include "exec/template-end.h"
