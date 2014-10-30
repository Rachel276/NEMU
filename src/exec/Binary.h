//#ifndef __BINARY_H__
//#define __BINARY_H__

#include "exec/helper.h"
//#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "binary-add/add.h"
#include "binary-cmp/cmp.h"

make_helper(bai_i2rm_b)
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 0: return add_i2rm_b(eip);
		case 7: return cmp_i2rm_b(eip);
	}
	return 0;
}

make_helper(bai_i2rm_v)
{
	ModR_M m;
	m.val =instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 0: return add_i2rm_v(eip);
		case 7: return cmp_i2rm_v(eip);
	} 
	return 0;
}

make_helper(bai_ib2rm_v)
{
	ModR_M m;
	m.val =instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 0: return add_ib2rm_v(eip);
		case 7: return cmp_ib2rm_v(eip);
	}
	return 0;
}

//#include "exec/template-end.h"

