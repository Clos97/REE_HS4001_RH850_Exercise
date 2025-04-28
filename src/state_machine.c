#include "state_machine.h"
#include <stdio.h>
#include "r_cg_macrodriver.h"
#include "Config_PORT.h"
#include "temperature_reading.h"
#include "Config_RTCA0.h"

// ===============================
// Internal State Function Prototypes
// ===============================

// These functions define the behavior for each state.
static void State_Init(void);
static void State_Idle(void);
static void State_ReadSensor(void);
static void State_UartLog(void);
static void State_Sleep(void);
static void State_WakeUp(void);
static void State_Reset(void);
static void State_Error(void);

// Make g_event extern, so that states can trigger an event
extern Event_t g_event;
extern TemperatureMeasurement g_temp_measurement;
extern rtc_counter_value_t g_rtc_value;
// ===============================
// Internal: Function Table for State Execution
// ===============================

// This table maps each state enum to its corresponding function.
static StateFunction_t stateTable[STATE_COUNT] = {
    State_Init,
    State_Idle,
    State_ReadSensor,
	State_UartLog,
	State_Sleep,
	State_WakeUp,
	State_Reset,
	State_Error
};

// ===============================
// Initialize the State Machine
// ===============================

/**
 * @brief Initializes the state machine structure.
 *        Sets the initial state and connects the state functions.
 *
 * @param sm Pointer to the state machine instance.
 */
void StateMachine_Init(StateMachine_t* sm) {
	uint8_t i;
    if (!sm) return;

    sm->currentState = STATE_INIT; // Set the initial state

    // Assign the state function table
    for (i = 0; i < STATE_COUNT; i++)
	{
		sm->stateFunctions[i] = stateTable[i];
	}
}

// ===============================
// Execute the Current State Logic
// ===============================

/**
 * @brief Executes the function assigned to the current state.
 *
 * @param sm Pointer to the state machine instance.
 */
void StateMachine_Run(StateMachine_t* sm) {
    if (!sm) return;

    if (sm->currentState < STATE_COUNT) {
        // Call the function associated with the current state
        sm->stateFunctions[sm->currentState]();
    } else {
        // Safety check in case the state is out of range
        printf("Invalid state: %d\n", sm->currentState);
    }
}

// ===============================
// Handle Events and Perform Transitions
// ===============================

/**
 * @brief Handles incoming events and transitions to the next state if applicable.
 *
 * @param sm Pointer to the state machine instance.
 * @param event The event that occurred.
 */
void StateMachine_HandleEvent(StateMachine_t* sm, Event_t event) {
    if (!sm) return;

    // The state doesn't matter if an error occures
    if(event == EVT_ERROR_DETECTED)
    {
    	sm->currentState = STATE_ERROR;
    	return;
    }

    switch (sm->currentState) {
        case STATE_INIT:
            if (event == EVT_SYSTEM_READY) {
            	uart_send_state_transition(sm->currentState, STATE_IDLE);
                sm->currentState = STATE_IDLE;
            }
            break;

        case STATE_IDLE:
        	if(event = EVT_TIMER_ELAPSED)
        	{
        		uart_send_state_transition(sm->currentState, STATE_READ_SENSOR);
        		sm->currentState = STATE_READ_SENSOR;
        	}
            break;

        case STATE_READ_SENSOR:
        	if(event == EVT_SENSOR_READ_SUCCESS){
        		uart_send_state_transition(sm->currentState, STATE_SEND_LOG_UART);
        		sm->currentState = STATE_SEND_LOG_UART;
        	}
            break;
        case STATE_SEND_LOG_UART:
        	if(event == EVT_UART_SEND_COMPLETE){
				uart_send_state_transition(sm->currentState, STATE_IDLE);
				sm->currentState = STATE_IDLE;
			}
        	break;
        case STATE_SLEEP:
        	break;
        case STATE_WAKEUP:
        	break;
        case STATE_ERROR:
            if (event == EVT_RESET) {
                sm->currentState = STATE_IDLE;
            }
            break;

        default:
            break;
    } // end switch-case


} // end StateMachine_HandleEvent

// ===============================
// State Logic Implementations
// ===============================

/**
 * @brief Logic for the INIT state.
 */
static void State_Init(void) {
    // Initialization logic (hardware init, boot checks, etc.)
	R_Systeminit();


    // Start I2C Peripheral
    R_Config_RIIC0_Start();

    // Start Communication via UART
    R_Config_UART0_Start();

    //Start the Standard Interval Timer for periodic reads
    R_Config_TAUB0_0_Start();

    // Enable RTC
    R_Config_RTCA0_Start();

    g_rtc_value.day 	= 	15;
    g_rtc_value.month 	= 	4;
    g_rtc_value.year 	=	2025;
    g_rtc_value.hour	=	13;
    g_rtc_value.min		=	35;
    g_rtc_value.sec		=	0;

    R_Config_RTCA0_Set_CounterValue(g_rtc_value);


    // Init DMA -> TODO



    R_Config_DMAC00_Start();

    // Enable all Interrupts
    EI();
    // When init is successful -> Raise an event
    g_event = EVT_SYSTEM_READY;
}

/**
 * @brief Logic for the IDLE state.
 */
static void State_Idle(void) {

}

/**
 * @brief Logic for the READ_SENSOR state.
 */
static void State_ReadSensor(void) {

    // Toggle the LED
    PORT.P8 = (PORT.P8 ==_PORT_Pn5_OUTPUT_LOW)?_PORT_Pn5_OUTPUT_HIGH:_PORT_Pn5_OUTPUT_LOW;

    // Temperature & humidity measurement
    start_measurement(&g_temp_measurement);

    switch(g_temp_measurement.status)
    {
		case TEMP_STATE_DATA_VALID:
			break; // just continue
		case TEMP_STATE_DATA_INVALID:
			g_event = EVT_SENSOR_READ_FAIL;
			return;
		case TEMP_CNT_OVERFLOW:
			g_event = EVT_ERROR_DETECTED;
			return;
    }

	calc_humidity(&g_temp_measurement);
	calc_temperature(&g_temp_measurement);

    // get timestamp
    R_Config_RTCA0_Get_CounterBufferValue(&g_rtc_value);

    sprintf(g_temp_measurement.timestamp, "%02d:%02d:%02d", g_rtc_value.hour, g_rtc_value.min, g_rtc_value.sec);

    g_event = EVT_SENSOR_READ_SUCCESS;
}

/**
 * @brief Logic for the UART_LOGGING state.
 */
static void State_UartLog(void) {

    // Waiting for an action (e.g. user input or external trigger)
	uart_send_log_humidity_and_temperature(g_temp_measurement); // TODO: The program gets stuck in here
	g_event = EVT_UART_SEND_COMPLETE;
}

/**
 * @brief Logic for the SLEEP state.
 */
static void State_Sleep(void) {

    // Waiting for an action (e.g. user input or external trigger)
}

/**
 * @brief Logic for the WAKEUP state.
 */
static void State_WakeUp(void) {
    // Waiting for an action (e.g. user input or external trigger)
}

/**
 * @brief Logic for the RESET state.
 */
static void State_Reset(void) {

    // Reset the Error State -> Blinking counter etc.
    R_Config_TAUB0_0_Start();
	R_Config_TAUB0_1_Stop();
}


/**
 * @brief Logic for the ERROR state.
 */
static void State_Error(void) {

    // Handle error, maybe log and try to recover
    // Start the Error Timer for fast LED flashing
    R_Config_TAUB0_0_Stop();
    R_Config_TAUB0_1_Start();
}


