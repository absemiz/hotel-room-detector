#include "initialization.h"

#define TARGET_WIFI_CONFIG() {                                                                  \
    .sta = {                                                                                    \
        .ssid = "ANDROMEDA",                                                                    \
        .password = "elmacik.ph796",                                                            \
        .threshold.authmode = WIFI_AUTH_WPA2_PSK                                                \
    },                                                                                          \
}

bool initialize_non_volatile_storage(void)
{
    esp_err_t error;

    if ((error = nvs_flash_init()) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "nvs_flash_init: (%s)", esp_err_to_name(error));

        return false;
    }

    return true;
}

bool initialize_network_interface(esp_netif_t **network_interface)
{
    esp_err_t error;

    if ((error = esp_netif_init()) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_netif_init: (%s)", esp_err_to_name(error));

        return false;
    }

    if ((*network_interface = esp_netif_create_default_wifi_sta()) == NULL)
    {
        ESP_LOGE(ERROR_TAG, "esp_netif_create_default_wifi_sta: returned null pointer.");

        return false;
    }

    return true;
}

bool initialize_event_loop(void)
{
    esp_err_t error;

    if ((error = esp_event_loop_create_default()) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "initialize_event_loop: (%s)", esp_err_to_name(error));

        return false;
    }

    return true;
}

bool initialize_wifi(void)
{
    esp_err_t error;

    wifi_init_config_t wifi_init_config_default = WIFI_INIT_CONFIG_DEFAULT();

    if ((error = esp_wifi_init(&wifi_init_config_default)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_wifi_init: (%s)", esp_err_to_name(error));

        deinitialize_wifi();

        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_wifi_init success.");

    if ((error = esp_wifi_set_mode(WIFI_MODE_STA)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_wifi_set_mode: (%s)", esp_err_to_name(error));
            
        deinitialize_wifi();

        return false;
    }

    if ((error = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL)) != ESP_OK || (error = esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_event_handler_instance_register: (%s)", esp_err_to_name(error));
            
        deinitialize_wifi();

        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_wifi_set_mode success.");

    wifi_config_t target_config = TARGET_WIFI_CONFIG();

    if ((error = esp_wifi_set_config(ESP_IF_WIFI_STA, &target_config)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_wifi_set_config: (%s)", esp_err_to_name(error));

        deinitialize_wifi();

        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_wifi_set_config success.");

    if ((error = esp_wifi_start()) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_wifi_start: (%s)", esp_err_to_name(error));

        deinitialize_wifi();

        return false;
    }

    ESP_LOGI(INFO_TAG, "WiFI initialization completed successfully.");

    esp_wifi_connect();

    return true;
}

void deinitialize_wifi(void)
{
    esp_wifi_deinit();
}

void deinitialize_network_interface(void)
{
    esp_netif_deinit();
}
