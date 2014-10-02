#include "common.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, LQ, MQ, NQ, AND, OR, LS, RS, HEX, REG, NUM

	/* TODO: Add more token types */

};

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
	{"0x", HEX},                    // heaxadecimal number
	{"\\$", REG},                   // reg name
	{"[0-9]", NUM}                    // decimal number

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
	int type;
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
			    nr_token++;
				tokens[nr_token].type = rules[i].token_type;
				tokens[nr_token].str[0] = '\0';
				if (rules[i].token_type == NUM || rules[i].token_type == HEX || rules[i].token_type == REG){
					for (j = 0; j < substr_len ;j ++)tokens[nr_token].str[j] = e [j + position - substr_len];	
					tokens[nr_token].str[j] = '\0';}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	for (i=1;i<=nr_token;i++)
		printf("%d %s\n",tokens[i].type,tokens[i].str);

	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */
	assert(0);
	return 0;
}

