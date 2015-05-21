/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 *************************************************************************/

/*INPUT("processor.o")*/

/* not PIC32 specific, so define them here */
BMXDUPBA = 0xBF882030;
BMXDKPBA = 0xBF882010;
BMXDUDBA = 0xBF882020;

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);

/*************************************************************************
 * Memory Address Equates
 *************************************************************************/

_ebase_address           = 0x9D005000;
_RESET_ADDR              = 0x9D006000;
/*
_BEV_EXCPT_ADDR          = 0x9D006380;
_DBG_EXCPT_ADDR          = 0x9D006480;
_DBG_CODE_ADDR           = 0xBFC02000;
_GEN_EXCPT_ADDR          = _ebase_address + 0x180;
*/

/*************************************************************************
 * Memory Regions
 *
 * Memory regions without attributes cannot be used for orphaned sections.
 * Only sections specifically assigned to these regions can be allocated
 * into these regions.
 *************************************************************************/
MEMORY
{
  exception_mem              : ORIGIN = 0x9D005000, LENGTH = 0xA00
  kseg0_boot_mem             : ORIGIN = 0x9D005A00, LENGTH = 0x200
  kseg1_boot_mem             : ORIGIN = 0x9D006000, LENGTH = 0x10
  kseg0_program_mem    (rx)  : ORIGIN = 0x9D006010, LENGTH = 0x79FF0
/*
  debug_exec_mem             : ORIGIN = 0xBFC02000, LENGTH = 0xFF0
*/
  config3                    : ORIGIN = 0xBFC02FF0, LENGTH = 0x4
  config2                    : ORIGIN = 0xBFC02FF4, LENGTH = 0x4
  config1                    : ORIGIN = 0xBFC02FF8, LENGTH = 0x4
  config0                    : ORIGIN = 0xBFC02FFC, LENGTH = 0x4
  boot_software_key          : ORIGIN = 0xA0000000, LENGTH = 0x4
  kseg1_data_mem       (w!x) : ORIGIN = 0xA0000004, LENGTH = 0x7FFC
  sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000
}
SECTIONS
{
  .boot_software_key_sec : {
    *(.boot_software_key_sec)
  } > boot_software_key
  .config_BFC02FF0 : {
    KEEP(*(.config_BFC02FF0))
  } > config3
  .config_BFC02FF4 : {
    KEEP(*(.config_BFC02FF4))
  } > config2
  .config_BFC02FF8 : {
    KEEP(*(.config_BFC02FF8))
  } > config1
  .config_BFC02FFC : {
    KEEP(*(.config_BFC02FFC))
  } > config0
}
