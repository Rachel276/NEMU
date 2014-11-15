#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "exec/helper.h"
#include "cpu/modrm.h"
#include "control-jmp/jmp.h"
#include "data-push/push.h"
#include "binary-sub/sub.h"
#include "data-pop/pop.h"

make_helper(cti_mpr_v) {
	ModR_M m;
	m.val =instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 2: return call_rm_v(eip);
		case 4: return jmp_rm_v(eip);
		case 6: return push_m_v(eip);
	}
	return 0;
}

make_helper(dmi_m_v) {
	ModR_M m;
	m.val =instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 0: return pop_m_v(eip);
	}
	return 0;
}

#endif
