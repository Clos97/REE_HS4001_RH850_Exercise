/*
 * UART_Functions.c
 *
 *  Created on: 15.04.2025
 *      Author: Philipp Clostermeyer
 */

#include "UART_Functions.h"

extern Event_t g_event;
extern uint8_t uart_isr_status_flag_send_complete;
extern TemperatureMeasurement g_temp_measurement;
char g_tx_buf1[UART_BUFFER_SIZE];

void uart_send_state_transition(State_t old_state, State_t new_state)
{
	const char* char_state_old;
	const char* char_state_new;
	volatile MD_STATUS err = MD_OK;
	uint16_t length = 0;


	switch(old_state)
	{
	    case 0x00: char_state_old = "INITIALISATION"; break;
	    case 0x01: char_state_old = "IDLE"; break;
	    case 0x02: char_state_old = "READ SENSOR"; break;
	    case 0x03: char_state_old = "SEND LOG MESSAGE"; break;
	    case 0x04: char_state_old = "SLEEPING"; break;
	    case 0x05: char_state_old = "WAKEUP"; break;
	    case 0x06: char_state_old = "RESET"; break;
	    case 0x07: char_state_old = "ERROR"; break;
	    default:   char_state_old = "STATE NOT DEFINED"; break;
	}

	switch(new_state)
	{
		case 0x00: char_state_new = "INITIALISATION"; break;
		case 0x01: char_state_new = "IDLE"; break;
		case 0x02: char_state_new = "READ SENSOR"; break;
		case 0x03: char_state_new = "SEND LOG MESSAGE"; break;
		case 0x04: char_state_new = "SLEEPING"; break;
		case 0x05: char_state_new = "WAKEUP"; break;
		case 0x06: char_state_new = "RESET"; break;
		case 0x07: char_state_new = "ERROR"; break;
		default:   char_state_new = "STATE NOT DEFINED"; break;
	}



	// char_state in den Buffer einfügen
	length = sprintf(g_tx_buf1, "Transition from %s to %s \r\n", char_state_old, char_state_new);

	uart_isr_status_flag_send_complete = 0;

	// Only sent the real length
	err = R_Config_UART0_Send(g_tx_buf1, length);

	if(err != MD_OK)
	{
		g_event = EVT_ERROR_DETECTED;
		return;
	}

	// Blocking while sending
	while(uart_isr_status_flag_send_complete != 1);

	uart_isr_status_flag_send_complete = 0;
}

void uart_send_log_humidity_and_temperature(TemperatureMeasurement measurement)
{

	volatile MD_STATUS err = MD_OK;
	uint16_t length = 0;

	// char_state in den Buffer einfügen
	length = sprintf(g_tx_buf1,
	    "+-------------------------------+\r\n"
	    "| %s | %5.2f C | %5.2f %%  |\r\n"
	    "+-------------------------------+\r\n",
	    measurement.timestamp, measurement.temperature_celsius, measurement.relative_humidity);


	// Only sent the real length
	err = R_Config_UART0_Send(g_tx_buf1, length);

	if(err != MD_OK)
	{
		g_event = EVT_ERROR_DETECTED;
		return;
	}

	// Blocking while sending
	while(uart_isr_status_flag_send_complete != 1);

	uart_isr_status_flag_send_complete = 0;
}
