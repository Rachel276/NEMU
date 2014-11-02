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

make_helper(idiv_a2rm_v) {
	return (suffix == 'l' ? idiv_a2rm_l(eip) : idiv_a2rm_w(eip));
}
