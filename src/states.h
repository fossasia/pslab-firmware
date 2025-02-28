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
    typedef enum EState {
        STATE_STANDBY,
        STATE_RUNCOMMAND,
        NUM_STATES
    } EState;

    typedef EState (*StateFunc)(void); /**< Type for state functions. */

    /**
     *
     * @brief Run the state machine.
     * @param Current state
     * @return Next state
     */
    EState STATES_run_state(EState current_state);

#ifdef	__cplusplus
}
#endif

#endif	/* STATES_H */
