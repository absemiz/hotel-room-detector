idf_component_register(
    SRCS "lookups.c" "relay_module.c" "rf_receiver.c" "rf_transmitter.c" "door_sensor.c" "deep_sleep.c" "log_tags.c" "main.c" "initialization.c" "event_handlers.c" "tcp_client.c" "pir_sensor.c" "rf_transmitter.c" "rf_receiver.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash esp_wifi esp_netif esp_eth driver vfs console lwip esp_driver_gpio
)
