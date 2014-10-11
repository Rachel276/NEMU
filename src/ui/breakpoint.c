#include "ui/breakpoint.h"

#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_,*whead;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1;  i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].next = &bp_pool[i + 1];
	} 
	bp_pool[i].next = NULL;

	head = NULL; whead = NULL;
	free_ = bp_pool;
}

BP* new_bp();
void free_bp(BP *bp);
uint32_t expr(char*, bool );

BP* new_bp()
{
	if (free_ == NULL)assert(0);
	BP *t;
	t = free_;free_ = free_-> next;
	t -> next = head; head = t;
	return t;
}

BP* new_wp()
{
	if (free_ == NULL)assert(0);
	BP *t;
	t = free_;free_ = free_-> next;
	t -> next = whead; whead = t;
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
	if (t == NULL)
	{
		 head = bp -> next;
	     bp ->next = free_;
	     free_ = bp;
	}
	else 
	{
		t -> next = bp -> next;
	    bp -> next = free_;
	    free_ = bp;
	}
}

void free_wp(BP *bp)
{
	BP *t = whead;
	while (t != NULL)
	{
		 if (t -> next == bp)break;
		 t = t -> next;
	}
	if (t == NULL)
	{
		whead = bp -> next;
		bp ->next = free_;
		free_ = bp;
	}
	else
	{
		t -> next = bp -> next;
		bp -> next = free_;
		free_ = bp;
	}
}


BP* find_addr()
{
	BP *t;
	for (t=head;t!=NULL;t=t->next)
		if (t->addr==cpu.eip)break;
	return t;
}
BP* findb_NO(int NO)
{
	BP *t;
	for (t=head;t!=NULL;t=t->next)
		if (t->NO==NO)break;
	return t;
}
BP* findw_NO(int NO)
{
	BP *t;
	for (t=whead;t!=NULL;t=t->next)
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
void load_watchpoint()
{
	BP *t;
	for (t=whead;t!=NULL;t=t->next)
		t->prekey=expr(t->expr,1);
}
void print_b()
{
	BP *t;
	for (t=head;t!=NULL;t=t->next)
		printf("Breakpoint %d 's address is 0x%08x\n",t->NO,t->addr);
}
void print_w()
{
	BP *t;
	for (t=whead;t!=NULL;t=t->next)
		printf("Watch %d 's value is %d\n",t->NO,t->prekey);
}
void delete_all()
{
	BP *t,*p;
	for (t=head;t!=NULL;)
	{
		p=t;t=t->next;
		swaddr_write(p->addr,1,p->prekey);
		free_bp(p);
	}
	for (t=head;t!=NULL;)
	{
		p=t;t=t->next;
		swaddr_write(p->addr,1,p->prekey);
		free_wp(p);
	}
}
bool check_w()
{
	BP *t;
	int r;
	bool flag=0;
//	printf("- -\n");
	for (t = whead;t != NULL;t = t -> next)
	{  
//		printf("%d %s\n",t->NO,t->expr);
		r = expr(t->expr,1);
//	    printf("%s %d %d\n",t->expr,r,t->prekey);
		if (r != t -> prekey){t ->prekey = r;flag=1;}
 	}
	return flag;
//	printf("= =\n");
}
/* TODO: Implement the function of breakpoint */
