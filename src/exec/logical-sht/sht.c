#include "exec/helper.h"

#define DATA_BYTE 1
#include "sht-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sht-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sht-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(sal_o2rm_v) {
	return (suffix == 'l' ? sal_o2rm_l(eip) : sal_o2rm_w(eip));
}

make_helper(sal_c2rm_v) {
	return (suffix == 'l' ? sal_c2rm_l(eip) : sal_c2rm_w(eip));
}

make_helper(sal_ib2rm_v) {
	return (suffix == 'l' ? sal_ib2rm_l(eip) : sal_ib2rm_w(eip));
}

make_helper(sar_o2rm_v) {
	return (suffix == 'l' ? sar_o2rm_l(eip) : sar_o2rm_w(eip));
}

make_helper(sar_c2rm_v) {
	return (suffix == 'l' ? sar_c2rm_l(eip) : sar_c2rm_w(eip));
}

make_helper(sar_ib2rm_v) {
	return (suffix == 'l' ? sar_ib2rm_l(eip) : sar_ib2rm_w(eip));
}


