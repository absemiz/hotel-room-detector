#include "pir_sensor.h"

static pir_sensor_t _pir_sensor;

static void _pir_sensor_push_back_reading(int reading);

static void _pir_sensor_clear_readings(void);

static void _pir_sensor_send_readings(void);

bool pir_sensor_initialize(void) 
{
    esp_err_t error;

    gpio_config_t configuration = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pin_bit_mask = (1ULL << PIR_SENSOR_PIN)
    };

    memset(&_pir_sensor, 0, sizeof(_pir_sensor));

    if ((error = gpio_config(&configuration)) != ESP_OK) 
    {
        ESP_LOGE(ERROR_TAG, "gpio_config: (%s)", esp_err_to_name(error));

        return false;
    }

    _pir_sensor.readings_capacity = sizeof(_pir_sensor.readings) * 8U;

    return true;
}

void pir_sensor_task(void *arguments)
{
    while (_pir_sensor.readings_size != _pir_sensor.readings_capacity)
    {
        _pir_sensor_push_back_reading(gpio_get_level(PIR_SENSOR_PIN));
        vTaskDelay(pdMS_TO_TICKS(PIR_SENSOR_POLL_PERIOD_IN_MS));
    }
}

inline uint8_t pir_sensor_low_count(void)
{
    return _pir_sensor.low_count;
}

inline uint8_t pir_sensor_high_count(void)
{
    return _pir_sensor.high_count;
}

static void _pir_sensor_push_back_reading(int reading)
{
    if (reading)
    {
        _pir_sensor.readings |= ((uint64_t)(0x80) >> _pir_sensor.readings_size);

        _pir_sensor.high_count++;

        ESP_LOGI(INFO_TAG, "PIR sensor high detected. (%d)", _pir_sensor.readings_size);

        vTaskDelay(pdMS_TO_TICKS(PIR_SENSOR_SENSOR_HIGH_DURATION_IN_MS));
    }

    else
    {
        _pir_sensor.readings &= ~((uint64_t)(0x80) >> _pir_sensor.readings_size);
        _pir_sensor.low_count++;

        ESP_LOGI(INFO_TAG, "PIR sensor low detected. (%d)",  _pir_sensor.readings_size);
    }

    _pir_sensor.readings_size++;
}

static void _pir_sensor_clear_readings(void)
{
    _pir_sensor.high_count = 0;
    _pir_sensor.readings_size = 0;
}

static void _pir_sensor_send_readings(void)
{
    char message[64];
    int message_size;
    
    message_size = snprintf(message, sizeof(message), "{\"pir_readings\":\"%lld\"}\n", _pir_sensor.readings);

    if (tcp_client_is_connected())
    {
        tcp_client_send_message(message, message_size);

        tcp_client_receive_message(tcp_client_get_buffer(), TCP_CLIENT_BUFFER_SIZE);

        ESP_LOGI(INFO_TAG, "Received message: (%s)", tcp_client_get_buffer());
    }
}