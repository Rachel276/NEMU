#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(test_i2r_, SUFFIX)){
    DATA_TYPE imm = instr_fetch(eip ,DATA_BYTE);
	int reg_code = instr_fetch(eip + DATA_BYTE, 1) & 0x7;
	DATA_TYPE res = REG(reg_code) & imm;
	DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
	if (sf && res == 1)cpu.SF = 1;
	else cpu.SF = 0;
	if (res == 0)cpu.ZF = 1;
	else cpu.ZF = 0;
	int i,num=0;
	for (i = 0,sf = 1,res = res & 0xff;i < 8; i++)
	{
		if (sf && res == 1)num++;
		sf <<= 1;
	}
	if (num % 2 == 1)cpu.PF = 1;
	else cpu.PF = 0;
	cpu.CF = cpu.OF = 0;

	print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(reg_code));
	return DATA_BYTE + 1;
}

make_helper(concat(test_i2rm_, SUFFIX)){
	ModR_M m;
	DATA_TYPE imm = instr_fetch(eip ,DATA_BYTE);
	m.val = instr_fetch(eip + DATA_BYTE + 1, 1);
	if (m.mod == 3){
//		REG(m.R_M) = REG(m.R_M) & imm;
		DATA_TYPE res = REG(m.R_M) & imm;
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (sf && res == 1)cpu.SF = 1;
		else cpu.SF = 0;
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 1,res = res & 0xff;i < 8; i++)
		{
			if (sf && res == 1)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		cpu.CF = cpu.OF = 0;

		print_asm("test" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
 	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + DATA_BYTE + 1, &addr);
//		MEM_W(addr, MEM_R(addr) & imm);
		DATA_TYPE res = MEM_R(addr) & imm;
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (sf && res == 1)cpu.SF = 1;
		else cpu.SF = 0;
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 1,res = res & 0xff;i < 8; i++)
		{
			if (sf && res == 1)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		cpu.CF = cpu.OF = 0;

		print_asm("test" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
 	}
}

make_helper(concat(test_r2rm_, SUFFIX)){
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
//		REG(m.reg) = REG(m.reg) & REG(m.R_M);
		DATA_TYPE res = REG(m.reg) & REG(m.R_M);
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (sf && res == 1)cpu.SF = 1;
		else cpu.SF = 0;
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 1,res = res & 0xff;i < 8; i++)
		{
			if (sf && res == 1)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		cpu.CF = cpu.OF = 0;
		print_asm("test" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
 	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
//		REG(m.reg) = MEM_R(addr) & REG(m.reg);
		DATA_TYPE res = MEM_R(addr) & REG(m.reg);
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (sf && res == 1)cpu.SF = 1;
		else cpu.SF = 0;
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 1,res = res & 0xff;i < 8; i++)
		{
			if (sf && res == 1)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		print_asm("test" str(SUFFIX) " %%%s,%s",REG_NAME(m.reg), ModR_M_asm);
		return len + 1;
 	}
}

#include "exec/template-end.h"
