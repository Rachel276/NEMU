#ifndef __BINARY_H_
#define __BINARY_H__

#include "exec/helper.h"
//#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "binary-add/add.h"
#include "binary-cmp/cmp.h"
#include "binary-sub/sub.h"
#include "binary-idiv/idiv.h"
#include "logical-test/test.h"
#include "logical-xor/xor.h"
#include "logical-and/and.h"
#include "logical-or/or.h"

make_helper(bai_i2rm_b)
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.opcode) { 
		case 0: return add_i2rm_b(eip);
		case 1: return or_i2rm_b(eip);
		case 3: return sbb_i2rm_b(eip);
		case 4: return and_i2rm_b(eip);
		case 5: return sub_i2rm_b(eip);
		case 6: return xor_i2rm_b(eip);
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
		case 1: return or_i2rm_v(eip);
		case 3: return sbb_i2rm_v(eip);
		case 4: return and_i2rm_v(eip);
		case 5: return sub_i2rm_v(eip);
		case 6: return xor_i2rm_v(eip);
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
		case 1: return or_ib2rm_v(eip);
		case 3: return sbb_ib2rm_v(eip);
		case 4: return and_ib2rm_v(eip);
		case 5: return sub_ib2rm_v(eip);
		case 6: return xor_ib2rm_v(eip);
		case 7: return cmp_ib2rm_v(eip);
	 }    
	return 0;
}

make_helper(bai_rm_b)
{
	ModR_M m;
	m.val =instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 0: return test_i2rm_b(eip);
		case 7: return idiv_rm_b(eip);
	}  
	return 0;
}

make_helper(bai_rm_v)
{
	ModR_M m;
	m.val =instr_fetch(eip + 1, 1);
	switch (m.opcode) {
		case 0: return test_i2rm_v(eip);
		case 7: return idiv_a2rm_v(eip);
	} 
	return 0;
}
//#include "exec/template-end.h"
#endif
