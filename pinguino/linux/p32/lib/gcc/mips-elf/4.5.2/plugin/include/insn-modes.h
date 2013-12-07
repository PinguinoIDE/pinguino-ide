/* Generated automatically from machmode.def and config/mips/mips-modes.def
   by genmodes.  */

#ifndef GCC_INSN_MODES_H
#define GCC_INSN_MODES_H

enum machine_mode
{
  VOIDmode,                /* machmode.def:169 */
  BLKmode,                 /* machmode.def:173 */
  CCmode,                  /* machmode.def:201 */
  CCV2mode,                /* config/mips/mips-modes.def:39 */
  CCV4mode,                /* config/mips/mips-modes.def:43 */
  CCDSPmode,               /* config/mips/mips-modes.def:48 */
  BImode,                  /* machmode.def:176 */
  QImode,                  /* machmode.def:181 */
  HImode,                  /* machmode.def:182 */
  SImode,                  /* machmode.def:183 */
  DImode,                  /* machmode.def:184 */
  TImode,                  /* machmode.def:185 */
  QQmode,                  /* machmode.def:204 */
  HQmode,                  /* machmode.def:205 */
  SQmode,                  /* machmode.def:206 */
  DQmode,                  /* machmode.def:207 */
  TQmode,                  /* machmode.def:208 */
  UQQmode,                 /* machmode.def:210 */
  UHQmode,                 /* machmode.def:211 */
  USQmode,                 /* machmode.def:212 */
  UDQmode,                 /* machmode.def:213 */
  UTQmode,                 /* machmode.def:214 */
  HAmode,                  /* machmode.def:216 */
  SAmode,                  /* machmode.def:217 */
  DAmode,                  /* machmode.def:218 */
  TAmode,                  /* machmode.def:219 */
  UHAmode,                 /* machmode.def:221 */
  USAmode,                 /* machmode.def:222 */
  UDAmode,                 /* machmode.def:223 */
  UTAmode,                 /* machmode.def:224 */
  SFmode,                  /* machmode.def:196 */
  DFmode,                  /* machmode.def:197 */
  TFmode,                  /* config/mips/mips-modes.def:26 */
  SDmode,                  /* machmode.def:236 */
  DDmode,                  /* machmode.def:237 */
  TDmode,                  /* machmode.def:238 */
  CQImode,                 /* machmode.def:232 */
  CHImode,                 /* machmode.def:232 */
  CSImode,                 /* machmode.def:232 */
  CDImode,                 /* machmode.def:232 */
  CTImode,                 /* machmode.def:232 */
  SCmode,                  /* machmode.def:233 */
  DCmode,                  /* machmode.def:233 */
  TCmode,                  /* machmode.def:233 */
  V4QImode,                /* config/mips/mips-modes.def:31 */
  V2HImode,                /* config/mips/mips-modes.def:31 */
  V8QImode,                /* config/mips/mips-modes.def:29 */
  V4HImode,                /* config/mips/mips-modes.def:29 */
  V2SImode,                /* config/mips/mips-modes.def:29 */
  V4QQmode,                /* config/mips/mips-modes.def:33 */
  V2HQmode,                /* config/mips/mips-modes.def:33 */
  V4UQQmode,               /* config/mips/mips-modes.def:34 */
  V2UHQmode,               /* config/mips/mips-modes.def:34 */
  V2HAmode,                /* config/mips/mips-modes.def:35 */
  V2UHAmode,               /* config/mips/mips-modes.def:36 */
  V2SFmode,                /* config/mips/mips-modes.def:30 */
  MAX_MACHINE_MODE,

  MIN_MODE_RANDOM = VOIDmode,
  MAX_MODE_RANDOM = BLKmode,

  MIN_MODE_CC = CCmode,
  MAX_MODE_CC = CCDSPmode,

  MIN_MODE_INT = QImode,
  MAX_MODE_INT = TImode,

  MIN_MODE_PARTIAL_INT = VOIDmode,
  MAX_MODE_PARTIAL_INT = VOIDmode,

  MIN_MODE_FRACT = QQmode,
  MAX_MODE_FRACT = TQmode,

  MIN_MODE_UFRACT = UQQmode,
  MAX_MODE_UFRACT = UTQmode,

  MIN_MODE_ACCUM = HAmode,
  MAX_MODE_ACCUM = TAmode,

  MIN_MODE_UACCUM = UHAmode,
  MAX_MODE_UACCUM = UTAmode,

  MIN_MODE_FLOAT = SFmode,
  MAX_MODE_FLOAT = TFmode,

  MIN_MODE_DECIMAL_FLOAT = SDmode,
  MAX_MODE_DECIMAL_FLOAT = TDmode,

  MIN_MODE_COMPLEX_INT = CQImode,
  MAX_MODE_COMPLEX_INT = CTImode,

  MIN_MODE_COMPLEX_FLOAT = SCmode,
  MAX_MODE_COMPLEX_FLOAT = TCmode,

  MIN_MODE_VECTOR_INT = V4QImode,
  MAX_MODE_VECTOR_INT = V2SImode,

  MIN_MODE_VECTOR_FRACT = V4QQmode,
  MAX_MODE_VECTOR_FRACT = V2HQmode,

  MIN_MODE_VECTOR_UFRACT = V4UQQmode,
  MAX_MODE_VECTOR_UFRACT = V2UHQmode,

  MIN_MODE_VECTOR_ACCUM = V2HAmode,
  MAX_MODE_VECTOR_ACCUM = V2HAmode,

  MIN_MODE_VECTOR_UACCUM = V2UHAmode,
  MAX_MODE_VECTOR_UACCUM = V2UHAmode,

  MIN_MODE_VECTOR_FLOAT = V2SFmode,
  MAX_MODE_VECTOR_FLOAT = V2SFmode,

  NUM_MACHINE_MODES = MAX_MACHINE_MODE
};

#define CONST_MODE_SIZE
#define CONST_MODE_BASE_ALIGN
#define CONST_MODE_IBIT const
#define CONST_MODE_FBIT const

#endif /* insn-modes.h */
