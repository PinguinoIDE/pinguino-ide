/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 * linker file for PIC32MX250F128B with USB bootloader
 * Jean-Pierre Mandon may-2012
 *************************************************************************/
INPUT("processor.o")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);

/**********************************************************************
 * _ebase_address value MUST BE same as the ORIGIN value of exception_mem
 * _RESET_ADDR value MUST BE same as the ORIGIN value of kseg1_boot_mem
 *
 * After any kind of reset, the PIC32 CPU will jump to the reset vector,
 * that resides at the fixed virtual address 0x9FC00000 (0xBFC00000).
 * which points to _reset (cf. crt0.S)
 **********************************************************************/

_ebase_address           = 0x9D003000;
_RESET_ADDR              = 0x9D004000;

/*************************************************************************
 * Memory Regions
 *
 * - exception_mem  contains interrupt vector table starting at ebase (cacheable)
 * - kseg1_boot_mem contains reset vector, bootstrap exception handler, debug exception handler (non-cacheable)
 * - kseg0_boot_mem contains C startup module (cacheable)
 *************************************************************************/
MEMORY
{
  exception_mem               : ORIGIN = 0x9D003000, LENGTH = 0xA00
  kseg0_boot_mem              : ORIGIN = 0x9D003A00, LENGTH = 0x200
  kseg1_boot_mem              : ORIGIN = 0x9D004000, LENGTH = 0x10
  kseg0_program_mem    (rx)   : ORIGIN = 0x9D004010, LENGTH = 0x1BFF0

  configsfrs                  : ORIGIN = 0xBFC00BF0, LENGTH = 0x10
  config3                     : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
  config2                     : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
  config1                     : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
  config0                     : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
  boot_software_key           : ORIGIN = 0xA0000000, LENGTH = 0x4
  kseg1_data_mem       (w!x)  : ORIGIN = 0xA0000004, LENGTH = 0x7FFC
  sfrs                        : ORIGIN = 0xBF800000, LENGTH = 0x100000
}

/*************************************************************************
 * Configuration-word sections.
 *************************************************************************/

SECTIONS
{
  .config_BFC00BF0 : {
    KEEP(*(.config_BFC00BF0))
  } > config3
  .config_BFC00BF4 : {
    KEEP(*(.config_BFC00BF4))
  } > config2
  .config_BFC00BF8 : {
    KEEP(*(.config_BFC00BF8))
  } > config1
  .config_BFC00BFC : {
    KEEP(*(.config_BFC00BFC))
  } > config0
}
