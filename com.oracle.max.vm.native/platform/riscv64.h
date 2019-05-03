/* Author: Michalis Vardoulakis <vardoulakism@gmail,com>
 */

#ifndef __riscv64_h__
#define __riscv64_h__ 1

#include "word.h"

#if os_LINUX
#   include <sys/ucontext.h>
#   include <sys/user.h>
    typedef struct user_regs_struct *riscv64_OsTeleIntegerRegisters;
    typedef struct user_fpregs_struct *riscv64_OsTeleFloatingPointRegisters;
    typedef struct user_regs_struct *riscv64_OsTeleStateRegisters;
    typedef struct {
        Word low;
        Word high;
    } XMMRegister;
#else
#   error
#endif /*os_LINUX*/

typedef struct riscv64_CanonicalIntegerRegisters {
    Word x0;
    Word x1;
    Word x2;
    Word x3;
    Word x4;
    Word x5;
    Word x6;
    Word x7;
    Word x8;
    Word x9;
    Word x10;
    Word x11;
    Word x12;
    Word x13;
    Word x14;
    Word x15;
    Word x16;
    Word x17;
    Word x18;
    Word x19;
    Word x20;
    Word x21;
    Word x22;
    Word x23;
    Word x24;
    Word x25;
    Word x26;
    Word x27;
    Word x28;
    Word x29;
    Word x30;
    Word x31;
} riscv64_CanonicalIntegerRegistersAggregate, *riscv64_CanonicalIntegerRegisters;

typedef struct riscv64_CanonicalFloatingPointRegisters {
    Word f0;
    Word f1;
    Word f2;
    Word f3;
    Word f4;
    Word f5;
    Word f6;
    Word f7;
    Word f8;
    Word f9;
    Word f10;
    Word f11;
    Word f12;
    Word f13;
    Word f14;
    Word f15;
    Word f16;
    Word f17;
    Word f18;
    Word f19;
    Word f20;
    Word f21;
    Word f22;
    Word f23;
    Word f24;
    Word f25;
    Word f26;
    Word f27;
    Word f28;
    Word f29;
    Word f30;
    Word f31;
} riscv64_CanonicalFloatingPointRegistersAggregate, *riscv64_CanonicalFloatingPointRegisters;

typedef struct riscv64_CanonicalStateRegisters {
    //TODO fill in state registers for riscv64
} riscv64_CanonicalStateRegistersAggregate, *riscv64_CanonicalStateRegisters;

extern void riscv64_canonicalizeTeleIntegerRegisters(riscv64_OsTeleIntegerRegisters osTeleIntegerRegisters, riscv64_CanonicalIntegerRegisters canonicalIntegerRegisters);

extern void riscv64_canonicalizeTeleFloatingPointRegisters(riscv64_OsTeleFloatingPointRegisters osTeleFloatingPointRegisters, riscv64_CanonicalFloatingPointRegisters canonicalFloatingPointRegisters);

extern void riscv64_canonicalizeTeleStateRegisters(riscv64_OsTeleStateRegisters osTeleStateRegisters, riscv64_CanonicalStateRegisters canonicalStateRegisters);

extern void riscv64_printCanonicalIntegerRegisters(riscv64_CanonicalIntegerRegisters canonicalIntegerRegisters);

extern void riscv64_printCanonicalFloatingPointRegisters(riscv64_CanonicalFloatingPointRegisters canonicalFloatingPointRegisters);

extern void riscv64_printCanonicalStateRegisters(riscv64_CanonicalStateRegisters canonicalStateRegisters);

#endif /*__riscv64_h__*/