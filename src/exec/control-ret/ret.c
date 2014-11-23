#include "exec/helper.h"

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ret-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(ret_v) {
	return (suffix == 'l' ? ret_l(eip) : ret_w(eip));
}

make_helper(ret_i_v) {
	return (suffix == 'l' ? ret_i_l(eip) : ret_i_w(eip));
}
