#ifndef _HRD__RELAY_MODULE__H__
#define _HRD__RELAY_MODULE__H__

#include <driver/gpio.h>
#include <esp_log.h>

#define RELAY_PIN  GPIO_NUM_5
#define RELAY_ON   1
#define RELAY_OFF  0

bool relay_module_initialize(void);
void relay_module_turn_on(void);
void relay_module_turn_off(void);
void relay_module_toggle(void);
bool relay_module_get_state(void);

#endif
