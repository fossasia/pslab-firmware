#ifndef WAVEGENERATOR_H
#define	WAVEGENERATOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define WAVE_TABLE_FULL_LENGTH          512
    #define WAVE_TABLE_SHORT_LENGTH         32

    extern int WAVEGENERATOR_table_1[WAVE_TABLE_FULL_LENGTH];
    extern int WAVEGENERATOR_table_2[WAVE_TABLE_FULL_LENGTH];

    /**
     * @brief Replace default wave table pattern 1 with a user defined sequence
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_load_wave_1(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Reads the content of wave table pattern 1
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_read_wave_1(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Replace default wave table pattern 2 with a user defined sequence
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_load_wave_2(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Reads the content of wave table pattern 2
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_read_wave_2(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Generate sine wave from SIN1 pin
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_set_sine_1(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Generate sine wave from SIN2 pin
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_set_sine_2(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Generate two sine waves from SIN1 and SIN2 pins
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_set_sine_dual(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Generate PWM wave from SQR1 pin
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_set_square_1(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Generate PWM wave from SQR2 pin
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_set_square_2(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Generate PWM wave from all SQR pins
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_set_square_all(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Link system oscillator to an output pin
     * @return SUCCESS
     */
    enum Status WAVEGENERATOR_map_reference(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif

#endif	/* WAVEGENERATOR_H */
