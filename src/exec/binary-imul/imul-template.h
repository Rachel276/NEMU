#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "common.h"
#include "memory.h"

make_helper(concat(imul_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		printf("%d %d ",REG(R_EAX),REG(m.R_M));
		if (DATA_BYTE == 1) {
			int16_t res = (int8_t)(DATA_TYPE_S)REG(R_AL) * (int8_t)(DATA_TYPE_S)REG(m.R_M);
			REG(R_AX) = res;
			eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		}
		else if (DATA_BYTE == 2){
			int32_t res = (int16_t)(DATA_TYPE_S)REG(R_AX) * (int16_t)(DATA_TYPE_S)REG(m.R_M);
			REG(R_AX) = (int16_t)(res & 0x0000ffff);
			REG(R_DX) = (int16_t)((res & 0xffff0000) >> 16);
			eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		}
		else {
			int64_t res = (int32_t)(DATA_TYPE_S)REG(R_EAX) * (int32_t)(DATA_TYPE_S)REG(m.R_M);
			REG(R_EAX) = (int32_t)(res & 0x00000000ffffffff);
			REG(R_EDX) = (int32_t)((res & 0xffffffff00000000) >> 32);
			eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		} 
		eflags.OF = eflags.CF;
		print_asm("imul" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if (DATA_BYTE == 1){
			int16_t res = (int8_t)(DATA_TYPE_S)REG(R_AL) * (int8_t)(DATA_TYPE_S)MEM_R(addr);
			REG(R_AX) = res;
			eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		}
		else if (DATA_BYTE == 2){ 
			int32_t res = (int16_t)(DATA_TYPE_S)REG(R_AX) * (int16_t)(DATA_TYPE_S)MEM_R(addr);
			REG(R_AX) = (int16_t)(res & 0x0000ffff);
			REG(R_DX) = (int16_t)((res & 0xffff0000) >> 16);
			eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		}
		else { 
			int64_t res = (int32_t)(DATA_TYPE_S)REG(R_EAX) * (int32_t)(DATA_TYPE_S)MEM_R(addr);
			REG(R_EAX) = (int32_t)(res & 0x00000000ffffffff);
			REG(R_EDX) = (int32_t)((res & 0xffffffff00000000) >> 32);
			eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		}
		eflags.OF = eflags.CF;
		print_asm("imul" str(SUFFIX) " %s", ModR_M_asm);
		return 1 + len;
	}
}

make_helper(concat(imul_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		int64_t res = (int64_t)(DATA_TYPE_S)REG(m.reg) * (int64_t)(DATA_TYPE_S)REG(m.R_M);
		REG(m.reg) = res;
		eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		eflags.OF = eflags.CF;
		print_asm("imul" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		int64_t res = (int64_t)(DATA_TYPE_S)REG(m.reg) * (int64_t)(DATA_TYPE_S)MEM_R(addr);
		REG(m.reg) = res;
		eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		eflags.OF = eflags.CF;
		print_asm("imul" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return 1 + len;
	}
}

make_helper(concat(imul_i_rm_r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm;
	if (m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		int64_t res = (int64_t)(DATA_TYPE_S)REG(m.R_M) * (int64_t)imm;
		REG(m.reg) = res;
		eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		eflags.OF = eflags.CF;
		if (m.R_M == m.reg) 
			print_asm("imul" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		else print_asm("imul" str(SUFFIX) " $0x%x,%%%s,%%%s", imm, REG_NAME(m.R_M), REG_NAME(m.reg));
		return 1 + 1 + DATA_BYTE;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		int64_t res = (int64_t)(DATA_TYPE_S)MEM_R(addr) * (int64_t)imm;
		REG(m.reg) = res;
		eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		eflags.OF = eflags.CF;
		print_asm("imul" str(SUFFIX) " $0x%x,%s,%%%s", imm, ModR_M_asm, REG_NAME(m.reg));
		return 1 + len + DATA_BYTE;
	}
}

make_helper(concat(imul_ib_rm_r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE imm;
	if (m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, 1);
		int64_t res = (int64_t)(DATA_TYPE_S)REG(m.R_M) * (int64_t)imm;
		REG(m.reg) = res;
		eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		eflags.OF = eflags.CF;
		if (m.R_M == m.reg) 
			print_asm("imul" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		else print_asm("imul" str(SUFFIX) " $0x%x,%%%s,%%%s", imm, REG_NAME(m.R_M), REG_NAME(m.reg));
		return 1 + 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		int64_t res = (int64_t)(DATA_TYPE_S)MEM_R(addr) * (int64_t)imm;
		REG(m.reg) = res;
		eflags.CF = !(res >> ((DATA_BYTE << 3) - 1)) || (res >> ((DATA_BYTE << 3) - 1) != 1);
		eflags.OF = eflags.CF;
		print_asm("imul" str(SUFFIX) " $0x%x,%s,%%%s", imm, ModR_M_asm, REG_NAME(m.reg));
		return 1 + len + 1;
	}
}

#include "exec/template-end.h"
