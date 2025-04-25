/*
 * temperature_reading.h
 *
 *  Created on: 25.04.2025
 *      Author: a5153359
 */

#ifndef TEMPERATURE_READING_H_
#define TEMPERATURE_READING_H_

#include "r_cg_macrodriver.h"

#define SAVETY_CNT					4000000 // With 8 MHz clk -> 2s
#define DELAY_CNT					200000// delay with 8 MHz CLK
#define SENSOR_SLAVE_ADDRESS		0x44 // std address for HS3001
#define BYTES_CNT_I2C_MESSAGE		4U

typedef enum{
	TEMP_STATE_DATA_VALID,
	TEMP_STATE_DATA_INVALID,
	TEMP_CNT_OVERFLOW


}TempReading_State_t;

typedef struct {
	char timestamp [8];
	uint16_t temperature_raw;
	uint16_t relative_humidity_raw;
	float temperature_celsius;
	float relative_humidity;
	TempReading_State_t status;

} TemperatureMeasurement;

void start_measurement(TemperatureMeasurement *measurement);
void calc_humidity(TemperatureMeasurement *measurement);
void calc_temperature(TemperatureMeasurement *measurement);

#endif /* TEMPERATURE_READING_H_ */
