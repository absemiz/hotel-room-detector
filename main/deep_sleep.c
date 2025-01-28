#include "deep_sleep.h"

RTC_DATA_ATTR static int _wakeup_count = 0;

static volatile bool _wakeup_task_completed = false;

static void _on_wifi_got_ip(const ip_event_got_ip_t* got_ip_data);

bool deep_sleep_state_initialize(void)
{
    esp_err_t error;

    gpio_num_t wakeup_pin;
    int wakeup_level;

    #ifdef PIR_SENSOR_ESP32
        wakeup_pin = RF_RECEIVER_PIN;
        wakeup_level = 1;
    #endif

    #ifdef DOOR_SENSOR_ESP32
        wakeup_pin = DOOR_SENSOR_PIN;
        wakeup_level = 1;
    #endif

    if ((error = esp_sleep_enable_ext0_wakeup(wakeup_pin, wakeup_level)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_sleep_enable_ext0_wakeup: (%s)", esp_err_to_name(error));
        return false;
    }

    rtc_gpio_pulldown_en(wakeup_pin);
    rtc_gpio_pullup_dis(wakeup_pin);

    return true;
}

bool deep_sleep_state_enter(void)
{
    ESP_LOGI(INFO_TAG, "Entering deep sleep state.");

    esp_deep_sleep_start();
}

void deep_sleep_state_wakeup_task(void)
{
    ESP_LOGI(INFO_TAG, "Waking up from deep sleep state.");

    _wakeup_count++;
    _wakeup_task_completed = false;

    #ifdef PIR_SENSOR_ESP32

        esp_netif_t *network_interface;

        event_handler_set_on_wifi_got_ip(_on_wifi_got_ip);

        initialize_non_volatile_storage();
        initialize_event_loop();
        initialize_network_interface(&network_interface);
        initialize_wifi();

    #endif

    #ifdef DOOR_SENSOR_ESP32

        rf_transmitter_initialize();
        
        rf_transmitter_start_stream();

        vTaskDelay(pdMS_TO_TICKS(RF_TRANSMITTER_STREAM_PERIOD_IN_MS));

        rf_transmitter_stop_stream();

        _wakeup_task_completed = true;

    #endif
}

bool deep_sleep_state_is_wakeup_task_completed(void)
{
    return _wakeup_task_completed;
}

int deep_sleep_state_wakeup_count(void)
{
    return _wakeup_count;
}

static void _on_wifi_got_ip(const ip_event_got_ip_t* got_ip_data)
{
    #ifdef PIR_SENSOR_ESP32

        int message_size;
        char* message_buffer = (char*)tcp_client_get_buffer();

        ESP_LOGI(INFO_TAG, "Initializing TCP client.");
        tcp_client_initialize();
        ESP_LOGI(INFO_TAG, "Connecting TCP client.");
        tcp_client_connect();

        pir_sensor_initialize();
        pir_sensor_task(NULL);
            
        message_size = snprintf(message_buffer, TCP_CLIENT_BUFFER_SIZE, "{\"sensor\":\"pir\",\"room\":\"%d\",\"low\":\"%d\",\"high\":\"%d\"}", ROOM_ID, pir_sensor_low_count(), pir_sensor_high_count()) + 1;

        ESP_LOGI(INFO_TAG, "Sending message.");
        
        tcp_client_send_message(message_buffer, message_size);
        tcp_client_close();
        
        _wakeup_task_completed = true;

    #endif

    #ifdef DOOR_SENSOR_ESP32

    #endif
}