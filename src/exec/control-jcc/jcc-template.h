#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

make_helper(concat(ja_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("ja" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.CF == 0 &&  cpu.ZF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jae_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("jae" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.CF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jb_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("jb" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.CF == 1)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jbe_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("jbe" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.CF == 1 ||  cpu.ZF == 1)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jcxz_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, 1);
		print_asm("jcxz" str(SUFFIX) " %x", eip + rel + 2);
		if (REG(R_ECX) == 0)return 1 + 2;
	    else return rel + 2;
}

make_helper(concat(je_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
//	    printf("%x\n",rel);	
		print_asm("je" str(SUFFIX) " %x", eip + rel + 2); 
//		printf("%d\n",cpu.ZF);
		if (cpu.ZF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jg_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("jg" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.ZF == 0 && cpu.SF == cpu.OF)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jge_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("jge" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.SF == cpu.OF)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jl_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		print_asm("jl" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.SF != cpu.OF)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jle_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
		//printf("%\n",(int)(rel));
		print_asm("jle" str(SUFFIX) " %x", eip + rel + 2);
		if (cpu.ZF == 1 || cpu.SF != cpu.OF)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jne_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("jne" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.ZF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jno_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("jno" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.OF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jnp_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("jnp" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.PF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jns_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("jns" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.SF == 0)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jo_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("jo" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.OF == 1)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(jp_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("jp" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.PF == 1)return DATA_BYTE + 2;
		else return rel + 2;
}

make_helper(concat(js_rel_, SUFFIX)) {
		DATA_TYPE_S rel = instr_fetch(eip + 1,DATA_BYTE);
		print_asm("js" str(SUFFIX) " %x",eip + rel + 2);
		if (cpu.SF == 1)return DATA_BYTE + 2;
		else return rel + 2;
}

#include "exec/template-end.h"
