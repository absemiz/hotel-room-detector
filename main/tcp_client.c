#include "tcp_client.h"

static tcp_client_t _tcp_client;

static bool _initialized = false;
static bool _connected = false;

static bool _tcp_client_initialize_address(void)
{
    memset(&_tcp_client.address, 0, sizeof(_tcp_client.address));

    _tcp_client.address.sin_family = AF_INET;
    _tcp_client.address.sin_port = htons(TCP_CLIENT_TARGET_PORT);
    _tcp_client.address.sin_len = sizeof(_tcp_client.address);

    if (inet_pton(AF_INET, TCP_CLIENT_TARGET_IP, &_tcp_client.address.sin_addr.s_addr) <= 0)
    {
        ESP_LOGE(ERROR_TAG, "inet_pton: Invalid address.");
        return false;
    }

    return true;
}

static ssize_t _tcp_client_receive_chunk(void *buffer, size_t buffer_capacity, ssize_t *buffer_size)
{
    ssize_t bytes_received = 0;

    if ((bytes_received = recv(_tcp_client.socket, ((uint8_t*)(buffer) + *buffer_size), buffer_capacity, 0)) > 0)
    {
        *buffer_size += bytes_received;
    }

    else if (bytes_received == 0)
    {
        ESP_LOGI(INFO_TAG, "Chunk received successfully.");
    }

    else
    {
        ESP_LOGE(ERROR_TAG, "_tcp_client_receive error.");
    }

    return bytes_received;
}

bool tcp_client_initialize(void)
{
    if ((_tcp_client.socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ESP_LOGE(ERROR_TAG, "socket: Socket creation failed.");

        return false;
    }

    if (!_tcp_client_initialize_address())
    {
        ESP_LOGE(ERROR_TAG, "tcp_client_initialize_address: Address initialization failed.");

        return false;
    }

    _initialized = true;

    return _initialized;
}

bool tcp_client_connect(void)
{
    int code;

    if ((code = connect(_tcp_client.socket, (const struct sockaddr*)(&_tcp_client.address), sizeof(_tcp_client.address))) < 0)
    {
        ESP_LOGE(ERROR_TAG, "tcp_client_connect: Socket connection failed. Code: %zd", code);

        close(_tcp_client.socket);

        return false;
    }

    ESP_LOGI(INFO_TAG, "Socket connected successfully.");

    _connected = true;

    return _connected;
}

bool tcp_client_send_message(const void* message, size_t message_size)
{
    ssize_t bytes_send;

    if ((bytes_send = send(_tcp_client.socket, message, message_size, 0)) < 0)
    {
        ESP_LOGE(ERROR_TAG, "send: Returned negative integer, %zd", bytes_send);

        return false;
    }

    ESP_LOGI(INFO_TAG, "%zd bytes send", bytes_send);

    return true;
}

ssize_t tcp_client_receive_message(void* buffer, size_t buffer_size)
{
    ssize_t bytes_received = 0;

    while (_tcp_client_receive_chunk(buffer, buffer_size, &bytes_received) > 0)
    {
        if (*((char*)(buffer) + bytes_received) == '\n')
        {
            *((char*)(buffer) + bytes_received) = '\0';

            break;
        }
    }
  
    ESP_LOGI(INFO_TAG, "%zd bytes received", bytes_received);

    return bytes_received;
}

bool tcp_client_close(void)
{
    return closesocket(_tcp_client.socket) != -1;
}

inline bool tcp_client_is_initialized()
{
    return _initialized;
}

inline bool tcp_client_is_connected(void)
{
    return _connected;
}

inline uint8_t* tcp_client_get_buffer(void)
{
    return _tcp_client.buffer;
}