#include "common.h"
#include "lib/misc.h"
#include "cpu/cache.h"
#include "nemu.h"
#include <stdlib.h>

#define BLOCK_WIDTH 6
#define SET_WIDTH 7
#define WAY_WIDTH 3
#define TAG_WIDTH (27 - BLOCK_WIDTH - SET_WIDTH)

#define NR_SET (1 << SET_WIDTH)
#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_TAG (1 << TAG_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)

#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

CacheL1_addr temp;

uint32_t cacheL2_read(hwaddr_t, size_t);
void cacheL2_write(hwaddr_t, size_t, uint32_t);

void init_cacheL1() {
	int i, j;
	for (i = 0; i < NR_SET; i ++)
		for (j = 0; j < NR_WAY; j ++)
			cacheL1[i][j].valid = false;
}

static void _cacheL1_read(hwaddr_t addr, void *data) {
	test(addr < HW_MEM_SIZE,"addr = %x\n",addr);
	
	temp.addr = addr & (~ BURST_MASK);
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	uint32_t offset = temp.offset;

	int i, way;
	bool hit = false;
	for (i = 0;i < NR_WAY; i ++)
		if (cacheL1[set][i].valid && cacheL1[set][i].tag == tag) {
			hit = true;
			break;
		}
	if (hit) memcpy(data, cacheL1[set][i].block + offset, BURST_LEN);
	else {
		temp.addr = 0;
		temp.tag = tag;
		temp.set = set;
		hit = false;
		for (i = 0;i < NR_WAY; i ++)
			if (!cacheL1[set][i].valid) {
				hit = true;
				break;
			}
		if (hit) way = i;
		else way = rand() % 8;

		for (i = 0;i < NR_BLOCK; i ++)
			cacheL1[set][way].block[i] = cacheL2_read(temp.addr + i, 1);
		memcpy(data, cacheL1[set][way].block + offset, BURST_LEN);
		cacheL1[set][way].valid = true;
		cacheL1[set][way].tag = tag;
	}
}

static void _cacheL1_write(hwaddr_t addr, void *data,uint8_t *mask) {
	test(addr < HW_MEM_SIZE, "addr = %x\n", addr);

	temp.addr = addr & ~BURST_MASK;
	uint32_t set = set;
	uint32_t tag = tag;
	uint32_t offset = offset;

	int i;
	bool hit = false;
	for (i = 0;i < NR_WAY;i ++)
		if (cacheL1[set][i].valid && cacheL1[set][i].tag == tag) {
			hit = true;
			break;
		}
	if (hit) 
		memcpy_with_mask(cacheL1[set][i].block + offset, data, BURST_LEN, mask);
}

uint32_t cacheL1_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	_cacheL1_read(addr, temp);

	if( (addr ^ (addr + len - 1)) & ~(BURST_MASK)  ) 
		_cacheL1_read(addr + BURST_LEN, temp + BURST_LEN);

	return *(uint32_t *)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void cacheL1_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_MASK);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	_cacheL1_write(addr, temp, mask);

	if( (addr ^ (addr + len - 1)) & ~(BURST_MASK)  ) 
		_cacheL1_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	cacheL2_write(addr, len, data);
}

