/*
 * CDH.h
 *
 *  Created on: Jan 28, 2022
 *      Author: leocelente
 */

#ifndef INC_CDH_H_
#define INC_CDH_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "zenith.h"
#include "timers.h"

#define TIMER_OBC_INIT_VALUE 10000
#define TIMER_RAS_INIT_VALUE 3000
#define TIMER_GPS_INIT_VALUE 1000
#define TIMER_RADIO_INIT_VALUE 4000

#define TIMEOUT_SHORT 10
#define TIMEOUT_MEDIUM 100

/* Cria fragment como um uint8_t[8] */
typedef uint8_t obc_blob_t[30];
typedef struct {
	uint8_t index;
	uint8_t data[8];
} fragment_t;

/* Dados do Grupo1 */
typedef struct {
	float preassure_primary;
	float temp_1, temp_2, temp_3;
	float v_1, v_2, v_3;
	float i_1, i_2, i_3;
	float humidity, temp_amb;
	float batt_charge;
} ras_blob_values_t;

typedef union {
	ras_blob_values_t values;
	uint8_t raw[sizeof(ras_blob_values_t)];
} ras_blob_t;

typedef enum {
	RX, TX
} event_type_t;

typedef struct {
	event_type_t type;
	int size;
	uint8_t data[64];
} radio_event_t;

typedef struct {
	uint8_t day, month, year;
	uint8_t second, minute, hour;
	float latitude, longitude, altitude;
//	float speed_x, speed_y, speed_z;
} gps_event_t;

typedef enum {
	ZENITH_FULL = 0,
	ZENITH_REDUCED = 1,
	OBSAT = 2
} radio_packet_types_t;

typedef struct {
	float latitude, longitude, altitude;
	float preassure_primary;
	float temp_1, temp_2, temp_3;
	float v_1, v_2, v_3;
	float i_1, i_2, i_3;
	float batt_charge;
	uint8_t seconds, minutes, hours;
	uint8_t packet_id;
} radio_full_packet_values_t;

typedef union {
	radio_full_packet_values_t values;
	uint8_t raw[sizeof(radio_full_packet_values_t)];
} radio_full_packet_t;

typedef struct {
	float latitude, longitude, altitude;
	float preassure_primary;
	float batt_charge;
	uint8_t seconds, minutes, hours;
	uint8_t packet_id;
} radio_reduced_packet_values_t;

typedef union {
	radio_reduced_packet_values_t values;
	uint8_t raw[sizeof(radio_reduced_packet_values_t)];
} radio_reduced_packet_t;

typedef struct {
	uint8_t id;
	obc_blob_t data;
} radio_obsat_packet_values_t;

typedef union {
	radio_obsat_packet_values_t values;
	uint8_t raw[sizeof(radio_obsat_packet_values_t)];
} radio_obsat_packet_t;

/* CAN */

typedef enum {
	REQ_DATA_RAS = 0x0C0,
	RES_DATA_RAS_FRAG0 = 0x0C1,
	RES_DATA_RAS_FRAG1 = 0x0C2,
	RES_DATA_RAS_FRAG2 = 0x0C3,
	RES_DATA_RAS_FRAG3 = 0x0C4,
	RES_DATA_RAS_FRAG4 = 0x0C5,
	RES_DATA_RAS_FRAG5 = 0x0C6,
	RES_DATA_RAS_FRAG6 = 0x0C7,
	REQ_DATA_OBC = 0x0D0,
	RES_DATA_OBC_FRAG0 = 0x0D1,
	RES_DATA_OBC_FRAG1 = 0x0D2,
	RES_DATA_OBC_FRAG2 = 0x0D3,
	RES_DATA_OBC_FRAG3 = 0x0D4,
	// TODO: que porra é essa
	REQ_SET_CFG_OBC = 0x0F0,
	RES_SET_CFG_RAS = 0x0F1,
	REQ_GET_CFG_OBC = 0x0F2,
	RES_GET_CFG_RAS = 0x0F3,
	SYNC_TIME = 0x0F4,
	REQ_HEARTBEAT = 0x0F5,
	RES_HEARTBEAT_RAS = 0x0F6,
	RES_HEARTBEAT_OBC = 0x0F7
} CAN_Messages_Ids_t;

typedef struct {
	event_type_t type;
	CAN_Messages_Ids_t ID;
	uint8_t data[8];
} CAN_Event_t;

typedef enum {
	CMD_CONFIG
} radio_commads_t;

typedef struct {
	uint16_t radio_timer;
	uint16_t obc_timer;
	uint16_t ras_timer;
	uint16_t gps_timer;
} config_values_t;

typedef union {
	config_values_t values;
	uint8_t raw[8];
} config_t;

#endif /* INC_CDH_H_ */
