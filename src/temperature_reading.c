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

	// Extract the raw data from the message
	// Check status bit
	//if((rx_buf[0] &0xC0) == 0x00) // This is not working -> die 0 am Anfang wird nicht erkannt
	if((rx_buf[0] >> 7) == 0 && (rx_buf[0] >> 6) == 0)
	{
		// Data is Valid
		// calc Humidity
		measurement->relative_humidity_raw = ((rx_buf[0] & 0x3F) << 8) | rx_buf[1];

		// Calc temp
		measurement->temperature_raw = (rx_buf[2] << 8) | (rx_buf[3] & 0xFC);
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
