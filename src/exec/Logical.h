#ifndef __LOGICAL_H__
#define __LOGICAL_H__

#include "exec/helper.h"
#include "cpu/modrm.h"
#include "logical-sht/sht.h"

make_helper(li_o2rm_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 4: return sal_o2rm_b(eip);
		case 7: return sar_o2rm_b(eip);
	}
	return 0;
}

make_helper(li_o2rm_v) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 4: return sal_o2rm_v(eip);
		case 7: return sar_o2rm_v(eip);
	}
	return 0;
}

make_helper(li_c2rm_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 4: return sal_c2rm_b(eip);
		case 7: return sar_c2rm_b(eip);
	}
	return 0;
}

make_helper(li_c2rm_v) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 4: return sal_c2rm_v(eip);
		case 7: return sar_c2rm_v(eip);
	}
	return 0;
}

make_helper(li_ib2rm_b) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 4: return sal_ib2rm_b(eip);
		case 7: return sar_ib2rm_b(eip);
	}
	return 0;
}

make_helper(li_ib2rm_v) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 4: return sal_ib2rm_v(eip);
		case 7: return sar_ib2rm_v(eip);
	}
	return 0;
}

#endif
