#ifndef __HRD_DEEP_SLEEP_H__
#define __HRD_DEEP_SLEEP_H__

#include <driver/rtc_io.h>
#include <esp_sleep.h>

#include "tcp_client.h"
#include "initialization.h"
#include "lookups.h"
#include "pir_sensor.h"
#include "door_sensor.h"

#define ROOM_ID             0

bool deep_sleep_state_initialize(void);
bool deep_sleep_state_enter(void);

void deep_sleep_state_wakeup_task(void);
bool deep_sleep_state_is_wakeup_task_completed(void);
int deep_sleep_state_wakeup_count(void);

#endif