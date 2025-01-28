#ifndef _HRD__PIR_SENSOR__H__
#define _HRD__PIR_SENSOR__H__


#include <string.h>

#include <driver/gpio.h>

#include <esp_log.h>

#include <freertos/FreeRTOS.h>

#include "log_tags.h"
#include "tcp_client.h"

#define PIR_SENSOR_PIN                                              GPIO_NUM_4

#define PIR_SENSOR_SENSOR_HIGH_DURATION_IN_MS                       2000
#define PIR_SENSOR_POLL_PERIOD_IN_MS                                1000

#define PIR_SENSOR_TASK_FUNCTION                                    pir_sensor_task
#define PIR_SENSOR_TASK_NAME                                        "PIR Sensor Task"
#define PIR_SENSOR_TASK_STACK_SIZE                                  4096
#define PIR_SENSOR_TASK_PARAMETERS                                  NULL
#define PIR_SENSOR_TASK_PRIORITY                                    1
#define PIR_SENSOR_TASK_HANDLE                                      NULL
#define PIR_SENSOR_TASK_ARGUMENTS                                   PIR_SENSOR_TASK_FUNCTION, PIR_SENSOR_TASK_NAME, PIR_SENSOR_TASK_STACK_SIZE, PIR_SENSOR_TASK_PARAMETERS, PIR_SENSOR_TASK_PRIORITY, PIR_SENSOR_TASK_HANDLE


typedef 
struct pir_sensor_t
{
    uint64_t                    readings;
    uint8_t                     readings_size;
    uint8_t                     readings_capacity;
    uint8_t                     low_count;
    uint8_t                     high_count;
    uint32_t                    padding;
} 
pir_sensor_t;

bool pir_sensor_initialize(void);
void pir_sensor_task(void *arguments);

uint8_t pir_sensor_low_count(void);
uint8_t pir_sensor_high_count(void);
uint32_t pir_sensor_scan_duration_in_ms(void);

#endif