#include "ui/ui.h"
#include "ui/breakpoint.h"
#include "nemu.h"

#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#define LOADER_START 0x100000
int nemu_state = END;

void cpu_exec(uint32_t);
void restart();
uint32_t swaddr_read(swaddr_t, size_t);
BP* new_bp();
void free_bp(BP*);
BP* find_addr();
BP* find_NO(int);

/* We use the readline library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	} 

	return line_read;
}

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(nemu_state == RUNNING) {
		nemu_state = INT;
 	} 
} 

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

static void cmd_c() {
	if (nemu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return;
 	}

	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_r() {
	if(nemu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf("%c", &c);
			scanf(" %c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
 	 		}
 	   	}
 	}  

restart_:
	restart();
	nemu_state = STOP;
	cmd_c();
}

void main_loop() {
	char *cmd;
	int j,pieces=0,N,bpis=0;
	swaddr_t addr;
	BP *t;
	uint32_t step;
	while(1) {
		cmd = rl_gets();
		char *p = strtok(cmd, " ");
		if(p == NULL) { continue; }
		nemu_state = RUNNING;

		if(strcmp(p, "c") == 0) { cmd_c(); }
		else if(strcmp(p, "r") == 0) { cmd_r(); pieces=0;continue;}	
		else if(strcmp(p, "q") == 0) { return; }
		else if(strcmp(p, "si") == 0)
		{ 
			p=strtok(NULL," ");
			if (p==NULL)step=1;
			else
			{
				for (j=0,step=0;j<strlen(p);j++)
					      step=step*10+p[j]-'0';
			 }
			if (pieces==0)restart();
			nemu_state = RUNNING;
			cpu_exec(step);
			pieces++;
		}   
		else if (strcmp(p, "info") == 0)
		{
		    p=strtok(NULL," ");
		   	if(p[0]=='r')
			{
				printf("eax            0x%08x        %08d\n",cpu.eax,cpu.eax);
			    printf("ecx            0x%08x        %08d\n",cpu.ecx,cpu.ecx);
			    printf("edx            0x%08x        %08d\n",cpu.edx,cpu.edx);
			    printf("ebx            0x%08x        %08d\n",cpu.ebx,cpu.ebx);
			    printf("esp            0x%08x        0x%08x\n",cpu.esp,cpu.esp);
			    printf("ebp            0x%08x        0x%08x\n",cpu.ebp,cpu.ebp);
			    printf("esi            0x%08x        %08d\n",cpu.esi,cpu.esi);
			    printf("edi            0x%08x        %08d\n",cpu.edi,cpu.edi);	
     		    printf("eip            0x%08x        0x%08x\n",cpu.eip,cpu.eip);
			}
			else if(p[0]=='b')
			{
				t=find_addr();
				printf("Breakpoint %d at 0x%08x\n",t -> NO,t -> addr);
			}

		}
		else if (strcmp(p, "x") == 0)
		{
			p=strtok(NULL," ");
			for (j=0,N=0;j<strlen(p);j++)N=N*10+p[j]-'0';
			p=strtok(NULL," ");
			sscanf(p,"%x",&addr); 
            while (N>0)
			{
	     		printf("0x%08x: 0x%08x\n",addr,swaddr_read(addr,4));
				addr+=4;
				N--;
		 	} 
		} 
		else if (strcmp(p, "b") == 0)
		{
			p = strtok(NULL,"*");
			sscanf(p,"%x",&addr);
			printf("0x%08x: 0x%08x\n",addr,swaddr_read(addr,4));
			swaddr_write(addr,1,0xcc);
            printf("0x%08x: 0x%08x\n",addr,swaddr_read(addr,4));
			t = new_bp();
			t -> prekey = swaddr_read(addr,1);
			t -> addr = addr;
			bpis++;
			t -> NO = bpis;
			printf("%d %x 0x%08x\n",t->NO,t->prekey,t->addr);
	 	}
		else if (strcmp(p, "d") == 0)
		{
			p = strtok(NULL," ");
			for (j=0,N=0;j<strlen(p);j++)N=N*10+p[j]-'0';
			t=find_NO(N);
			free_bp(t);
	 	 } 
		/* TODO: Add more commands */

		else { printf("Unknown command '%s'\n", p); }
	//	pieces++;
	}   
}
