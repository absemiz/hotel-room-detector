#include <stdio.h>

#include <sys/socket.h>

#include <esp_wifi.h>
#include <esp_mac.h>
#include <esp_err.h>
#include <esp_log.h>

#include "initialization.h"
#include "event_handlers.h"
#include "tcp_client.h"
#include "pir_sensor.h"
#include "deep_sleep.h"

#define TIMEOUT_IN_MS                     60000
#define POLL_PERIOD_IN_MS                 100

#define PIR_SENSOR_ESP32

void app_main(void)
{
    deep_sleep_state_initialize();

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
    {
        deep_sleep_state_wakeup_task();

        int timeout = 0;

        while (!deep_sleep_state_is_wakeup_task_completed() && timeout < TIMEOUT_IN_MS)
        {
            vTaskDelay(pdMS_TO_TICKS(POLL_PERIOD_IN_MS));
            timeout += POLL_PERIOD_IN_MS;
        }

        if (timeout >= TIMEOUT_IN_MS)
        {
            ESP_LOGE(ERROR_TAG, "Timeout waiting for wakeup task to complete.");
        }
    }

    deep_sleep_state_enter();
}