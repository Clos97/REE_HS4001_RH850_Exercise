/*
 * temperature_reading.h
 *
 *  Created on: 15.04.2025
 *      Author: a5153359
 */

#ifndef TEMPERATURE_READING_H_
#define TEMPERATURE_READING_H_

typedef struct {

	char timestamp [8];
	float temperature_celsius;
	float relative_humidity;
} TemperatureMeasurement;

#endif /* TEMPERATURE_READING_H_ */
