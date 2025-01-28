#ifndef _HRD__EVENT_HANDLERS__H__
#define _HRD__EVENT_HANDLERS__H__

#include <stdio.h>

#include <nvs_flash.h>

#include <esp_wifi.h>
#include <esp_mac.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_netif.h>

#include <freertos/FreeRTOS.h>

#include "pir_sensor.h"
#include "tcp_client.h"

typedef struct event_handlers_t {
    void (*on_wifi_start)(const wifi_event_t* wifi_event_data);
    void (*on_wifi_connect)(const wifi_event_sta_connected_t* wifi_connected_data);
    void (*on_wifi_disconnect)(const wifi_event_sta_disconnected_t* wifi_disconnected_data);
    void (*on_wifi_got_ip)(const ip_event_got_ip_t* got_ip_data);
} event_handlers_t;

void event_handler(void *arguments, esp_event_base_t event_base, int32_t event_type, void *event_data);

void event_handler_set_on_wifi_start(void (*on_wifi_start)(const wifi_event_t* wifi_event_data));
void event_handler_set_on_wifi_connect(void (*on_wifi_connect)(const wifi_event_sta_connected_t* wifi_connected_data));
void event_handler_set_on_wifi_disconnect(void (*on_wifi_disconnect)(const wifi_event_sta_disconnected_t* wifi_disconnected_data));
void event_handler_set_on_wifi_got_ip(void (*on_wifi_got_ip)(const ip_event_got_ip_t* got_ip_data));

#endif