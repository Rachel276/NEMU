#include "ui/breakpoint.h"

#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].next = NULL;

	head = NULL;
	free_ = bp_pool;
}

BP* new_bp();
void free_bp(BP *bp);

BP* new_bp()
{
	if (free_ == NULL)assert(0);
	BP *t;
	t = free_;free_ = free_-> next;
	t -> next = head; head = t;
	return t;
}

void free_bp(BP *bp)
{
	BP *t = head;
	while (t != NULL)
	{
		if (t -> next == bp)break;
		t = t -> next;
	}
	t -> next = bp -> next;
	bp -> next = free_;
	free_ = bp;
}
BP* find_addr()
{
	BP *t;
	for (t=head;t!=NULL;t=t->next)
		if (t->addr==cpu.eip)break;
	return t;
}
BP* find_NO(int NO)
{
	BP *t;
	for (t=head;t!=NULL;t=t->next)
		if (t->NO==NO)break;
	return t;
}
void load_breakpoint()
{
	BP *t;
	for (t=head;t!=NULL;t=t->next)
	{
		t->prekey=swaddr_read(t->addr,1);
		swaddr_write(t->addr,1,0xcc);
	}
}
/* TODO: Implement the function of breakpoint */
