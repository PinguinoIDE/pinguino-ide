/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 *************************************************************************/
INPUT("processor.o")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);

/*************************************************************************
 * _ebase_address & _RESET_ADDR multiple of 0x1000
 *************************************************************************/
_ebase_address           = 0x9D003000;
_RESET_ADDR              = 0x9D004000;
/*
_BEV_EXCPT_ADDR          = _RESET_ADDR + 0x380;
_GEN_EXCPT_ADDR          = _ebase_address + 0x180;
*/
/*************************************************************************
 * Memory Regions
 *************************************************************************/

MEMORY
{
  exception_mem              : ORIGIN = 0x9D003000, LENGTH = 0xA00
  kseg0_boot_mem             : ORIGIN = 0x9D003A00, LENGTH = 0x200
  kseg1_boot_mem             : ORIGIN = 0x9D004000, LENGTH = 0x10
  kseg0_program_mem    (rx)  : ORIGIN = 0x9D004010, LENGTH = 0x3FF0
  
  configsfrs                 : ORIGIN = 0xBFC00BF0, LENGTH = 0x10
  config3                    : ORIGIN = 0xBFC00BF0, LENGTH = 0x4
  config2                    : ORIGIN = 0xBFC00BF4, LENGTH = 0x4
  config1                    : ORIGIN = 0xBFC00BF8, LENGTH = 0x4
  config0                    : ORIGIN = 0xBFC00BFC, LENGTH = 0x4
  boot_software_key          : ORIGIN = 0xA0000000, LENGTH = 0x4
  kseg1_data_mem       (w!x) : ORIGIN = 0xA0000004, LENGTH = 0x1FFC
  sfrs                       : ORIGIN = 0xBF800000, LENGTH = 0x100000
}

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
