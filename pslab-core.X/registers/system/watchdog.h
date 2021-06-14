#ifndef WATCHDOG_H
#define	WATCHDOG_H

/**
  Section: Type defines
 */

/**
 * Enables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WATCHDOG_TimerSoftwareEnable();
 * </code>
 */
inline static void WATCHDOG_TimerSoftwareEnable(void) {
    RCONbits.SWDTEN = 1;
}

/**
 * Disables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WATCHDOG_TimerSoftwareDisable();
 * </code>
 */
inline static void WATCHDOG_TimerSoftwareDisable(void) {
    RCONbits.SWDTEN = 0;
}

/**
 * Clears the Watch Dog Timer (WDT).
 * @example
 * <code>
 * WATCHDOG_TimerClear();
 * </code>
 */
inline static void WATCHDOG_TimerClear(void) {
    ClrWdt();
}

#endif	/* WATCHDOG_H */
