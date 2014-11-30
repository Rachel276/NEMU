#include "common.h"
#include "lib/misc.h"
#include "cpu/cache.h"
#include <stdlib.h>

#define GET_SET(x) (x & 0x3ffc0)
#define GET_TAG(x) (x & 0x7fc0000)
#define GET_ADDR(x) (x & 0x3f)
#define GET_MEMORY(x) (x & 0x7ffffc0)

#define block_size 64
#define set_size 4096
#define way_size 16

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t addr, size_t ,uint32_t);

void init_cacheL2() {
	int i, j;
	for (i = 0;i < set_size; i ++)
		for (j = 0;j < way_size; j ++)
			cacheL2[i][j].dirty = cacheL2[i][j].valid = false;
}

uint32_t visit_dram(hwaddr_t addr){
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
	for (i = 0; i < way_size; i ++)
		if (!cacheL2[set][i].valid){//we  have empty space
			hit = true;way = i;
			break;
		}
	if (!hit) {
		way = rand() % 16;
		if (cacheL2[set][way].dirty)
			for (i = 0; i < block_size; i ++)
				dram_write(GET_MEMORY(addr) + i, 1 , cacheL2[set][way].data[i]);
	}
	for (i = 0; i < block_size; i ++)
		cacheL2[set][way].data[i] = dram_read(GET_MEMORY(addr) + i, 1);
	cacheL2[set][way].valid = true;
	cacheL2[set][way].dirty = false;
	cacheL2[set][way].tag = GET_TAG(addr);
	return way;
}

uint8_t* cacheL2_read(hwaddr_t addr) {
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit =false;
	
	for (i = 0;i < way_size; i ++)
		if (GET_TAG(addr) == cacheL2[set][i].tag && cacheL2[set][i].valid) {
			hit = true;
			way = i;
			break;
		}
	if (!hit)
		way = visit_dram(addr);

	return cacheL2[set][way].data;
}

void cacheL2_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t set = GET_SET(addr);
	int i, way;
	bool hit = false;
	for (i = 0;i < set_size; i ++)
		if (cacheL2[set][i].valid && cacheL2[set][i].tag == GET_TAG(addr)){
			way = i;
			hit = true;
			break;
		}
	if (!hit) 
		way = visit_dram(addr);
	
	uint32_t offset = GET_ADDR(addr);
	if  (offset + len <= block_size){
		memcpy(cacheL2[set][way].data + offset, &data, len);
		cacheL2[set][way].dirty = true;
	}
	else {
		cacheL2[set][way].dirty = true;
		cacheL2_write(addr, block_size - len, data);
		cacheL2_write(addr + block_size - offset, len - (block_size - offset), data >> (8 * (block_size - offset)));
	}
}

#undef GET_SET
#undef GET_TAG
#undef GET_ADDR
#undef GET_MEMORY
#undef block_size
#undef set_size
#undef way_size
