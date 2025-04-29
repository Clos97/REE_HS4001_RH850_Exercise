#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

// ===============================
// State Machine Core Definitions
// ===============================

// Define the possible states
typedef enum {
    STATE_INIT,
    STATE_IDLE,
	STATE_READ_SENSOR,
	STATE_SEND_LOG_UART,
	STATE_SLEEP,
	STATE_WAKEUP,
	STATE_RESET,
    STATE_ERROR,
    STATE_COUNT // Always keep this last for bounds checking
} State_t;

// Define the possible events that can trigger state transitions
typedef enum {
	EVT_SYSTEM_READY,
	EVT_TIMER_ELAPSED,
	EVT_SENSOR_READ_SUCCESS,
	EVT_SENSOR_READ_FAIL, // Not in Use
	EVT_UART_SEND_COMPLETE,
	EVT_BUTTON_PRESS,
	EVT_RTC_ALARM,
	EVT_WAKEUP_FROM_SLEEP,
	EVT_ERROR_DETECTED,
	EVT_RESET,
    EVT_NONE
} Event_t;

// State machine function pointer type
typedef void (*StateFunction_t)(void);

// State machine structure
typedef struct {
    State_t currentState;
    StateFunction_t stateFunctions[STATE_COUNT];
} StateMachine_t;

// ===============================
// Function Declarations
// ===============================

/**
 * @brief Initializes the state machine.
 * @param sm Pointer to the StateMachine structure.
 */
void StateMachine_Init(StateMachine_t* sm);

/**
 * @brief Handles a given event and updates the state accordingly.
 * @param sm Pointer to the StateMachine structure.
 * @param event The event to handle.
 */
void StateMachine_HandleEvent(StateMachine_t* sm, Event_t event);

/**
 * @brief Runs the function associated with the current state.
 * @param sm Pointer to the StateMachine structure.
 */
void StateMachine_Run(StateMachine_t* sm);

#ifdef __cplusplus
}
#endif

#endif // STATE_MACHINE_H
