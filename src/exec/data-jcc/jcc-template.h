#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(je_rel_, SUFFIX)) {
		DATA_TYPE rel = instr_fetch(eip, DATA_BYTE);
		
		print_asm("je" str(SUFFIX) " %x", eip + rel); 
		if (cpu.ZF == 1)return DATA_BYTE +1;
		else return rel;
}

#include "exec/template-end.h"
