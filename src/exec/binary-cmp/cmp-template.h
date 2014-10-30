#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(cmp_i2a_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	DATA_TYPE res = REG(R_EAX) - imm;
	DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
	if (MSB(REG(R_EAX)) != MSB(imm) && MSB(res) == MSB(imm))cpu.OF = 1;
	else cpu.OF = 0;
	cpu.SF = MSB(res);
	if (res == 0)cpu.ZF = 1;
	else cpu.ZF = 0;
	if (REG(R_EAX) < imm)cpu.CF = 1;
	else cpu.CF = 0;
	int i,num=0;
	for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
	{ 
		if ((sf & res) == sf)num++;
		sf <<= 1;
 	}
	if (num % 2 == 1)cpu.PF = 1;
	else cpu.PF = 0;

	print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
	return DATA_BYTE + 1;
} 

make_helper(concat(cmp_i2rm_, SUFFIX)) { 
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		DATA_TYPE res = REG(m.R_M) - imm;
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (MSB(REG(m.R_M)) != MSB(imm) && MSB(res) == MSB(imm))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (REG(m.R_M) < imm)cpu.CF = 1;
		else cpu.CF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		DATA_TYPE res = MEM_R(addr) - imm;
		
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (MSB(MEM_R(addr)) != MSB(imm) && MSB(res) == MSB(imm))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (MEM_R(addr) < imm)cpu.CF = 1;
		else cpu.CF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == 1)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	} 
}

make_helper(concat(cmp_ib2rm_, SUFFIX)){
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		int i,num = 0;
		imm = instr_fetch(eip + 1 + 1, 1) & 0xff;
		DATA_TYPE sf = imm >> 7;
		for (i = 8, sf = sf << 8; i < DATA_BYTE * 8; i ++, sf <<= 1)
			imm = imm | sf;

		DATA_TYPE res = REG(m.R_M) - imm;
		if (MSB(REG(m.R_M)) != MSB(imm) && MSB(res) == MSB(imm))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (REG(m.R_M) < imm)cpu.CF = 1;
		else cpu.CF = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
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

		DATA_TYPE res = MEM_R(addr) - imm;
		if (MSB(MEM_R(addr)) != MSB(imm) && MSB(res) == MSB(imm))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (MEM_R(addr) < imm)cpu.CF = 1;
		else cpu.CF = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 2;
	} 
}

make_helper(concat(cmp_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		DATA_TYPE res = REG(m.R_M) - REG(m.reg);
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (MSB(REG(m.R_M)) != MSB(REG(m.reg)) && MSB(res) == MSB(REG(m.reg)))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (REG(m.R_M) < REG(m.reg))cpu.CF = 1;
		else cpu.CF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME    (m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = MEM_R(addr) - REG(m.reg);
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (MSB(MEM_R(addr)) != MSB(REG(m.reg)) && MSB(res) == MSB(REG(m.reg)))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (MEM_R(addr) < REG(m.reg))cpu.CF = 1;
		else cpu.CF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("cmp" str(SUFFIX) " %%%s,%s",REG_NAME(m.reg), ModR_M_asm)    ;
		return len + 1;
	}
}

make_helper(concat(cmp_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		DATA_TYPE res = REG(m.reg) - REG(m.R_M);
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (MSB(REG(m.reg)) != MSB(REG(m.R_M)) && MSB(res) == MSB(REG(m.R_M)))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (REG(m.reg) < REG(m.R_M))cpu.CF = 1;
		else cpu.CF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(    m.reg));
		return 2;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		DATA_TYPE res = REG(m.reg) - MEM_R(addr);
		DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
		if (MSB(MEM_R(addr)) != MSB(REG(m.reg)) && MSB(res) == MSB(MEM_R(addr)))cpu.OF = 1;
		else cpu.OF = 0;
		cpu.SF = MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (REG(m.reg) < MEM_R(addr))cpu.CF = 1;
		else cpu.CF = 0;
		int i,num=0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{ 
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		print_asm("mov" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm)    ;
		return len + 1;
	}
}

#include "exec/template-end.h"

