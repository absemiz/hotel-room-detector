#include "initialization.h"

#define TARGET_WIFI_SSID "<target wifi ssid>"
#define TARGET_WIFI_PASSWORD "<target wifi password>"

#define TARGET_WIFI_CONFIG() {                                                                  \
    .sta = {                                                                                    \
        .ssid = TARGET_WIFI_SSID,                                                               \
        .password = TARGET_WIFI_PASSWORD,                                                       \
        .threshold.authmode = WIFI_AUTH_WPA2_PSK                                                \
    },                                                                                          \
}

static uint8_t _esp_now_peer_address[6] = {0x24, 0x6F, 0x28, 0x00, 0x00, 0x00};

static void _on_esp_now_sent(const uint8_t *mac_address, esp_now_send_status_t status);

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

bool esp_now_initialize(void)
{
    esp_err_t error;

    if (!initialize_network_interface(NULL) || !initialize_event_loop() || !initialize_wifi())
    {
        ESP_LOGE(ERROR_TAG, "Failed to initialize necessary network components.");
        return false;
    }

    if ((error = esp_now_init()) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_now_init: (%s)", esp_err_to_name(error));

        deinitialize_wifi();
        deinitialize_network_interface();

        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_now_init success.");

    if ((error = esp_now_register_send_cb(_on_esp_now_sent)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_now_register_send_cb: (%s)", esp_err_to_name(error));

        esp_now_deinit();
        deinitialize_wifi();
        deinitialize_network_interface();

        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_now_register_send_cb success.");

    esp_now_peer_info_t peer_info = {};
    memset(&peer_info, 0, sizeof(peer_info));
    memcpy(peer_info.peer_addr, _esp_now_peer_address, 6);
    peer_info.channel = 0;
    peer_info.ifidx = ESP_IF_WIFI_STA;
    peer_info.encrypt = false;

    if ((error = esp_now_add_peer(&peer_info)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_now_add_peer: (%s)", esp_err_to_name(error));

        esp_now_deinit();
        deinitialize_wifi();
        deinitialize_network_interface();

        return false;
    }

    ESP_LOGI(INFO_TAG, "ESP-NOW peer added successfully.");

    return true;
}

bool esp_now_initialize_receiver(void(*on_message_received)(const esp_now_recv_info_t *info, const uint8_t *data, int data_length))
{
    esp_err_t error;

    if (!initialize_network_interface(NULL) || !initialize_event_loop() || !initialize_wifi())
    {
        ESP_LOGE(ERROR_TAG, "Failed to initialize necessary network components.");
        return false;
    }

    if ((error = esp_now_init()) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_now_init: (%s)", esp_err_to_name(error));
        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_now_init success.");

    if ((error = esp_now_register_recv_cb(on_message_received)) != ESP_OK)
    {
        ESP_LOGE(ERROR_TAG, "esp_now_register_recv_cb: (%s)", esp_err_to_name(error));

        esp_now_deinit();
        return false;
    }

    ESP_LOGI(INFO_TAG, "esp_now_register_recv_cb success.");

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

static void _on_esp_now_sent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
    ESP_LOGI("ESP-NOW", "Message sent with status: %s", status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}