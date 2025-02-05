#include "lookups.h"

static uint8_t _target_esp_mac_address[6] = {0x24, 0x6F, 0x28, 0x00, 0x00, 0x00};

uint8_t* target_esp_mac_address(void)
{
    return _target_esp_mac_address;
}