#include "boot_config.h"
  
 /* Force a guaranteed location in application for the bootloader to jump to */
    .section app_reset_section, code, address(BOOT_CONFIG_APPLICATION_RESET_ADDRESS), keep
    .global _AppResetVector

/* The bootloader jumps here to run the application code */
    _AppResetVector:
     goto __resetPRI 
