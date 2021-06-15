/**
 * @file states.h
 * @brief Main state machine.
 */

#ifndef STATES_H
#define	STATES_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief States.
     */
    typedef enum {
        STATE_STANDBY,
        STATE_RUNCOMMAND,
        NUM_STATES
    } state_t;

    typedef state_t state_func_t(void); /**< Type for state functions. */

    /**
     *
     * @brief Run the state machine.
     * @param Current state
     * @return Next state
     */
    state_t STATES_RunState(state_t current_state);

#ifdef	__cplusplus
}
#endif

#endif	/* STATES_H */
