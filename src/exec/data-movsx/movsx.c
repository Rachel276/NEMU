#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movsx-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(movsx_rmb2r_v) {
	return (suffix == 'l' ? movsx_rmb2r_l(eip) : movsx_rmb2r_w(eip));
}

make_helper(movsx_rmw2r_l) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		reg_l(m.reg) = (int32_t)(int16_t)reg_w(m.R_M);
		print_asm("movsxl %%%s,%%%s", regsw[m.R_M], regsl[m.reg]);
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		reg_l(m.reg) = (int32_t)(int16_t)swaddr_read(addr, 1);
		print_asm("movsxl %s,%%%s", ModR_M_asm, regsl[m.reg]);
		return 1 + len;
	}
}


