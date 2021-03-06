#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(movs_m_, SUFFIX)) {
	MEM_W(reg_l(R_EDI), MEM_R(reg_l(R_ESI)));
	if (eflags.DF)  {
		reg_l(R_EDI) -= DATA_BYTE;
		reg_l(R_ESI) -= DATA_BYTE;
	}
	else { 
			reg_l(R_EDI) += DATA_BYTE;
			reg_l(R_ESI) += DATA_BYTE;
	}

	print_asm("movs" str(SUFFIX) "  %%ds:(%%esi),%%es:(%%edi)");
	return 1;
}

#include "exec/template-end.h"
