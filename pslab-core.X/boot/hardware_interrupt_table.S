#include "boot_config.h"

    .equ remap_offset, 4
    
    .macro remap_to_application NAME
    .weak __\NAME
    .global __\NAME
    .equiv __\NAME, __remapped_DefaultInterrupt
    .section remapped\NAME, code, address(BOOT_CONFIG_USER_IVT_TABLE_ADDRESS+remap_offset), keep
    goto __\NAME
    .equ remap_offset, remap_offset + 4
    .endm
    
    .macro keep_in_bootloader NAME
    .endm
    
    .macro remap_to_application_default NAME
    .endm
    
    /* First entry in the remap table is the default interrupt, and it is
     * required and not implemented in hardware vectors, so we will implement
     * it here. */
    .weak __remapped_DefaultInterrupt
    .global __remapped_DefaultInterrupt
    .section __remapped_DefaultInterrupt, code, address(BOOT_CONFIG_USER_IVT_TABLE_ADDRESS), keep
    goto __DefaultInterrupt
    
    /* All other entries are hardware vectors and can be selectively forwarded
     * by the user.  Pull them in via the interrupt configuration file. */
    #define REMAP_TABLE
    .include "boot/interrupts.S"
