#ifndef WAVEGENERATOR_H
#define	WAVEGENERATOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief Replace default wave table pattern 1 with a user defined sequence
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_LoadWaveForm1(void);
    
    /**
     * @brief Replace default wave table pattern 2 with a user defined sequence
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_LoadWaveForm2(void);

#ifdef	__cplusplus
}
#endif

#endif	/* WAVEGENERATOR_H */

