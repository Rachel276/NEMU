#include "nemu.h"

typedef union {
	struct {
		uint32_t limit_15_0     : 16;
		uint32_t base_15_0      : 16;
		uint32_t base_23_16     : 8;
		uint32_t type           : 4;
		uint32_t seg_type       : 1;
		uint32_t DPL            : 2;
		uint32_t P				: 1;
		uint32_t limit_19_16	: 4;
		uint32_t AVL			: 1;
		uint32_t op_type		: 1;
		uint32_t B				: 1;
		uint32_t G				: 1;
		uint32_t base_31_24		: 8;
	};
	uint8_t val[8];
} SegDescriptor;

uint32_t lnaddr_read(lnaddr_t, size_t);
uint8_t current_sreg;

lnaddr_t segment_translate(swaddr_t addr, size_t len, uint8_t sreg_code) {
	lnaddr_t gdt;
	lnaddr_t table_addr;
	SegDescriptor SD;
	if (cpu.cr0.PE == 0)return addr;
	gdt = cpu.gdtr.base;
	table_addr = gdt + sreg(sreg_code).INDEX * 8;
	int i;
	for (i = 0;i < 8; i ++)
		SD.val[i] = lnaddr_read(table_addr + i, 1);
	uint32_t base = (SD.base_31_24 << 24) | (SD.base_23_16 << 16) | SD.base_15_0;
	return base + addr;
}

