#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

#define logical_flags(res);\
	eflags.SF = ((DATA_TYPE_S)(res) < 0);\
	eflags.ZF = ((res) == 0);\
	eflags.PF = 0;\
	while (res != 0){\
		eflags.PF = !eflags.PF;\
		res = res & (res - 1);\
	}

make_helper(concat(sal_o2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = REG(m.R_M) << 1;
		eflags.CF =  MSB(REG(m.R_M));
		REG(m.R_M) = imm;
		if (eflags.CF != MSB(imm))eflags.OF = 1;
		else eflags.OF = 0;

		logical_flags(imm);
		
		print_asm("sal" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr) << 1;
		eflags.CF =  MSB(MEM_R(addr));
		MEM_W(addr, imm);
        if (eflags.CF != MSB(imm))eflags.OF = 1;
		else eflags.OF = 0;

		logical_flags(imm);

		print_asm("sal" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
} 

make_helper(concat(sar_o2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = REG(m.R_M) >> 1;
		eflags.OF =  MSB(REG(m.R_M));
		eflags.CF = REG(m.R_M) & 0x1;
		REG(m.R_M) = imm;
		
		logical_flags(imm);

		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}  
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr) >> 1;
		eflags.OF =  MSB(MEM_R(addr));
		eflags.CF = MEM_R(addr) & 0x1;
		MEM_W(addr, imm);

		logical_flags(imm);

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
}

make_helper(concat(shr_o2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = (REG(m.R_M) >> 1) & ~(1 << ((DATA_BYTE << 3) - 1));
		eflags.OF =  MSB(REG(m.R_M));
		eflags.CF = REG(m.R_M) & 0x1;
		REG(m.R_M) = imm;
		
		logical_flags(imm);

		print_asm("shr" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = (MEM_R(addr) >> 1) & ~(1 << ((DATA_BYTE << 3) - 1));
		eflags.OF =  MSB(MEM_R(addr));
		eflags.CF = MEM_R(addr) & 0x1;
		MEM_W(addr, imm);

		logical_flags(imm);

		print_asm("shr" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
}

make_helper(concat(sal_c2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		res = REG(m.R_M) << REG(R_CL);
		if (REG(R_CL) == 0)eflags.CF =  MSB(REG(m.R_M));
		else eflags.CF = REG(m.R_M) >> ((DATA_BYTE << 3) - REG(R_CL));
		REG(m.R_M) = res;
		
		logical_flags(res);

		print_asm("sal" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		res = MEM_R(addr) << REG(R_CL);
		if (REG(R_CL) == 0)eflags.CF =  MSB(MEM_R(addr));
		else eflags.CF = MEM_R(addr) >> ((DATA_BYTE << 3) - REG(R_CL));
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("sal" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	} 
}

make_helper(concat(sar_c2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){  
		res = REG(m.R_M) >> REG(R_CL);
		if (REG(R_CL) == 0)eflags.CF =  REG(m.R_M) & 0x1;
		else eflags.CF = REG(m.R_M) >> (REG(R_CL) - 1);
		REG(m.R_M) = res;
		
		logical_flags(res);

		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		res = MEM_R(addr) >> REG(R_CL);
		if (REG(R_CL) == 0)eflags.CF =  MEM_R(addr);
		else eflags.CF = MEM_R(addr) >> (REG(R_CL) - 1);
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
 	} 
}

make_helper(concat(shr_c2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){  
		if (REG(R_CL) == 0) {
			eflags.CF =  REG(m.R_M) & 0x1;
			res = REG(m.R_M) >> REG(R_CL);
		}
		else {
			eflags.CF = REG(m.R_M) >> (REG(R_CL) - 1);
			res = (REG(m.R_M) >> REG(R_CL)) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (REG(R_CL)));
		}
		REG(m.R_M) = res;
		
		logical_flags(res);

		print_asm("shr" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if (REG(R_CL) == 0) {
			eflags.CF = MEM_R(addr) & 0x1;
			res = MEM_R(addr) >> REG(R_CL);
		}
		else {
			eflags.CF = MEM_R(addr) >> (REG(R_CL) - 1);
			res = (MEM_R(addr) >> REG(R_CL)) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (REG(R_CL)));
		}
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("shr" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
 	} 
}

make_helper(concat(sal_ib2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, 1) & 0x1f;
		if (imm == 0)eflags.CF = MSB(REG(m.R_M));
		else eflags.CF = REG(m.R_M) >> ((DATA_BYTE << 3) - imm);
		res = REG(m.R_M) << imm;
		REG(m.R_M) = res;
		
		logical_flags(res);

		print_asm("sal" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1) & 0x1f;
		if (imm == 0)eflags.CF = MSB(MEM_R(addr));
		else eflags.CF = MEM_R(addr) >> ((DATA_BYTE << 3) - imm);
		res = MEM_R(addr) << imm;
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("sal" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 1 + 1;
	}  
}

make_helper(concat(sar_ib2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, 1) & 0x1f;
		if (imm == 0)eflags.CF = REG(m.R_M);
		else eflags.CF = REG(m.R_M) >> (imm - 1);
		res = REG(m.R_M) >> imm;
		REG(m.R_M) = res;
		
		logical_flags(res);

		print_asm("sar" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1) & 0x1f;
		if (imm == 0)eflags.CF = MEM_R(addr);
		else eflags.CF = MEM_R(addr) >> (imm - 1);
		res = MEM_R(addr) >> imm;
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("sar" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 1 + 1;
	}  
}

make_helper(concat(shr_ib2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, 1) & 0x1f;
		if (imm == 0) {
			eflags.CF = REG(m.R_M);
			res = REG(m.R_M);
		}
		else {
			eflags.CF = REG(m.R_M) >> (imm - 1);
			res = (REG(m.R_M) >> imm) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (imm - 1));
		}
		REG(m.R_M) = res;
		
		logical_flags(res);

		print_asm("shr" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1) & 0x1f;
		if (imm == 0) {
			eflags.CF = MEM_R(addr);
			res = MEM_R(addr);
		}
		else {
			eflags.CF = MEM_R(addr) >> (imm - 1);
			res = (MEM_R(addr) >> imm) & ~((1 << ((DATA_BYTE << 3) - 1)) >> (imm - 1));
		}
		MEM_W(addr,res);

		logical_flags(res);

		print_asm("shr" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 1 + 1;
	}  
}

/*make_helper(concat(sar_o2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm,sf;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = REG(m.R_M) >> 1;
		eflags.SF = MSB(imm);eflags.CF = LSB(imm);
		if (imm == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		REG(m.R_M) = imm;
		int i,num = 0;
		for (i = 0,sf = 0x1,imm = imm & 0xff;i < 8; i++)
		{
			if ((sf & imm) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;
		
		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = MEM_R(addr) >> 1;
		eflags.SF = MSB(imm);eflags.CF = LSB(imm);
		if (imm == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		MEM_W(addr,imm);
		int i,num = 0;
		for (i = 0,sf = 0x1,imm = imm & 0xff;i < 8; i++)
		{
			if ((sf & imm) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(sar_c2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm,sf,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){ 
		imm = REG(R_ECX) & 0xff;
		res = REG(m.R_M) >> imm;
		eflags.SF = MSB(res);eflags.CF = LSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		REG(m.R_M) = res;
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;
		
		print_asm("sar" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = REG(R_ECX) & 0xff;
		res = MEM_R(addr) >> imm;
		eflags.SF = MSB(res);eflags.CF = LSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		MEM_W(addr,res);
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("sar" str(SUFFIX) " %s", ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(sar_ib2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE_S imm,sf,res;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		imm = instr_fetch(eip + 1 + 1, 1);
		res = REG(m.R_M) >> imm;
	//	printf("%d ",REG(R_EAX));
		eflags.SF = MSB(res);eflags.CF = LSB(res);
	//	printf("%d ",REG(R_EAX));
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
	//	printf("%d ",REG(R_EAX));
		REG(m.R_M) = res;
	//	printf("%d ",REG(R_EAX));
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
	  // printf("%d ",REG(R_EAX));	   
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		//printf("%d\n",REG(R_EAX));	
		print_asm("sar" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		res = MEM_R(addr) >> imm;
		eflags.SF = MSB(res);eflags.CF = LSB(res);
		if (res == 0)eflags.ZF = 1;
		else eflags.ZF = 0;
		MEM_W(addr,res);
		int i,num = 0;
		for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
		{
			if ((sf & res) == sf)num++;
			sf <<= 1;
		}
		if (num % 2 == 1)eflags.PF = 1;
		else eflags.PF = 0;

		print_asm("sar" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 1 + 1;
	}     
}*/

#include "exec/template-end.h"
