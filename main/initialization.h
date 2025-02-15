#ifndef _HRD__INITIALIZATION__H__
#define _HRD__INITIALIZATION__H__

#include <stdio.h>

#include <nvs_flash.h>

#include <esp_wifi.h>
#include <esp_mac.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_now.h>

#include "event_handlers.h"
#include "log_tags.h"

bool initialize_non_volatile_storage(void);
bool initialize_network_interface(esp_netif_t **network_interface);
bool initialize_event_loop(void);
bool initialize_wifi(void);

bool esp_now_initialize(void);
bool esp_now_initialize_receiver(void(*on_message_received)(const esp_now_recv_info_t *info, const uint8_t *data, int data_length));

void deinitialize_wifi(void);
void deinitialize_network_interface(void);

#endif