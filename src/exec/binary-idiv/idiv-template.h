#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "common.h"
#include "memory.h"

make_helper(concat(idiv_rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE quo,rem;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		quo = reg_w(R_EAX) / REG(m.R_M);
		rem = reg_w(R_EAX) % REG(m.R_M);
		reg_w(R_EAX) = (rem & 0xff00) | (quo & 0x00ff);

		print_asm("idiv" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {  
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = reg_w(R_EAX) / MEM_R(addr);
		rem = reg_w(R_EAX) % MEM_R(addr);
		reg_w(R_EAX) = (rem & 0xff00) | (quo & 0x00ff);

		print_asm("idiv" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1 ;
	}  
}

make_helper(concat(idiv_a2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE quo,rem;
	unsigned long long imm ;
	m.val = instr_fetch(eip + 1, 1);
	imm = (REG(R_EDX) << ((DATA_BYTE - 1)* 8)) | (REG(R_EAX));
	printf("%lld ",imm);
	if (m.mod == 3 ){
		quo = imm / REG(m.R_M);
		rem = imm % REG(m.R_M);
		printf("0x%x 0x%x\n",quo,rem);
		REG(R_EAX) = quo;
		REG(R_EDX) = rem;

		print_asm("idiv" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		quo = imm / MEM_R(addr);
		rem = imm % MEM_R(addr);
		printf("0x%x 0x%x\n",quo,rem);
		REG(R_EAX) = quo;
		REG(R_EDX) = rem;

		print_asm("idiv" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
}

#include "exec/template-end.h"
