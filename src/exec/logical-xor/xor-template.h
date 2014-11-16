#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

#define logical_flags(res);\
	eflags.SF = ((DATA_TYPE_S)(res) < 0);\
	eflags.ZF = (res == 0);\
	eflags.PF = 0;\
	while (res){\
		eflags.PF = !eflags.PF;\
		res = res & (res - 1);\
	}

make_helper(concat(xor_i2a_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE res = REG(R_EAX) ^ imm;
	REG(R_EAX) = res;

	logical_flags(res);

	print_asm("xor" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return DATA_BYTE + 1;
}

make_helper(concat(xor_i2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		DATA_TYPE res = REG(m.R_M) ^ imm;
		REG(m.R_M) = res;
		logical_flags(res);

		print_asm("xor" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		DATA_TYPE res = MEM_R(addr) ^ imm;
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("xor" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
}

make_helper(concat(xor_ib2rm_, SUFFIX)){
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		int i;
		imm = instr_fetch(eip + 1 + 1, 1) & 0xff;
		DATA_TYPE sf = imm >> 7;
		for (i = 8, sf = sf << 8; i < DATA_BYTE * 8; i ++, sf <<= 1)
			imm = imm | sf;
		DATA_TYPE res = REG(m.R_M) ^ imm;
		REG(m.R_M) = res;

		logical_flags(res);

		print_asm("xor" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		int i;
		imm = instr_fetch(eip + 1 + len, 1) & 0xff;
		DATA_TYPE sf = imm >> 7;
		for (i = 0, sf = sf << 8; i < DATA_BYTE * 8; i ++, sf <<= 1)
			imm = imm | sf;

		DATA_TYPE res = MEM_R(addr) ^ imm;
		MEM_W(addr,res); 

		logical_flags(res);
		
		print_asm("xor" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 2;
	}
}

make_helper(concat(xor_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE res = REG(m.R_M) ^ REG(m.reg);
		REG(m.R_M) = res;
		logical_flags(res);

		print_asm("xor" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME    (m.R_M));
		 return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = MEM_R(addr) ^ REG(m.reg);
		MEM_W(addr,res);
		logical_flags(res);

		print_asm("xor" str(SUFFIX) " %%%s,%s",REG_NAME(m.reg), ModR_M_asm)     ;
		return len + 1;
	} 
}

make_helper(concat(xor_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		DATA_TYPE res = REG(m.reg) ^ REG(m.R_M);
		REG(m.reg) = res;
		logical_flags(res);

		print_asm("xor" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(    m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = REG(m.reg) ^ MEM_R(addr);
		REG(m.reg) = res;
		logical_flags(res);
		
		print_asm("xor" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm)    ;
		return len + 1;
	}
}

#include "exec/template-end.h"


