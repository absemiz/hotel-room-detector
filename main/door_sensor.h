#ifndef __HRD_DOOR_SENSOR_H__
#define __HRD_DOOR_SENSOR_H__

#include <driver/gpio.h>

#define DOOR_SENSOR_PIN GPIO_NUM_4

bool door_sensor_initialize(void);


#endif