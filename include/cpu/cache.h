#ifndef __CPU_CACHE_H__
#define __CPU_CACHE_H__

#include "common.h"

typedef struct{
	bool valid;
	uint32_t tag;// 16 bits
	uint8_t data[64];
} CPU_cacheL1;

typedef struct{
	bool valid,dirty;
	uint32_t tag;
	uint8_t data[64];
} CPU_cacheL2;

CPU_cacheL1 cacheL1[128][8];
CPU_cacheL2 cacheL2[4096][16];
#endif
