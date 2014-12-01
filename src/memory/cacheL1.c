#include "common.h"
 #include "lib/misc.h"
#include "cpu/cache.h"
#include <stdlib.h>

#define GET_SET(x) (x & 0x1fc0)
#define GET_TAG(x) (x & 0x7ffe000)
#define GET_ADDR(x) (x & 0x3f)
#define GET_MEMORY(x) (x & 0x7ffffc0)

#define block_size 64
#define set_size 128
#define way_size 8

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t ,uint32_t);

void init_cacheL11(){
	int i, j;
	for (i = 0;i < set_size; i ++)
		for (j = 0;j < way_size; j ++)
			cacheL1[i][j].valid = false;
}

uint32_t visit_dram1(hwaddr_t addr){
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
	for (i = 0; i < way_size; i ++)
		if (!cacheL1[set][i].valid){//we  have empty space
			hit = true;
			break;
		}
	if (hit) way = i;
	else way = rand() % 8;

	for (i = 0; i < block_size; i ++)
		cacheL1[set][way].data[i] = dram_read(GET_MEMORY(addr) + i, 1);
	cacheL1[set][way].valid = true;
	cacheL1[set][way].tag = GET_TAG(addr);
	return way;
}

uint32_t cacheL11_read(hwaddr_t addr, size_t len){
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
	for (i = 0; i < way_size; i ++)
		if (GET_TAG(addr) == cacheL1[set][i].tag && cacheL1[set][i].valid) {
			hit = true;
			break;
		}
	if (hit) way = i;
	else way = visit_dram1(addr);

	uint32_t offset = GET_ADDR(addr);
	if  (offset + len < block_size)
		return *(uint32_t *)(cacheL1[set][way].data + offset) & (~0u >> ((4  - len) << 3));
	else {
		uint32_t low = cacheL11_read(addr, block_size - offset);
		uint32_t high = cacheL11_read(addr + block_size - offset, len - (block_size - offset));
		return (high << (8 * (block_size - offset))) | low;
	}
}

void cacheL11_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;

	for (i = 0; i < way_size; i ++)
		if (GET_TAG(addr) == cacheL1[set][i].tag && cacheL1[set][i].valid) {
			hit = true;
			break;
		}
	if (hit) way = i;
	else way = visit_dram1(addr);

	uint32_t offset = GET_ADDR(addr);
	if  (offset + len <= block_size){
		memcpy(cacheL1[set][way].data + offset, &data, len);
		for (i = 0; i < len; i ++)
			dram_write(GET_MEMORY(addr) + i, 1 , cacheL1[set][way].data[i + offset]);	
	}
	else {
		cacheL11_write(addr, block_size - len, data);
		cacheL11_write(addr + block_size - offset, len - (block_size - offset), data >> (8 * (block_size - offset)));
	}
}

#undef GET_SET
#undef GET_TAG
#undef GET_ADDR
#undef GET_ADDR
#undef block_size
#undef set_size
#undef way_size

