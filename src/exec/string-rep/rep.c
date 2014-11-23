#include "exec/helper.h"

#define DATA_BYTE 1
#include "rep-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "rep-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "rep-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(rep_movs_m_v) {
	return (suffix == 'l' ? rep_movs_m_l(eip) : rep_movs_m_w(eip));
}
