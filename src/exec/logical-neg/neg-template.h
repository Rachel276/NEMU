#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(neg_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3){
		if (REG(m.R_M) == 0)eflags.CF = 0;
		else eflags.CF = 1;
		REG(m.R_M) = -REG(m.R_M);

		print_asm("neg" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		return 1 + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if (MEM_R(addr) == 0)eflags.CF = 0;
		else eflags.CF = 1;
		MEM_W(addr,MEM_R(addr));

		print_asm("neg" str(SUFFIX) "%s", ModR_M_asm);
		return len + 1;
	}
}

#include "exec/template-end.h"

