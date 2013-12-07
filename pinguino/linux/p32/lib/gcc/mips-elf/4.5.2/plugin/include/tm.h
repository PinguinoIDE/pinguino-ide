#ifndef GCC_TM_H
#define GCC_TM_H
#define TARGET_CPU_DEFAULT ((MASK_EXPLICIT_RELOCS)|MASK_RELAX_PIC_CALLS)
#ifdef IN_GCC
# include "options.h"
# include "config/elfos.h"
# include "config/newlib-stdint.h"
# include "config/mips/mips.h"
# include "config/mips/elf.h"
# include "defaults.h"
#endif
#if defined IN_GCC && !defined GENERATOR_FILE && !defined USED_FOR_TARGET
# include "insn-constants.h"
# include "insn-flags.h"
#endif
#endif /* GCC_TM_H */
