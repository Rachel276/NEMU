#include "exec/helper.h"

#include "all-instr.h"

typedef int (*helper_fun)(swaddr_t);


/* TODO: Add more instructions!!! */

make_helper(nop){
	return 1;
}

helper_fun opcode_table [256] = {
/* 0x00 */	add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v, 
/* 0x04 */	add_i2a_b, add_i2a_v, inv, inv, 
/* 0x08 */	or_r2rm_b, or_r2rm_v, or_rm2r_b, or_rm2r_v, 
/* 0x0c */	or_i2a_b, or_i2a_v, inv, opcode, 
/* 0x10 */	adc_r2rm_b, adc_r2rm_v, adc_rm2r_b, adc_rm2r_v, 
/* 0x14 */	adc_i2a_b, adc_i2a_v, inv, inv, 
/* 0x18 */	sbb_r2rm_b, sbb_r2rm_v, sbb_rm2r_b, sbb_rm2r_v, 
/* 0x1c */	sbb_i2a_b, sbb_i2a_v, inv, inv, 
/* 0x20 */  and_r2rm_b, and_r2rm_v, and_rm2r_b, and_rm2r_v, 
/* 0x24 */	and_i2a_b, and_i2a_v, inv, inv,
/* 0x28 */	sub_r2rm_b, sub_r2rm_v, sub_rm2r_b, sub_rm2r_v, 
/* 0x2c */	sub_i2a_b, sub_i2a_v, inv, inv, 
/* 0x30 */	xor_r2rm_b, xor_r2rm_v, xor_rm2r_b, xor_rm2r_v, 
/* 0x34 */	xor_i2a_b, xor_i2a_v, inv, inv,
/* 0x38 */	cmp_r2rm_b, cmp_r2rm_v, cmp_rm2r_b, cmp_rm2r_v, 
/* 0x3c */	cmp_i2a_b, cmp_i2a_v, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	push_r_v, push_r_v, push_r_v, push_r_v, 
/* 0x54 */	push_r_v, push_r_v, push_r_v, push_r_v,
/* 0x58 */	pop_r_v, pop_r_v, pop_r_v, pop_r_v, 
/* 0x5c */	pop_r_v, pop_r_v, pop_r_v, pop_r_v, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, data_size, inv,
/* 0x68 */	push_i_v, imul_i_rm_r_v, push_i_b, imul_ib_rm_r_v, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	jo_rel_b, jno_rel_b, jb_rel_b, jae_rel_b,
/* 0x74 */	je_rel_b, jne_rel_b, jbe_rel_b, ja_rel_b,
/* 0x78 */	js_rel_b, jns_rel_b, jp_rel_b, jnp_rel_b, 
/* 0x7c */	jl_rel_b, jge_rel_b, jle_rel_b, jg_rel_b, 
/* 0x80 */	bai_i2rm_b, bai_i2rm_v, nemu_trap, bai_ib2rm_v, 
/* 0x84 */	test_r2rm_b, test_r2rm_v, xchg_rm2r_b, xchg_rm2r_v, 
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, lea_m2r_v, inv, dmi_m_v, 
/* 0x90 */	nop, xchg_r2a_v, xchg_r2a_v, xchg_r2a_v,
/* 0x94 */	xchg_r2a_v, xchg_r2a_v, xchg_r2a_v, xchg_r2a_v,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	test_i2a_b, test_i2a_v, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	li_ib2rm_b, li_ib2rm_v, ret_i_v, ret_v,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, leave_v, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	li_o2rm_b, li_o2rm_v, li_c2rm_b, li_c2rm_v,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, jcxz_rel_v,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_rel_v, jmp_rel_v, inv, jmp_rel_b,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, opcode,
/* 0xf4 */	inv, inv, bai_rm_b, bai_rm_v,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, cti_mpr_v
};

helper_fun opcode_table1 [256] = {
/* 0x00 */	inv, inv, inv, inv, 
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, inv, 
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, inv, 
/* 0x54 */	inv, inv, inv, inv,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, inv, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	inv, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	jo_rel_v, jno_rel_v, jb_rel_v, jae_rel_v, 
/* 0x84 */	je_rel_v, jne_rel_v, jbe_rel_v, ja_rel_v, 
/* 0x88 */	js_rel_v, jns_rel_v, jp_rel_v, jnp_rel_v,
/* 0x8c */	jl_rel_v, jge_rel_v, jle_rel_v, jg_rel_v, 
/* 0x90 */	seto_rm_b, setno_rm_b, setb_rm_b, setae_rm_b,
/* 0x94 */	sete_rm_b, setne_rm_b, setbe_rm_b, seta_rm_b,
/* 0x98 */	sets_rm_b, setns_rm_b, setp_rm_b, setnp_rm_b, 
/* 0x9c */	setl_rm_b, setge_rm_b, setle_rm_b, setg_rm_b, 
/* 0xa0 */	inv, inv, inv, inv,
/* 0xa4 */	rep_movs_m_b, rep_movs_m_v, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, imul_rm2r_v,
/* 0xb0 */	inv, inv, inv, inv,
/* 0xb4 */	inv, inv, movzx_rmb2r_v, movzx_rmw2r_l,
/* 0xb8 */	inv, inv, inv, inv, 
/* 0xbc */	inv, inv, movsx_rmb2r_v, movsx_rmw2r_l, 
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, inv, inv,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	inv, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};

make_helper(opcode) {
//	eip = eip + 1;
//	printf("%x\n",eip);
	return opcode_table1[ instr_fetch(eip + 1, 1) ](eip + 1) + 1;
}

make_helper(exec) {
	return opcode_table[ instr_fetch(eip, 1) ](eip);
}
