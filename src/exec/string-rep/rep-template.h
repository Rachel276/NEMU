#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(exec);
make_helper(concat(rep_movs_m_, SUFFIX)) {
	int len = 1;
	char rep_out[40];
	while (reg_l(R_CX)) {
		len = exec(eip + 1);
		reg_l(R_CX)--;
	}
	strcpy(rep_out,assembly);
	print_asm("rep" str(SUFFIX) " %s",rep_out);
	return 1 + len;
}

#include "exec/template-end.h"
