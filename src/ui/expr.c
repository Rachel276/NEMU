#include "common.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include "cpu/reg.h"

enum {
	NOTYPE = 256, EQ, LQ, MQ, NQ, AND, OR, LS, RS, HEX, REG, NUM, DEREF, NEG, VIA

	/* TODO: Add more token types */

};

uint32_t swaddr_read(swaddr_t, size_t);
swaddr_t find_tokens(char virable[]);

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\+", '+'},					// plus
	{"-", '-'},                     // minus
	{"\\*", '*'},                   // multiple point
	{"/", '/'},                     // divide
	{"%", '%'},                     // mod
	{"<=", LQ},                     // <=
	{">=", MQ},                     // >=
	{"==", EQ},                     // equal
	{"!=", NQ},                     // unequal
	{"&&", AND},                    // and
	{"\\|\\|", OR},                     // or
	{"!", '!'},                     // is zero
    {"<<", LS},                     // left shift
    {">>", RS},                     // right shift
    {"&", '&'},                     // &
    {"\\|", '|'},                   // |
    {"\\^", '^'},                   // ^
	{"~", '~'},                     // ~
	{"\\(", '('},                     // (
	{"\\)", ')'},                     // )
	{"<", '<'},                     // less than
	{">", '>'},                     // more than
	{"0x([0-9]|[a-f]|[A-F])+", HEX},                    // heaxadecimal number
	{"\\$([a-z]|[A-Z])+", REG},                   // reg name
	{"[0-9]+", NUM},                    // decimal number
	{"{[a-z]|[A-Z]|_}+{[a-z]|[A-z]|[0-9]|_}*", VIA}     //viarable

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type,level;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
    int j;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				position += substr_len;


				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
 //               printf("%d\n",position);
	/*			switch(rules[i].token_type) {
					default: assert(0);
				}
*/
			    if (i!=0){
					nr_token++;
					tokens[nr_token].type = rules[i].token_type;
					for (j=0;j<substr_len;j++) 
						tokens[nr_token].str[j] = substr_start[j];
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	//for (i=1;i<=nr_token;i++)
	//	printf("%d %s\n",tokens[i].type,tokens[i].str);

	return true; 
}

int  find_operator(int p,int q)
{
	int t=1,i,l=0;
	bool used;
//	for (i=1;i<=nr_token;i++)
	i=p;
	while (i<=q)
	{	
		if (tokens[i].type == NUM || tokens[i].type == HEX || tokens[i].type==REG || tokens[i].type==VIA){i++;continue;}
		if (tokens[i].type == '(')
		{
			used=0;
			while (i<=nr_token){
				 if (tokens[i].type == ')'){used=1;break;}
		       	 i++;
			}
			if (used==0)assert(0);
			i++;
			continue;
		}
		if (l<=tokens[i].level){t=i;l=tokens[i].level;}
		i++;
	}
	return t;
}

bool check_parentheses(int p,int q,bool* success)
{
	int i,t=0;
	bool used=0,il=0;
	for (i=p;i<=q;i++)
	{ 
		if (t==0 && used)il=1;
 		if (tokens[i].type == '('){used=1;t++;}
		if (tokens[i].type == ')')t--;
		if (t<0){*success=0;return false;}
  	}
	if (t!=0){*success=0;return false;}
	if (il==1 || tokens[p].type!='(' || tokens[q].type!=')')return false;
	else return true;
}
uint32_t eval(int p,int q,bool* success)
{
	uint32_t num=0;
//	printf("%d %d\n",p,q);
	if (p > q)
	{
		*success=false;
		return 0;
	}
	else if (p == q){
		if (tokens[p].type == NUM) sscanf(tokens[p].str,"%d",&num);
		else if (tokens[p].type == HEX) sscanf(tokens[p].str,"%x",&num);
		else if (tokens[p].type == REG)
		{
			if (strcmp(tokens[p].str,"$eax")==0) num = cpu.eax;
			else if (strcmp(tokens[p].str,"$ebx")==0) num = cpu.ebx;
		    else if (strcmp(tokens[p].str,"$ecx")==0) num = cpu.ecx;
			else if (strcmp(tokens[p].str,"$edx")==0) num = cpu.edx;
			else if (strcmp(tokens[p].str,"$esp")==0) num = cpu.esp;
			else if (strcmp(tokens[p].str,"$ebp")==0) num = cpu.ebp;
			else if (strcmp(tokens[p].str,"$esi")==0) num = cpu.esp;
			else if (strcmp(tokens[p].str,"$edi")==0) num = cpu.edi;
			else if (strcmp(tokens[p].str,"$eip")==0) num = cpu.eip;
			else {*success=false;return 0;}
		}
		else if (tokens[p].type == VIA)
		{
			num = find_tokens(tokens[p].str);
			if (num  == -1){*success = false; return 0;}
		}
		else {*success=false;return 0;}
//		printf("<%d\n",num);
		return num;
	}
	else if (check_parentheses(p,q,success) == true){
		return eval(p + 1,q - 1,success);
	}
	else if (*success==true)
	{
		int op = find_operator(p,q);		
		uint32_t val1,val2;
//		printf("%d\n",op);
		if (tokens[op].level == 1)
		{
			val1=eval(p+1,q,success);
			switch(tokens[op].type){
				case DEREF:num = swaddr_read(val1,1);break;
				case '!':num = !val1;break;
				case '~':num = ~val1;break;
				case NEG:num = -val1;break;
				default :*success=0;return 0;
		 	}
	//		printf("%d\n",num);
			return num;
		}
//		printf("%d\n",op);
		val1=eval(p,op-1,success);
		val2=eval(op+1,q,success);
	//	printf("-%d %d\n",val1,val2);
 		switch(tokens[op].type){
		 	case '+':return val1+val2;
			case '-':return val1-val2;
			case '*':return val1*val2;
			case '/':return val1/val2;
			case '%':return val1%val2;
			case '<':return val1<val2;
			case '>':return val1>val2;
			case LQ :return val1<=val2;
			case MQ :return val1>=val2;
			case EQ :return val1==val2;
			case NQ :return val1!=val2;
			case AND:return val1&&val2;
			case OR :return val1||val2;
			case LS :return val1<<val2;
			case RS :return val1>>val2;
			case '&':return val1&val2;
		 	case '|':return val1|val2;
			case '^':return val1^val2;
			default:assert(0);
		} 
	}
	else return 0;
}

uint32_t expr(char *e, bool *success) {
//	printf("%d\n",make_token(e));
	memset(tokens,0,sizeof(tokens));
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */
	int i;
	for (i = 1; i <= nr_token; i ++)
	   if (tokens[i].type == '*' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != REG && tokens[i-1].type != HEX && tokens[i-1].type != VIA && tokens[i-1].type !=')')))
		   tokens[i].type = DEREF;
	for (i = 1; i <= nr_token; i ++)
       if (tokens[i].type == '-' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != REG && tokens[i-1].type != HEX && tokens[i-1].type != VIA && tokens[i-1].type !=')')))
     	   tokens[i].type = NEG;
	for (i = 1; i <= nr_token; i ++){
	    switch(tokens[i].type){
		     case DEREF:
			 case '!':
			 case '~':
			 case NEG:
			    tokens[i].level=1;
				break;
			 case '/':
			 case '*':
			 case '%':
			    tokens[i].level=2;
				break;
			 case '+':
			 case '-':
			    tokens[i].level=3;
				break;
			 case LS:
			 case RS:
				tokens[i].level=4;
				break;
			 case '>':
			 case LQ:
			 case '<':
			 case MQ:
				tokens[i].level=5;
				break;
			 case EQ:
			 case NQ:
				tokens[i].level=6;
				break;
			 case '&':
				tokens[i].level=7;
				break;
			 case '^':
				tokens[i].level=8;
				break;
			 case '|':
				tokens[i].level=9;
				break;
			 case AND:
				tokens[i].level=10;
				break;
			 case OR:
				tokens[i].level=11;
				break;
 		}
	}
	return eval(1,nr_token,success);
//	assert(0);
//	return 0;
}

