/* This code will block out the space reserved by the boot loader so that the 
 * application code can't link into that area.
 *
 * The value in the address() field defines the start address of the
 * boot code.
 *
 * The number on the .space defines the size of this block in PC units (2 per
 * instruction).
 */
#include "boot_config.h"
    .equ    ERASE_PAGE_MASK,(~((2048) - 1)) 
    .equ    LAST_PAGE_START_ADDRESS, (0x2AFEA & ERASE_PAGE_MASK)
    
    
    .equ    PROGRAM_MEMORY_ORIGIN, (0x200)
   
    .section *, code, address(PROGRAM_MEMORY_ORIGIN), noload, keep
boot_loader_memory:
    .space (BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW  - PROGRAM_MEMORY_ORIGIN), 0x00

    .section *, code, address(LAST_PAGE_START_ADDRESS), noload, keep
config_page_memory:
    .space (0x2AFEA-LAST_PAGE_START_ADDRESS), 0x00
    
