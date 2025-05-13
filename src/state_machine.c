#include "state_machine.h"
#include <stdio.h>
#include "r_cg_macrodriver.h"
#include "Config_PORT.h"
#include "temperature_reading.h"
#include "Config_RTCA0.h"
#include "Config_DMAC00.h"
// ===============================
// Internal State Function Prototypes
// ===============================

// These functions define the behavior for each state.
static void State_Init(void);
static void State_Idle(void);
static void State_SendMeasReq(void);
static void State_calcSensorValues(void);
static void State_UartLog(void);
static void State_Sleep(void);
static void State_WakeUp(void);
static void State_Reset(void);
static void State_Error(void);

// Make g_event extern, so that states can trigger an event
extern Event_t g_event;
extern TemperatureMeasurement g_temp_measurement;
extern rtc_counter_value_t g_rtc_value;
rtc_alarm_value_t g_alarmValue = 0;
// ===============================
// Internal: Function Table for State Execution
// ===============================

// This table maps each state enum to its corresponding function.
static StateFunction_t stateTable[STATE_COUNT] = {
    State_Init,
    State_Idle,
    State_SendMeasReq,
	State_calcSensorValues,
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
    	uart_send_state_transition(sm->currentState, STATE_ERROR);
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
        	if(event == EVT_TIMER_ELAPSED)
        	{
        		uart_send_state_transition(sm->currentState, STATE_SEND_MEAS_REQ);
        		sm->currentState = STATE_SEND_MEAS_REQ;
        	}
        	if(event == EVT_DMA_TRANSFER_FINISHED)
			{
				uart_send_state_transition(sm->currentState, STATE_CALC_SENS_VALUES);
				sm->currentState = STATE_CALC_SENS_VALUES;
			}
        	if(event == EVT_BUTTON_PRESS)
			{
				uart_send_state_transition(sm->currentState, STATE_SLEEP);
				sm->currentState = STATE_SLEEP;
			}
            break;

        case STATE_SEND_MEAS_REQ:
        	if(event == EVT_SEND_MEAS_REQ){
        		uart_send_state_transition(sm->currentState, STATE_IDLE);
        		sm->currentState = STATE_IDLE;
        	}
        	if(event == EVT_DMA_TRANSFER_FINISHED){
				uart_send_state_transition(sm->currentState, STATE_CALC_SENS_VALUES);
				sm->currentState = STATE_CALC_SENS_VALUES;
			}
        	if(event == EVT_BUTTON_PRESS)
			{
				uart_send_state_transition(sm->currentState, STATE_SLEEP);
				sm->currentState = STATE_SLEEP;
			}
            break;
        case STATE_CALC_SENS_VALUES:
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
        	if(event == EVT_BUTTON_PRESS)
			{
				uart_send_state_transition(sm->currentState, STATE_SLEEP);
				sm->currentState = STATE_SLEEP;
			}
        	break;
        case STATE_SLEEP:
        	if(event == EVT_BUTTON_PRESS)
			{
				uart_send_state_transition(sm->currentState, STATE_WAKEUP);
				sm->currentState = STATE_WAKEUP;
			}
        	if(event == EVT_RTC_ALARM)
			{
				uart_send_state_transition(sm->currentState, STATE_WAKEUP);
				sm->currentState = STATE_WAKEUP;
			}

        	break;
        case STATE_WAKEUP:
        	if(event == EVT_SYSTEM_READY)
        	{
        		sm->currentState = STATE_IDLE;
        	}
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

    g_rtc_value.hour	=	bin_to_bcd(13);
    g_rtc_value.min		=	bin_to_bcd(17);
    g_rtc_value.sec		=	bin_to_bcd(0);

    R_Config_RTCA0_Set_CounterValue(g_rtc_value);
    R_Config_RTCA0_Set_AlarmOff();
    // Internal Interrupt INT12
    R_Config_INTC_INTP12_Start();


    // Prepare the Stop mode -> Stand by Controller
    R_Config_STBC_Prepare_Stop_Mode();

    // Init the DMA
    R_Config_DMAC00_Create_UserInit();

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
static void State_SendMeasReq(void) {

    // Toggle the LED
    //PORT.P8 = (PORT.P8 ==_PORT_Pn5_OUTPUT_LOW)?_PORT_Pn5_OUTPUT_HIGH:_PORT_Pn5_OUTPUT_LOW;

    // Temperature & humidity measurement
    //start_measurement(&g_temp_measurement);

	// Turn the DMA on
	R_Config_DMAC00_Start();
	R_Config_RIIC0_Start();
	start_measurement_with_dma();

	// This must be done because most of the time the DMA is faster than this is reached
	g_event = (g_event == EVT_NONE) ? EVT_SEND_MEAS_REQ: g_event;

	R_Config_RIIC0_Stop();
//    switch(g_temp_measurement.status)
//    {
//		case TEMP_STATE_DATA_VALID:
//			break; // just continue
//		case TEMP_STATE_DATA_INVALID:
//			g_event = EVT_SENSOR_READ_FAIL;
//			return;
//		case TEMP_CNT_OVERFLOW:
//			g_event = EVT_ERROR_DETECTED;
//			return;
//    }
//
//	calc_humidity(&g_temp_measurement);
//	calc_temperature(&g_temp_measurement);
//
//    // get timestamp
//    R_Config_RTCA0_Get_CounterBufferValue(&g_rtc_value);
//
//    sprintf(g_temp_measurement.timestamp, "%02d:%02d:%02d", bcd_to_bin(g_rtc_value.hour), bcd_to_bin(g_rtc_value.min), bcd_to_bin(g_rtc_value.sec));
//
//    g_event = (g_event == EVT_NONE)?EVT_SENSOR_READ_SUCCESS:g_event; // Only assign if meanwhile a error or button press didn't occure
}

static void State_calcSensorValues(void) {

	uint8_t rx_buf[BYTES_CNT_I2C_MESSAGE];
	uint32_t i = 0;
	uint32_t *ptr_address = DMA_SRC_ADDRESS + 0x04; // Offset the start since the first read is always zero

	// Turn The DMA off
	R_Config_DMAC00_Stop();
	for (i = 0; i < BYTES_CNT_I2C_MESSAGE; i++) {
	    rx_buf[i] = (uint8_t)(*ptr_address);
	    ptr_address = ptr_address + 1;
	}

	// extract the raw data
	get_measurement_values(&g_temp_measurement, &rx_buf);

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

	sprintf(g_temp_measurement.timestamp, "%02d:%02d:%02d", bcd_to_bin(g_rtc_value.hour), bcd_to_bin(g_rtc_value.min), bcd_to_bin(g_rtc_value.sec));

	g_event = (g_event == EVT_NONE)?EVT_SENSOR_READ_SUCCESS:g_event; // Only assign if meanwhile a error or button press didn't occure


}

/**
 * @brief Logic for the UART_LOGGING state.
 */
static void State_UartLog(void) {

    // Waiting for an action (e.g. user input or external trigger)
	uart_send_log_humidity_and_temperature(g_temp_measurement);
	g_event = (g_event == EVT_NONE)?EVT_UART_SEND_COMPLETE:g_event; // Only assign if meanwhile a error or button press didn't occure
}

/**
 * @brief Logic for the SLEEP state.
 */
static void State_Sleep(void) {
		// Stop the interval Timer
		//R_Config_TAUB0_0_Stop(); -> Should be turned off anyways

		// Turn the LED OFF -> save more power
		PORT.P8 = _PORT_Pn5_OUTPUT_LOW;

		// Define RTC Alarm Time in 1 minute

		// get timestamp
		R_Config_RTCA0_Get_CounterBufferValue(&g_rtc_value);
		R_Config_RTCA0_Get_AlarmValue(&g_alarmValue);

		// Add 1 minute to the BCD time
		if (g_rtc_value.min == 0x59)
		{
		    // Reset minutes to 00 and increment hour (with BCD correction)
		    g_alarmValue.alarmwm = 0x00;

		    if ((g_rtc_value.hour & 0x0F) < 9) {
		        g_alarmValue.alarmwh = g_rtc_value.hour + 0x01;
		    } else {
		        g_alarmValue.alarmwh = (g_rtc_value.hour & 0xF0) + 0x10;
		    }

		    // Optional: handle overflow from 0x23 → 0x00 if nötig
		    if (g_alarmValue.alarmwh == 0x24) {
		        g_alarmValue.alarmwh = 0x00;
		    }
		}
		else
		{
		    // BCD Minute Addition (inline)
		    if ((g_rtc_value.min & 0x0F) < 9) {
		        g_alarmValue.alarmwm = g_rtc_value.min + 0x01;
		    } else {
		        g_alarmValue.alarmwm = (g_rtc_value.min & 0xF0) + 0x10;
		    }

		    g_alarmValue.alarmwh = g_rtc_value.hour;
		}


		//g_alarmValue.alarmww = 0x7f; // No Weekday specified
		// Set the new alarm Value
		R_Config_RTCA0_Set_AlarmValue(g_alarmValue);


		R_Config_RTCA0_Set_AlarmOn();

		// Enter LPM
		R_Config_STBC_Start_Stop_Mode();

}

/**
 * @brief Logic for the WAKEUP state.
 */
static void State_WakeUp(void) {
	// Turn RTC Alarm Off
	R_Config_RTCA0_Set_AlarmOff();
    // Start the Interval Timer again
	//R_Config_TAUB0_0_Start();

	g_event = EVT_SYSTEM_READY;
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


