#include "common.h"
 #include "lib/misc.h"
#include "cpu/cache.h"
#include <stdlib.h>

#define block_size 64
#define set_size 128
#define way_size 8
#define DATA_MASK 3

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t ,uint32_t);

CacheL1_addr temp;

/*void init_cacheL11(){
	int i, j;
	for (i = 0;i < set_size; i ++)
		for (j = 0;j < way_size; j ++)
			cacheL1[i][j].valid = false;
}

uint32_t visit_dram1(hwaddr_t addr){
	temp.addr = addr & (~ DATA_MASK);
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
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
		cacheL1[set][way].block[i] = dram_read(temp.addr + i, 1);
	cacheL1[set][way].valid = true;
	cacheL1[set][way].tag = tag;
	return way;
}

uint32_t cacheL11_read(hwaddr_t addr, size_t len){
	temp.addr = addr & (~DATA_MASK);
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	uint32_t offset = temp.offset;
	int i, way;
	bool hit = false;
	for (i = 0; i < way_size; i ++)
		if (tag == cacheL1[set][i].tag && cacheL1[set][i].valid) {
			hit = true;
			break;
		}
	if (hit) way = i;
	else way = visit_dram1(addr);

	if  (offset + len <= block_size)
		return *(uint32_t *)(cacheL1[set][way].block + offset) & (~0u >> ((4  - len) << 3));
	else {
		uint32_t low = cacheL11_read(addr, block_size - offset);
		uint32_t high = cacheL11_read(addr + block_size - offset, len - (block_size - offset));
		return (high << (8 * (block_size - offset))) | low;
	}
}

void cacheL11_write(hwaddr_t addr, size_t len, uint32_t data) {
	temp.addr = addr & (~ DATA_MASK);
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	uint32_t offset = temp.offset;
	int i, way;
	bool hit = false;

	for (i = 0; i < way_size; i ++)
		if (tag == cacheL1[set][i].tag && cacheL1[set][i].valid) {
			hit = true;
			break;
		} 
	if (hit) way = i;
	else way = visit_dram1(addr);

	if  (offset + len <= block_size){
		memcpy(cacheL1[set][way].block + offset, &data, len);
		for (i = 0; i < len; i ++)
			dram_write(temp.addr + i, 1 , cacheL1[set][way].block[i + offset]);	
	}
	else {
		cacheL11_write(addr, block_size - len, data);
		cacheL11_write(addr + block_size - offset, len - (block_size - offset), data >> (8 * (block_size - offset)));
	} 
}*/

#undef GET_SET
#undef GET_TAG
#undef GET_ADDR
#undef GET_ADDR
#undef block_size
#undef set_size
#undef way_size

