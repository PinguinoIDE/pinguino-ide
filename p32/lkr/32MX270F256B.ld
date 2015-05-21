/***********************************************************************
 * PIC32MX270F256B object file
 * Contains Memory Regions definitions
 * 2015-03-24 Régis Blanchot
 * TO USE ONLY WITH PINGUINO HID USB BOOTLOADER v1.3 or sup.
 * more info :
 * https://sourceware.org/binutils/docs-2.23.1/ld/File-Commands.html#File-Commands
 **********************************************************************/

/*
INPUT("processor.o")
OUTPUT_FORMAT("elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(_reset)
*/
PROVIDE(_vector_spacing = 0x00000001);

/* not PIC32 specific, so define them here */
BMXDUPBA = 0xBF882030;
BMXDKPBA = 0xBF882010;
BMXDUDBA = 0xBF882020;

/**********************************************************************
 * _ebase_address value MUST BE same as the ORIGIN value of exception_mem
 * _RESET_ADDR value MUST BE same as the ORIGIN value of kseg1_boot_mem
 * They MUST BE ALIGNED with 4K address boundary
 * After any kind of reset, the PIC32 CPU will jump to the reset vector,
 * that resides at the fixed virtual address 0x9FC00000 (0xBFC00000).
 * which points to _reset (cf. crt0.S)
 **********************************************************************/

_ebase_address           = 0x9D002000;
_RESET_ADDR              = 0x9D003000;

/***********************************************************************
 * Memory Regions
 ***********************************************************************
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler,
 * - kseg0_boot_mem contains C startup module (cacheable)
**********************************************************************/

MEMORY
{

    /* 0x200 + 64 vectors * 32 bytes = 0xA00 */

    exception_mem              : ORIGIN = 0x9D002000, LENGTH = 0xA00

    /* Startup code is 0x1B0 bytes long */

    kseg0_boot_mem             : ORIGIN = 0x9D002A00, LENGTH = 0x200

    /* Reset vector */

    kseg1_boot_mem             : ORIGIN = 0x9D003000, LENGTH = 0x10

    /* User Program code 0x40000 − 0x3010 = 0x3B000 bytes free */

    kseg0_program_mem    (rx)  : ORIGIN = 0x9D003010, LENGTH = 0x3CFF0
  
    /* Device Configuration Registers (configuration bits) */

    configsfrs                 : ORIGIN = 0xBFC00BF0, LENGTH = 0x10
    config3                    : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
    config2                    : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
    config1                    : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
    config0                    : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
    sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000

    /* PIC32MX270F256B has 64 KB RAM, or 0x10000 */

    boot_software_key          : ORIGIN = 0xA0000000, LENGTH = 0x4
    kseg1_data_mem       (w!x) : ORIGIN = 0xA0000004, LENGTH = 0xFFFC

}

SECTIONS
{
  /*********************************************************************
   *** Config. Sections
   ********************************************************************/

  .config_BFC00BF0 :
  {
    KEEP(*(.config_BFC00BF0))
  } > config3

  .config_BFC00BF4 :
  {
    KEEP(*(.config_BFC00BF4))
  } > config2
  
  .config_BFC00BF8 :
  {
    KEEP(*(.config_BFC00BF8))
  } > config1
  
  .config_BFC00BFC :
  {
    KEEP(*(.config_BFC00BFC))
  } > config0
}
