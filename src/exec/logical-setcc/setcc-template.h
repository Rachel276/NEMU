#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(seta_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if (m.mod == 3) {
			print_asm("seta" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.CF == 0 &&  eflags.ZF == 0)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		} 
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("seta" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.CF == 0 &&  eflags.ZF == 0)MEM_W(addr,1);
			else MEM_W(addr,0);
			return len + 1;
		}
}

make_helper(concat(setae_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setae" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.CF == 0)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setae" str(SUFFIX) " %s",  ModR_M_asm);
			if (eflags.CF == 0)MEM_W(addr,1);
			else MEM_W(addr,0);
			return len + 1;
		}
}

make_helper(concat(setb_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if (m.mod == 3) {	
			print_asm("setb" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.CF == 1)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		} 
		else  {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setb" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.CF == 1)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		} 
}

make_helper(concat(setbe_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if (m.mod == 3) {
			print_asm("setbe" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.CF == 1 ||  eflags.ZF == 1)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setbe" str(SUFFIX) " %s",  ModR_M_asm);
			if (eflags.CF == 1 ||  eflags.ZF == 1)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(sete_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("sete" str(SUFFIX) " %%%s", REG_NAME(m.R_M)); 
			if (eflags.ZF == 1)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("sete" str(SUFFIX) " %s",  ModR_M_asm);
			if (eflags.ZF == 1)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setg_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setg" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.ZF == 0 && eflags.SF == eflags.OF)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setg" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.ZF == 0 && eflags.SF == eflags.OF)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setge_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setge" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.SF == eflags.OF)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setge" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.SF == eflags.OF)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setl_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setl" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.SF != eflags.OF)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setl" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.SF != eflags.OF)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setle_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setle" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.ZF == 1 || eflags.SF != eflags.OF)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setle" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.ZF == 1 || eflags.SF != eflags.OF)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setne_rm_, SUFFIX)) {
		ModR_M m;
		printf("%x\n",eip);
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) { 
			print_asm("setne" str(SUFFIX) " %%%s",REG_NAME(m.R_M));
			if (eflags.ZF == 0)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setne" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.ZF == 0)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setno_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setno" str(SUFFIX) " %%%s",REG_NAME(m.R_M));
			if (eflags.OF == 0)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		} 
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setno" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.OF == 0)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setnp_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setnp" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
			if (eflags.PF == 0)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setnp" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.PF == 0)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setns_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setns" str(SUFFIX) " %%%s",REG_NAME(m.R_M));
			if (eflags.SF == 0)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setns" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.SF == 0)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(seto_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("seto" str(SUFFIX) " %%%s",REG_NAME(m.R_M));
			if (eflags.OF == 1)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("seto" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.OF == 1)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(setp_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("setp" str(SUFFIX) " %%%s",REG_NAME(m.R_M));
			if (eflags.PF == 1)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("setp" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.PF == 1)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

make_helper(concat(sets_rm_, SUFFIX)) {
		ModR_M m;
		m.val = instr_fetch(eip + 1, 1);
		if(m.mod == 3) {
			print_asm("sets" str(SUFFIX) " %%%s",REG_NAME(m.R_M));
			if (eflags.SF == 1)REG(m.R_M) = 1;
			else REG(m.R_M) = 0;
			return 1 + 1;
		}
		else {
			swaddr_t addr;
			int len = read_ModR_M(eip + 1, &addr);
			print_asm("sets" str(SUFFIX) " %s", ModR_M_asm);
			if (eflags.SF == 1)MEM_W(addr, 1);
			else MEM_W(addr, 0);
			return len + 1;
		}
}

#include "exec/template-end.h"
