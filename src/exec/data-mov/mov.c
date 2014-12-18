#include "exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

make_helper(mov_i2r_v) {
	return (suffix == 'l' ? mov_i2r_l(eip) : mov_i2r_w(eip));
}

make_helper(mov_i2rm_v) {
	return (suffix == 'l' ? mov_i2rm_l(eip) : mov_i2rm_w(eip));
}

make_helper(mov_r2rm_v) {
	return (suffix == 'l' ? mov_r2rm_l(eip) : mov_r2rm_w(eip));
}

make_helper(mov_rm2r_v) {
	return (suffix == 'l' ? mov_rm2r_l(eip) : mov_rm2r_w(eip));
}

make_helper(mov_a2moffs_v) {
	return (suffix == 'l' ? mov_a2moffs_l(eip) : mov_a2moffs_w(eip));
}

make_helper(mov_moffs2a_v) {
	return (suffix == 'l' ? mov_moffs2a_l(eip) : mov_moffs2a_w(eip));
}

make_helper(mov_cr2r) {
	int reg_code = instr_fetch(eip + 1, 1) & 0x7;
	reg_l(reg_code) = cpu.cr0.val;
	print_asm("movl  %%cr0, %%%s", regsl[reg_code]);
	return 1 + 1;
}

make_helper(mov_r2cr) {
	int reg_code = instr_fetch(eip + 1, 1) & 0x7;
	cpu.cr0.val = reg_l(reg_code);
	print_asm("movl  %%%s, %%cr0", regsl[reg_code]);
	return 1 + 1;
}

make_helper(mov_s2rm) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		reg_w(m.R_M) = sreg(m.reg).val;
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", sregs[m.reg], regsw[m.R_M]);
		return 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		swaddr_write(addr, 2, sreg(m.reg).val);

		print_asm("mov" str(SUFFIX) " %%%s,%s", sregs[m.reg], ModR_M_asm);
		return len + 1;
	} 
}

make_helper(mov_rm2s) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		sreg(m.reg).val = reg_w(m.R_M);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", regsw[m.R_M], sregs[m.reg]);
		return 1 + 1;
	} 
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		sreg(m.reg).val = swaddr_read(addr, 2);

		print_asm("mov" str(SUFFIX) " %s,%%%s", ModR_M_asm, sregs[m.reg]);
		return len + 1;
	} 
}






