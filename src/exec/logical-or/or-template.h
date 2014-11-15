#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(or_i2a_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE res = REG(R_EAX) | imm;
	REG(R_EAX) = res;
	DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
	eflags.CF = eflags.OF = 0;
	eflags.SF = MSB(res);
	if (res == 0)eflags.ZF = 1;
	else eflags.ZF = 0;
	int i,num=0;
	for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
	{
		if ((sf & res) == sf)num++;
		sf <<= 1;
	}
	if (num % 2 == 1)eflags.PF = 1;
	else eflags.PF = 0;

	print_asm("or" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return DATA_BYTE + 1;
}

make_helper(concat(or_i2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		DATA_TYPE res = REG(m.R_M) | imm;
		eflags.CF = eflags.OF = 0;
		REG(m.R_M) = res;
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("or" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		DATA_TYPE res = MEM_R(addr) | imm;
		MEM_W(addr,res);

		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		eflags.CF = eflags.OF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == 1)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("or" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
}

make_helper(concat(or_ib2rm_, SUFFIX)){
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		int i,num = 0;
		imm = instr_fetch(eip + 1 + 1, 1) & 0xff;
		DATA_TYPE sf = imm >> 7;
		for (i = 8, sf = sf << 8; i < DATA_BYTE * 8; i ++, sf <<= 1)
			imm = imm | sf;
		DATA_TYPE res = REG(m.R_M) | imm;
		REG(m.R_M) = res;
		eflags.CF = eflags.OF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("or" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 3;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		int i,num = 0;
		imm = instr_fetch(eip + 1 + len, 1) & 0xff;
		DATA_TYPE sf = imm >> 7;
		for (i = 0, sf = sf << 8; i < DATA_BYTE * 8; i ++, sf <<= 1)
			imm = imm | sf;

		DATA_TYPE res = MEM_R(addr) | imm;
		MEM_W(addr,res);
		eflags.CF = eflags.OF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("or" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 2;
	}
}

make_helper(concat(or_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE res = REG(m.R_M) | REG(m.reg);
		REG(m.R_M) = res;

		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		eflags.CF = eflags.OF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		 print_asm("or" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME    (m.R_M));
		 return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = MEM_R(addr) | REG(m.reg);
		MEM_W(addr,res);

		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		eflags.CF = eflags.OF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("or" str(SUFFIX) " %%%s,%s",REG_NAME(m.reg), ModR_M_asm)     ;
		return len + 1;
	}
}

make_helper(concat(or_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		DATA_TYPE res = REG(m.reg) | REG(m.R_M);
		REG(m.reg) = res;

		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		eflags.OF = eflags.CF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;
		print_asm("or" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(    m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = REG(m.reg) | MEM_R(addr);
		REG(m.reg) = res;

		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		eflags.OF = eflags.CF = 0;
		eflags.SF = MSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;
		print_asm("or" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm)    ;
		return len + 1;
	}
}

#include "exec/template-end.h"


