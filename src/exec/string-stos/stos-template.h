#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

extern uint8_t current_sreg;
make_helper(concat(stos_m_, SUFFIX)) {
	current_sreg = ES;
	MEM_W(reg_l(R_EDI), REG(R_EAX));
	if (eflags.DF)  
		reg_l(R_EDI) -= DATA_BYTE;
	else  
		reg_l(R_EDI) += DATA_BYTE;

	print_asm("stos" str(SUFFIX) "  %%%s:%%es:(%%edi)", REG_NAME(R_EAX));
	return 1;
}

#include "exec/template-end.h"
