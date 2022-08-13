;/*******************************************************************************
;  flash.s for PIC24 and dsPIC33 Cores
;
;  Summary:
;    Assembly language optimized helper routines required flash functions
;
;  Description:
;    Assembly language optimized for flash access
;
;*******************************************************************************/
;
;// DOM-IGNORE-BEGIN
;/*******************************************************************************
;  Copyright (C) 2016 Microchip Technology Inc.
;
;  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any
;  derivatives created by any person or entity by or on your behalf, exclusively
;  with Microchip's products.  Microchip and its licensors retain all ownership
;  and intellectual property rights in the accompanying software and in all
;  derivatives here to.
;
;  This software and any accompanying information is for suggestion only.  It
;  does not modify Microchip's standard warranty for its products.  You agree
;  that you are solely responsible for testing the software and determining its
;  suitability.  Microchip has no obligation to modify, test, certify, or
;  support the software.
;
;  THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER
;  EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED
;  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR
;  PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS,
;  COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
;
;  IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
;  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,
;  INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE,
;  EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
;  ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF
;  MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
;  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
;  CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
;  FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
;
;  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
;  TERMS.
;*******************************************************************************/
    .equ    WRITE_DWORD_CODE,    0x4001
    .equ    ERASE_PAGE_CODE,     0x4003
    .equ    ERASE_WRITE_OP_MASK, 0x4003
    .equ    FLASH_ERASE_WRITE_OP_MASK, 0x4003
    .equ    FLASH_ERASE_PAGE_SIZE_IN_INSTRUCTIONS, 1024
    .equ    FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS,     1024*2
    .equ    FLASH_ERASE_PAGE_MASK,        (~(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS - 1)) 

    .data
    .global _FlashKey
_FlashKey: .long 0xFFFFFFFF

    .text
    .pushsection    .NVM_flash, code	


/**
 * ;void  FLASH_Unlock(uint32_t key);
 * Used to program the flash key in memory variable located in RAM
 * the valid key is 0x00AA0055
 *
 *
 * @param key       32-bit (unsigned long) for the key
 *                      
 *   Registers used:  w0 w1 
 *                  
 * Inputs:
 *  w0,w1 = long data - Flash Key to save   (32 bits)
 *
 */    
       
    .global         _FLASH_Unlock
    .type           _FLASH_Unlock, @function
    reset
 _FLASH_Unlock:
    mov     #_FlashKey, W2
    mov     W0, [W2++]
    mov     W1, [W2]
    return;
    
/**
 * ;void  FLASH_Lock();
 * Locks the flash from programming by resetting the NVMKEY in memory
  *
 *
 * @param key       32-bit (unsigned long) for the key
 *                      
 *   Registers used:  w0 w1 
 *                  
 * Inputs:
 *  w0,w1 = long data - Flash Key to save   (32 bits)
 *
 */    
       
    .global         _FLASH_Lock
    .type           _FLASH_Lock, @function
    .extern         NVMKEY
    
    reset
 _FLASH_Lock:
    clr W0
    clr W1
    rcall _FLASH_Unlock
    clr NVMKEY    
    return;

 /**
 * ;void FLASH_SendNvmKey(uint32_t key);
 * Procedure to take the NVM key from memory and program the flash controller
 * with it.  A valid key is 0x00AA0055
 *
 *
 * @param key        32-bit (unsigned long) key
 *
 *   Registers used:    w0 w1 (Preserved)
 *                  
 *   Inputs:
 *   w0,w1 = long data - Flash Key to use   (32 bits)
 */    
    reset       
    .global         _FLASH_SendNvmKey
    .type           _FLASH_SendNvmKey, @function
    .extern         NVMKEY
    .extern         TBLPAG
    
    reset  
_FLASH_SendNvmKey:
    push    W0
    push    W1
    push    W2

    mov    #_FlashKey, w1

    ; Disable interrupts
    mov    INTCON2, W2	    ; Save Global Interrupt Enable bit.
    bclr   INTCON2, #15	    ; Disable interrupts

    ; Write the KEY sequence
    mov    [W1++], W0
    mov    W0,     NVMKEY
    mov    [W1],   W0
    mov    W0,     NVMKEY
    bset   NVMCON, #15
    
    ; Insert two NOPs after programming
    nop
    nop
    
    ; Wait for operation to complete
prog_wait:
    btsc NVMCON, #15
    bra prog_wait
    
   ; Re-enable interrupts,
    btsc    W2,#15
    BSET    INTCON2, #15    ; Restore Global Interrupt Enable bit.

    pop    W2
    pop    W1
    pop    W0
    return
    


/**
 * ;uint32_t FLASH_ReadWord24(uint32_t address);
 * Reads the 24 bit instruction located at the address passed to this function.
 *
 *
 * @param address       24-bit (unsigned long) specifying a target address 
 *                      that needs to be read.  Needs to be aligned to an even
 *                      address.
 *
 *   Registers used:    w0 w1 w2
 *                      TBLPAG Preserved
 *                  
 *   Inputs:
 *   w0,w1 = long data - Address in flash to read   (24 bits)
 * 
 *  outputs:
 *   w0,w1 = 32 bit data contains the flash data read.   
 **/    

    reset    
    .global         _FLASH_ReadWord24
    .type           _FLASH_ReadWord24, @function
    .extern         TBLPAG

    
   _FLASH_ReadWord24:
    mov         TBLPAG, W2
    mov         W1, TBLPAG    ; Little endian, w1 has MSW, w0 has LSX
    tblrdh      [W0], W1      ; read MSW of data to high latch
    tblrdl      [W0], W0      ; read LSW of data 
    mov         W2, TBLPAG    ; Restore register, 
    return

/**
 * ;uint16_t FLASH_ReadWord16(uint32_t address);
 * Reads the 16 bit instruction located at the address passed to this function.
 * Address must be aligned to an even address.
 *
 *
 * @param address       24-bit (unsigned long) specifying a target address 
 *                      that needs to be read.  Needs to be aligned to an even
 *                      address.
 *
 *   Registers used:    w0 w1 w2
 *                      TBLPAG Preserved
 *                  
 *   Inputs:
 *   w0,w1 = long data - Address in flash to read   (24 bits)
 * 
 *  outputs:
 *   w0 = 16 bit data contains the flash data read.   
 *
 **/    

    reset    
    .global         _FLASH_ReadWord16
    .type           _FLASH_ReadWord16, @function
    .extern         TBLPAG

    
   _FLASH_ReadWord16:
    mov         TBLPAG, W2
    mov         W1, TBLPAG     ; Little endian, w1 has MSW, w0 has LSX
    tblrdl      [W0], W0       ; read LSW of data
    mov         W2, TBLPAG     ; restore register
    return
   


/**
 * void FLASH_ErasePage(unsigned long address);
 * Erases the page containing the specified address. Be very careful if calling
 * this function as the function will erase any legal page, 
 *
 * NOTE: This function can take upwards of 45ms on some device families and 
 * target pages to complete. At frequencies appreciably slower than the maximum 
 * device frequency, even longer may be required. Therefore, you may need to 
 * issue a ClrWdt() call before calling this function, assuming the Watchdog 
 * timer is enabled. This also means that you should not configure a watchdog 
 * timeout of less than ~64ms, even when you pre-clear the timeout. This 
 * function does NOT internally clear the watchdog for you as it is packaged as 
 * a library routine where not everyone would desire such behavior.
 * 
 * @param address  24-bit (unsigned long) specifying the first address on the page
 *                      to be erased. Must be page aligned.
 * 
 *   Registers used:    w0 w1 w2
 *                      TBLPAG Preserved
 *                  
 *   Inputs:
 *   w0,w1 = long data - Address in flash to erase   (24 bits)
 * 
 *  outputs:
 *   0 - Fail
 *   1 - Pass
 *
 **/ 
    
    .global         _FLASH_ErasePage
    .type           _FLASH_ErasePage, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         NVMADRU 
    .extern         NVMADR
   reset



_FLASH_ErasePage:

    mov     #FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS-1, w2     ;    get mask and validate all lower bits = 0
    and     w2, w0, w2
    bra     NZ,3f 

    mov     #ERASE_PAGE_CODE, w2
    mov     w2, NVMCON
    mov     w0, NVMADR
    mov     w1, NVMADRU        ; MSB
    
    call    _FLASH_SendNvmKey

4:
    mov     #1, w0                 ; default return true
    btsc    NVMCON, #13            ; if error bit set, 
3:  mov     #0, w0                 ;   return false
    return;

 /**
 * ;void FLASH_WriteDoubleWord24(uint32_t flashAddress, uint32_t instructionData0, uint32_t instructionData1);
 *  Writes two 24-bit instruction to the flash at the flashAdddress 
 *  Only the lower 24 bits of each instruction will be written since the flash
 *  is only 24 bits wide and all data in the upper 8 bits will be lost.
 *
 *
 * @param flashAddress  32 bit value specifying a target address in flash 
 *                      that this firmware will write to.  It needs to be on a
 *                      addresses divisible by 4.
 *  
 *  @param uint32_t instructionData0  24 bit instruction  to be written first.
 *  @param uint32_t instructionData1  24 bit instruction  to be written second.
 *
 *
 *   Registers used:    w0 w1 w2 w3 w4 w5
 *                      TBLPAG Preserved
 *                  
 *   Inputs:
 *   w0,w1 = long data - Address in flash to write   (24 bits)
 *   w2,w3 = long data - 24 bits of data to write to flash   (24 bits)
 *   w4,w5 = long data - 24 bits of data to write to flash   (24 bits)
 * 
 *  outputs:
 *   none 
 *
 **/ 
  
    .global         _FLASH_WriteDoubleWord24
    .type           _FLASH_WriteDoubleWord24, @function
    .extern         TBLPAG
    .extern         NVMCON
    .extern         NVMADRU
    .extern         NVMADR
    reset    

_FLASH_WriteDoubleWord24:
    btsc    NVMCON, #15     ; Loop, blocking until last NVM operation is complete (WR is clear)
    bra     _FLASH_WriteDoubleWord24

    btsc    w0, #0          ; Check for a valid address Bit 0 and Bit 1 clear
    bra     3f
    btsc    w0, #1
    bra     3f

good24:
    mov     W1,NVMADRU
    mov     W0,NVMADR       
    
    mov     #WRITE_DWORD_CODE, W0 
    mov     W0, NVMCON
    
    mov     TBLPAG, W0          ; save it
    mov     #0xFA,W1
    mov     W1,TBLPAG
    mov     #0,W1
    
                                ; Perform the TBLWT instructions to write the latches
    tblwtl  W2,[W1]
    tblwth  W3,[W1++]
    tblwtl  W4,[W1]
    tblwth  w5,[W1++]
    
    call    _FLASH_SendNvmKey


    mov     W0, TBLPAG

    mov     #1, w0               ; default return true
    btsc    NVMCON, #13          ; if error bit set, 
3:  mov     #0, w0               ;   return false

    return;


   	
 /**
 * ;void FLASH_WriteDoubleWord16(uint32_t flashAddress, uint16_t Data0, uint16_t Data1);
 *  Writes two 16-bit words to the flash at the flashAdddress 
 *  The upper 8 bits of the 24 bit flash entry will have its  data programmed as 
 *  0xFF.
  *
 *
 * @param flashAddress  32 bit value specifying a target address in flash 
 *                      that this firmware will write to.  It needs to be on a
 *                      addresses divisible by 4.
 *  
 *  @param uint16_t Data0  16 bit word to be written first.
 *  @param uint16_t Data1  16 bit word to be written second.
 *
 *   Registers used:    w0 w1 w2 w3
 *                      TBLPAG Preserved
 *                  
 *   Inputs:
 *   w0,w1 = long data - Address in flash to write   (24 bits)
 *   w2    = 16 bit data - 16 bits of data to write to flash   
 *   w3    = 16 bit data - 16 bits of data to write to flash   
 * 
 *  outputs:
 *   none 
 *
 **/
 
    .global    _FLASH_WriteDoubleWord16
    .type      _FLASH_WriteDoubleWord16, @function
    .extern    TBLPAG
    .extern    NVMCON
    .extern    NVMADRU
    .extern    NVMADR
    reset    

 _FLASH_WriteDoubleWord16:
    btsc    NVMCON, #15     ; Loop, blocking until last NVM operation is complete (WR is clear)
    bra     _FLASH_WriteDoubleWord16

    btsc    w0, #0          ; Check for a valid address Bit 0 and Bit 1 clear
    bra     3f
    btsc    w0, #1
    bra     3f

good16:
    mov     W1,NVMADRU
    mov     W0,NVMADR  

    mov     TBLPAG, W1      ; save it
    
    mov     #WRITE_DWORD_CODE, W0 
    mov     W0, NVMCON
    
    mov     #0xFA,W0
    mov     W0,TBLPAG
    mov     #0,W0

    tblwtl  W2,[W0]         ; Perform the TBLWT instructions to write the latches
    mov     #0xFF,W2
    tblwth  W2,[W0++]  
    tblwtl  W3,[W0]
    tblwth  W2,[W0++] 
   
    call    _FLASH_SendNvmKey


    mov     w1, TBLPAG

    mov     #1, w0          ; default return true
    btsc    NVMCON, #13     ; if error bit set, 
3:  mov     #0, w0          ;   return false
    return;


   





   
;uint16_t FLASH_GetErasePageOffset(uint32_t address)
    .global     _FLASH_GetErasePageOffset
    .type       _FLASH_GetErasePageOffset, @function

_FLASH_GetErasePageOffset:
    mov     #((~FLASH_ERASE_PAGE_MASK) & 0xFFFF), W2
    and     w2, w0, w0
    return

;uint32_t FLASH_GetFlashPageAddress(uint32_t address);

    .global     _FLASH_GetErasePageAddress
    .type       _FLASH_GetErasePageAddress, @function

_FLASH_GetErasePageAddress:
    
    mov     #(FLASH_ERASE_PAGE_MASK & 0xFFFF)	, W2	;LSW
    and     w2, w0, w0
    mov     #((FLASH_ERASE_PAGE_MASK >> 16) & 0xFFFF), W2 ; MSW
    and     w2, w1, w1

    return

   .popsection 
    
