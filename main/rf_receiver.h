#ifndef _HRD__RF_RECEVICER__H__
#define _HRD__RF_RECEIVER__H__

#include <driver/gpio.h>

#include "log_tags.h"

#define RF_RECEIVER_PIN                          GPIO_NUM_32

bool rf_receiver_initialize(void);

#endif