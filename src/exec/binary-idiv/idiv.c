#include "exec/helper.h"

#define DATA_BYTE 1
#include "idiv-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "idiv-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "idiv-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(idiv_rm_v) {
	return (suffix == 'l' ? idiv_rm_l(eip) : idiv_rm_w(eip));
}
