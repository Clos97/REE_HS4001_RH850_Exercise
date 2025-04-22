/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018, 2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : r_cg_main.c
* Version          : 1.0.140
* Device(s)        : R7F701649
* Description      : This file implements main function.
***********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_smc_entry.h"
/* Start user code for include. Do not edit comment generated here */
#include "state_machine.h"
#include "UART_Functions.h"
#include "temperature_reading.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
// Globale State Machine Instanz
StateMachine_t g_StateMachine;

// Globale Event-Variable (könnte auch eine Queue oder ein Bitfeld sein)
volatile Event_t g_event = EVT_NONE;

// Maybe thes variables are obsolete -> State machine needs to handle this
MD_STATUS	err;
extern volatile uint8_t          g_riic0_state;
uint8_t g_useStateMachine = 1U; // Turn on the state machine -> Off for testing the individual peripherals
extern uint8_t uart_isr_status_flag_send_complete;
TemperatureMeasurement g_temp_measurement;
rtc_counter_value_t g_rtc_value;
/* End user code. Do not edit comment generated here */
void r_main_userinit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    r_main_userinit();
    /* Start user code for main. Do not edit comment generated here */

    if(g_useStateMachine == 1U)
    {
		// Init the state machine
		StateMachine_Init(&g_StateMachine);


		// Start the state machine with the INIT State
		g_StateMachine.stateFunctions[g_StateMachine.currentState]();

		// Main Loop
		while (1)
		{
			//When an event is raised, the new State must be handled
			if (g_event != EVT_NONE)
			{
				StateMachine_HandleEvent(&g_StateMachine, g_event);

				// Reset the event after handling
				g_event = EVT_NONE;

				// Processing of the State function
				g_StateMachine.stateFunctions[g_StateMachine.currentState]();
			}
		} // end while
	} // End use state machine
} // end main

/***********************************************************************************************************************
* Function Name: r_main_userinit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_main_userinit(void)
{
    DI();
    /* Start user code for r_main_userinit. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
