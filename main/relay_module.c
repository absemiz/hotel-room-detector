#include "relay_module.h"

static const char *TAG = "RelayModule";
static bool _relay_state = false;

bool relay_module_initialize(void) 
{
    esp_err_t error;

    gpio_config_t configuration = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pin_bit_mask = (1ULL << RELAY_PIN)
    };

    if ((error = gpio_config(&configuration)) != ESP_OK) 
    {
        ESP_LOGE(TAG, "gpio_config failed: (%s)", esp_err_to_name(error));
        return false;
    }

    gpio_set_level(RELAY_PIN, RELAY_OFF);
    _relay_state = false;

    ESP_LOGI(TAG, "Relay Module Initialized.");
    return true;
}

void relay_module_turn_on(void) 
{
    gpio_set_level(RELAY_PIN, RELAY_ON);
    _relay_state = true;
    ESP_LOGI(TAG, "Relay turned ON.");
}

void relay_module_turn_off(void) 
{
    gpio_set_level(RELAY_PIN, RELAY_OFF);
    _relay_state = false;
    ESP_LOGI(TAG, "Relay turned OFF.");
}

void relay_module_toggle(void) 
{
    if (_relay_state)
        relay_module_turn_off();
    else
        relay_module_turn_on();
}

bool relay_module_get_state(void) 
{
    return _relay_state;
}
