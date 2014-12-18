#include "common.h"

extern uint8_t current_sreg;
lnaddr_t segment_translate(swaddr_t, size_t, uint8_t);

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

/* Memory accessing interfaces */
uint32_t cacheL1_read(hwaddr_t, size_t);
void cacheL1_write(hwaddr_t, size_t, uint32_t);

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return cacheL1_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	cacheL1_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
	lnaddr_write(lnaddr, len, data);
}

static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return cacheL1_read(addr, len);
}

uint32_t lnaddr_read_instr(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return hwaddr_read_instr(addr, len);
}

uint32_t instr_fetch(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = segment_translate(addr, len, current_sreg);
	return lnaddr_read_instr(lnaddr, len);
}
