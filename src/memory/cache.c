#include "common.h"
#include "lib/misc.h"
#include "cpu/cache.h"
#include <stdlib.h>

#define GET_SET(x) (x & 0x1fc0)
#define GET_TAG(x) (x & 0x7ffe000) 
#define GET_ADDR(x) (x & 0x3f)

#define block_size 64
#define set_size 128
#define way_size 8

uint8_t* cacheL2_read(hwaddr_t);
void cacheL2_write(hwaddr_t addr, size_t len, uint32_t data);

void init_cacheL1(){
	int i, j;
	for (i = 0;i < set_size; i ++)
		for (j = 0;j < way_size; j ++)
			cacheL1[i][j].valid = false;
}

uint32_t visit_cacheL2(hwaddr_t addr) {
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
	for (i = 0; i < way_size; i ++)
		if (!cacheL1[set][i].valid) {
			hit = true;
			break;
		}  
	if (hit) way = i;
	else way = rand() % 8;

	memcpy(cacheL1[set][way].data, cacheL2_read(addr), block_size);
	cacheL1[set][way].valid = true;
	cacheL1[set][way].tag = GET_TAG(addr);
	return way;
}

uint32_t cacheL1_read(hwaddr_t addr, size_t len){
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
	for (i = 0; i < way_size; i ++)
		if (GET_TAG(addr) == cacheL1[set][i].tag && cacheL1[set][i].valid) {
			hit = true;
			break;
		}
	if (hit) way = i;
	else way = visit_cacheL2(addr);

	uint32_t offset = GET_ADDR(addr);
	if 	(offset + len <= block_size)
		return *(uint32_t *)(cacheL1[set][way].data + offset) & (~0u >> ((4 - len) << 3));
	else {
		uint32_t low = cacheL1_read(addr, block_size - offset);
		uint32_t high = cacheL1_read(addr + block_size - offset, len - (block_size - offset));
		return (high << (8 * (block_size - offset))) | low;
	}
}

void cacheL1_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
 
	for (i = 0; i < way_size; i ++)
		if (GET_TAG(addr) == cacheL1[set][i].tag && cacheL1[set][i].valid) {
			hit = true;
			break;
		}
	if (hit) way = i;
	else way = visit_cacheL2(addr);

	uint32_t offset = GET_ADDR(addr);
	if  (offset + len <= block_size){
		memcpy(cacheL1[set][way].data + offset, &data, len);
		cacheL2_write(addr, len, data);
	}
	else {
		cacheL1_write(addr, block_size - len, data);
		cacheL1_write(addr + block_size - offset, len - (block_size - offset), data >> (8 * (block_size - offset)));
		cacheL2_write(addr, len, data);
	}
}

#undef GET_SET
#undef GET_TAG
#undef GET_ADDR
#undef block_size
#undef set_size
#undef way_size
