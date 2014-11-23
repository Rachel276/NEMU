#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

#define mflags(ret, lhs, imm);\
	eflags.SF = ((DATA_TYPE_S)(rst) < 0);\
	eflags.ZF = ((rst) == 0);\
	eflags.CF = lhs < imm;\
	eflags.OF = (MSB(lhs) != MSB(imm)) && (MSB(rst) == MSB(imm));\
	eflags.PF = 0;\
	while (rst){\
		eflags.PF = !eflags.PF;\
		rst = rst & (rst - 1);\
	}

make_helper(concat(cmp_i2a_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE rst = REG(R_EAX) - imm;
	DATA_TYPE lhs = REG(R_EAX);
	mflags(rst,lhs,imm);
//	REG(R_EAX) = rst;
	print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return DATA_BYTE + 1;
}  

make_helper(concat(cmp_i2rm_, SUFFIX)) { 
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		DATA_TYPE rst = REG(m.R_M) - imm;
		DATA_TYPE lhs = REG(m.R_M);
		mflags(rst,lhs,imm);
	//	REG(m.R_M) = rst;
		print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}  
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		DATA_TYPE rst = MEM_R(addr) - imm;
		DATA_TYPE lhs = MEM_R(addr);
		mflags(rst,lhs,imm);
	//	MEM_W(addr,rst);
		print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	} 
}

make_helper(concat(cmp_ib2rm_, SUFFIX)){
	ModR_M m;
	DATA_TYPE_S imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = (DATA_TYPE_S)(instr_fetch(eip + 1 + 1, 1));
		DATA_TYPE rst = REG(m.R_M) - imm;
		DATA_TYPE lhs = REG(m.R_M);
		mflags(rst,lhs,imm);
	//	REG(m.R_M) = rst;
		print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}  
	else {    
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = (DATA_TYPE_S)(instr_fetch(eip + 1 + len, 1));
		DATA_TYPE rst = MEM_R(addr) - imm;
		DATA_TYPE lhs = MEM_R(addr);
	//	MEM_W(addr,rst);
		mflags(rst,lhs,imm);
	//	printf("0x%x 0x%x 0x%x\n",res,MEM_R(addr),imm);
		print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 2;
	} 
}

make_helper(concat(cmp_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm = REG(m.reg);
	if (m.mod == 3){
		DATA_TYPE rst = REG(m.R_M) - imm;
		DATA_TYPE lhs = REG(m.R_M);
		mflags(rst,lhs,imm);
	//	REG(m.R_M) = rst;
	//	printf("%d %d\n",REG(m.R_M) ,REG(m.reg));

		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME    (m.R_M));
		return 2;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE rst = MEM_R(addr) - imm;
		DATA_TYPE lhs = MEM_R(addr);
		mflags(rst,lhs,imm);
	//	MEM_W(addr,rst);
		print_asm("cmp" str(SUFFIX) " %%%s,%s",REG_NAME(m.reg), ModR_M_asm)    ;
		return len + 1;
	}
}

make_helper(concat(cmp_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE lhs = REG(m.reg);
	if(m.mod == 3) {
		DATA_TYPE imm = REG(m.R_M);
		DATA_TYPE rst = lhs - imm;
		mflags(rst,lhs,imm);
	//	REG(m.reg) = rst;
		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(    m.reg));
		return 2;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);	
		DATA_TYPE imm = MEM_R(addr);
		DATA_TYPE rst = lhs - imm;
		mflags(rst,lhs,imm);
	//	REG(m.reg) = rst;
		print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm)    ;
		return len + 1;
	}
}

#include "exec/template-end.h"

