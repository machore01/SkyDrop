/*
 * fc.h
 *
 *  Created on: 23.1.2015
 *      Author: horinek
 */

#ifndef FC_H_
#define FC_H_

#include "../common.h"

#define NUMBER_OF_ALTIMETERS	3
#define AUDIO_PROFILE_SIZE		41

#include "conf.h"

#define ALT_ABS_QNH1	(0 << 4)
#define ALT_ABS_QNH2	(1 << 4)
//#define ALT_ABS_GPS		(2 << 4)
//#define ALT_DIFF_GPS		(3 << 4)
#define ALT_DIFF		(4 << 4)

typedef struct
{
	uint8_t flags;
	int16_t delta;
	int16_t altitude;
} alt_data_t;

typedef struct
{

} gps_data_t;

extern gps_data_t gps_data;

typedef struct
{
	// --- RAW from sensors ---
	vector_i16_t mag_data;
	vector_i16_t acc_data;
	vector_i16_t gyro_data;

	bool gps_data_new;
	gps_data_t gps_data;

	// --- CONST from cfg ---
	float QNH1;
	float QNH2;

	float digital_vario_dampening;
	float avg_vario_dampening;

	vector_i16_t mag_bias;
	vector_i16_t mag_sensitivity;

	vector_i16_t acc_bias;
	vector_i16_t acc_sensitivity;

	uint16_t buzzer_freq[AUDIO_PROFILE_SIZE];
	uint16_t buzzer_pause[AUDIO_PROFILE_SIZE];
	uint16_t buzzer_length[AUDIO_PROFILE_SIZE];

	int16_t audio_lift;
	int16_t audio_sink;
	uint8_t audio_fluid;
	uint8_t audio_volume;
	uint8_t audio_supress;

	uint8_t usb_mode;

	uint8_t autostart_sensitivity;

	// --- CALC ---
	bool baro_valid;
	float pressure;
	float vario;
	float digital_vario;
	float avg_vario;

	float altitude1;
	alt_data_t altimeter[NUMBER_OF_ALTIMETERS];

	vector_float_t mag_f;
	vector_float_t acc_f;
	vector_float_t gyro_f;

	uint32_t epoch_flight_start;
	float start_altitude;
	bool in_flight;
} flight_data_t;

void fc_init();
void fc_step();
void fc_deinit();

void fc_pause();
void fc_continue();

float fc_alt_to_qnh(float alt, float pressure);
float fc_press_to_alt(float pressure, float qnh);
float fc_alt_to_press(float alt, float qnh);

extern volatile flight_data_t fc;

#endif /* FC_H_ */