#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(jmp_rel_v) {
	return (suffix == 'l' ? jmp_rel_l(eip) : jmp_rel_w(eip));
}

make_helper(jmp_rm_v) {
	return (suffix == 'l' ? jmp_rm_l(eip) : jmp_rm_w(eip));
}

make_helper(jmp_ptr) {
	uint16_t cs = instr_fetch(eip + 5, 2);
	uint32_t imm = instr_fetch(eip + 1, 4);

	cpu.cs.val = cs;
	cpu.eip = imm - (1 + 6);

	print_asm("ljmp  $%x,$%x", cs, imm);
	return 1 + 6;
}
