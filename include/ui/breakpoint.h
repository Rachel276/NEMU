#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	struct breakpoint *next;
    swaddr_t prekey;
	int addr;
	char expr[51];
	/* TODO: Add more members if necessary */


} BP;

#endif
