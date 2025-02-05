#ifndef _HRD__RF_TRANSMITTER__H__
#define _HRD__RF_TRANSMITTER__H__

#include <driver/gpio.h>
#include <esp_log.h>

#include "log_tags.h"

#define RF_TRANSMITTER_PIN                          GPIO_NUM_2
#define RF_TRANSMITTER_STREAM_PERIOD_IN_MS          30000

bool rf_transmitter_initialize(void);
void rf_transmitter_start_stream(void);
void rf_transmitter_stop_stream(void);

#endif

