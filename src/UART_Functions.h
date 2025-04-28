/*
 * UART_Functions.h
 *
 *  Created on: 15.04.2025
 *      Author: a5153359
 */

#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include "state_machine.h"
#include "r_cg_macrodriver.h"
#include "temperature_reading.h"

#define UART_BUFFER_SIZE (120U)



void uart_send_state_transition(State_t old_state, State_t new_state);
void uart_send_log_humidity_and_temperature(TemperatureMeasurement measurement);
#endif /* UART_FUNCTIONS_H_ */

