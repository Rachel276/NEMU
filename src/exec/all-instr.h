#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-movsx/movsx.h"
#include "data-push/push.h"
#include "data-xchg/xchg.h"
#include "data-leave/leave.h"

#include "binary-add/add.h"
#include "binary-cmp/cmp.h"
#include "binary-sub/sub.h"
#include "binary-sbb/sbb.h"
#include "binary-idiv/idiv.h"
#include "binary-imul/imul.h"

#include "logical-test/test.h"
#include "logical-setcc/setcc.h"
#include "logical-xor/xor.h"
#include "logical-and/and.h"
#include "logical-or/or.h"

#include "control-jcc/jcc.h"
#include "control-jmp/jmp.h"
#include "control-call/call.h"
#include "control-ret/ret.h"

#include "misc-lea/lea.h"

#include "misc/misc.h"
#include "opcode/opcode.h"
#include "Binary.h"
#include "Control.h"
#include "Logical.h"
