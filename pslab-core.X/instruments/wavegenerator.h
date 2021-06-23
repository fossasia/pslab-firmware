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
     * @brief Reads the content of wave table pattern 1
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_ReadWaveForm1(void);
    
    /**
     * @brief Replace default wave table pattern 2 with a user defined sequence
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_LoadWaveForm2(void);
    
    /**
     * @brief Reads the content of wave table pattern 2
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_ReadWaveForm2(void);
    
    /**
     * @brief Generate sine wave from SIN1 pin
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_SetSine1(void);
    
    /**
     * @brief Generate sine wave from SIN2 pin
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_SetSine2(void);
    
    /**
     * @brief Generate two sine waves from SIN1 and SIN2 pins
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_SetSineDual(void);
    
    /**
     * @brief Generate PWM wave from SQR1 pin
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_SetSquare1(void);
    
    /**
     * @brief Generate PWM wave from SQR2 pin
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_SetSquare2(void);
    
    /**
     * @brief Generate PWM wave from all SQR pins
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_SetSquareAll(void);
    
    /**
     * @brief Link system oscillator to an output pin
     * @return SUCCESS
     */
    response_t WAVEGENERATOR_MapReference(void);

#ifdef	__cplusplus
}
#endif

#endif	/* WAVEGENERATOR_H */
