#include "rf_transmitter.h"

bool rf_transmitter_initialize(void)
{
    esp_err_t error;

    gpio_config_t configuration = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pin_bit_mask = (1ULL << RF_TRANSMITTER_PIN)
    };

    if ((error = gpio_config(&configuration)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "gpio_config: (%s)", esp_err_to_name(error));

        return false;
    }

    if ((error = gpio_set_level(RF_TRANSMITTER_PIN, 0)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "Failed to set initial GPIO level (error: %s)", esp_err_to_name(error));
        return false;
    }

    ESP_LOGI(INFO_TAG, "RF transmitter initialized successfully.");

    return true;
}

void rf_transmitter_start_stream(void)
{
    esp_err_t error;

    if ((error = gpio_set_level(RF_TRANSMITTER_PIN, 1)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "Failed to start stream (error: %s)", esp_err_to_name(error));
    }
    else 
    {
        ESP_LOGI(INFO_TAG, "RF transmitter stream started.");
    }
}

void rf_transmitter_stop_stream(void)
{
    esp_err_t error;

    if ((error = gpio_set_level(RF_TRANSMITTER_PIN, 0)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "Failed to stop stream (error: %s)", esp_err_to_name(error));
    }
    else 
    {
        ESP_LOGI(INFO_TAG, "RF transmitter stream stopped.");
    } 
}