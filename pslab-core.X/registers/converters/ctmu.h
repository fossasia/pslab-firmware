#ifndef CTMU_H
#define	CTMU_H

#include <xc.h>

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
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CTMU_H */
