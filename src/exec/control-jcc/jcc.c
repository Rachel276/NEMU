#include "exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

extern char suffix;

make_helper(ja_rel_v) {
	return (suffix == 'l' ? ja_rel_l(eip) : ja_rel_w(eip));
}

make_helper(jae_rel_v) {
	return (suffix == 'l' ? jae_rel_l(eip) : jae_rel_w(eip));
}

make_helper(jb_rel_v) {
	return (suffix == 'l' ? jb_rel_l(eip) : jb_rel_w(eip));
}

make_helper(jbe_rel_v) {
	return (suffix == 'l' ? jbe_rel_l(eip) : jbe_rel_w(eip));
}

make_helper(jcxz_rel_v) {
	return (suffix == 'l' ? jcxz_rel_l(eip) : jcxz_rel_w(eip));
}

make_helper(je_rel_v) {
	return (suffix == 'l' ? je_rel_l(eip) : je_rel_w(eip));
}

make_helper(jg_rel_v) {
	return (suffix == 'l' ? jg_rel_l(eip) : jg_rel_w(eip));
}

make_helper(jge_rel_v) {
	return (suffix == 'l' ? jge_rel_l(eip) : jge_rel_w(eip));
}

make_helper(jl_rel_v) {
	return (suffix == 'l' ? jl_rel_l(eip) : jl_rel_w(eip));
}

make_helper(jle_rel_v) {
	return (suffix == 'l' ? jle_rel_l(eip) : jle_rel_w(eip));
}

make_helper(jne_rel_v) {
	return (suffix == 'l' ? jne_rel_l(eip) : jne_rel_w(eip));
}

make_helper(jno_rel_v) {
	return (suffix == 'l' ? jno_rel_l(eip) : jno_rel_w(eip));
}

make_helper(jnp_rel_v) {
	return (suffix == 'l' ? jnp_rel_l(eip) : jnp_rel_w(eip));
}

make_helper(jns_rel_v) {
	return (suffix == 'l' ? jns_rel_l(eip) : jns_rel_w(eip));
}

make_helper(jo_rel_v) {
	return (suffix == 'l' ? jo_rel_l(eip) : jo_rel_w(eip));
}

make_helper(jp_rel_v) {
	return (suffix == 'l' ? jp_rel_l(eip) : jp_rel_w(eip));
}

make_helper(js_rel_v) {
	return (suffix == 'l' ? js_rel_l(eip) : js_rel_w(eip));
}
