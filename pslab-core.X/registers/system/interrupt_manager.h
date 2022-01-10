#ifndef _INTERRUPT_MANAGER_H
#define _INTERRUPT_MANAGER_H

/**
  @Summary
    Initializes the interrupt priorities of the PIC24EP256GP204

  @Description
    This routine sets the interrupt priorities of the modules that have been configured
    for the PIC24EP256GP204

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_Initialize ();
    }
    </code>

 */
void INTERRUPT_Initialize(void);

/**
  @Summary
    Enables global interrupts of the PIC24EP256GP204

  @Description
    This routine enables the global interrupt bit for the PIC24EP256GP204

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_GlobalEnable ();
    }
    </code>

 */
inline static void INTERRUPT_GlobalEnable(void) {
    __builtin_enable_interrupts();
}

/**
  @Summary
    Disables global interrupts of the PIC24EP256GP204

  @Description
    This routine disables the global interrupt bit for the PIC24EP256GP204

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_GlobalDisable ();
    }
    </code>

 */
inline static void INTERRUPT_GlobalDisable(void) {
    __builtin_disable_interrupts();
}

/**
  @Summary
    Returns the interrupt vector number of the interrupt which is pending.

  @Description
    This routine returns the interrupt vector number of the interrupt which is pending.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
        uint16_t ivrNum;
        ivrNum = INTERRUPT_VectorNumberGet();
    </code>

 */
inline static uint16_t INTERRUPT_VectorNumberGet(void) {
    return _VECNUM;
}

inline static void INTERRUPT_ClearPinChangeInterruptsFlag(void) {
    IFS1bits.CNIF = 0;
}

inline static void INTERRUPT_EnablePinChangeInterrupts(void) {
    IEC1bits.CNIE = 1;
}

inline static void INTERRUPT_DisablePinChangeInterrupts(void) {
    IEC1bits.CNIE = 0;
}

inline static void INTERRUPT_EnableExternalInterrupt2(void) {
    IEC1bits.INT2IE = 1;
}

inline static void INTERRUPT_DisableExternalInterrupt2(void) {
    IEC1bits.INT2IE = 0;
}

inline static void INTERRUPT_ClearExternalInterrupt2Flag(void) {
    IFS1bits.INT2IF = 0;
}

inline static void INTERRUPT_LA1PinChange(bool state) {
    _CNIEB10 = state ? 1 : 0;
}

inline static void INTERRUPT_LA2PinChange(bool state) {
    _CNIEB11 = state ? 1 : 0;
}

inline static void INTERRUPT_LA3PinChange(bool state) {
    _CNIEB12 = state ? 1 : 0;
}

inline static void INTERRUPT_LA4PinChange(bool state) {
    _CNIEB13 = state ? 1 : 0;
}

#endif
