#include "event_handlers.h"

#define STATIC_CAST(Type, Variable) ((Type)Variable)

static event_handlers_t _event_handlers = {
    .on_wifi_start = NULL,
    .on_wifi_connect = NULL,
    .on_wifi_got_ip = NULL,
    .on_wifi_disconnect = NULL
};

void event_handler_set_on_wifi_start(void (*on_wifi_start)(const wifi_event_t* wifi_event_data))
{
    _event_handlers.on_wifi_start = on_wifi_start;
}

void event_handler_set_on_wifi_connect(void (*on_wifi_connect)(const wifi_event_sta_connected_t* wifi_connected_data))
{
    _event_handlers.on_wifi_connect = on_wifi_connect;
}

void event_handler_set_on_wifi_disconnect(void (*on_wifi_disconnect)(const wifi_event_sta_disconnected_t* wifi_disconnected_data))
{
    _event_handlers.on_wifi_disconnect = on_wifi_disconnect;
}

void event_handler_set_on_wifi_got_ip(void (*on_wifi_got_ip)(const ip_event_got_ip_t* got_ip_data))
{
    _event_handlers.on_wifi_got_ip = on_wifi_got_ip;
}

void event_handler(void *arguments, esp_event_base_t event_base, int32_t event_type, void *event_data)
{
    if (event_base == WIFI_EVENT && event_type == WIFI_EVENT_STA_START && _event_handlers.on_wifi_start != NULL)
    {
        wifi_event_t *wifi_event_data = STATIC_CAST(wifi_event_t*, event_data);

        _event_handlers.on_wifi_start(wifi_event_data);
    }

    if (event_base == WIFI_EVENT && event_type == WIFI_EVENT_STA_CONNECTED && _event_handlers.on_wifi_connect != NULL)
    {
        wifi_event_sta_connected_t* wifi_connected_data = STATIC_CAST(wifi_event_sta_connected_t*, event_data);

        _event_handlers.on_wifi_connect(wifi_connected_data);
    }

    if (event_base == WIFI_EVENT && event_type == WIFI_EVENT_STA_DISCONNECTED && _event_handlers.on_wifi_disconnect != NULL)
    {
        wifi_event_sta_disconnected_t* wifi_disconnected_data = STATIC_CAST(wifi_event_sta_disconnected_t*, event_data);

        _event_handlers.on_wifi_disconnect(wifi_disconnected_data);
    }

    if (event_base == IP_EVENT && event_type == IP_EVENT_STA_GOT_IP && _event_handlers.on_wifi_got_ip != NULL)
    {
        ip_event_got_ip_t* got_ip_data = STATIC_CAST(ip_event_got_ip_t*, event_data);

        _event_handlers.on_wifi_got_ip(got_ip_data);
    }
}