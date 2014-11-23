#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

int exec(swaddr_t);
make_helper(concat(rep_movs_m_, SUFFIX)) {
	int len = 0;
	char rep_out[40];
	while (reg_l(R_ECX)) {
		len = exec(eip + 1);
		reg_l(R_ECX)--;
	}
	strcpy(rep_out,assembly);
	print_asm("rep" str(SUFFIX) " %s",rep_out);
	return 1 + len;
}

#include "exec/template-end.h"
