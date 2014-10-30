#include "exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(je_rel_v) {
	return (suffix == 'l' ? je_rel_l(eip) : je_rel_w(eip));
}
