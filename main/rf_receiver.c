#include "rf_receiver.h"

bool rf_receiver_initialize(void)
{
    esp_err_t error;

    gpio_config_t configuration = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pin_bit_mask = (1ULL << RF_RECEIVER_PIN)
    };

    if ((error = gpio_config(&configuration)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "gpio_config: (%s)", esp_err_to_name(error));

        return false;
    }

    if ((error = gpio_set_level(RF_RECEIVER_PIN, 0)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "Failed to set initial GPIO level (error: %s)", esp_err_to_name(error));
        return false;
    }

    ESP_LOGI(INFO_TAG, "RF receiver initialized successfully.");

    return true;
}