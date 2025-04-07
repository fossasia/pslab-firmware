#ifndef CTMU_H
#define	CTMU_H

#include <xc.h>

#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    void CTMU_Initialize(void);

    void CTMU_InitializeCON1(void);
    void CTMU_InitializeCON2(void);

    inline static void CTMU_Enable(void) {
        CTMUCON1bits.CTMUEN = 1;
    }

    inline static void CTMU_Disable(void) {
        CTMUCON1bits.CTMUEN = 0;
    }

    inline static void CTMU_DrainOutput(void) {
        CTMUCON1bits.IDISSEN = 1;
    }

    inline static void CTMU_FloatOutput(void) {
        CTMUCON1bits.IDISSEN = 0;
    }

    inline static void CTMU_EnableEdge1(void) {
        CTMUCON2bits.EDG1STAT = 1;
    }

    inline static void CTMU_DisableEdge1(void) {
        CTMUCON2bits.EDG1STAT = 0;
    }

    inline static void CTMU_EnableEdge2(void) {
        CTMUCON2bits.EDG2STAT = 1;
    }

    inline static void CTMU_DisableEdge2(void) {
        CTMUCON2bits.EDG2STAT = 0;
    }

    inline static void CTMU_EnableEdgeDelayGeneration(void) {
        CTMUCON1bits.TGEN = 1;
    }

    inline static void CTMU_DisableEdgeDelayGeneration(void) {
        CTMUCON1bits.TGEN = 0;
    }

    inline static void CTMU_DisableModule(void) {
        CTMU_Disable();
        CTMU_DisableEdge1();
        CTMU_DisableEdge2();
        CTMU_DisableEdgeDelayGeneration();
    }

    /**
     * @brief Start Charge Time Measurement Unit
     *
     * @description
     * This command function takes two arguments over serial:
     * 1. (uint8)  Configuration byte:
     *             | 7  | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *             | DG |            CSO            |
     *             DG: Delay generator enable/disable (1/0)
     *             CSO: Current source output:
     *                    00: Base * 1000
     *                    01: Base = 0.53 uA
     *                    10: Base * 10
     *                    11: Base * 100
     * 2. (uint8) Current trim
     *
     * It returns nothing over serial.
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    enum Status CTMU_start(
        uint8_t **args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Stop Charge Time Measurement Unit
     *
     * @description
     * This command function does not take any arguments over serial. It stops
     * and disables CTMU module.
     *
     * It returns nothing over serial.
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    enum Status CTMU_stop(
        uint8_t **args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CTMU_H */
