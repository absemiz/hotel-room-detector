#include "deep_sleep.h"

RTC_DATA_ATTR static int _wakeup_count = 0;

static volatile bool _wakeup_task_completed = false;

static void _on_wifi_got_ip(const ip_event_got_ip_t* got_ip_data);

bool deep_sleep_state_initialize(void)
{
    esp_err_t error;

    gpio_num_t wakeup_pin;
    int wakeup_level;

    wakeup_pin = DOOR_SENSOR_PIN;
    wakeup_level = 1;

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

    esp_netif_t *network_interface;

    event_handler_set_on_wifi_got_ip(_on_wifi_got_ip);

    initialize_non_volatile_storage();
    initialize_event_loop();
    initialize_network_interface(&network_interface);
    initialize_wifi();

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
    char message[128];

    pir_sensor_initialize();
    pir_sensor_task(NULL);

    if (pir_sensor_high_count() > 0)
    {
        snprintf(message, sizeof(message), "{\"sensor\":\"pir\",\"room\":\"%d\",\"low\":\"%d\",\"high\":\"%d\"}", 
            ROOM_ID, 
            pir_sensor_low_count(), 
            pir_sensor_high_count()
        );

        ESP_LOGI("ESP-NOW", "Sending message: %s", message);

        esp_err_t result = esp_now_send(target_esp_mac_address(), (uint8_t *)message, strlen(message));

        if (result == ESP_OK) 
        {
            ESP_LOGI("ESP-NOW", "Message sent successfully.");
        } 
        else 
        {
            ESP_LOGE("ESP-NOW", "Failed to send message, error: %d", result);
        }
    }
    
    _wakeup_task_completed = true;
}