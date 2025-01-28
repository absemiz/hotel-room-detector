#ifndef _HRD__TCP_CLIENT__H__
#define _HRD__TCP_CLIENT__H__

#include <esp_log.h>
#include <lwip/sockets.h>

#include "log_tags.h"

#define TCP_CLIENT_TARGET_IP               "192.168.1.7"
#define TCP_CLIENT_TARGET_PORT             8080

#define TCP_CLIENT_BUFFER_SIZE  128

typedef
struct tcp_client_t
{
    int socket;
    struct sockaddr_in address;
    uint8_t buffer[TCP_CLIENT_BUFFER_SIZE];
}
tcp_client_t;

bool tcp_client_initialize(void);
bool tcp_client_connect(void);
bool tcp_client_send_message(const void* message, size_t message_size);
ssize_t tcp_client_receive_message(void* buffer, size_t buffer_size);
bool tcp_client_close(void);

bool tcp_client_is_initialized(void);
bool tcp_client_is_connected(void);
uint8_t* tcp_client_get_buffer(void);

#endif
