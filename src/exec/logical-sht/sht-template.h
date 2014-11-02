#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(sal_o2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm,sf;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = REG(m.R_M) << 1;
		cpu.SF = cpu.CF =  MSB(imm);
		if (imm == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (MSB(REG(m.R_M)) != MSB(imm))cpu.OF = 1;
		else cpu.OF = 0;
		REG(m.R_M) = imm;
		int i,num = 0;
		for (i = 0,sf = 0x1,imm = imm & 0xff;i < 8; i++)
		{
			if ((sf & imm) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		
		print_asm("sal" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr) << 1;
		cpu.SF = cpu.CF =  MSB(imm);
		if (imm == 0)cpu.ZF = 1;
        if (MSB(MEM_R(addr)) != MSB(imm))cpu.OF = 1;
		else cpu.OF = 0;
		MEM_W(addr,imm);
		int i,num = 0;
		for (i = 0,sf = 0x1,imm = imm & 0xff;i < 8; i++)
		{
			if ((sf & imm) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("sal" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
}

make_helper(concat(sal_c2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm,sf,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = REG(R_ECX) & 0xff;
		res = REG(m.R_M) << imm;
		cpu.SF = cpu.CF =  MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (MSB(REG(m.R_M)) != MSB(res))cpu.OF = 1;
		else cpu.OF = 0;
		REG(m.R_M) = res;
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		
		print_asm("sal" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = REG(R_ECX) & 0xff;
		res = MEM_R(addr) << imm;
		cpu.SF = cpu.CF =  MSB(res);
		if (res == 0)cpu.ZF = 1;
        if (MSB(MEM_R(addr)) != MSB(res))cpu.OF = 1;
		else cpu.OF = 0;
		MEM_W(addr,res);
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("sal" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
}

make_helper(concat(sal_ib2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm,sf,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, 1);
		res = REG(m.R_M) << imm;
		cpu.SF = cpu.CF =  MSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		if (MSB(REG(m.R_M)) != MSB(res))cpu.OF = 1;
		else cpu.OF = 0;
		REG(m.R_M) = res;
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		
		print_asm("sal" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		res = MEM_R(addr) << imm;
		cpu.SF = cpu.CF =  MSB(res);
		if (res == 0)cpu.ZF = 1;
        if (MSB(MEM_R(addr)) != MSB(res))cpu.OF = 1;
		else cpu.OF = 0;
		MEM_W(addr,res);
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("sal" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1 + 1;
	} 
}


make_helper(concat(sar_o2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm,sf;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = REG(m.R_M) >> 1;
		cpu.SF = MSB(imm);cpu.CF = LSB(imm);
		if (imm == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		REG(m.R_M) = imm;
		int i,num = 0;
		for (i = 0,sf = 0x1,imm = imm & 0xff;i < 8; i++)
		{
			if ((sf & imm) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		
		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr) >> 1;
		cpu.SF = MSB(imm);cpu.CF = LSB(imm);
		if (imm == 0)cpu.ZF = 1;
		MEM_W(addr,imm);
		int i,num = 0;
		for (i = 0,sf = 0x1,imm = imm & 0xff;i < 8; i++)
		{
			if ((sf & imm) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(sar_c2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm,sf,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = REG(R_ECX) & 0xff;
		res = REG(m.R_M) >> imm;
		cpu.SF = MSB(res);cpu.CF = LSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		REG(m.R_M) = res;
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		
		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = REG(R_ECX) & 0xff;
		res = MEM_R(addr) >> imm;
		cpu.SF = MSB(res);cpu.CF = LSB(res);
		if (res == 0)cpu.ZF = 1;
		MEM_W(addr,res);
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(sar_ib2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm,sf,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, 1);
		res = REG(m.R_M) >> imm;
		cpu.SF = MSB(res);cpu.CF = LSB(res);
		if (res == 0)cpu.ZF = 1;
		else cpu.ZF = 0;
		REG(m.R_M) = res;
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;
		
		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		res = MEM_R(addr) >> imm;
		cpu.SF = MSB(res);cpu.CF = LSB(res);
		if (res == 0)cpu.ZF = 1;
		MEM_W(addr,res);
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)cpu.PF = 1;
		else cpu.PF = 0;

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1 + 1;
	} 
}

#include "exec/template-end.h"
