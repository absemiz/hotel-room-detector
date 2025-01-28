#include "door_sensor.h"

bool door_sensor_initialize(void)
{
    esp_err_t error;

    gpio_config_t configuration = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pin_bit_mask = (1ULL << DOOR_SENSOR_PIN)
    };

    if ((error = gpio_config(&configuration)) != ESP_OK) 
    {
        ESP_LOGE(DOOR_SENSOR_PIN, "gpio_config: (%s)", esp_err_to_name(error));

        return false;
    }

    if ((error = gpio_set_level(DOOR_SENSOR_PIN, 0)) != ESP_OK) 
    {
        ESP_LOGE(DOOR_SENSOR_PIN, "Failed to set initial GPIO level (error: %s)", esp_err_to_name(error));
        return false;
    }

    ESP_LOGI(DOOR_SENSOR_PIN, "Door sensor initialized successfully.");

    return true;
}

