#include "exec/helper.h"

#define DATA_BYTE 1
#include "stos-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "stos-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "stos-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(stos_m_v) {
	return (suffix == 'l' ? stos_m_l(eip) : stos_m_w(eip));
}
