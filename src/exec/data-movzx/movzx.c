#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "movzx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movzx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movzx-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(movzx_rmb2r_v) {
	return (suffix == 'l' ? movzx_rmb2r_l(eip) : movzx_rmb2r_w(eip));
}

make_helper(movzx_rmw2r_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		reg_l(m.reg) = (uint32_t)(uint16_t)reg_w(m.R_M);
		print_asm("movzxl %%%s,%%%s", regsw[m.R_M], regsl[m.reg]);
		return 2;
	}
	else { 
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		reg_l(m.reg) = (uint32_t)(uint16_t)swaddr_read(addr, 1);
		print_asm("movzxl %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}
