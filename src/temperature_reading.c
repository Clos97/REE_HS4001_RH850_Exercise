/*
 * temperature_reading.c
 *
 *  Created on: 25.04.2025
 *      Author: a5153359
 */
#include <stdio.h>
#include "temperature_reading.h"
#include "r_cg_macrodriver.h"
#include "Config_RIIC0.h"

// Definition of global variables
extern uint8_t 				i2c_transmit_flag;
extern uint8_t				i2c_receive_flag;

uint32_t					cnt_sfty						= 0;
uint8_t 					rx_buf[BYTES_CNT_I2C_MESSAGE] 	= {0x00};

void start_measurement(TemperatureMeasurement *measurement)
{

	// Measurement request
	i2c_transmit_flag 	= 0;
	i2c_receive_flag	= 0;
	cnt_sfty 			= 0;

	R_Config_RIIC0_Master_Send(SENSOR_SLAVE_ADDRESS,"", 0);

	while(i2c_transmit_flag	==	0U)
	{
		if(cnt_sfty >= SAVETY_CNT)
		{
			measurement->status = TEMP_CNT_OVERFLOW;
			return;
		}
		cnt_sfty++;
	}

	cnt_sfty = 0;

	// Delay
	while(cnt_sfty < DELAY_CNT)
	{
		cnt_sfty++;
	}

	cnt_sfty = 0;
	// Measure
	R_Config_RIIC0_Master_Receive(SENSOR_SLAVE_ADDRESS, rx_buf, BYTES_CNT_I2C_MESSAGE);

	while(i2c_receive_flag	==	0U)
	{
		if(cnt_sfty >= SAVETY_CNT)
		{
			measurement->status = TEMP_CNT_OVERFLOW;
			return;
		}
		cnt_sfty++;
	}

	// Now take the pointer values and paste them into the rx buffer

}

void start_measurement_with_dma()
{
	// Start a new measurement with the request
	i2c_transmit_flag 	= 0;
	cnt_sfty			= 0;
	INTC2.ICRIIC0TI.BIT.MKRIIC0TI = _INT_PROCESSING_ENABLED; // Enable the Transmited interrupt (is disabled in the receive custom function)
	INTC2.ICRIIC0RI.BIT.MKRIIC0RI = _INT_PROCESSING_ENABLED; // same here

	R_Config_RIIC0_Master_Send(SENSOR_SLAVE_ADDRESS,"", 0);

	while(i2c_transmit_flag	==	0U)
	{
		if(cnt_sfty >= SAVETY_CNT)
		{
			return;
		}
		cnt_sfty++;
	}

	cnt_sfty = 0;

	// Reset the I2C peripheral
	R_Config_RIIC0_Stop();
	// Delay
	while(cnt_sfty < DELAY_CNT)
	{
		cnt_sfty++;
	}
	R_Config_RIIC0_Start();
	cnt_sfty = 0;

	// Get the new measurement values
	R_Config_RIIC0_Master_Receive_custom(SENSOR_SLAVE_ADDRESS);
}

void get_measurement_values(TemperatureMeasurement *measurement, uint8_t *buffer)
{
	if((buffer[0] >> 7) == 0 && (buffer[0] >> 6) == 0)
	{
		// Data is Valid
		// calc Humidity
		measurement->relative_humidity_raw = ((buffer[0] & 0x3F) << 8) | buffer[1];

		// Calc temp
		measurement->temperature_raw = (buffer[2] << 8) | (buffer[3] & 0xFC);
		measurement->temperature_raw >>= 2;

		// Indicate that the data is valid
		measurement->status = TEMP_STATE_DATA_VALID;
	}else{
		measurement->status = TEMP_STATE_DATA_INVALID;
	}
}

void calc_humidity(TemperatureMeasurement *measurement)
{
	measurement->relative_humidity = (measurement->relative_humidity_raw / 16383.0f) * 100.0f;
}

void calc_temperature(TemperatureMeasurement *measurement)
{
	measurement->temperature_celsius = ((measurement->temperature_raw / 16383.0f) * 165.0f) - 40.0f;
}
