/*
 * CDH.c
 *
 *  Created on: Jan 28, 2022
 *      Author: leocelente
 */

#include "CDH.h"

/* Tasks Handles */
extern osThreadId_t CAN_TaskHandle;
extern osThreadId_t GPS_TaskHandle;
extern osThreadId_t RAS_Blob_TaskHandle;
extern osThreadId_t OBC_Blob_TaskHandle;
extern osThreadId_t Storage_TaskHandle;
extern osThreadId_t PktBuilder_TaskHandle;
extern osThreadId_t Radio_TaskHandle;
extern osThreadId_t Stimulus_TaskHandle;
extern osThreadId_t Executer_TaskHandle;

/* Queues Handles */
extern osMessageQueueId_t CAN_QueueHandle;
extern osMessageQueueId_t Radio_QueueHandle;
extern osMessageQueueId_t GPS_QueueHandle;
extern osMessageQueueId_t OBC_Fragment_QueueHandle;
extern osMessageQueueId_t RAS_Fragment_QueueHandle;
extern osMessageQueueId_t OBC_Blob_QueueHandle;
extern osMessageQueueId_t RAS_Blob_QueueHandle;

/* Timer Handles */
extern osTimerId_t OBC_TimerHandle;
extern osTimerId_t RAS_TimerHandle;
extern osTimerId_t Radio_TimerHandle;
extern osTimerId_t GPS_TimerHandle;

/* Mutexes Handles */
extern osMutexId_t UART_MutexHandle;
extern osMutexId_t Config_MutexHandle;

/* Global State */
static config_t config = { .values = { .radio_timer = TIMER_RADIO_INIT_VALUE, //
		.obc_timer = TIMER_OBC_INIT_VALUE, //
		.ras_timer = TIMER_RAS_INIT_VALUE, //
		.gps_timer = TIMER_GPS_INIT_VALUE, } };

/* Implementation Functions */

/* CAN GateKeeper Task */
void CAN_TaskFun(void *argument) {
	CAN_Event_t event = { 0 };
	int index = 0;
	fragment_t frag = { 0 };
	while (true) {
		osMessageQueueGet(CAN_QueueHandle, &event, 0, osWaitForever);
		switch (event.type) {
		case RX:
			debug("[CAN] Received message: %.2X - [ %.2X ...]", event.ID,
					event.data[0])
			switch (event.ID) {
			case RES_DATA_RAS_FRAG0 ... RES_DATA_RAS_FRAG6:
				index = event.ID - RES_DATA_RAS_FRAG0;
				frag.index = index;
				memcpy(frag.data, event.data, sizeof(frag.data));
				osMessageQueuePut(RAS_Fragment_QueueHandle, &frag, 0,
				TIMEOUT_MEDIUM);
				break;
			case RES_DATA_OBC_FRAG0 ... RES_DATA_OBC_FRAG3:
				index = event.ID - RES_DATA_OBC_FRAG0;
				frag.index = index;
				memcpy(frag.data, event.data, sizeof(frag.data));
				osMessageQueuePut(OBC_Fragment_QueueHandle, &frag, 0,
				TIMEOUT_MEDIUM);
				break;
			default:
				break;
			}

			break;
		case TX:
			debug("[CAN] Transmitting message: %.2X - [ %.2X ... ]", event.ID,
					event.data[0])
			if (event.ID == REQ_DATA_RAS) {
				osThreadFlagsSet(Stimulus_TaskHandle, 0x01);
			} else if (event.ID == REQ_DATA_OBC) {
				osThreadFlagsSet(Stimulus_TaskHandle, 0x02);
			}
			break;
		default:
			break;
		}
	}

}

/* Fetches GPS data */
void GPS_TaskFun(void *argument) {
	CAN_Event_t time_sync = { .type = TX, .ID = SYNC_TIME, .data = { 0 } };
	osTimerStart(GPS_TimerHandle, config.values.gps_timer);
	gps_event_t event = { 0 };
	event.day = 0x29;
	while (true) {
		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		memcpy(time_sync.data, &event, 8);
		debug("[GPS] Fetch GPS Data [%.2X]", event.day++)
		osMessageQueuePut(CAN_QueueHandle, &time_sync, 0, TIMEOUT_SHORT);
		osMessageQueuePut(GPS_QueueHandle, &event, 0, TIMEOUT_MEDIUM);
		osThreadFlagsSet(Storage_TaskHandle, 0x01 << 2);
	}
}

/* Constructs Blobs from RAS Fragments */
void RAS_Blob_TaskFun(void *argument) {
	osTimerStart(RAS_TimerHandle, config.values.ras_timer);
	fragment_t fragment = { 0 };
	static const size_t last_index = 6;
	static const size_t index_sum = 21; // 0 + 1 ... + 6
	int count = 0;
	while (true) {
		osMessageQueueGet(RAS_Fragment_QueueHandle, &fragment, 0,
		osWaitForever);
		uint8_t index = fragment.index;
		ras_blob_t blob;
		int size = (index == last_index) ? sizeof(blob) - (last_index * 8) : 8;
		memcpy(&blob.raw[index * 8], fragment.data, size);
		count += index;
		if (count != index_sum && index != last_index) {
			// if has not reach the end
			continue;
		} else if (index == last_index && count != index_sum) {
			// blob has ended, but parts were missing.
			debug("--[!RAS!] Failed to build a Blob,  %d %d", index, count)
			count = 0; // discard, start over
			continue;
		}
		count = 0;
		debug("[RAS] Pushing to Queue Front")
		xQueueSendToFront(RAS_Blob_QueueHandle, &blob, TIMEOUT_SHORT);
		osThreadFlagsSet(Storage_TaskHandle, 0x01);
	}
}

/* Constructs Blobs from OBC Fragments */
void OBC_Blob_TaskFun(void *argument) {
	osTimerStart(OBC_TimerHandle, config.values.gps_timer);
	fragment_t fragment = { 0 };
	static const size_t last_index = 3;
	static const size_t index_sum = 6; // 0 + 1 ... + 3
	int count = 0;
	while (true) {
		osMessageQueueGet(OBC_Fragment_QueueHandle, &fragment, 0,
		osWaitForever);
		uint8_t index = fragment.index;
		obc_blob_t blob;
		int size = (index == last_index) ? sizeof(blob) - (last_index * 8) : 8;
		memcpy(&blob[index * 8], fragment.data, size);
		count += index;
		if (count != index_sum && index != last_index) {
			// if has not reach the end
			continue;
		} else if (index == last_index && count != index_sum) {
			// blob has ended, but parts were missing.
			debug("--[!OBC!] Failed to build a Blob, %d %d", index, count)
			count = 0; // discard, start over
			continue;
		}
		count = 0;
		debug("[OBC] Pushing to Queue Front")
		xQueueSendToFront(OBC_Blob_QueueHandle, &blob, TIMEOUT_SHORT);
		osThreadFlagsSet(Storage_TaskHandle, 0x01 << 1);
	}
}

/* Controls Storage Devices */
void Storage_TaskFun(void *argument) {
	ras_blob_t ras = { .values = { 0 } };
	obc_blob_t obc = { 0 };
	gps_event_t gps = { 0 };
	while (true) {
		uint32_t flag = osThreadFlagsWait(0x0F, osFlagsWaitAny, osWaitForever);
		switch (flag) {
		case 0x1 << 0:
			xQueuePeek(RAS_Blob_QueueHandle, &ras, 10);
			debug("[Storage] Saving RAS data to Flash [%.2X...]", ras.raw[0])
			debug("[Storage] Saving RAS data to SDCard[%.2X...]", ras.raw[0])
			break;
		case 0x1 << 1:
			xQueuePeek(OBC_Blob_QueueHandle, &obc, 10);
			debug("[Storage] Saving OBC data to Flash [%.2X ...]", obc[0])
			debug("[Storage] Saving OBC data to SDCard[%.2X ...]", obc[0])
			break;
		case 0x1 << 2:
			xQueuePeek(GPS_QueueHandle, &gps, 10);
			debug("[Storage] Saving GPS data to Flash [%.2X...]", gps.day)
			debug("[Storage] Saving GPS data to SDCard[%.2X...]", gps.day)
			break;
		default:
			break;
		}

	}
}

static void build_reduced(radio_reduced_packet_t *restrict reduced_packet,
		ras_blob_t const *restrict ras, gps_event_t const *restrict gps) {
	reduced_packet->values.altitude = gps->altitude;
	reduced_packet->values.latitude = gps->latitude;
	reduced_packet->values.longitude = gps->longitude;
	reduced_packet->values.batt_charge = ras->values.batt_charge;
	reduced_packet->values.preassure_primary = ras->values.preassure_primary;
	reduced_packet->values.hours = gps->hour;
	reduced_packet->values.minutes = gps->minute;
	reduced_packet->values.seconds = gps->second;
}

static void build_full(radio_full_packet_t *restrict full_packet,
		ras_blob_t const *restrict ras, gps_event_t const *restrict gps) {
	full_packet->values.altitude = gps->altitude;
	full_packet->values.latitude = gps->latitude;
	full_packet->values.longitude = gps->longitude;
	full_packet->values.batt_charge = ras->values.batt_charge;
	full_packet->values.hours = gps->hour;
	full_packet->values.minutes = gps->minute;
	full_packet->values.seconds = gps->second;
	full_packet->values.i_1 = ras->values.i_1;
	full_packet->values.i_2 = ras->values.i_2;
	full_packet->values.i_3 = ras->values.i_3;
	full_packet->values.v_1 = ras->values.v_1;
	full_packet->values.v_2 = ras->values.v_2;
	full_packet->values.v_3 = ras->values.v_3;
	full_packet->values.temp_1 = ras->values.temp_1;
	full_packet->values.temp_2 = ras->values.temp_2;
	full_packet->values.temp_3 = ras->values.temp_3;
}

/* Packet Builder Task */
void PktBuilder_TaskFun(void *argument) {
	uint8_t counter = 0;

	radio_full_packet_t full_packet = { .values = { 0 } };
	radio_reduced_packet_t reduced_packet = { .values = { 0 } };
	radio_obsat_packet_t obsat_packet = { .values = { 0 } };

	radio_event_t packet = { .type = TX, .data = { 0 } };

	gps_event_t gps = { 0 };
	obc_blob_t obc = { 0 };
	ras_blob_t ras = { .values = { 0 } };

	while (true) {
		osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
		int packet_type = counter % sizeof(radio_packet_types_t);
		switch (packet_type) {
		case ZENITH_FULL:
			debug("[Packet] Building a FullPacket")
			osMessageQueueGet(RAS_Blob_QueueHandle, &ras, 0, TIMEOUT_MEDIUM);
			osMessageQueueGet(GPS_QueueHandle, &gps, 0, TIMEOUT_MEDIUM);
			build_full(&full_packet, &ras, &gps);
			full_packet.values.packet_id = counter;
			packet.size = sizeof(full_packet);
			memcpy(packet.data, full_packet.raw, packet.size);
			osMessageQueuePut(Radio_QueueHandle, &packet, 0, TIMEOUT_MEDIUM);
			break;
		case ZENITH_REDUCED:
			debug("[Packet] Building a Reduced Packet")
			osMessageQueueGet(RAS_Blob_QueueHandle, &ras, 0, TIMEOUT_MEDIUM);
			osMessageQueueGet(GPS_QueueHandle, &gps, 0, TIMEOUT_MEDIUM);
			build_reduced(&reduced_packet, &ras, &gps);
			reduced_packet.values.packet_id = counter;
			packet.size = sizeof(reduced_packet);
			memcpy(packet.data, reduced_packet.raw, packet.size);
			osMessageQueuePut(Radio_QueueHandle, &packet, 0, TIMEOUT_MEDIUM);
			break;
		case OBSAT:
			debug("[Packet] Building a OBSAT Packet")
			osMessageQueueGet(OBC_Blob_QueueHandle, &obc, 0, TIMEOUT_MEDIUM);
			obsat_packet.values.id = counter;
			memcpy(obsat_packet.raw, obc, sizeof(obsat_packet));
			packet.size = sizeof(obsat_packet);
			memcpy(packet.data, obsat_packet.raw, packet.size);
			osMessageQueuePut(Radio_QueueHandle, &packet, 0, TIMEOUT_MEDIUM);
			break;
		default:
			break;
		}
		counter++;
	}
}

/* Radio GateKeeper Task */
void Radio_TaskFun(void *argument) {
	osTimerStart(Radio_TimerHandle, config.values.radio_timer);
	radio_event_t event = { 0 };
	while (true) {
		osMessageQueueGet(Radio_QueueHandle, &event, 0, osWaitForever);
		switch (event.type) {
		case RX:
			debug("[Radio] Received a packet of size: %d [%.2X]", event.size,
					event.data[0])
			switch (event.data[0]) {
			case CMD_CONFIG:
				if (osMutexAcquire(Config_MutexHandle, 1)) {
					memcpy(config.raw, &event.data[1], sizeof(config.raw));
					osMutexRelease(Config_MutexHandle);
				}
				break;
			default:
				break;
			}
			break;
		case TX:
			debug("[Radio] Transmitting a packet of size: %d [%.2X]",
					event.size, event.data[0])
			break;
		default:
			break;
		}
	}
}

/* Stimulus Task: Simulates OBC and RAS */
void Stimulus_TaskFun(void *argument) {
	while (true) {
		uint32_t flag = osThreadFlagsWait(0x03, osFlagsWaitAny, osWaitForever);
		static const uint16_t ras_ids[] = { RES_DATA_RAS_FRAG0, RES_DATA_RAS_FRAG1,
				RES_DATA_RAS_FRAG2, RES_DATA_RAS_FRAG3, RES_DATA_RAS_FRAG4,
				RES_DATA_RAS_FRAG5, RES_DATA_RAS_FRAG6 };
		static const uint16_t obc_ids[] = { RES_DATA_OBC_FRAG0, RES_DATA_OBC_FRAG1,
				RES_DATA_OBC_FRAG2, RES_DATA_OBC_FRAG3 };
		switch (flag) {
		case 1:
			osDelay(1);
			for (int i = 0; i < sizeof(ras_ids) / 2; ++i) {
				CAN_Event_t can_evt = { .ID = ras_ids[i], .type = RX, .data = {
						ras_ids[i], [1 ... 7]=0xAA } };
				osMessageQueuePut(CAN_QueueHandle, &can_evt, 0, TIMEOUT_MEDIUM);
				osDelay(1);
			}
			break;
		case 2:
			osDelay(2);
			for (int i = 0; i < sizeof(obc_ids) / 2; ++i) {
				CAN_Event_t can_evt = { .ID = obc_ids[i], .type = RX, .data = {
						obc_ids[i], [1 ... 7]=0xBB } };
				osMessageQueuePut(CAN_QueueHandle, &can_evt, 0, TIMEOUT_MEDIUM);
				osDelay(1);
			}
			break;
		default:
			break;
		}
	}
}

/* Called on OBC Timer Callback */
void OBC_TimerFun(void *argument) {
	CAN_Event_t request = { .type = TX, .ID = REQ_DATA_OBC, .data = { 0 } };
	osMessageQueuePut(CAN_QueueHandle, &request, 0, 0);
	if (osMutexAcquire(Config_MutexHandle, 0)) {
		xTimerChangePeriod(OBC_TimerHandle, config.values.obc_timer, 0);
		osMutexRelease(Config_MutexHandle);
	}
}

/* Called on RAS Timer Callback */
void RAS_TimerFun(void *argument) {
	CAN_Event_t request = { .type = TX, .ID = REQ_DATA_RAS, .data = { 0 } };
	osMessageQueuePut(CAN_QueueHandle, &request, 0, 0);
	if (osMutexAcquire(Config_MutexHandle, 0)) {
		xTimerChangePeriod(OBC_TimerHandle, config.values.ras_timer, 0);
		osMutexRelease(Config_MutexHandle);
	}
}

/* Called on Radio Timer Callback */
void Radio_TimerFun(void *argument) {
	osThreadFlagsSet(PktBuilder_TaskHandle, 0x01);
	if (osMutexAcquire(Config_MutexHandle, 0)) {
		xTimerChangePeriod(OBC_TimerHandle, config.values.radio_timer, 0);
		osMutexRelease(Config_MutexHandle);
	}
}

/* Called on GPS Timer Callback */
void GPS_TimerFun(void *argument) {
	osThreadFlagsSet(GPS_TaskHandle, 0x01);
	if (osMutexAcquire(Config_MutexHandle, 0)) {
		xTimerChangePeriod(OBC_TimerHandle, config.values.gps_timer, 0);
		osMutexRelease(Config_MutexHandle);
	}

}

