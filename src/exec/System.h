#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "exec/helper.h"
#include "system-lgdt/lgdt.h"

make_helper(si_m) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {  
		case 2: return lgdt_m(eip);
	}
	return 0;
}

#endif
