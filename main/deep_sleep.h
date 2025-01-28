#ifndef __HRD_DEEP_SLEEP_H__
#define __HRD_DEEP_SLEEP_H__

#include <driver/rtc_io.h>
#include <esp_sleep.h>

#include "tcp_client.h"
#include "initialization.h"

#define ROOM_ID             0

#define PIR_SENSOR_ESP32

#ifdef PIR_SENSOR_ESP32
#include "pir_sensor.h"
#include "rf_receiver.h"
#endif

#ifdef DOOR_SENSOR_ESP32
#include "door_sensor.h"
#include "rf_transmitter.h"
#endif

bool deep_sleep_state_initialize(void);
bool deep_sleep_state_enter(void);

void deep_sleep_state_wakeup_task(void);
bool deep_sleep_state_is_wakeup_task_completed(void);
int deep_sleep_state_wakeup_count(void);


#endif